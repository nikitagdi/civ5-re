import argparse
import contextlib
import io
import json
import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from luaapi import cli  # noqa: E402
from luaapi.tests import synth  # noqa: E402


class Cfg:
    def __init__(self, root, game=None, out=None):
        self.root, self.game, self.out = Path(root), game, out

    def path(self, *key):
        return {("game_dir",): self.game, ("lua-api", "out"): self.out}[key]


def args(**kw):
    ns = dict(no_usage=False, out=None, show=None)
    ns.update(kw)
    return argparse.Namespace(**ns)


def run(cfg, **kw):
    out, err = io.StringIO(), io.StringIO()
    with contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
        code = cli.execute(cfg, args(**kw))
    return code, out.getvalue(), err.getvalue()


def tree_bytes(base):
    return {p.relative_to(base).as_posix(): p.read_bytes() for p in sorted(Path(base).rglob("*")) if p.is_file()}


class Cli(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.root = Path(self.tmp.name) / "repo"
        (self.root / "build").mkdir(parents=True)
        self.addCleanup(self.tmp.cleanup)

    def put_inputs(self, binfacts=True):
        (self.root / "build" / "typedb.json").write_text(json.dumps(synth.typedb()), encoding="utf-8")
        if binfacts:
            facts = dict(synth.binfacts(), schema=1)
            (self.root / "build" / "binfacts.json").write_text(json.dumps(facts), encoding="utf-8")

    def test_register(self):
        p = argparse.ArgumentParser()
        sub = p.add_subparsers()
        cli.register(sub)
        ns = p.parse_args(["lua-api", "--no-usage", "--out", "x", "--show", "UI.A"])
        self.assertEqual((ns.no_usage, ns.out, ns.show), (True, "x", "UI.A"))

    def test_missing_typedb_exits_2_with_hint(self):
        code, _, err = run(Cfg(self.root))
        self.assertEqual(code, 2)
        self.assertIn("civ5re typedb", err)

    def test_writes_build_outputs(self):
        self.put_inputs()
        code, out, _ = run(Cfg(self.root), no_usage=True)
        self.assertIn(code, (0, 1))
        b = self.root / "build"
        self.assertTrue((b / "lua-api.json").is_file())
        self.assertTrue((b / "lua-api-coverage.txt").is_file())
        self.assertTrue((b / "lua-stubs" / "tables" / "UI.lua").is_file())
        self.assertIn("usage scan skipped (--no-usage)", out)
        doc = json.loads((b / "lua-api.json").read_text(encoding="utf-8"))
        self.assertEqual(doc["schema"], "civ5re-lua-api/1")
        self.assertFalse(doc["inputs"]["game_scanned"])

    def test_no_binfacts_is_partial_not_a_failure(self):
        self.put_inputs(binfacts=False)
        code, out, _ = run(Cfg(self.root), no_usage=True)
        self.assertEqual(code, 0)
        self.assertIn("partial", out)
        self.assertTrue(json.loads((self.root / "build" / "lua-api.json").read_text("utf-8"))["coverage"]["partial"])

    def test_repeat_is_byte_identical_and_copy_matches(self):
        self.put_inputs()
        copy = Path(self.tmp.name) / "copy"
        run(Cfg(self.root), no_usage=True, out=str(copy))
        first = tree_bytes(self.root / "build")
        run(Cfg(self.root), no_usage=True, out=str(copy))
        self.assertEqual(first, tree_bytes(self.root / "build"))
        wanted = {k: v for k, v in first.items() if k.startswith("lua-")}
        self.assertEqual(wanted, tree_bytes(copy))

    def test_config_out_is_used_and_stale_stubs_removed(self):
        self.put_inputs()
        copy = Path(self.tmp.name) / "copy"
        (copy / "lua-stubs").mkdir(parents=True)
        (copy / "lua-stubs" / "old.lua").write_text("x", encoding="utf-8")
        run(Cfg(self.root, out=copy), no_usage=True)
        self.assertTrue((copy / "lua-api.json").is_file())
        self.assertFalse((copy / "lua-stubs" / "old.lua").exists())

    def test_usage_scan_with_game_dir(self):
        self.put_inputs()
        game = Path(self.tmp.name) / "game"
        (game / "Assets").mkdir(parents=True)
        (game / "Assets" / "a.lua").write_text("UI.GetHeadSelectedUnit()\nUI.Nope()\n", encoding="utf-8")
        code, out, _ = run(Cfg(self.root, game=game))
        self.assertIn("scanned 1 game Lua files", out)
        doc = json.loads((self.root / "build" / "lua-api.json").read_text("utf-8"))
        self.assertTrue(doc["inputs"]["game_scanned"])
        self.assertIn("undeclared  UI.Nope", (self.root / "build" / "lua-api-coverage.txt").read_text("utf-8"))

    def test_empty_game_dir_skips_usage(self):
        self.put_inputs()
        _, out, _ = run(Cfg(self.root, game=None))
        self.assertIn("game_dir is empty or missing", out)

    def test_show(self):
        self.put_inputs()
        code, out, _ = run(Cfg(self.root), no_usage=True, show="UI.GetHeadSelectedUnit")
        self.assertEqual(code, 0)
        self.assertIn("/// lua UI.GetHeadSelectedUnit() -> Unit?", out)
        self.assertIn("0x089468e6", out)
        self.assertEqual(run(Cfg(self.root), no_usage=True, show="UI.Nope")[0], 1)
        self.assertFalse((self.root / "build" / "lua-api.json").exists())

    def test_error_diagnostics_exit_1(self):
        db = synth.typedb()
        db["functions"][0]["markers"]["lua"][0]["text"] = "UI.GetHeadSelectedUnit("
        (self.root / "build" / "typedb.json").write_text(json.dumps(db), encoding="utf-8")
        code, out, _ = run(Cfg(self.root), no_usage=True)
        self.assertEqual(code, 1)
        self.assertIn("LUA1", out)


if __name__ == "__main__":
    unittest.main()
