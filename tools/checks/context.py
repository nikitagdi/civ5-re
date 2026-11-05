"""Run settings and the Context handed to rules; inputs are loaded lazily and failures are cached."""
import bisect
import dataclasses
import hashlib
import json
import os
import subprocess
import time
from pathlib import Path

from . import includes
from .api import InputError

WAIVERS_REL = "tools/checks/waivers.json"
EVIDENCE_FILES = {
    "source_paths": "tools/seed/data/source_paths_unique.txt",
    "sdk_lib_paths": "tools/seed/data/sdk_lib_paths.txt",
    "sdk_map": "tools/seed/data/sdk_map.txt",
}
_SKIP_DIRS = {".git", "build", "__pycache__", ".venv"}


@dataclasses.dataclass
class Settings:
    repo: Path
    cfg: object = None
    mode: str = "full"                  # full | staged | files | root | report
    root: Path = None                   # --root DIR
    files: list = dataclasses.field(default_factory=list)
    only: list = None
    skip: list = dataclasses.field(default_factory=list)
    json: bool = False
    jobs: int = 0
    build: Path = None                  # default <repo>/build
    typedb_build: Path = None           # default <build> (full/staged) or <build>/checks/root/<key> (root)
    world: dict = None                  # selftest: {binfacts, ghidra|False, source_paths, sdk_lib_paths, sdk_map}
    waivers: object = "default"         # "default" | Path | None
    export_coverage: str = None
    clang_prefix: list = None           # tests: command that stands in for clang++
    write_last: bool = True
    selftest: bool = True

    def __post_init__(self):
        self.repo = Path(self.repo)
        self.build = Path(self.build) if self.build else self.repo / "build"
        self.root = Path(self.root).resolve() if self.root else None

    @property
    def out(self):
        return self.build / "checks"


class Waiver:
    def __init__(self, rule, key, reason, line):
        self.rule, self.key, self.reason, self.line = rule, key, reason, line
        self.matched = False
        self.stale = False

    @property
    def valid(self):
        return bool(self.reason)


def _entry_lines(text):
    lines, line, depth, in_str, esc = [], 1, 0, False, False
    for ch in text:
        if ch == "\n":
            line += 1
        if in_str:
            if esc:
                esc = False
            elif ch == "\\":
                esc = True
            elif ch == '"':
                in_str = False
            continue
        if ch == '"':
            in_str = True
        elif ch in "[{":
            depth += 1
            if ch == "{" and depth == 2:
                lines.append(line)
        elif ch in "]}":
            depth -= 1
    return lines


def parse_waivers(text, name=WAIVERS_REL):
    try:
        data = json.loads(text)
    except ValueError as e:
        raise InputError(name, f"invalid JSON: {e}") from None
    if not isinstance(data, list):
        raise InputError(name, "must be a JSON list of {rule, key, reason}")
    lines = _entry_lines(text)
    out = []
    for i, e in enumerate(data):
        if not isinstance(e, dict) or not isinstance(e.get("rule"), str) or not isinstance(e.get("key"), str) \
                or not isinstance(e.get("reason", ""), str):
            raise InputError(name, f"entry {i + 1} must be an object with string rule, key and reason")
        out.append(Waiver(e["rule"], e["key"], e.get("reason", "").strip(), lines[i] if i < len(lines) else 1))
    return out


class GhidraList:
    def __init__(self, path, sha256, rows):
        self.path, self.sha256, self.rows = path, sha256, rows
        self.starts = [r[0] for r in rows]
        self.by_start = {r[0]: r for r in rows}

    def fastcc(self, start):
        r = self.by_start.get(start)
        return bool(r) and any(f.startswith("r=") for f in r[3].split())

    def containing(self, addr):
        i = bisect.bisect_right(self.starts, addr) - 1
        if i >= 0 and addr < self.rows[i][1]:
            return self.rows[i]
        return None


def parse_ghidra(text, name="build/ghidra-functions.txt"):
    sha, version, rows = None, None, []
    for n, raw in enumerate(text.splitlines(), 1):
        if raw.startswith("#"):
            body = raw[1:].strip()
            if body.startswith("civ5re ghidra-functions"):
                version = body.split()[-1]
            for tok in body.split():
                if tok.startswith("binary_sha256="):
                    sha = tok.split("=", 1)[1].lower()
            continue
        if not raw.strip():
            continue
        f = raw.split("\t")
        if len(f) < 3:
            raise InputError(name, f"line {n}: expected 'start end name flags'")
        try:
            rows.append((int(f[0], 16), int(f[1], 16), f[2], f[3] if len(f) > 3 else ""))
        except ValueError:
            raise InputError(name, f"line {n}: bad address") from None
    if version != "2":
        raise InputError(name, f"unsupported format version {version!r} (need 'civ5re ghidra-functions 2')")
    rows.sort()
    return GhidraList(name, sha, rows)


class Context:
    TREE = includes.TREE

    def __init__(self, settings):
        self.settings = settings
        self.repo = settings.repo
        self.mode = settings.mode
        self.base = settings.root or settings.repo       # directory the relative paths refer to
        self.tree_root = self.base / "Civ5Expansion2"
        self.scope = None                                 # set by the runner per rule: None = everything
        self.timings = {}
        self._cache = {}
        self._text = {}

    def _lazy(self, name, loader):
        if name not in self._cache:
            t = time.perf_counter()
            try:
                self._cache[name] = (True, loader())
            except InputError as e:
                self._cache[name] = (False, e)
            self.timings[name] = time.perf_counter() - t
        ok, v = self._cache[name]
        if not ok:
            raise v
        return v

    @property
    def cfg(self):
        return self.settings.cfg

    @property
    def jobs(self):
        n = self.settings.jobs
        if not n and self.cfg is not None:
            n = int(self.cfg.get("checks", "jobs") or 0)
        return n or os.cpu_count() or 1

    @property
    def export_coverage(self):
        return self.settings.export_coverage or (
            self.cfg.get("checks", "export_coverage") if self.cfg is not None else "report")

    def require(self, need):
        getattr(self, {"files": "files", "typedb": "typedb", "binfacts": "binfacts",
                       "clang": "clang", "ghidra": "ghidra"}[need])

    @property
    def files(self):
        return self._lazy("files", self._list_files)

    def _list_files(self):
        if self.settings.root:
            out = []
            for d, dirs, names in os.walk(self.base):
                dirs[:] = sorted(x for x in dirs if x not in _SKIP_DIRS)
                for n in names:
                    out.append(Path(d, n).relative_to(self.base).as_posix())
            return sorted(out)
        try:
            r = subprocess.run(["git", "ls-files", "-co", "--exclude-standard", "-z"], cwd=self.repo,
                               capture_output=True)
        except OSError as e:
            raise InputError("git", f"not available: {e}") from None
        if r.returncode != 0:
            raise InputError("git", r.stderr.decode("utf-8", "replace").strip() or "git ls-files failed")
        paths = set(p for p in r.stdout.decode("utf-8", "surrogateescape").split("\0") if p)
        return sorted(p for p in paths if (self.repo / p).is_file())

    @property
    def fileset(self):
        return self._lazy("fileset", lambda: set(self.files))

    @property
    def tree_files(self):
        return [p for p in self.files if includes.is_tree_source(p)]

    def in_scope(self, path):
        return self.scope is None or path in self.scope

    def scoped(self, pred=None):
        """Repo paths of the files this rule has to look at (staged/files: the affected ones)."""
        return [p for p in self.files if self.in_scope(p) and (pred is None or pred(p))]

    def abspath(self, path):
        return self.base / path

    def read(self, path):
        if path not in self._text:
            f = self.base / path
            if not f.is_file() and path.startswith(includes.THIRD):
                f = self.repo / path             # --root without its own third_party/
            self._text[path] = f.read_bytes().decode("utf-8-sig", "replace")
        return self._text[path]

    @property
    def third_party(self):
        p = self.base / "third_party"
        return p if p.is_dir() else self.repo / "third_party"

    def _third_dirs(self):
        f = self.third_party / "include_dirs.txt"
        if not f.is_file():
            return ()
        out = []
        for line in f.read_text(encoding="utf-8", errors="replace").splitlines():
            line = line.strip()
            if line and not line.startswith("#") and line != "<clang>":
                out.append(line)
        return tuple(out)

    @property
    def includes(self):
        def exists(c):
            if c in self.fileset:
                return True
            # --root without its own third_party/: the repository's one, as for the real tree
            return c.startswith(includes.THIRD) and self.third_party != self.base / "third_party"                 and (self.repo / c).is_file()
        return self._lazy("includes", lambda: includes.build(self.files, self.read, exists, self._third_dirs()))

    @property
    def waivers(self):
        def load():
            w = self.settings.waivers
            if w is None:
                return []
            p = (self.base / WAIVERS_REL) if w == "default" else Path(w)
            if w == "default" and not p.is_file():
                p = self.repo / WAIVERS_REL
            if not p.is_file():
                return []
            return parse_waivers(p.read_text(encoding="utf-8-sig"))
        return self._lazy("waivers", load)

    def evidence(self, name):
        """Lines of source_paths | sdk_lib_paths | sdk_map (the world's copy under selftest)."""
        def load():
            w = (self.settings.world or {}).get(name)
            p = Path(w) if w else self.repo / EVIDENCE_FILES[name]
            if not p.is_file():
                raise InputError(EVIDENCE_FILES[name], "file not found")
            return p.read_text(encoding="utf-8-sig").splitlines()
        return self._lazy("evidence:" + name, load)

    @property
    def binfacts(self):
        def load():
            try:
                from binfacts import api
            except ImportError as e:
                raise InputError("binfacts", f"module is not available ({e})") from None
            path = (self.settings.world or {}).get("binfacts")
            try:
                return api.load(str(path)) if path else api.load()
            except AttributeError:
                raise InputError("binfacts", "binfacts.api.load is not available yet") from None
            except (OSError, ValueError) as e:
                raise InputError("binfacts", f"cannot read: {e}") from None
            except Exception as e:
                if type(e).__name__ == "BinfactsError":
                    raise InputError("binfacts", str(e)) from None
                raise
        return self._lazy("binfacts", load)

    @property
    def typedb(self):
        def load():
            try:
                from typedb import api, build
            except ImportError as e:
                raise InputError("typedb", f"module is not available ({e})") from None
            tp = self.base / "third_party"
            try:
                path = build.build_db(self.tree_root, self.typedb_build, markers="strict",
                                      third_party=tp if tp.is_dir() else None)
                return api.load(path)
            except (AttributeError, TypeError) as e:
                raise InputError("typedb", f"typedb API mismatch: {e}") from None
            except Exception as e:
                raise InputError("typedb", f"{type(e).__name__}: {e}") from None
        return self._lazy("typedb", load)

    @property
    def typedb_build(self):
        s = self.settings
        if s.typedb_build:
            return Path(s.typedb_build)
        if s.root:
            key = hashlib.sha1(str(s.root).encode("utf-8")).hexdigest()[:12]
            return s.build / "checks" / "root" / key
        return s.build

    @property
    def clang(self):
        """(exe, version) of clang++; InputError when LLVM is missing."""
        def load():
            exe = None
            if self.settings.clang_prefix:
                exe = self.settings.clang_prefix
            elif self.cfg is not None:
                try:
                    exe = [str(self.cfg.tool("clang++"))]
                except Exception as e:
                    raise InputError("llvm_dir", str(e)) from None
            if not exe or (not self.settings.clang_prefix and not Path(exe[0]).is_file()):
                raise InputError("clang", f"clang++ not found ({exe[0] if exe else 'llvm_dir is not configured'})")
            try:
                r = subprocess.run([*exe, "--version"], capture_output=True, text=True, timeout=60)
            except (OSError, subprocess.TimeoutExpired) as e:
                raise InputError("clang", f"cannot run {exe[0]}: {e}") from None
            first = (r.stdout.strip().splitlines() or ["unknown"])[0]
            return exe, first
        return self._lazy("clang", load)

    def clang_args(self, mode="alone"):
        try:
            from typedb import api
        except ImportError as e:
            raise InputError("typedb", f"module is not available ({e})") from None
        try:
            return list(api.clang_args(mode, tree=self.tree_root, third_party=self.third_party,
                                       build=self.typedb_build))
        except AttributeError:
            raise InputError("typedb", "typedb.api.clang_args is not available yet") from None

    @property
    def ghidra(self):
        """GhidraList or None when there is no list; a list of another binary is an InputError."""
        def load():
            w = self.settings.world
            if w is not None:
                p = w.get("ghidra")
                expected = None
            else:
                p = self.settings.build / "ghidra-functions.txt"
                expected = self._target_sha()
            if not p or not Path(p).is_file():
                return None
            g = parse_ghidra(Path(p).read_text(encoding="utf-8-sig"))
            if expected and g.sha256 and g.sha256 != expected:
                raise InputError("build/ghidra-functions.txt",
                                 f"binary_sha256 {g.sha256[:12]} differs from bins.lock {expected[:12]}: "
                                 "run civ5re ghidra functions")
            return g
        return self._lazy("ghidra", load)

    def _target_sha(self):
        from civ5re import binlock
        try:
            return binlock.target(binlock.load(self.repo / "bins.lock")).sha256
        except (binlock.BinlockError, OSError) as e:
            raise InputError("bins.lock", str(e)) from None

    def inputs_info(self, plugins):
        info = {"binfacts_sha256": None, "typedb": None, "ghidra_functions": None, "clang": None,
                "plugins": sorted(plugins)}
        for name in ("binfacts", "typedb", "ghidra", "clang"):
            if name in self._cache and self._cache[name][0]:
                v = self._cache[name][1]
                if name == "binfacts":
                    info["binfacts_sha256"] = (v.get("binary") or {}).get("sha256")
                elif name == "typedb":
                    info["typedb"] = {"ok": v.get("ok"), "tree_sha": (v.get("inputs") or {}).get("tree_sha")}
                elif name == "ghidra" and v is not None:
                    info["ghidra_functions"] = v.path if isinstance(v.path, str) else Path(v.path).name
                elif name == "clang":
                    info["clang"] = v[1]
        return info
