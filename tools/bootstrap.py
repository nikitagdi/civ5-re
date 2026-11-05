import os
import re
import shutil
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
TOOLS = ROOT / "tools"
MIN_PY = (3, 11)
REEXEC_GUARD = "CIV5RE_BOOTSTRAP_REEXEC"
HOOKS_DIR = ".githooks"
HOOKS = ("pre-commit", "post-commit")
LLVM_TOOLS = ("llvm-cxxfilt", "clang++", "ld.lld")


def _py_launcher_candidates():
    """(tag, version) pairs from `py -0p`, highest version first."""
    try:
        out = subprocess.run(["py", "-0p"], capture_output=True, text=True, timeout=30).stdout
    except (OSError, subprocess.SubprocessError):
        return []
    found = []
    for line in out.splitlines():
        m = re.match(r"\s*(-V:(\d+)\.(\d+)\S*|-(\d+)\.(\d+)\S*)", line)
        if not m:
            continue
        major, minor = (m.group(2), m.group(3)) if m.group(2) else (m.group(4), m.group(5))
        found.append((m.group(1), (int(major), int(minor))))
    return sorted((c for c in found if c[1] >= MIN_PY), key=lambda c: c[1], reverse=True)


def _find_new_python():
    for tag, _ in _py_launcher_candidates():
        try:
            out = subprocess.run(["py", tag, "-c", "import sys;print(sys.executable)"],
                                 capture_output=True, text=True, timeout=30).stdout.strip()
        except (OSError, subprocess.SubprocessError):
            continue
        if out:
            return out
    for minor in range(20, MIN_PY[1] - 1, -1):
        exe = shutil.which(f"python3.{minor}")
        if exe:
            return exe
    return None


def ensure_python():
    if sys.version_info >= MIN_PY:
        return
    if os.environ.get(REEXEC_GUARD):
        sys.exit("bootstrap: re-exec did not reach Python >= 3.11")
    exe = _find_new_python()
    if exe is None:
        sys.exit("bootstrap: Python >= 3.11 is required (bins.lock is read with tomllib). "
                 "Install it from python.org and make sure `py -0p` lists it.")
    print(f"python: {sys.version.split()[0]} is too old, restarting with {exe}")
    sys.stdout.flush()
    env = dict(os.environ, **{REEXEC_GUARD: "1"})
    sys.exit(subprocess.call([exe, str(Path(__file__).resolve()), *sys.argv[1:]], env=env))


class Report:
    def __init__(self):
        self.errors = 0
        self.warnings = 0

    def ok(self, msg):
        print(f"ok: {msg}")

    def changed(self, msg):
        print(f"changed: {msg}")

    def info(self, msg):
        print(f"info: {msg}")

    def warn(self, msg):
        self.warnings += 1
        print(f"warning: {msg}")

    def error(self, msg):
        self.errors += 1
        print(f"error: {msg}")


def venv_python():
    sub = "Scripts/python.exe" if os.name == "nt" else "bin/python"
    return ROOT / ".venv" / sub


def step_venv(rep):
    import venv
    vpy = venv_python()
    if not vpy.exists():
        venv.create(ROOT / ".venv", with_pip=False)
        rep.changed("created .venv")
    code = ("import sys, sysconfig;"
            "print('%d.%d' % sys.version_info[:2]);print(sysconfig.get_path('purelib'))")
    out = subprocess.run([str(vpy), "-c", code], capture_output=True, text=True)
    if out.returncode != 0:
        rep.error(f".venv is broken ({out.stderr.strip() or 'cannot run python'}); delete .venv and rerun")
        return
    version, purelib = out.stdout.split("\n")[:2]
    if tuple(int(x) for x in version.split(".")) < MIN_PY:
        rep.error(f".venv uses Python {version}; delete .venv and rerun")
        return
    rep.ok(f".venv (Python {version})")
    pth = Path(purelib) / "civ5re.pth"
    want = str(TOOLS) + "\n"
    if not pth.exists() or pth.read_text(encoding="utf-8") != want:
        pth.write_text(want, encoding="utf-8", newline="")
        rep.changed(f"wrote {pth}")
    else:
        rep.ok("civ5re.pth -> tools/")


def step_hooks(rep):
    cur = subprocess.run(["git", "config", "--get", "core.hooksPath"], cwd=ROOT,
                         capture_output=True, text=True)
    if cur.returncode not in (0, 1):
        rep.error(f"git config failed: {cur.stderr.strip()}")
        return
    if cur.stdout.strip() != HOOKS_DIR:
        res = subprocess.run(["git", "config", "core.hooksPath", HOOKS_DIR], cwd=ROOT,
                             capture_output=True, text=True)
        if res.returncode != 0:
            rep.error(f"git config core.hooksPath failed: {res.stderr.strip()}")
            return
        rep.changed(f"core.hooksPath = {HOOKS_DIR}")
    else:
        rep.ok(f"core.hooksPath = {HOOKS_DIR}")
    for hook in HOOKS:
        if (ROOT / HOOKS_DIR / hook).is_file():
            rep.ok(f"hook {hook}")
        else:
            rep.info(f"hook {hook}: not installed")


def step_template(rep, config):
    p = ROOT / config.LOCAL_FILE
    if p.exists():
        rep.ok(f"{config.LOCAL_FILE} exists")
        return
    p.write_text(config.local_template(), encoding="utf-8", newline="")
    rep.changed(f"wrote {config.LOCAL_FILE} template")


def step_config(rep, config):
    try:
        cfg = config.load(ROOT)
    except config.ConfigError as e:
        rep.error(str(e))
        return None
    missing = cfg.missing_required()
    for k in missing:
        rep.error(f"required key {k.label} is empty: set it in {config.LOCAL_FILE} or env {k.env}")
    if not missing:
        rep.ok("config parsed")
    return cfg


def step_paths(rep, cfg, config):
    for k in config.KEYS:
        if k.kind not in ("dir", "dirs"):
            continue
        value = cfg.get(k.section, k.name) if k.section else cfg.get(k.name)
        if not value:
            if k.needed_by and not k.required:
                rep.warn(f"{k.label} is empty (needed by: {k.needed_by}); set it in "
                         f"{config.LOCAL_FILE} or env {k.env}")
            continue
        parts = value.split(";") if k.kind == "dirs" else [value]
        for part in parts:
            p = Path(part)
            p = p if p.is_absolute() else ROOT / p
            if p.is_dir():
                continue
            msg = f"{k.label} = {part} ({cfg.source(*([k.section] if k.section else []), k.name)}): not a directory"
            (rep.error if not k.section else rep.warn)(msg)
    llvm = cfg.path("llvm_dir")
    if llvm is not None and llvm.is_dir():
        missing = [t for t in LLVM_TOOLS if not cfg.tool(t).is_file()]
        for t in missing:
            rep.error(f"llvm_dir: {cfg.tool(t)} not found")
        if not missing:
            rep.ok("llvm tools: " + ", ".join(LLVM_TOOLS))


def step_binlock(rep, cfg, binlock):
    lock = ROOT / "bins.lock"
    try:
        entries = binlock.load(lock)
        binlock.target(entries)
    except binlock.BinlockError as e:
        rep.error(str(e))
        return
    bin_dir = cfg.path("bin_dir")
    if bin_dir is None:
        rep.warn("bin_dir is empty: bins.lock not verified")
        return
    for e in entries:
        status, msg = binlock.verify(e, bin_dir)
        if status == "ok":
            rep.ok(msg)
        elif status == "missing":
            if e.role == "target":
                rep.warn(f"{msg} (checks may work from the cache)")
            else:
                rep.info(msg)
        else:
            rep.error(msg)


def main():
    ensure_python()
    sys.path.insert(0, str(TOOLS))
    from civ5re import binlock, config
    rep = Report()
    print(f"python: {sys.version.split()[0]} ({sys.executable})")
    step_venv(rep)
    step_hooks(rep)
    step_template(rep, config)
    cfg = step_config(rep, config)
    if cfg is not None:
        step_paths(rep, cfg, config)
        step_binlock(rep, cfg, binlock)
    print(f"bootstrap: {rep.errors} error(s), {rep.warnings} warning(s)")
    return 1 if rep.errors else 0


if __name__ == "__main__":
    sys.exit(main())
