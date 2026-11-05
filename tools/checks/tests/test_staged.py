import unittest

from checks import plugins, runner, staged
from checks.api import Rule

from .helpers import H, TmpCase, git, init_repo, write


class Probe(Rule):
    id = "Z9"
    title = "records the scope it is given"
    needs = ("files",)
    scope = "entity"
    seen = []

    def run(self, ctx):
        Probe.seen.append(None if ctx.scope is None else sorted(ctx.scope))
        return []


def rules_for(repo):
    own = [r for r in plugins.load_builtin(repo).rules if r.id in ("T1", "T2", "T3", "W0")]
    return own + [Probe()]


class StagedMode(TmpCase):
    def run_staged(self, **kw):
        Probe.seen = []
        return runner.run_rules(self.settings("staged", **kw), rules_for(self.dir))

    def status(self, res):
        return {r["id"]: r["status"] for r in res.rules}

    def test_partial_indexing_is_exit_2(self):
        init_repo(self.dir)
        write(self.dir, {"Civ5Expansion2/a.h": H})
        git(self.dir, "add", "Civ5Expansion2/a.h")
        write(self.dir, {"Civ5Expansion2/a.h": H + "// edit\n"})
        res = self.run_staged()
        self.assertEqual(res.exit, 2)
        self.assertIn("partial indexing is not supported", res.findings[0].message)
        self.assertIn("git stash -k", res.findings[0].message)

    def test_untracked_file_in_closure_is_exit_2(self):
        init_repo(self.dir)
        write(self.dir, {"Civ5Expansion2/a.h": H + '#include "b.h"\n', "Civ5Expansion2/b.h": H})
        git(self.dir, "add", "Civ5Expansion2/a.h")
        res = self.run_staged()
        self.assertEqual(res.exit, 2)
        self.assertIn("untracked", res.findings[0].message)
        self.assertIn("Civ5Expansion2/b.h", res.findings[0].message)

    def test_untracked_includer_is_exit_2(self):
        init_repo(self.dir, {"Civ5Expansion2/b.h": H})
        write(self.dir, {"Civ5Expansion2/b.h": H + "// edit\n", "Civ5Expansion2/c.cpp": '#include "b.h"\n'})
        git(self.dir, "add", "Civ5Expansion2/b.h")
        self.assertEqual(self.run_staged().exit, 2)

    def test_deletion_is_a_full_run(self):
        init_repo(self.dir, {"Civ5Expansion2/a.h": H, "src/bad.h": "x"})
        git(self.dir, "rm", "-q", "Civ5Expansion2/a.h")
        res = self.run_staged()
        self.assertTrue(all(s == "ran" for s in self.status(res).values()), self.status(res))
        self.assertEqual(Probe.seen, [None])
        self.assertIn("src/bad.h", [f.path for f in res.findings])
        self.assertEqual(res.exit, 1)

    def test_header_edit_runs_closure_for_entity_rules(self):
        init_repo(self.dir, {"Civ5Expansion2/a.h": H, "Civ5Expansion2/b.h": H + '#include "a.h"\n',
                             "Civ5Expansion2/c.cpp": '#include "b.h"\n', "Civ5Expansion2/z.h": H,
                             "src/bad.h": "x"})
        write(self.dir, {"Civ5Expansion2/a.h": H + "// edit\n"})
        git(self.dir, "add", "Civ5Expansion2/a.h")
        res = self.run_staged()
        self.assertEqual(Probe.seen, [["Civ5Expansion2/a.h", "Civ5Expansion2/b.h", "Civ5Expansion2/c.cpp"]])
        self.assertEqual(self.status(res)["T3"], "ran")
        self.assertEqual(self.status(res)["W0"], "skipped")
        self.assertEqual(res.findings, [])
        self.assertEqual(res.exit, 0)

    def test_cpp_edit_is_only_itself(self):
        init_repo(self.dir, {"Civ5Expansion2/a.h": H, "Civ5Expansion2/c.cpp": '#include "a.h"\n'})
        write(self.dir, {"Civ5Expansion2/c.cpp": '#include "a.h"\n// edit\n'})
        git(self.dir, "add", "Civ5Expansion2/c.cpp")
        self.run_staged()
        self.assertEqual(Probe.seen, [["Civ5Expansion2/c.cpp"]])

    def test_markdown_only_commit_runs_only_t1_t2(self):
        init_repo(self.dir, {"src/bad.h": "x"})
        write(self.dir, {"notes/new.md": "x\n"})
        git(self.dir, "add", "notes/new.md")
        res = self.run_staged()
        st = self.status(res)
        self.assertEqual(sorted(k for k, v in st.items() if v == "ran"), ["T1", "T2"])
        self.assertEqual([(f.rule, f.path) for f in res.findings], [("T2", "notes/new.md")])
        self.assertEqual(res.exit, 1)

    def test_tool_change_is_a_full_run(self):
        init_repo(self.dir)
        write(self.dir, {"tools/checks/x.py": "x = 1\n"})
        git(self.dir, "add", "tools/checks/x.py")
        res = self.run_staged()
        self.assertEqual(Probe.seen, [None])
        self.assertTrue(any("full run" in n for n in res.notes))

    def test_files_mode_uses_the_same_plan(self):
        init_repo(self.dir, {"Civ5Expansion2/a.h": H, "Civ5Expansion2/b.h": H + '#include "a.h"\n'})
        Probe.seen = []
        res = runner.run_rules(self.settings("files", files=["Civ5Expansion2/a.h"]), rules_for(self.dir))
        self.assertEqual(Probe.seen, [["Civ5Expansion2/a.h", "Civ5Expansion2/b.h"]])
        self.assertEqual(res.exit, 0)
        bad = runner.run_rules(self.settings("files", files=["nope.h"]), rules_for(self.dir))
        self.assertEqual(bad.exit, 2)


class PlanTable(unittest.TestCase):
    def plan(self, *paths, status="M"):
        return staged.plan([staged.Change(status, p) for p in paths], plugins_graph())

    def test_evidence_change_selects_o_rules_for_the_whole_tree(self):
        class O2(Rule):
            id = "O2"
            title = "t"
        p = self.plan("tools/seed/data/sdk_map.txt")
        self.assertEqual(p.scope(O2()), staged.Scope(None, None))
        self.assertIsNone(p.scope(Probe()))


def plugins_graph():
    from checks import includes
    return includes.build([], lambda p: "", lambda p: False)


if __name__ == "__main__":
    unittest.main()
