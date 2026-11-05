import functools
import json
import shutil
import tempfile
import unittest
from pathlib import Path

HERE = Path(__file__).resolve().parent
FIXTURES = HERE / "fixtures"
THIRD_PARTY = FIXTURES / "third_party"


@functools.lru_cache(maxsize=None)
def llvm_missing():
    from typedb import compile as cc
    try:
        cc.tools()
    except Exception as e:  # noqa: BLE001 - any config/tool problem means "skip live tests"
        return str(e)
    return None


def needs_llvm(cls):
    reason = llvm_missing()
    return unittest.skipIf(reason is not None, f"LLVM not available: {reason}")(cls)


_TMP = None


def tmp_root():
    global _TMP
    if _TMP is None:
        _TMP = Path(tempfile.mkdtemp(prefix="typedb-tests-"))
        import atexit
        atexit.register(shutil.rmtree, _TMP, True)
    return _TMP


def fixture_tree(name):
    p = FIXTURES / name
    if not p.is_dir():
        p = FIXTURES / "diag" / name
    return p / "Civ5Expansion2"


@functools.lru_cache(maxsize=None)
def built(name, markers="strict"):
    """Build a fixture once per test run; returns (path, db)."""
    from typedb.build import build_db
    out = build_db(fixture_tree(name), tmp_root() / f"{name}-{markers}", markers=markers, third_party=THIRD_PARTY)
    return out, json.loads(out.read_text(encoding="utf-8"))
