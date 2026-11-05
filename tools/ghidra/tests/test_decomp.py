import json
import tempfile
import unittest
from pathlib import Path

from civ5re import config
from ghidra import cli, decomp as dc, headless as hl
from ghidra.tests.test_headless import Recorder, fake_ghidra, fake_repo

SYMS = [
    {"addr": "0x08000100", "demangled": "Foo::Foo()", "mangled": "_ZN3FooC1Ev", "scope": "Foo", "type": "func",
     "section": ".text"},
    {"addr": "0x08000100", "demangled": "Foo::Foo()", "mangled": "_ZN3FooC2Ev", "scope": "Foo", "type": "func",
     "section": ".text"},
    {"addr": "0x08000200", "demangled": "Foo::Bar(int)", "mangled": "_ZN3Foo3BarEi", "scope": "Foo",
     "type": "func", "section": ".text"},
    {"addr": "0x08000300", "demangled": "FooBar::Baz()", "mangled": "_ZN6FooBar3BazEv", "scope": "FooBar",
     "type": "func", "section": ".text"},
    {"addr": "0x08000400", "demangled": "Foo::s_x", "mangled": "_ZN3Foo3s_xE", "scope": "Foo", "type": "object",
     "section": ".data"},
    {"addr": "0x00000000", "demangled": "Foo::Imp()", "mangled": "imp", "scope": "Foo", "type": "func",
     "section": "UND"},
]


class Select(unittest.TestCase):
    def test_hex_forms(self):
        self.assertEqual(dc.select(SYMS, ["0x893170c", "0893170C"]), ["0893170c"])

    def test_exact_name_beats_regex(self):
        self.assertEqual(dc.select(SYMS, ["Foo::Bar"]), ["08000200"])
        self.assertEqual(dc.select(SYMS, ["Foo::Foo()"]), ["08000100"])

    def test_regex(self):
        self.assertEqual(dc.select(SYMS, [r"^Foo(Bar)?::\w+\("]), ["08000100", "08000200", "08000300"])

    def test_class_is_exact_and_funcs_only(self):
        self.assertEqual(dc.select(SYMS, classes=["Foo"]), ["08000100", "08000200"])

    def test_unmatched_selectors_are_listed(self):
        with self.assertRaises(dc.SelectError) as e:
            dc.select(SYMS, ["Nope::x"], ["Nada"])
        self.assertIn("Nope::x", str(e.exception))
        self.assertIn("--class Nada", str(e.exception))

    def test_bad_regex(self):
        with self.assertRaises(dc.SelectError):
            dc.select(SYMS, ["(["])

    def test_list_file(self):
        with tempfile.TemporaryDirectory() as t:
            p = Path(t) / "l.txt"
            p.write_text("# c\n\n0x08000100\n  Foo::Bar \n", encoding="utf-8")
            self.assertEqual(dc.read_list(p), ["0x08000100", "Foo::Bar"])


class Freshness(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.d = Path(self.tmp.name)

    def test_index_roundtrip_and_split(self):
        dc.write_index(self.d, {"08000200": ("B", "0x10", "s1", dc.FORMAT), "08000100": ("A", "0x20", "s2", dc.FORMAT)})
        self.assertEqual((self.d / "index.tsv").read_text().splitlines()[1].split("\t")[0], "08000100")
        (self.d / "08000100.txt").write_text("x")
        (self.d / "08000200.txt").write_text("x")
        todo, fresh = dc.split_fresh(self.d, ["08000100", "08000200", "08000300"], "s2")
        self.assertEqual(fresh, ["08000100"])
        self.assertEqual(todo, ["08000200", "08000300"])

    def test_missing_file_or_force_or_unknown_plan(self):
        dc.write_index(self.d, {"08000100": ("A", "0x20", "s2", dc.FORMAT)})
        self.assertEqual(dc.split_fresh(self.d, ["08000100"], "s2")[0], ["08000100"])
        (self.d / "08000100.txt").write_text("x")
        self.assertEqual(dc.split_fresh(self.d, ["08000100"], "s2", force=True)[0], ["08000100"])
        self.assertEqual(dc.split_fresh(self.d, ["08000100"], None)[0], ["08000100"])

    def test_old_format_is_stale(self):
        (self.d / "index.tsv").write_text("addr\tname\tsize\tplan_sha256\n08000100\tA\t0x20\ts2\n")
        (self.d / "08000100.txt").write_text("x")
        self.assertEqual(dc.read_index(self.d)["08000100"][3], "1")
        self.assertEqual(dc.split_fresh(self.d, ["08000100"], "s2"), (["08000100"], []))
        dc.write_index(self.d, {"08000100": ("A", "0x20", "s2", "1")})
        self.assertEqual(dc.split_fresh(self.d, ["08000100"], "s2")[0], ["08000100"])

    def test_request_carries_the_symbol_size(self):
        syms = SYMS + [dict(SYMS[0], mangled="x", size=99), dict(SYMS[2], size=0)]
        sizes = dc.symbol_sizes(syms)
        self.assertEqual(sizes, {"08000100": 99})
        dc.write_request(self.d / "r.txt", ["08000100", "08000200"], sizes)
        self.assertEqual((self.d / "r.txt").read_text(), "0x08000100 0x63\n0x08000200\n")

    def test_merge_run(self):
        (self.d / ".run.tsv").write_text("08000100\tFoo::Foo\t420\tok\n08000999\t-\t0\tno function\n")
        dumped, missing = dc.merge_run(self.d, dc.read_run(self.d), "sha")
        self.assertEqual((dumped, missing), (1, ["08000999"]))
        self.assertEqual(dc.read_index(self.d), {"08000100": ("Foo::Foo", "0x1a4", "sha", dc.FORMAT)})


class Command(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        t = Path(self.tmp.name)
        self.g = fake_ghidra(t)
        self.r = fake_repo(t)
        self.proj = t / "proj"
        self.proj.mkdir()
        (self.proj / "civ5re.gpr").write_text("g")
        (self.proj / "civ5re.rep").mkdir()
        (self.r / "build" / "ghidra").mkdir(parents=True)
        (self.r / "build" / "binfacts.json").write_text(json.dumps({"symbols": SYMS}))
        (self.r / "build" / "ghidra" / "last-apply.json").write_text(json.dumps({"applied_plan_sha256": "P1"}))
        (self.r / "civ5re.cfg").write_text("[ghidra]\nheap = 4G\nmcp_port = 8089\n")
        (self.r / "civ5re.local.cfg").write_text(f"ghidra_dir = {self.g}\nghidra_project_dir = {self.proj}\n")
        self.rec = Recorder()
        self.ctx = cli.Ctx(config.load(root=self.r, environ={}),
                           headless_factory=lambda c: hl.Headless(self.g, "4G", self.r, c.logs,
                                                                  runner=self.fake_run))
        self.lines = []

    def fake_run(self, argv, env, logfile):
        """What Civ5reDecomp would do: dump every requested address."""
        self.rec.calls.append((argv, env))
        arg = {a.split("=", 1)[0]: a.split("=", 1)[1] for a in argv if a.startswith("--")}
        out = Path(arg["--out"])
        rows = ""
        for line in Path(arg["--in"]).read_text().splitlines():
            a = line.split()[0][2:]
            (out / f"{a}.txt").write_text(f"# 0x{a}\n## asm\n## c\n")
            rows += f"{a}\tN{a}\t16\tok\n"
        (out / ".run.tsv").write_text(rows)
        return 0

    def run_cmd(self, sels, classes=(), force=False, **kw):
        self.lines.clear()
        return cli.run_decomp(self.ctx, sels, list(classes), force, self.lines.append, **kw)

    def test_dump_then_fresh_then_stale(self):
        self.assertEqual(self.run_cmd([], ["Foo"]), 0)
        self.assertRegex(self.lines[-1], r"^decomp: 2 dumped, 0 fresh \[")
        argv = self.rec.calls[0][0]
        self.assertIn("-readOnly", argv)
        self.assertTrue((self.r / "build" / "decomp" / "08000100.txt").is_file())
        self.run_cmd([], ["Foo"])
        self.assertRegex(self.lines[-1], r"^decomp: 0 dumped, 2 fresh")
        self.assertEqual(len(self.rec.calls), 1)
        self.run_cmd([], ["Foo"], force=True)
        self.assertRegex(self.lines[-1], r"^decomp: 2 dumped, 0 fresh")
        (self.r / "build" / "ghidra" / "last-apply.json").write_text(json.dumps({"applied_plan_sha256": "P2"}))
        self.run_cmd(["0x08000100"])
        self.assertRegex(self.lines[-1], r"^decomp: 1 dumped, 0 fresh")
        self.assertEqual(dc.read_index(self.r / "build" / "decomp")["08000100"][2], "P2")

    def test_locked_project_waits_then_exits_2(self):
        (self.proj / "civ5re.lock").write_text("")
        sleeps = []
        with self.assertRaises(cli.InputError) as e:
            self.run_cmd(["Foo::Bar"], lock_wait=3, sleep=sleeps.append)
        self.assertEqual(len(sleeps), 3)
        self.assertIn("locked", str(e.exception))
        self.assertEqual(self.rec.calls, [])

    def test_lock_released_while_waiting(self):
        lock = self.proj / "civ5re.lock"
        lock.write_text("")
        self.assertEqual(self.run_cmd(["Foo::Bar"], lock_wait=5, sleep=lambda s: lock.unlink()), 0)

    def test_nothing_selected(self):
        with self.assertRaises(cli.InputError):
            self.run_cmd([])


if __name__ == "__main__":
    unittest.main()
