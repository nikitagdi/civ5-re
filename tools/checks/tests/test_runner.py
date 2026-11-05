import json
import re
import sys
import unittest
from unittest import mock

from checks import output, plugins, runner, selftest
from checks.api import Finding, InputError, Rule, RuleSkip

from .helpers import H, TmpCase, write

LINE_RE = re.compile(r"^[^:]+:\d+: [A-Z]+\d+ ")
BUILTIN = ("T1", "T2", "T3", "W0")


def only_loaded(rules, modules=()):
    loaded = plugins.Loaded()
    loaded.rules, loaded.modules = list(rules), list(modules)
    return loaded


def builtin(repo, ids=BUILTIN):
    return [r for r in plugins.load_builtin(repo).rules if r.id in ids]


class Crashing(Rule):
    id = "Z1"
    title = "always raises"

    def run(self, ctx):
        raise RuntimeError("boom")


class NeedsBinfacts(Rule):
    id = "Z2"
    title = "needs binfacts"
    needs = ("binfacts",)

    def run(self, ctx):
        return [self.finding("a.h", 1, str(len(ctx.binfacts)))]


class NeedsTypedb(Rule):
    id = "Z3"
    title = "needs typedb"
    needs = ("typedb",)

    def run(self, ctx):
        return []


class Skipping(Rule):
    id = "Z4"
    title = "skips itself"

    def run(self, ctx):
        raise RuleSkip("not applicable here")


class Garbage(Rule):
    id = "Z5"
    title = "returns a non-Finding"

    def run(self, ctx):
        return ["text"]


class Output(TmpCase):
    def setUp(self):
        super().setUp()
        write(self.dir, {"src/x.h": "", "Civ5Expansion2/A.h": H + '#include "nope.h"\n'})

    def result(self, **kw):
        return runner.run_rules(self.settings("root", root=self.dir, waivers=None, **kw), builtin(self.dir))

    def test_text_lines_and_summary(self):
        res = self.result()
        text = output.render_text(res)
        lines = text.splitlines()
        self.assertEqual(sorted(l for l in lines if LINE_RE.match(l)), sorted([
            "Civ5Expansion2/A.h:3: T3 include not found: \"nope.h\"", "src/x.h:1: T1 source outside the mirror"]))
        self.assertTrue(any(re.match(r"^rules 4 \+ plugins 0: ran 4 · error 2 · warn 0 · waivers 0 \(matched 0\) · ", l)
                            for l in lines), text)
        self.assertTrue(lines[-1].startswith("re-check: .venv\\Scripts\\python -m civ5re check --only T1,T3 --files "))

    def test_warning_reports_are_compact_and_errors_complete(self):
        report = Finding(rule="Z9", severity="warn", message="report", detail=[f"row {i}" for i in range(50)])
        error = Finding(rule="Z8", severity="error", path="a.h", line=1, message="bad", detail=[f"row {i}" for i in range(50)])
        lines = output.render_findings([report, error])
        self.assertEqual(sum(l.startswith("    row") for l in lines), 9 + 50)  # the head line takes one of 10
        self.assertEqual(sum(1 for l in lines if "Z9 [warn] 41 more output lines not shown" in l), 1)
        self.assertEqual(len(output.render_findings([report], limit=100)), 51)

    def test_json_keys(self):
        p = self.result().to_json()
        self.assertEqual(list(p), ["schema", "mode", "exit", "seconds", "inputs", "selftest", "rules", "findings", "counts"])
        self.assertEqual(p["schema"], "civ5re.check/1")
        self.assertEqual(set(p["inputs"]), {"binfacts_sha256", "typedb", "ghidra_functions", "clang", "plugins"})
        self.assertEqual(set(p["selftest"]), {"status", "fixtures", "failed"})
        self.assertEqual(set(p["counts"]), {"error", "warn", "waived", "waivers"})
        self.assertEqual(set(p["rules"][0]), {"id", "title", "plugin", "status", "reason", "seconds", "error", "warn"})
        self.assertEqual(set(p["findings"][0]), {"rule", "severity", "path", "line", "message", "code", "detail",
                                                 "entity", "mangled", "expected", "actual", "key", "waived"})
        json.dumps(p)

    def test_keys_are_stable_across_runs_and_line_shifts(self):
        a = {(f.rule, f.path): f.key for f in self.result().findings}
        write(self.dir, {"Civ5Expansion2/A.h": H + "// shifted\n\n" + '#include "nope.h"\n'})
        b = {(f.rule, f.path): f.key for f in self.result().findings}
        self.assertEqual(a, b)
        self.assertEqual(a[("T1", "src/x.h")], "T1:src/x.h")

    def test_duplicate_keys_get_a_counter(self):
        write(self.dir, {"Civ5Expansion2/A.h": H + '#include "nope.h"\n#include "nope.h"\n'})
        keys = sorted(f.key for f in self.result().findings if f.rule == "T3")
        self.assertEqual(keys, ["T3:Civ5Expansion2/A.h:missing:nope.h", "T3:Civ5Expansion2/A.h:missing:nope.h#2"])

    def test_waived_finding_is_not_an_error(self):
        wf = self.dir / "w.json"
        write(self.dir, {"w.json": json.dumps([{"rule": "T1", "key": "T1:src/x.h", "reason": "legacy"}])})
        res = runner.run_rules(self.settings("root", root=self.dir, waivers=wf), builtin(self.dir))
        self.assertEqual((res.counts["error"], res.counts["waived"], res.counts["waivers"], res.matched), (1, 1, 1, 1))
        self.assertEqual(res.exit, 1)
        self.assertNotIn("src/x.h", output.render_text(res))
        self.assertTrue([f for f in res.to_json()["findings"] if f["waived"]])

    def test_waiver_without_reason_is_a_w0_error_and_does_not_waive(self):
        wf = self.dir / "w.json"
        write(self.dir, {"w.json": '[\n  {"rule": "T1", "key": "T1:src/x.h"}\n]\n'})
        res = runner.run_rules(self.settings("root", root=self.dir, waivers=wf), builtin(self.dir))
        w0 = [f for f in res.findings if f.rule == "W0"]
        self.assertEqual([(f.path, f.line, f.severity) for f in w0], [("tools/checks/waivers.json", 2, "error")])
        self.assertEqual(res.counts["waived"], 0)

    def test_stale_waiver_is_a_warning_only_for_rules_that_ran(self):
        wf = self.dir / "w.json"
        write(self.dir, {"w.json": json.dumps([{"rule": "T1", "key": "T1:gone.h", "reason": "x"},
                                               {"rule": "B1", "key": "B1:gone.h", "reason": "x"}])})
        res = runner.run_rules(self.settings("root", root=self.dir, waivers=wf), builtin(self.dir))
        w0 = [f for f in res.findings if f.rule == "W0"]
        self.assertEqual([f.severity for f in w0], ["warn"])
        self.assertIn("T1:gone.h", w0[0].message)

    def test_broken_waivers_file_is_exit_2(self):
        wf = self.dir / "w.json"
        write(self.dir, {"w.json": "{not json"})
        res = runner.run_rules(self.settings("root", root=self.dir, waivers=wf), builtin(self.dir))
        self.assertEqual(res.exit, 2)
        self.assertTrue(any(f.rule == "input" for f in res.findings))

    def test_only_and_skip(self):
        res = self.result(only=["T1"])
        self.assertEqual([r["id"] for r in res.rules], ["T1"])
        res = self.result(skip=["T1", "W0"])
        self.assertEqual([r["id"] for r in res.rules], ["T2", "T3"])
        res = self.result(only=["X9"])
        self.assertEqual(res.exit, 2)
        self.assertIn("unknown rule id X9", res.findings[0].message)


class Failures(TmpCase):
    def run_with(self, rules, **kw):
        write(self.dir, {"a.txt": ""})
        return runner.run_rules(self.settings("root", root=self.dir, waivers=None, **kw), rules)

    def test_crashed_rule_is_a_finding_for_that_rule_and_exit_2(self):
        res = self.run_with([Crashing()])
        self.assertEqual(res.exit, 2)
        f = res.findings[0]
        self.assertEqual((f.rule, f.message), ("Z1", "rule crashed"))
        self.assertTrue(any("boom" in d for d in f.detail))
        self.assertIn("civ5re: Z1 rule crashed", output.render_text(res))
        self.assertEqual(res.rules[0]["reason"], "rule crashed")

    def test_crash_does_not_stop_other_rules(self):
        res = self.run_with([Crashing(), *builtin(self.dir, ("T1",))])
        self.assertEqual({r["id"]: r["status"] for r in res.rules}, {"T1": "ran", "Z1": "ran"})

    def test_non_finding_return_is_a_crash(self):
        self.assertEqual(self.run_with([Garbage()]).findings[0].message, "rule crashed")

    def test_missing_binfacts_is_exit_2_naming_the_input(self):
        res = self.run_with([NeedsBinfacts()], world={"binfacts": self.dir / "missing.json"})
        self.assertEqual(res.exit, 2)
        self.assertEqual(res.rules[0]["status"], "skipped")
        self.assertIn("binfacts", res.rules[0]["reason"])
        text = output.render_text(res)
        self.assertIn("civ5re: Z2 skipped: input binfacts unavailable", text)
        self.assertIn("civ5re: input binfacts:", text)

    def test_missing_typedb_module_is_exit_2_naming_the_input(self):
        with mock.patch.dict(sys.modules, {"typedb.api": None, "typedb.build": None}):
            res = self.run_with([NeedsTypedb()])
        self.assertEqual(res.exit, 2)
        self.assertIn("input typedb unavailable", res.rules[0]["reason"])

    def test_input_is_reported_once_for_several_rules(self):
        class Again(NeedsBinfacts):
            id = "Z6"
        res = self.run_with([NeedsBinfacts(), Again()], world={"binfacts": self.dir / "missing.json"})
        self.assertEqual(len([f for f in res.findings if f.rule == "input"]), 1)

    def test_rule_skip_is_not_an_error(self):
        res = self.run_with([Skipping()])
        self.assertEqual(res.exit, 0)
        self.assertEqual((res.rules[0]["status"], res.rules[0]["reason"]), ("skipped", "not applicable here"))
        self.assertIn("civ5re: Z4 skipped: not applicable here", output.render_text(res))

    def test_findings_for_other_rules_cannot_be_forged(self):
        class Liar(Rule):
            id = "Z7"
            title = "t"

            def run(self, ctx):
                return [Finding(rule="T1", message="m", path="a.txt", line=0)]
        f = self.run_with([Liar()]).findings[0]
        self.assertEqual((f.rule, f.line), ("Z7", 1))


class ExitPriority(unittest.TestCase):
    def test_table(self):
        table = {
            (True, True, True): 2, (True, False, False): 2, (False, True, True): 3, (False, True, False): 3,
            (False, False, True): 1, (False, False, False): 0,
        }
        for args, want in table.items():
            self.assertEqual(runner.exit_code(*args), want, args)


class PluginsAndSelftest(TmpCase):
    PLUGIN = (
        "from checks.api import Rule\n"
        "class {cls}(Rule):\n"
        "    id = '{id}'\n"
        "    title = 'flags bad.txt'\n"
        "    needs = ('files',)\n"
        "    def run(self, ctx):\n"
        "        return [self.finding(p, 1, 'bad file') for p in ctx.scoped(lambda p: p.endswith('bad.txt'))]\n")

    def plugin(self, module="zed", id="Z1", text=None):
        write(self.dir, {f"tools/{module}/checkrules.py": text or self.PLUGIN.format(cls=id, id=id)})

    def test_plugin_rule_is_listed_and_run(self):
        self.plugin()
        loaded = plugins.load_all(self.dir)
        self.assertEqual(loaded.errors, [])
        z = [r for r in loaded.rules if r.id == "Z1"][0]
        self.assertEqual(z.plugin, "zed")
        write(self.dir, {"bad.txt": ""})
        res = runner.run_rules(self.settings("root", root=self.dir, waivers=None, only=["Z1"]), loaded.rules,
                               plugin_modules=loaded.modules)
        self.assertEqual([(f.rule, f.path) for f in res.findings], [("Z1", "bad.txt")])
        self.assertEqual(res.inputs["plugins"], ["zed"])
        self.assertEqual(res.rules[0]["plugin"], "zed")

    def test_duplicate_id_builtin_vs_plugin_is_exit_2(self):
        self.plugin(id="T1")
        loaded = plugins.load_all(self.dir)
        self.assertTrue(any("duplicate rule id T1" in e for e in loaded.errors), loaded.errors)
        self.assertEqual(sum(1 for r in loaded.rules if r.id == "T1"), 1)
        res = runner.run_check(self.settings("root", root=self.dir, waivers=None, only=["T1"]))
        self.assertEqual(res.exit, 2)
        self.assertIn("duplicate rule id T1", output.render_text(res))

    def test_two_plugins_with_one_id(self):
        self.plugin("a")
        self.plugin("b")
        self.assertTrue(any("duplicate rule id Z1" in e for e in plugins.load_all(self.dir).errors))

    def test_broken_plugin_import_is_exit_2_naming_the_file(self):
        self.plugin("broken", text="import no_such_module_xyz\n")
        loaded = plugins.load_all(self.dir)
        self.assertEqual(len(loaded.errors), 1)
        self.assertIn("tools/broken/checkrules.py", loaded.errors[0])
        res = runner.run_check(self.settings("root", root=self.dir, waivers=None, only=["T1"]))
        self.assertEqual(res.exit, 2)
        self.assertIn("tools/broken/checkrules.py", output.render_text(res))

    def test_invalid_rule_definitions_are_reported(self):
        self.plugin("lower", text=self.PLUGIN.format(cls="Q", id="bad1"))
        self.plugin("noneeds", text=self.PLUGIN.format(cls="Q", id="Q1").replace("('files',)", "('magic',)"))
        errs = plugins.load_all(self.dir).errors
        self.assertTrue(any("bad1" in e for e in errs) and any("unknown needs" in e for e in errs), errs)

    def test_plugin_without_fixtures_fails_selftest(self):
        self.plugin()
        loaded = plugins.load_all(self.dir)
        res, errors = selftest.run(self.settings(), loaded, only=["Z1"])
        self.assertEqual(res["status"], "failed")
        msgs = [f["message"] for f in res["failed"]]
        self.assertTrue(any("no ok-* fixture" in m for m in msgs) and any("no bad-* fixture" in m for m in msgs), msgs)

    def fixtures(self, ok_files, bad_files, expect):
        base = self.dir / "tools/zed/fixtures/Z1"
        for name, files, exp in (("ok-clean", ok_files, ""), ("bad-found", bad_files, expect)):
            write(base / name / "tree", files)
            write(base / name, {"expect.txt": exp})

    def test_plugin_fixtures_pass(self):
        self.plugin()
        self.fixtures({"a.txt": ""}, {"x/bad.txt": ""}, "x/bad.txt:1 Z1\n")
        res, errors = selftest.run(self.settings(), plugins.load_all(self.dir), only=["Z1"])
        self.assertEqual((res["status"], res["failed"], errors), ("ok", [], []))

    def test_wrong_expectations_fail_selftest(self):
        self.plugin()
        self.fixtures({"bad.txt": ""}, {"x/bad.txt": ""}, "x/bad.txt:2 Z1\n")
        res, _ = selftest.run(self.settings(), plugins.load_all(self.dir), only=["Z1"])
        cases = sorted((f["case"], f["message"][:10]) for f in res["failed"])
        self.assertEqual(cases, [("bad-found", "expected x"), ("bad-found", "unexpected"), ("ok-clean", "unexpected")])

    def test_rule_that_never_fires_fails_its_bad_fixture(self):
        self.plugin(text=self.PLUGIN.format(cls="Z1", id="Z1").replace("p.endswith('bad.txt')", "False"))
        self.fixtures({"a.txt": ""}, {"bad.txt": ""}, "bad.txt:1 Z1\n")
        res, _ = selftest.run(self.settings(), plugins.load_all(self.dir), only=["Z1"])
        self.assertEqual(res["status"], "failed")

    def test_crashing_rule_fails_its_fixture(self):
        self.plugin(text=self.PLUGIN.format(cls="Z1", id="Z1").replace("return [", "raise ValueError('x'); return ["))
        self.fixtures({"a.txt": ""}, {"bad.txt": ""}, "bad.txt:1 Z1\n")
        res, _ = selftest.run(self.settings(), plugins.load_all(self.dir), only=["Z1"])
        self.assertTrue(all("rule crashed" in f["message"] for f in res["failed"]), res)

    def test_selftest_cache_follows_the_tool_hash(self):
        self.plugin()
        self.fixtures({"a.txt": ""}, {"x/bad.txt": ""}, "x/bad.txt:1 Z1\n")
        s = self.settings()
        loaded = only_loaded([r for r in plugins.load_all(self.dir).rules if r.id == "Z1"], ["zed"])
        with mock.patch.object(selftest, "run", wraps=selftest.run) as run:
            first, _ = selftest.ensure(s, loaded)
            second, _ = selftest.ensure(s, loaded)
            self.assertEqual((first["status"], second["status"]), ("ok", "cached"))
            self.assertEqual(run.call_count, 1)
            write(self.dir, {"tools/zed/fixtures/Z1/ok-clean/tree/b.txt": ""})
            third, _ = selftest.ensure(s, loaded)
            self.assertEqual(third["status"], "ok")
            self.assertEqual(run.call_count, 2)


class SelftestPieces(unittest.TestCase):
    def test_compare(self):
        f = [Finding(rule="T1", path="a.h", line=1, message="x outside")]
        self.assertEqual(selftest.compare(f, [("a.h", 1, "outside")]), [])
        self.assertEqual(len(selftest.compare(f, [])), 1)
        self.assertEqual(len(selftest.compare(f, [("a.h", 2, "outside")])), 2)
        self.assertEqual(len(selftest.compare(f, [("a.h", 1, "other")])), 2)

    def test_parse_expect(self):
        self.assertEqual(selftest.parse_expect("# c\n\nA/b.h:12 T1 some text\nc.h:3\n"),
                         [("A/b.h", 12, "T1 some text"), ("c.h", 3, "")])
        with self.assertRaises(ValueError):
            selftest.parse_expect("no colon here")

    def test_patch_merges_dicts_and_extends_lists(self):
        base = {"symbols": [{"mangled": "b", "addr": "0x2"}, {"mangled": "a", "addr": "0x1"}],
                "stats": {"aliases": {"addresses": 1, "names": 2}}}
        out = selftest.apply_patch(base, {"symbols+": [{"mangled": "c", "addr": "0x0"}], "symbols-": ["a"],
                                          "stats": {"aliases": {"names": 5}}})
        self.assertEqual([s["mangled"] for s in out["symbols"]], ["c", "b"])
        self.assertEqual(out["stats"]["aliases"], {"addresses": 1, "names": 5})
        self.assertEqual(len(base["symbols"]), 2)

    def test_world_matches_the_binfacts_schema(self):
        world = json.loads((selftest.WORLD / "binfacts.json").read_text(encoding="utf-8"))
        self.assertEqual(selftest.check_world(world), [])
        self.assertGreaterEqual(len(world["symbols"]), 40)
        drifted = dict(world, schema=999)
        self.assertTrue(any("schema" in e for e in selftest.check_world(drifted)))
        broken = json.loads(json.dumps(world))
        del broken["symbols"][0]["variant"]
        self.assertTrue(any("variant" in e for e in selftest.check_world(broken)))
        broken = json.loads(json.dumps(world))
        broken["stats"]["denominator"]["total"] += 1
        self.assertTrue(any("sum of by_kind" in e for e in selftest.check_world(broken)))

    def test_builtin_rules_of_this_module_pass_their_fixtures(self):
        from checks.context import Settings
        from civ5re import config
        cfg = config.load()
        res, errors = selftest.run(Settings(repo=cfg.root, cfg=cfg, selftest=False),
                                   only_loaded(builtin(cfg.root)), only=list(BUILTIN))
        self.assertEqual((res["failed"], errors), ([], []))
        self.assertGreaterEqual(res["fixtures"], 11)

    def test_every_builtin_rule_has_ok_and_bad_fixtures(self):
        from civ5re import config
        root = config.load().root
        for r in plugins.load_builtin(root).rules:
            cases = [c.name for c in selftest.discover_cases(plugins.fixture_dir(r, root, selftest.CHECKS_DIR))]
            self.assertTrue(any(c.startswith("ok-") for c in cases) and any(c.startswith("bad-") for c in cases), r.id)


if __name__ == "__main__":
    unittest.main()
