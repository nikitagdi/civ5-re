"""Pinned inputs of `seed env`: inputs.lock, Lekmod git objects, SDK map.

Lekmod is read through `git cat-file` / `ls-tree` / `rev-parse` only.
A mismatch with inputs.lock raises InputsError (CLI exit code 2).
"""
import hashlib
import subprocess
from dataclasses import dataclass
from pathlib import Path

DATA = Path(__file__).resolve().parent / "data"
ROOT = Path(__file__).resolve().parents[2]
LOCK = DATA / "inputs.lock"
SDK_MAP = DATA / "sdk_map.txt"

EVIDENCE = (
    "lekmod_pdb_paths_all.txt",
    "linux_port_all.txt",
    "sdk_lib_include_dirs.txt",
    "sdk_lib_paths.txt",
    "source_paths_unique.txt",
    "source_tree_dircounts.txt",
    "win_abs_all.txt",
    "win_rel_all.txt",
)

# template (tools/seed/data/compat/) -> path under third_party/
COMPAT = {
    "civ5re_prelude.h.in": "compat/civ5re_prelude.h",
    "Windows.h.in": "compat/Windows.h",
    "tbb.h.in": "compat/tbb.h",
    "sqlite3.h.in": "compat/sqlite3.h",
    "stubs-32.h.in": "compat/gnu/stubs-32.h",
    "hash_map.h.in": "compat/hash_map",
    "hash_set.h.in": "compat/hash_set",
}

LUA_TREE = ("sdk", "DLC/LEKMOD/CvGameCore_Expansion2/ThirdPartyLibs/Lua51/include")
LUA_ORIGINAL_LOCATION = "SDKs/Lua/5.1/include/"

_GIT_READONLY = {"cat-file", "ls-tree", "rev-parse"}

TREES = (
    ("sdk", "DLC/LEKMOD/CvGameCore_Expansion2/CvGameCoreDLLUtil/include"),
    ("sdk", "DLC/LEKMOD/CvGameCore_Expansion2/FirePlace/include"),
    ("sdk", "DLC/LEKMOD/CvGameCore_Expansion2/CvGameDatabase/include"),
    ("sdk", "DLC/LEKMOD/CvGameCore_Expansion2/CvLocalization/include"),
    ("sdk", "DLC/LEKMOD/CvGameCore_Expansion2/CvWorldBuilderMap/include"),
    ("sdk", "DLC/LEKMOD/CvGameCore_Expansion2/ThirdPartyLibs/Lua51/include"),
    ("win32", "LEKMOD_DLL/CvGameCoreDLL_Expansion2/FirePlace/include/FireWorks/Win32"),
)
# Public upstream commits of EnormousApplePie/Lekmod: sdk = "add v28.2 dll code"; win32 = bf3f4e9d ("v34.15 hotfix",
# the base of the gamecore fork; its FireWorks/Win32 tree is the one pinned in inputs.lock).
COMMITS = (
    ("sdk", "96fdef73d734e69520dbd17311179d7da29164fc"),
    ("win32", "bf3f4e9d3f9cd0bf4fc989a28680751c10ca7200"),
)


class InputsError(Exception):
    exit_code = 2


@dataclass(frozen=True)
class Entry:
    ref: str
    src: str
    dest: str
    origin: str


@dataclass
class Lock:
    commits: dict
    trees: list      # (ref, path, hash)
    files: dict      # repo-relative path -> sha256


def text_sha256(data):
    return hashlib.sha256(data.replace(b"\r\n", b"\n")).hexdigest()


def file_sha256(path):
    return text_sha256(Path(path).read_bytes())


def parse_lock(text):
    commits, trees, files = {}, [], {}
    for n, line in enumerate(text.splitlines(), 1):
        if not line.strip() or line.startswith("#"):
            continue
        f = line.split("\t")
        if f[0] == "commit" and len(f) == 3:
            commits[f[1]] = f[2]
        elif f[0] == "tree" and len(f) == 4:
            trees.append((f[1], f[2], f[3]))
        elif f[0] == "sha256" and len(f) == 3:
            files[f[2]] = f[1]
        else:
            raise InputsError(f"inputs.lock:{n}: bad line")
    return Lock(commits, trees, files)


def load_lock(path=None):
    path = Path(path) if path else LOCK
    if not path.is_file():
        raise InputsError(f"missing {path.name}")
    return parse_lock(path.read_text(encoding="utf-8"))


def format_lock(lock):
    out = ["# seed inputs: Lekmod commits and trees, sha256 of evidence and third_party files"]
    for ref in sorted(lock.commits):
        out.append(f"commit\t{ref}\t{lock.commits[ref]}")
    for ref, path, h in sorted(lock.trees, key=lambda t: (t[0], t[1])):
        out.append(f"tree\t{ref}\t{path}\t{h}")
    for path in sorted(lock.files):
        out.append(f"sha256\t{lock.files[path]}\t{path}")
    return "\n".join(out) + "\n"


def git(lekmod_dir, *args):
    if not args or args[0] not in _GIT_READONLY:
        raise InputsError(f"git {args[:1]} is not allowed on Lekmod")
    try:
        r = subprocess.run(["git", "-C", str(lekmod_dir), *args], capture_output=True)
    except OSError as e:
        raise InputsError(f"git unavailable: {e}") from None
    if r.returncode != 0:
        raise InputsError(f"git {' '.join(args)}: {r.stderr.decode('utf-8', 'replace').strip()}")
    return r.stdout


def rev_parse(lekmod_dir, rev):
    return git(lekmod_dir, "rev-parse", "--verify", "--quiet", rev).decode().strip()


def read_blob(lekmod_dir, commit, path):
    return git(lekmod_dir, "cat-file", "blob", f"{commit}:{path}")


def list_files(lekmod_dir, commit, path):
    out = git(lekmod_dir, "ls-tree", "-r", "--name-only", "-z", commit, "--", path)
    return sorted(p.decode("utf-8") for p in out.split(b"\0") if p)


def sdk_entries(path=None):
    path = Path(path) if path else SDK_MAP
    entries = []
    for line in path.read_text(encoding="utf-8").splitlines():
        if not line.strip() or line.startswith("#"):
            continue
        src, dest, origin = line.split("\t")
        ref, _, p = src.partition(":")
        entries.append(Entry(ref, p, dest, origin))
    return entries


def origin_counts(entries):
    n = {"binary": 0, "guess": 0}
    for e in entries:
        n[e.origin] += 1
    return n


def lua_files(lekmod_dir, lock=None):
    lock = lock or load_lock()
    commit = lock.commits[LUA_TREE[0]]
    return list_files(lekmod_dir, commit, LUA_TREE[1])


def compat_template(name):
    return (DATA / "compat" / name).read_text(encoding="utf-8")


def evidence_paths():
    return [f"tools/seed/data/{n}" for n in EVIDENCE]


def compute_lock(lekmod_dir, repo_root=None, keep_files=None):
    """Lock from the current git objects and evidence files; keep_files carries third_party entries."""
    repo_root = Path(repo_root) if repo_root else ROOT
    commits = {}
    for ref, c in COMMITS:
        full = rev_parse(lekmod_dir, f"{c}^{{commit}}")
        if not full:
            raise InputsError(f"commit {c} not found in Lekmod")
        commits[ref] = full
    trees = []
    for ref, path in TREES:
        h = rev_parse(lekmod_dir, f"{commits[ref]}:{path}")
        if not h:
            raise InputsError(f"{commits[ref][:7]}:{path} not found")
        trees.append((ref, path, h))
    files = {p: file_sha256(repo_root / p) for p in evidence_paths()}
    for p, h in (keep_files or {}).items():
        if p.startswith("third_party/"):
            files[p] = h
    return Lock(commits, trees, files)


def verify(lekmod_dir, repo_root=None, lock=None, third_party_dir=None):
    """Raises InputsError listing every mismatch."""
    repo_root = Path(repo_root) if repo_root else ROOT
    lock = lock or load_lock()
    bad = []
    for ref, expect in sorted(lock.commits.items()):
        try:
            got = rev_parse(lekmod_dir, f"{expect}^{{commit}}")
        except InputsError as e:
            raise InputsError(f"Lekmod: {e}") from None
        if got != expect:
            bad.append(f"commit {ref}: {got or 'missing'} != {expect}")
    for ref, path, expect in lock.trees:
        commit = lock.commits.get(ref)
        try:
            got = rev_parse(lekmod_dir, f"{commit}:{path}")
        except InputsError:
            got = ""
        if got != expect:
            bad.append(f"tree {path}: {got or 'missing'} != {expect}")
    for path, expect in sorted(lock.files.items()):
        if path.startswith("third_party/"):
            if third_party_dir is None:
                continue
            p = Path(third_party_dir).parent / path
            if not p.is_file():
                continue
        else:
            p = repo_root / path
            if not p.is_file():
                bad.append(f"{path}: missing")
                continue
        got = file_sha256(p)
        if got != expect:
            bad.append(f"{path}: sha256 {got} != {expect}")
    if bad:
        raise InputsError("inputs.lock mismatch:\n  " + "\n  ".join(bad))


def civ5xp_sha256(repo_root=None):
    from civ5re import binlock
    repo_root = Path(repo_root) if repo_root else ROOT
    return binlock.target(binlock.load(repo_root / "bins.lock")).sha256


def default_lekmod_dir():
    """Lekmod checkout from `[seed] lekmod_dir` (civ5re.local.cfg or env CIV5RE_SEED_LEKMOD_DIR), or None."""
    from civ5re import config
    try:
        return config.load(ROOT).path("seed", "lekmod_dir")
    except config.ConfigError:
        return None


def compat_files():
    """{path under third_party/: text} rendered from the templates in data/compat."""
    return {dest: compat_template(tpl) for tpl, dest in sorted(COMPAT.items())}


def check(lekmod_dir=None, repo_root=None, third_party_dir=None, err=None):
    """Verifies inputs.lock; returns 0, or 2 after printing the mismatch."""
    import sys
    lekmod_dir = lekmod_dir or default_lekmod_dir()
    if lekmod_dir is None:
        print("seed: lekmod_dir is not set: put it in civ5re.local.cfg ([seed] lekmod_dir) "
              "or env CIV5RE_SEED_LEKMOD_DIR (see civ5re.local.cfg.example)", file=err or sys.stderr)
        return 2
    try:
        verify(lekmod_dir, repo_root, third_party_dir=third_party_dir)
    except InputsError as e:
        print(f"seed: {e}", file=err or sys.stderr)
        return e.exit_code
    return 0
