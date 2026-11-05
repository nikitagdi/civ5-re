"""Public API of typedb: `load`, `index`, `clang_args` (the only source of clang flags for all modules)."""
from __future__ import annotations

import json
from pathlib import Path

from civ5re import config as _config
from typedb import compile as _cc
from typedb.compile import EnvError
from typedb.schema import SCHEMA, SchemaError

__all__ = ["load", "index", "clang_args", "build_db", "EnvError", "SchemaError", "SCHEMA", "default_path"]


def default_path():
    return _config.ROOT / "build" / "typedb.json"


def load(path=None, require_ok=False):
    p = Path(path) if path is not None else default_path()
    try:
        db = json.loads(p.read_text(encoding="utf-8"))
    except OSError as e:
        raise SchemaError(f"cannot read {p}: {e}") from None
    if db.get("schema") != SCHEMA:
        raise SchemaError(f"{p}: schema {db.get('schema')!r}, expected {SCHEMA}")
    if require_ok and not db.get("ok"):
        raise SchemaError(f"{p}: ok=false (typedb has error diagnostics)")
    return db


class Index:
    """Lookup by mangled name (incl. structor variants and instances), function key and `file:line`."""

    def __init__(self, db):
        self.db = db
        self.by_mangled = {}
        self.by_key = {}
        self.by_line = {}
        self.records = {r["name"]: r for r in db.get("records", ())}
        for f in db.get("functions", ()):
            self.by_key[f["key"]] = f
            names = [f.get("mangled")] + list((f.get("variants") or {}).values())
            for inst in f.get("instances", ()):
                names += [inst.get("mangled")] + list((inst.get("variants") or {}).values())
            for n in names:
                if n:
                    self.by_mangled.setdefault(n, f)
            self._line(f, f["file"], f["line"])
            for r in f.get("redecls", ()):
                self._line(f, r["file"], r["line"])
        for g in db.get("globals", ()):
            if g.get("mangled"):
                self.by_mangled.setdefault(g["mangled"], g)
            self._line(g, g["file"], g["line"])
        for coll in ("records", "enums", "typedefs", "cpp_defs"):
            for x in db.get(coll, ()):
                if x.get("file") and x.get("line"):
                    self._line(x, x["file"], x["line"])

    def _line(self, x, file, line):
        self.by_line.setdefault(f"{file}:{line}", []).append(x)

    def mangled(self, name):
        return self.by_mangled.get(name)

    def key(self, key):
        return self.by_key.get(key)

    def at(self, file, line=None):
        k = file if line is None else f"{file}:{line}"
        return self.by_line.get(k, [])

    def __getitem__(self, k):
        return self.by_key.get(k) or self.by_mangled.get(k) or (self.by_line.get(k) or [None])[0]


def index(db):
    return Index(db)


def clang_args(mode, *, tree=None, third_party=None, build=None, pch=None):
    """Flags for `mode`: pch (P0), tree (passes A/B), cpp (pass C), alone (checks B1/B2).

    `pch` (tree and cpp only) replaces the precompiled header the mode loads, for callers that precompile a
    larger unit themselves (seed's layout probes); the other flags are unchanged.

    `tree` defaults to the repo's Civ5Expansion2/, `build` to the repo's build/, `third_party` to the repo's
    third_party/. No source file or output is appended except in `pch` (which builds third_party.pch).
    """
    env = _cc.load_env(third_party)
    if tree is None and mode != "pch":
        tree = _config.ROOT / "Civ5Expansion2"
    if build is None:
        build = _config.ROOT / "build"
    return _cc.clang_args(mode, env, tree=tree, build=build, pch=pch)


def build_db(tree_root, build_dir, markers="strict", third_party=None, **kw):
    """Re-export of `typedb.build.build_db` → path to the JSON."""
    from typedb.build import build_db as _build_db
    return _build_db(tree_root, build_dir, markers, third_party, **kw)
