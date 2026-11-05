"""analyzeHeadless runs: cache key, heap, project lock, project copy and atomic replacement, stage logs."""

import hashlib
import json
import os
import re
import shutil
import subprocess
import time
from dataclasses import dataclass
from pathlib import Path

from . import projlock

GHIDRA_VERSION = "12.1.2"
PROCESSOR = "x86:LE:32:default"
CSPEC = "gcc"
PROGRAM = "Civ5XP"
PROJECT = "civ5re"
PREV = "civ5re.prev"
REBUILD_DIR = "rebuild"
META = "civ5re.json"
JAVA_DIR = Path("tools") / "ghidra" / "java"
PROFILE = Path("tools") / "ghidra" / "analysis.properties"


# `call next; pop reg` (a get-PC): E8 00 00 00 00 58..5F. Civ5reOptions.picCalls scans for the same bytes.


def pic_call_offsets(code):
    """Offsets of every `call next; pop reg` in `code` (overlapping matches included)."""
    return [m.start() for m in re.finditer(rb"(?=\xe8\x00\x00\x00\x00[\x58-\x5f])", code)]


SCRIPT_ERROR = re.compile(r"^.*(Failed to find source bundle|Failed to get OSGi bundle|SCRIPT ERROR|"
                          r"ERROR REPORT SCRIPT ERROR|Script .*failed|Failed to run script).*$", re.M)


PROJECT_LOCKED = re.compile(r"LockException|Unable to lock project")
LOCK_RETRIES = 4
LOCK_BACKOFF = 5.0


def project_locked(text):
    """True when analyzeHeadless output shows Ghidra could not take the project lock."""
    return bool(PROJECT_LOCKED.search(text))


class HeadlessError(Exception):
    pass


def ghidra_info(ghidra_dir):
    p = Path(ghidra_dir) / "Ghidra" / "application.properties"
    if not p.is_file():
        raise HeadlessError(f"{p} not found: check ghidra_dir")
    props = {}
    for line in p.read_text(encoding="utf-8", errors="replace").splitlines():
        if "=" in line and not line.lstrip().startswith("#"):
            k, v = line.split("=", 1)
            props[k.strip()] = v.strip()
    return {"version": props.get("application.version", ""),
            "revision": props.get("application.revision.ghidra", "")}


def check_ghidra(ghidra_dir):
    info = ghidra_info(ghidra_dir)
    if info["version"] != GHIDRA_VERSION:
        raise HeadlessError(f"Ghidra {info['version'] or '?'} found, {GHIDRA_VERSION} is pinned")
    return info


def analyze_headless(ghidra_dir):
    name = "analyzeHeadless.bat" if os.name == "nt" else "analyzeHeadless"
    p = Path(ghidra_dir) / "support" / name
    if not p.is_file():
        raise HeadlessError(f"{p} not found")
    return p


def script_path(repo, name):
    return Path(repo) / JAVA_DIR / name


def function_seeds(binfacts):
    """Sorted unique addresses that must be function starts before analysis: vtable slot targets (func/thunk,
    not __cxa_pure_virtual) and exported FUNC symbols with a size in a real section. `binfacts` is the parsed
    binfacts.json. Returns the seed-file text."""
    addrs = set()
    for v in binfacts.get("vtables", []):
        for g in v.get("groups", []):
            for sl in g.get("slots", []):
                if sl.get("kind") in ("func", "thunk") and sl.get("target"):
                    addrs.add(int(sl["target"], 16))
    for s in binfacts.get("symbols", []):
        if s.get("type") == "func" and s.get("size", 0) > 0 and s.get("section") not in ("UND", "*ABS*", None):
            addrs.add(int(s["addr"], 16))
    addrs.discard(0)
    return "# civ5re function seeds 1\n" + "".join(f"0x{a:08x}\n" for a in sorted(addrs))


def cache_key(binary_sha256, ghidra_dir, repo, seeds=""):
    """Full sha256 of everything the full-analysis cache depends on; typedb is deliberately not part of it.
    `seeds` is the function_seeds() text (a function of the binary and binfacts)."""
    info = ghidra_info(ghidra_dir)
    h = hashlib.sha256()

    def part(label, data):
        h.update(label.encode() + b"\0" + data + b"\0")

    part("binary", binary_sha256.encode())
    part("ghidra", f"{info['version']}+{info['revision']}".encode())
    for label, path in (("profile", Path(repo) / PROFILE), ("options", script_path(repo, "Civ5reOptions.java"))):
        part(label, path.read_bytes() if path.is_file() else b"<missing>")
    part("loader", f"{PROCESSOR} {CSPEC}".encode())
    part("seeds", seeds.encode())
    return h.hexdigest()


def key12(full):
    return full[:12]


def project_files(project_dir, name=PROJECT):
    d = Path(project_dir)
    return d / f"{name}.gpr", d / f"{name}.rep", d / f"{name}.lock"


def project_exists(project_dir, name=PROJECT):
    gpr, rep, _ = project_files(project_dir, name)
    return gpr.is_file() and rep.is_dir()


def is_open(project_dir, name=PROJECT):
    return project_files(project_dir, name)[2].exists()


def cache_dir(project_dir):
    return Path(project_dir) / "cache"


def cache_name(k12):
    return f"pristine-{k12}"


def cache_functions(project_dir, k12):
    return cache_dir(project_dir) / f"{cache_name(k12)}.functions.txt"


def cache_seeds(project_dir, k12):
    return cache_dir(project_dir) / f"{cache_name(k12)}.seeds.txt"


def cache_exists(project_dir, k12):
    return project_exists(cache_dir(project_dir), cache_name(k12))


def read_meta(project_dir):
    p = Path(project_dir) / META
    if not p.is_file():
        return None
    try:
        return json.loads(p.read_text(encoding="utf-8"))
    except json.JSONDecodeError:
        return None


def write_meta(project_dir, meta):
    p = Path(project_dir) / META
    tmp = p.with_name(p.name + ".tmp")
    tmp.write_text(json.dumps(meta, indent=1, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
    os.replace(tmp, p)


def remove_project(project_dir, name):
    gpr, rep, lock = project_files(project_dir, name)
    for p in (gpr, lock):
        if p.exists():
            p.unlink()
    if rep.exists():
        shutil.rmtree(rep)


def copy_project(src_dir, src_name, dst_dir, dst_name):
    sg, sr, _ = project_files(src_dir, src_name)
    if not (sg.is_file() and sr.is_dir()):
        raise HeadlessError(f"project {src_name} not found in {src_dir}")
    Path(dst_dir).mkdir(parents=True, exist_ok=True)
    remove_project(dst_dir, dst_name)
    dg, dr, _ = project_files(dst_dir, dst_name)
    shutil.copytree(sr, dr)
    shutil.copy2(sg, dg)


def replace_project(project_dir, meta):
    """civ5re -> civ5re.prev (one), .rebuild/civ5re -> civ5re, then the cache key goes to civ5re.json."""
    d = Path(project_dir)
    new_g, new_r, _ = project_files(d / REBUILD_DIR, PROJECT)
    if not (new_g.is_file() and new_r.is_dir()):
        raise HeadlessError(f"{d / REBUILD_DIR} holds no rebuilt project")
    cur_g, cur_r, cur_l = project_files(d, PROJECT)
    if cur_l.exists():
        raise HeadlessError("the working project is open (civ5re.lock): close it in Ghidra")
    prev_g, prev_r, _ = project_files(d, PREV)
    had_current = cur_g.exists() or cur_r.exists()
    if had_current:
        remove_project(d, PREV)
        if cur_r.exists():
            os.replace(cur_r, prev_r)
        if cur_g.exists():
            os.replace(cur_g, prev_g)
    try:
        os.replace(new_r, cur_r)
        os.replace(new_g, cur_g)
    except OSError:
        if had_current:
            for p in (cur_r, cur_g):
                if p.exists():
                    shutil.rmtree(p) if p.is_dir() else p.unlink()
            if prev_r.exists():
                os.replace(prev_r, cur_r)
            if prev_g.exists():
                os.replace(prev_g, cur_g)
        raise
    shutil.rmtree(d / REBUILD_DIR, ignore_errors=True)
    write_meta(d, meta)


@dataclass
class StageResult:
    stage: str
    code: int
    seconds: float
    log: Path

    @property
    def ok(self):
        return self.code == 0 and not self.script_error

    @property
    def script_error(self):
        try:
            text = self.log.read_text(encoding="utf-8", errors="replace")
        except OSError:
            return None
        m = SCRIPT_ERROR.search(text)
        return m.group(0).strip() if m else None


def _default_runner(argv, env, logfile):
    p = subprocess.run(argv, env=env, stdout=logfile, stderr=subprocess.STDOUT, stdin=subprocess.DEVNULL)
    return p.returncode


class Headless:
    def __init__(self, ghidra_dir, heap, repo, logs_dir, runner=None, clock=time.monotonic,
                 lock_wait=projlock.WAIT, sleep=time.sleep, out=print):
        self.lock_wait = lock_wait
        self.sleep = sleep
        self.out = out
        self.ghidra_dir = Path(ghidra_dir)
        self.heap = heap
        self.repo = Path(repo)
        self.logs_dir = Path(logs_dir)
        self.runner = runner or _default_runner
        self.clock = clock

    def env(self):
        env = dict(os.environ)
        if self.heap:
            env["GHIDRA_HEADLESS_MAXMEM"] = self.heap
        env["CIV5RE_REPO"] = str(self.repo)
        return env

    def script(self, name):
        return str(script_path(self.repo, name))

    @property
    def script_dir(self):
        return str(self.repo / JAVA_DIR)

    def run(self, stage, args):
        exe = analyze_headless(self.ghidra_dir)
        args = [str(a) for a in args]
        if "-scriptPath" not in args:
            first = min((args.index(f) for f in ("-preScript", "-postScript") if f in args), default=None)
            if first is not None:
                args[first:first] = ["-scriptPath", self.script_dir]
        argv = [str(exe)] + args
        self.logs_dir.mkdir(parents=True, exist_ok=True)
        log = self.logs_dir / (time.strftime("%Y%m%d-%H%M%S") + f"-{stage}.log")
        try:
            held = self.lock()
            held.__enter__()
        except projlock.LockTimeout as e:
            raise HeadlessError(f"the headless lock is busy: {e}") from None
        try:
            t0 = self.clock()
            with open(log, "w", encoding="utf-8", errors="replace", newline="\n") as f:
                for attempt in range(LOCK_RETRIES + 1):
                    f.write(f"== stage {stage}: {' '.join(argv)}\n")
                    f.write(f"== GHIDRA_HEADLESS_MAXMEM={self.heap}\n")
                    f.flush()
                    start = f.tell()
                    code = self.runner(argv, self.env(), f)
                    f.flush()
                    if code == 0 or attempt == LOCK_RETRIES or not self._locked_since(log, start):
                        break
                    delay = LOCK_BACKOFF * 2 ** attempt
                    f.write(f"\n== project locked by another process, retry {attempt + 1} in {delay:.0f} s\n")
                    f.flush()
                    self.sleep(delay)
                seconds = self.clock() - t0
                f.write(f"\n== stage {stage} finished in {seconds:.1f} s, exit {code}\n")
        finally:
            held.__exit__(None, None, None)
        return StageResult(stage, code, seconds, log)

    def lock(self):
        """The shared headless lock; reentrant, so a stage may hold it around several runs."""
        return projlock.project_lock(self.logs_dir.parent / projlock.LOCK_NAME, self.lock_wait, self.out)

    @staticmethod
    def _locked_since(log, offset):
        with open(log, "rb") as f:
            f.seek(offset)
            return project_locked(f.read().decode("utf-8", "replace"))

    def cache_args(self, project_dir, k12, binary, functions_out, force, seeds=None):
        args = [cache_dir(project_dir), cache_name(k12), "-import", binary, "-processor", PROCESSOR,
                "-cspec", CSPEC]
        if force:
            args.append("-overwrite")
        args += ["-preScript", self.script("Civ5reOptions.java"), f"--profile={self.repo / PROFILE}"]
        if seeds is not None:
            args.append(f"--seeds={seeds}")
        args += ["-postScript", self.script("Civ5reExport.java"), f"--out={functions_out}",
                 f"--cache-key={k12}"]
        return args

    def build_cache(self, project_dir, k12, binary, force=False, seeds_text=None):
        cache_dir(project_dir).mkdir(parents=True, exist_ok=True)
        seeds = None
        if seeds_text is not None:
            seeds = cache_seeds(project_dir, k12)
            seeds.write_text(seeds_text, encoding="utf-8", newline="\n")
        if force:
            remove_project(cache_dir(project_dir), cache_name(k12))
        return self.run("cache", self.cache_args(project_dir, k12, binary,
                                                 cache_functions(project_dir, k12), force, seeds))

    def export_functions(self, project_dir, k12):
        out = cache_functions(project_dir, k12)
        args = [cache_dir(project_dir), cache_name(k12), "-process", PROGRAM, "-noanalysis", "-readOnly",
                "-postScript", self.script("Civ5reExport.java"), f"--out={out}", f"--cache-key={k12}"]
        return self.run("export", args)

    def apply_args(self, project_dir, name, dry_run=False, prune_all=False, both=False):
        """both: a real apply followed by a dry run in one call (rebuild)."""
        args = [project_dir, name, "-process", PROGRAM, "-noanalysis"]
        if dry_run and not both:
            args.append("-readOnly")
        flags = []
        if prune_all:
            flags.append("--prune-all")
        args += ["-postScript", self.script("Civ5reApply.java"), f"--repo={self.repo}"] + flags
        if both:
            args += ["-postScript", self.script("Civ5reApply.java"), f"--repo={self.repo}", "--dry-run"]
        elif dry_run:
            args.append("--dry-run")
        return args

    def apply(self, project_dir, name=PROJECT, dry_run=False, prune_all=False, both=False, stage="apply"):
        return self.run(stage, self.apply_args(project_dir, name, dry_run, prune_all, both))

    def decomp_args(self, project_dir, request, out_dir, plan_sha, name=PROJECT):
        return [project_dir, name, "-process", PROGRAM, "-noanalysis", "-readOnly",
                "-postScript", self.script("Civ5reDecomp.java"), f"--in={request}", f"--out={out_dir}",
                f"--plan={plan_sha or '-'}"]

    def decomp(self, project_dir, request, out_dir, plan_sha, name=PROJECT):
        return self.run("decomp", self.decomp_args(project_dir, request, out_dir, plan_sha, name))

    def paramid(self, project_dir, name=PROJECT):
        args = [project_dir, name, "-process", PROGRAM, "-noanalysis", "-postScript",
                self.script("Civ5reParamId.java"), f"--profile={self.repo / PROFILE}"]
        return self.run("paramid", args)
