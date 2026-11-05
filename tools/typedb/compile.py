"""The only place for clang flags: environment files, PCH, running clang, cache keys."""
from __future__ import annotations

import hashlib
import json
import os
import re
import subprocess
from dataclasses import dataclass
from pathlib import Path

from civ5re import config as _config

TYPEDB_VERSION = "1.1.1"
MODES = ("pch", "tree", "cpp", "alone")
PRELUDE = "compat/civ5re_prelude.h"
CLANG_LINE = "<clang>"

COMMON = ["--target=i386-pc-linux-gnu", "-std=gnu++11", "-nostdinc", "-nostdinc++"]
TAIL = ["-ferror-limit=0", "-fno-color-diagnostics"]
PASS_A = ["-fsyntax-only", "-Xclang", "-ast-dump=json"]
PASS_B = ["-fno-access-control", "-S", "-emit-llvm", "-g", "-fstandalone-debug",
          "-fno-eliminate-unused-debug-types"]
PASS_C = ["-fsyntax-only", "-Xclang", "-ast-dump=json"]
DEPS = ["-MD", "-MF"]
EMIT_PCH = ["-x", "c++-header"]


class EnvError(Exception):
    """Tool or environment failure (exit code 2)."""


def _posix(p) -> str:
    return str(p).replace("\\", "/")


@dataclass
class Env:
    third_party: Path
    include_dirs: list
    pch_h: Path
    prelude: Path
    clang: Path
    cxxfilt: Path
    resource_include: Path


def tools(cfg=None):
    cfg = cfg or _config.load()
    clang = cfg.tool("clang++")
    cxxfilt = cfg.tool("llvm-cxxfilt")
    if not clang.is_file():
        raise EnvError(f"clang++ not found: {clang} (llvm_dir)")
    if not cxxfilt.is_file():
        raise EnvError(f"llvm-cxxfilt not found: {cxxfilt} (llvm_dir)")
    return clang, cxxfilt


def resource_include(clang: Path) -> Path:
    for p in sorted(clang.parent.parent.glob("lib/clang/*/include")):
        if (p / "stddef.h").is_file():
            return p
    try:
        out = subprocess.run([str(clang), "-print-resource-dir"], capture_output=True, text=True, check=True)
    except (OSError, subprocess.CalledProcessError) as e:
        raise EnvError(f"clang -print-resource-dir failed: {e}") from None
    return Path(out.stdout.strip()) / "include"


def read_include_dirs(third_party: Path, res_include: Path) -> list:
    f = third_party / "include_dirs.txt"
    if not f.is_file():
        raise EnvError(f"missing {_posix(f)}")
    dirs, seen_clang = [], False
    for n, raw in enumerate(f.read_bytes().decode("utf-8").split("\n"), 1):
        line = raw.strip()
        if not line or line.startswith("#"):
            continue
        if line == CLANG_LINE:
            seen_clang = True
            dirs.append(res_include)
            continue
        if "\\" in line or os.path.isabs(line):
            raise EnvError(f"include_dirs.txt:{n}: path must be relative to third_party/ with '/': {line!r}")
        d = third_party / line
        if not d.is_dir():
            raise EnvError(f"include_dirs.txt:{n}: no such folder third_party/{line}")
        dirs.append(d)
    if not seen_clang:
        raise EnvError("include_dirs.txt: no <clang> line")
    return dirs


_INCLUDE = re.compile(r'\s*#\s*include\s*[<"][^>"]+[>"]\s*(//.*)?$')


def check_pch_h(third_party: Path) -> Path:
    f = third_party / "pch.h"
    if not f.is_file():
        raise EnvError(f"missing {_posix(f)}")
    text = f.read_bytes().decode("utf-8")
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.S)
    first = True
    for n, raw in enumerate(text.split("\n"), 1):
        line = raw.strip()
        if not line or line.startswith("//"):
            continue
        if first:
            if not re.fullmatch(r'#\s*include\s*"compat/civ5re_prelude\.h"\s*(//.*)?', line):
                raise EnvError(f"pch.h:{n}: the first line of code must be #include \"{PRELUDE}\"")
            first = False
            continue
        if not _INCLUDE.match(line):
            raise EnvError(f"pch.h:{n}: only #include and comments are allowed: {line!r}")
    if first:
        raise EnvError("pch.h: no #include of the prelude")
    return f


def load_env(third_party=None, cfg=None) -> Env:
    tp = Path(third_party) if third_party is not None else _config.ROOT / "third_party"
    tp = tp.resolve()
    if not tp.is_dir():
        raise EnvError(f"third_party not found: {_posix(tp)}")
    clang, cxxfilt = tools(cfg)
    res = resource_include(clang)
    dirs = read_include_dirs(tp, res)
    pch_h = check_pch_h(tp)
    prelude = tp / PRELUDE
    if not prelude.is_file():
        raise EnvError(f"missing prelude third_party/{PRELUDE}")
    return Env(tp, dirs, pch_h, prelude, clang, cxxfilt, res)


def work_dir(build) -> Path:
    return Path(build).resolve() / "typedb"


def clang_args(mode, env: Env, tree=None, build=None, pch=None) -> list:
    """Flags for `mode` (pch|tree|cpp|alone). Pass-specific extras are the PASS_* lists above.

    `pch` (modes tree and cpp only): the precompiled header to load instead of the mode's own
    (`third_party.pch` / `headers.pch` in the work dir); for callers that precompile a bigger unit.
    """
    if mode not in MODES:
        raise ValueError(f"unknown clang_args mode {mode!r}")
    args = list(COMMON)
    for d in env.include_dirs:
        args += ["-isystem", _posix(d)]
    args += TAIL
    wd = work_dir(build) if build is not None else None
    if mode == "pch":
        if wd is None:
            raise ValueError("mode 'pch' needs build")
        return args + EMIT_PCH + [_posix(env.pch_h), "-o", _posix(wd / "third_party.pch")]
    if mode in ("tree", "cpp", "alone") and tree is None:
        raise ValueError(f"mode {mode!r} needs tree")
    tree = _posix(Path(tree).resolve())
    if mode == "tree":
        if wd is None:
            raise ValueError("mode 'tree' needs build")
        return args + ["-include-pch", _posix(pch if pch is not None else wd / "third_party.pch"), "-isystem", tree]
    if mode == "cpp":
        if wd is None:
            raise ValueError("mode 'cpp' needs build")
        return args + ["-include-pch", _posix(pch if pch is not None else wd / "headers.pch"), "-I", tree]
    return args + ["-include", _posix(env.prelude), "-I", tree]


def run(env: Env, args, cwd, stdout_path=None):
    """Run clang; return (returncode, stderr text). stdout goes to stdout_path when given."""
    cmd = [str(env.clang)] + [str(a) for a in args]
    try:
        if stdout_path is not None:
            with open(stdout_path, "wb") as out:
                p = subprocess.run(cmd, cwd=cwd, stdout=out, stderr=subprocess.PIPE)
        else:
            p = subprocess.run(cmd, cwd=cwd, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
    except OSError as e:
        raise EnvError(f"cannot run clang: {e}") from None
    err = p.stderr.decode("utf-8", "replace")
    if p.returncode not in (0, 1):
        raise EnvError(f"clang crashed (exit {p.returncode}): {err[-2000:]}")
    return p.returncode, err


def sha256_bytes(b: bytes) -> str:
    return hashlib.sha256(b).hexdigest()


def sha256_file(p) -> str:
    h = hashlib.sha256()
    with open(p, "rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def _stat_key(p: Path):
    st = p.stat()
    return [st.st_size, st.st_mtime_ns]


def clang_version(env: Env, cache_dir: Path) -> str:
    """`clang --version`, cached by the executable's size and mtime so a cached run does not start clang."""
    stamp = _stat_key(env.clang)
    f = cache_dir / "clang_version.json"
    try:
        d = json.loads(f.read_text(encoding="utf-8"))
        if d.get("stamp") == stamp and d.get("exe") == _posix(env.clang):
            return d["version"]
    except (OSError, ValueError, KeyError):
        pass
    try:
        out = subprocess.run([str(env.clang), "--version"], capture_output=True, text=True, check=True).stdout
    except (OSError, subprocess.CalledProcessError) as e:
        raise EnvError(f"clang --version failed: {e}") from None
    version = out.splitlines()[0].strip()
    cache_dir.mkdir(parents=True, exist_ok=True)
    f.write_text(json.dumps({"exe": _posix(env.clang), "stamp": stamp, "version": version}), encoding="utf-8")
    return version


def relative_flags(args, env: Env, tree=None, build=None) -> list:
    """Flags with machine paths replaced by stable names (for flags_sha)."""
    subs = [(_posix(env.third_party), "<third_party>"), (_posix(env.resource_include), "<clang>")]
    if build is not None:
        subs.append((_posix(work_dir(build)), "<build>"))
    if tree is not None:
        subs.append((_posix(Path(tree).resolve()), "<tree>"))
    out = []
    for a in args:
        for k, v in subs:
            a = a.replace(k, v)
        out.append(a)
    return out


def third_party_files(env: Env):
    out = []
    for root, dirs, files in os.walk(env.third_party):
        dirs.sort()
        for name in sorted(files):
            p = Path(root) / name
            out.append(p)
    return out


def code_version() -> str:
    """typedb version + sha of its own sources (a code change invalidates the cache)."""
    h = hashlib.sha256(TYPEDB_VERSION.encode())
    here = Path(__file__).resolve().parent
    for p in sorted(here.glob("*.py")):
        h.update(p.name.encode())
        h.update(p.read_bytes())
    return h.hexdigest()


def pch_key(env: Env, version: str, build) -> str:
    h = hashlib.sha256()
    h.update(code_version().encode())
    h.update(version.encode())
    h.update("\0".join(relative_flags(clang_args("pch", env, build=build), env, build=build)).encode())
    for f in (env.third_party / "include_dirs.txt", env.pch_h, env.prelude):
        h.update(sha256_file(f).encode())
    for p in third_party_files(env):
        rel = _posix(p.relative_to(env.third_party))
        h.update(f"{rel}\0{_stat_key(p)}\n".encode())
    for d in env.include_dirs:
        if d == env.resource_include:
            h.update(f"<clang>{_stat_key(env.clang)}".encode())
    return h.hexdigest()
