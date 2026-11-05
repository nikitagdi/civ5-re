import json
import os
import shutil
import socket
import subprocess
import tempfile
import unittest
from pathlib import Path

from civ5re import config
from ghidra import cli, gui, headless as hl
from ghidra.tests.helpers import DATA, SHA, FakeHeadless, FakePlugin, report_for, state_from_plan

REPO_ROOT = Path(__file__).resolve().parents[3]


def free_port():
    s = socket.socket()
    s.bind(("127.0.0.1", 0))
    p = s.getsockname()[1]
    s.close()
    return p


def git(cwd, *args):
    return subprocess.run(["git", "-C", str(cwd), "-c", "user.name=t", "-c", "user.email=t@t", *args],
                          capture_output=True, text=True)


class Repo:
    def __init__(self, test, lock=False, project=True, port=None, binary=None, drift=None, fake_changes=0):
        self.tmp = tempfile.TemporaryDirectory()
        test.addCleanup(self.tmp.cleanup)
        t = Path(self.tmp.name)
        self.root = t / "repo"
        self.proj = t / "proj"
        self.ghidra = t / "ghidra"
        self.bins = t / "bins"
        (self.ghidra / "Ghidra").mkdir(parents=True)
        (self.ghidra / "support").mkdir()
        (self.ghidra / "Ghidra" / "application.properties").write_text(
            "application.version=12.1.2\napplication.revision.ghidra=R\n")
        (self.ghidra / "support" / ("analyzeHeadless.bat" if os.name == "nt" else "analyzeHeadless")).write_text("")
        java = self.root / "tools" / "ghidra" / "java"
        java.mkdir(parents=True)
        (java / "Civ5reApply.java").write_text(
            f"class Civ5reApply {{ static final int SCRIPT_VERSION = {cli.planmod.SCRIPT_VERSION}; }}\n")
        (java / "Civ5reExport.java").write_text('class Civ5reExport { static final String VERSION = "1"; }\n')
        (java / "Civ5reOptions.java").write_text("class Civ5reOptions {}\n")
        (self.root / "tools" / "ghidra" / "analysis.properties").write_text("x = 1\n")
        self.sha = SHA
        (self.bins / "linux").mkdir(parents=True)
        if binary is not None:
            (self.bins / "linux" / "Civ5XP").write_bytes(binary)
            import hashlib
            self.sha = hashlib.sha256(binary).hexdigest()
        (self.root / "bins.lock").write_text(
            f'[[file]]\npath = "linux/Civ5XP"\nrole = "target"\nsha256 = "{self.sha}"\nsize = 1\norigin = "t"\n')
        (self.root / "civ5re.cfg").write_text("[ghidra]\nheap = 4G\nmcp_port = 8089\n")
        mcp = f"[ghidra]\nmcp_port = {port}\n" if port else ""
        (self.root / "civ5re.local.cfg").write_text(
            f"ghidra_dir = {self.ghidra}\nghidra_project_dir = {self.proj}\nbin_dir = {self.bins}\n{mcp}")
        self.proj.mkdir()
        if project:
            (self.proj / "civ5re.gpr").write_text("g")
            (self.proj / "civ5re.rep").mkdir()
        if lock:
            (self.proj / "civ5re.lock").write_text("")
        self.fake = None
        self.variant = 0
        self.drift, self.fake_changes = drift, fake_changes
        self.ctx = cli.Ctx(config.load(root=self.root, environ={}), headless_factory=self._factory,
                           ensure_inputs=self._inputs)

    def _factory(self, ctx):
        if self.fake is None:
            self.fake = FakeHeadless(ctx, changes=self.fake_changes, sha=self.sha, drift=self.drift)
        return self.fake

    def _inputs(self, ctx):
        ctx.build.mkdir(exist_ok=True)
        t = json.loads((DATA / "typedb.json").read_text(encoding="utf-8"))
        if self.variant:
            t["enums"][0]["values"][0]["value"] = self.variant
        (ctx.build / "typedb.json").write_text(json.dumps(t), encoding="utf-8")
        shutil.copyfile(DATA / "binfacts.json", ctx.build / "binfacts.json")
        text = (DATA / "ghidra-functions.txt").read_text(encoding="utf-8").replace(SHA, self.sha)
        (ctx.build / "ghidra-functions.txt").write_text(text, encoding="utf-8")

    def last(self):
        return json.loads(self.ctx.last_apply.read_text(encoding="utf-8"))

    def run(self, **kw):
        out = []
        rec = cli.run_apply(self.ctx, out=out.append, **kw)
        self.out = out
        return rec


class HeadlessRoute(unittest.TestCase):
    def test_apply_ok(self):
        r = Repo(self)
        rec = r.run()
        self.assertEqual((rec["status"], rec["route"], rec["trigger"]), ("ok", "headless", "manual"))
        self.assertEqual(rec["divergences"], 0)
        last = r.last()
        self.assertEqual(last["plan_sha256"], json.loads(r.ctx.plan_json.read_text())["header"]["plan_sha256"])
        self.assertEqual(last["applied_plan_sha256"], last["plan_sha256"])
        self.assertEqual(r.fake.calls, [("apply", str(r.proj), False, False, False)])
        self.assertEqual(cli.EXIT[rec["status"]], 0)
        for key in ("time", "commit", "dirty", "changes", "conflicts", "seconds", "message"):
            self.assertIn(key, last)

    def test_dry_run_does_not_touch_state(self):
        r = Repo(self)
        rec = r.run(dry_run=True)
        self.assertEqual(rec["status"], "ok")
        self.assertIn("dry run", rec["message"])
        self.assertFalse(r.ctx.state_json.exists())
        self.assertEqual(r.fake.calls, [("apply", str(r.proj), True, False, False)])
        self.assertIsNone(r.last()["applied_plan_sha256"])

    def test_dry_run_reports_changes(self):
        r = Repo(self, fake_changes=4)
        rec = r.run(dry_run=True)
        self.assertEqual((rec["status"], rec["changes"]), ("ok", 4))

    def test_divergence_gives_exit_1(self):
        def drift(state):
            state["funcs"][0]["name"] = "wrong"
        r = Repo(self, drift=drift)
        rec = r.run()
        self.assertEqual((rec["status"], rec["divergences"]), ("conflicts", 1))
        self.assertEqual(cli.EXIT[rec["status"]], 1)
        self.assertIsNone(r.last()["applied_plan_sha256"])

    def test_plan_conflicts_give_exit_1(self):
        r = Repo(self)
        orig = r._inputs

        def bad(ctx):
            orig(ctx)
            b = json.loads((ctx.build / "binfacts.json").read_text())
            for s in b["symbols"]:
                if s["mangled"] == "g_counter":
                    s["mangled"] = "g_other"
            (ctx.build / "binfacts.json").write_text(json.dumps(b))
        r.ctx.ensure_inputs = bad
        rec = r.run()
        self.assertEqual((rec["status"], rec["conflicts"]), ("conflicts", 1))

    def test_no_project_is_failed_with_a_hint(self):
        r = Repo(self, project=False)
        rec = r.run()
        self.assertEqual(rec["status"], "failed")
        self.assertIn("rebuild", rec["message"])
        self.assertEqual(cli.EXIT[rec["status"]], 2)
        self.assertEqual(r.last()["status"], "failed")

    def test_missing_input_is_failed(self):
        r = Repo(self)

        def boom(ctx):
            raise cli.InputError("the binfacts module is not available")
        r.ctx.ensure_inputs = boom
        rec = r.run()
        self.assertEqual(rec["status"], "failed")
        self.assertIn("binfacts", rec["message"])
        self.assertEqual(r.fake, None)

    def test_typedb_errors_are_failed(self):
        r = Repo(self)
        orig = r._inputs

        def bad(ctx):
            orig(ctx)
            t = json.loads((ctx.build / "typedb.json").read_text())
            t["ok"] = False
            t["diagnostics"] = [{"severity": "error", "code": "marker-missing", "file": "a.h", "line": 3,
                                 "message": "m"}]
            (ctx.build / "typedb.json").write_text(json.dumps(t))
        r.ctx.ensure_inputs = bad
        rec = r.run()
        self.assertEqual(rec["status"], "failed")
        self.assertIn("marker-missing", rec["message"])

    def test_apply_script_version_mismatch(self):
        r = Repo(self)
        (r.root / "tools" / "ghidra" / "java" / "Civ5reApply.java").write_text("int SCRIPT_VERSION = 9;")
        rec = r.run()
        self.assertEqual(rec["status"], "failed")
        self.assertIn("SCRIPT_VERSION", rec["message"])

    def test_no_report_is_failed(self):
        r = Repo(self)
        r._factory(r.ctx)
        r.fake.apply = lambda *a, **k: hl.StageResult("apply", 1, 0.1, Path("x.log"))
        rec = r.run()
        self.assertEqual(rec["status"], "failed")
        self.assertIn("apply-report.json", rec["message"])
        self.assertIn("exit 1", rec["message"])

    def test_aborted_apply_is_failed(self):
        r = Repo(self)
        r._factory(r.ctx)
        orig = r.fake.apply

        def aborted(project_dir, **kw):
            res = orig(project_dir, **kw)
            plan = json.loads(r.ctx.plan_json.read_text())
            r.fake._write("apply-report.json", report_for(plan, False, aborted="stale items 80 > limit 50"))
            return res
        r.fake.apply = aborted
        rec = r.run()
        self.assertEqual(rec["status"], "failed")
        self.assertIn("stale items", rec["message"])

    def test_old_report_is_not_reused(self):
        r = Repo(self)
        r.ctx.gdir.mkdir(parents=True)
        r.ctx.report_json.write_text(json.dumps({"plan_sha256": "x"}))
        r._factory(r.ctx)
        r.fake.apply = lambda *a, **k: hl.StageResult("apply", 0, 0.1, Path("x.log"))
        rec = r.run()
        self.assertEqual(rec["status"], "failed")


class Hook(unittest.TestCase):
    def test_second_hook_run_is_skipped_by_plan_sha(self):
        r = Repo(self)
        self.assertEqual(r.run(hook=True)["status"], "ok")
        self.assertEqual(len(r.fake.calls), 1)
        rec = r.run(hook=True)
        self.assertEqual((rec["status"], rec["trigger"]), ("skipped", "hook"))
        self.assertEqual(len(r.fake.calls), 1)
        self.assertEqual(cli.EXIT["skipped"], 0)
        self.assertEqual(r.last()["status"], "skipped")

    def test_changed_plan_is_applied_again(self):
        r = Repo(self)
        r.run(hook=True)
        r.variant = 1
        self.assertEqual(r.run(hook=True)["status"], "ok")
        self.assertEqual(len(r.fake.calls), 2)

    def test_manual_run_is_never_skipped(self):
        r = Repo(self)
        r.run(hook=True)
        self.assertEqual(r.run()["status"], "ok")
        self.assertEqual(len(r.fake.calls), 2)

    def test_dry_run_keeps_the_baseline(self):
        r = Repo(self)
        r.run(hook=True)
        r.run(dry_run=True)
        self.assertEqual(r.run(hook=True)["status"], "skipped")

    def test_failed_apply_is_not_a_baseline(self):
        r = Repo(self, project=False)
        self.assertEqual(r.run(hook=True)["status"], "failed")
        (r.proj / "civ5re.gpr").write_text("g")
        (r.proj / "civ5re.rep").mkdir()
        self.assertEqual(r.run(hook=True)["status"], "ok")

    def test_failure_is_recorded_for_the_hook(self):
        r = Repo(self, project=False)
        rec = r.run(hook=True)
        last = r.last()
        self.assertEqual((last["status"], last["trigger"]), ("failed", "hook"))
        self.assertTrue(last["message"])
        self.assertEqual(rec["status"], "failed")

    @unittest.skipUnless(shutil.which("git"), "git is required")
    def test_rebase_in_progress_is_skipped_and_dirty_is_reported(self):
        r = Repo(self)
        git(r.root, "init", "-q")
        (r.root / "a.h").write_text("int a;\n")
        git(r.root, "add", "a.h")
        git(r.root, "commit", "-q", "-m", "x")
        (r.root / "a.h").write_text("int a, b;\n")
        rec = r.run(hook=True)
        self.assertTrue(rec["dirty"])
        self.assertEqual(rec["status"], "ok")
        self.assertTrue(rec["commit"])
        (r.root / ".git" / "rebase-merge").mkdir()
        rec = r.run(hook=True)
        self.assertEqual((rec["status"], rec["message"]), ("skipped", "a rebase is in progress"))
        self.assertEqual(len(r.fake.calls), 1)


class GuiRoute(unittest.TestCase):
    def plugin(self, r, **kw):
        def on_run(payload):
            fake = FakeHeadless(r.ctx)
            fake.apply(r.proj, dry_run="--dry-run" in payload["args"])
        p = FakePlugin(str(r.proj / "civ5re.gpr"), on_run=on_run, **kw)
        self.addCleanup(p.close)
        return p

    def test_apply_into_open_ghidra(self):
        port = free_port()
        r = Repo(self, lock=True, port=port)
        r.ctx.apply = None
        p = self.plugin(r, port=port)
        rec = r.run()
        self.assertEqual((rec["status"], rec["route"]), ("ok", "gui"))
        path, payload = p.requests[0]
        self.assertEqual(path, "/run_ghidra_script?program=Civ5XP")
        script = Path(payload["script_name"])
        self.assertTrue(script.is_absolute())
        self.assertEqual(script, r.root / "tools" / "ghidra" / "java" / "Civ5reApply.java")
        self.assertEqual(payload["args"], f"--repo={r.root}")
        self.assertIsNone(r.fake)

    def test_dry_run_argument(self):
        port = free_port()
        r = Repo(self, lock=True, port=port)
        p = self.plugin(r, port=port)
        rec = r.run(dry_run=True)
        self.assertEqual(rec["status"], "ok")
        self.assertEqual(p.requests[0][1]["args"], f"--repo={r.root} --dry-run")
        self.assertFalse(r.ctx.state_json.exists())

    def test_script_execution_disabled(self):
        port = free_port()
        r = Repo(self, lock=True, port=port)
        self.plugin(r, port=port, status=403)
        rec = r.run(hook=True)
        self.assertEqual(rec["status"], "failed")
        self.assertIn("GHIDRA_MCP_ALLOW_SCRIPTS=1", rec["message"])
        self.assertIn("GHIDRA_MCP_ALLOW_SCRIPTS=1", r.last()["message"])
        self.assertEqual(r.last()["route"], "gui")
        self.assertEqual(cli.EXIT["failed"], 2)

    def test_foreign_project_on_a_neighbor_port_is_skipped(self):
        for _ in range(20):
            foreign = FakePlugin("C:/somewhere/other/other.gpr")
            self.addCleanup(foreign.close)
            try:
                mine_port = foreign.port + 1
                probe = socket.socket()
                probe.bind(("127.0.0.1", mine_port))
                probe.close()
                break
            except OSError:
                foreign.close()
        r = Repo(self, lock=True, port=foreign.port)
        mine = self.plugin(r, port=mine_port)
        rec = r.run()
        self.assertEqual(rec["status"], "ok")
        self.assertEqual(len(mine.requests), 1)
        self.assertEqual(foreign.requests, [])

    def test_open_project_without_plugin_is_failed_with_hint(self):
        r = Repo(self, lock=True, port=free_port())
        rec = r.run()
        self.assertEqual(rec["status"], "failed")
        self.assertIn("stale lock", rec["message"])
        self.assertIn("CodeBrowser", rec["message"])

    def test_program_must_be_open(self):
        port = free_port()
        r = Repo(self, lock=True, port=port)
        self.plugin(r, port=port, programs=("Other",))
        self.assertEqual(r.run()["status"], "failed")

    def test_find_instance_matches_project(self):
        info = {"project_path": "C:\\p\\civ5re.gpr", "programs": ["/Civ5XP"]}
        self.assertTrue(gui.match_instance(info, "C:\\p"))
        self.assertFalse(gui.match_instance(info, "C:\\q"))
        self.assertFalse(gui.match_instance({"project_path": "C:\\p\\civ5re.gpr", "programs": []}, "C:\\p"))

    def test_repo_with_spaces_is_refused(self):
        with self.assertRaises(gui.GuiError):
            gui.run_script(1, Path("C:/a b/x.java"), Path("C:/a b"), post=lambda *a: (200, ""))


class Commands(unittest.TestCase):
    def test_verify_command(self):
        r = Repo(self)
        out = []
        self.assertEqual(cli.cmd_verify(None, r.ctx, out=out.append), 2)
        r.run()
        self.assertEqual(cli.cmd_verify(None, r.ctx, out=out.append), 0)
        state = json.loads(r.ctx.state_json.read_text())
        state["funcs"][0]["cc"] = "__cdecl"
        r.ctx.state_json.write_text(json.dumps(state))
        out.clear()
        self.assertEqual(cli.cmd_verify(None, r.ctx, out=out.append), 1)
        self.assertTrue(any("function.cc" in line for line in out))
        state["plan_sha256"] = "0" * 64
        r.ctx.state_json.write_text(json.dumps(state))
        self.assertEqual(cli.cmd_verify(None, r.ctx, out=out.append), 2)

    def test_functions_without_cache_is_exit_2(self):
        r = Repo(self)
        out = []
        self.assertEqual(cli.cmd_functions(None, r.ctx, out=out.append), 2)
        self.assertIn("rebuild", out[0])
        hl.write_meta(r.proj, {"cache_key": "ab" * 32})
        self.assertEqual(cli.cmd_functions(None, r.ctx, out=out.append), 2)
        self.assertIn("rebuild", out[-1])

    def test_functions_copies_the_cache_list_and_reexports_an_old_one(self):
        r = Repo(self)
        k12 = "ab" * 6
        hl.write_meta(r.proj, {"cache_key": "ab" * 32})
        cache = hl.cache_dir(r.proj)
        cache.mkdir()
        (cache / f"pristine-{k12}.gpr").write_text("g")
        (cache / f"pristine-{k12}.rep").mkdir()
        lst = hl.cache_functions(r.proj, k12)
        text = (DATA / "ghidra-functions.txt").read_text(encoding="utf-8")
        lst.write_text(text, encoding="utf-8")
        out = []
        self.assertEqual(cli.cmd_functions(None, r.ctx, out=out.append), 0)
        self.assertEqual(r.ctx.functions_txt.read_text(encoding="utf-8"), text)
        self.assertEqual(r.fake, None)
        lst.write_text(text.replace("export=1", "export=0"), encoding="utf-8")
        self.assertEqual(cli.cmd_functions(None, r.ctx, out=out.append), 0)
        self.assertEqual(r.fake.calls, [("export", k12)])
        self.assertIn("export=1", r.ctx.functions_txt.read_text(encoding="utf-8"))

    def test_paramid_refuses_an_open_project(self):
        r = Repo(self, lock=True)
        out = []
        self.assertEqual(cli.cmd_paramid(None, r.ctx, out=out.append), 2)
        self.assertIn("open", out[0])
        r2 = Repo(self)
        self.assertEqual(cli.cmd_paramid(None, r2.ctx, out=lambda s: None), 0)
        self.assertEqual(r2.fake.calls[0][0], "paramid")

    def test_parser_registration(self):
        import argparse
        p = argparse.ArgumentParser()
        sub = p.add_subparsers()
        cli.register(sub)
        a = p.parse_args(["ghidra", "apply", "--dry-run", "--prune-all", "--hook"])
        self.assertTrue(a.dry_run and a.prune_all and a.hook)
        a = p.parse_args(["ghidra", "rebuild", "--yes", "--no-cache"])
        self.assertTrue(a.yes and a.no_cache)
        for cmd in ("verify", "functions", "paramid"):
            self.assertTrue(callable(p.parse_args(["ghidra", cmd]).func))


class Rebuild(unittest.TestCase):
    def repo(self, **kw):
        return Repo(self, project=False, binary=b"civ5 binary", **kw)

    def test_rebuild_from_scratch_then_again(self):
        r = self.repo()
        out = []
        rec = cli.run_rebuild(r.ctx, out=out.append)
        self.assertEqual(rec["status"], "ok", rec["message"])
        self.assertTrue(hl.project_exists(r.proj))
        meta = hl.read_meta(r.proj)
        self.assertEqual(len(meta["cache_key"]), 64)
        self.assertEqual(meta["binary_sha256"], r.sha)
        self.assertEqual([c[0] for c in r.fake.calls], ["cache", "apply"])
        self.assertEqual(r.fake.calls[1][2:], (False, False, True))
        self.assertTrue(r.ctx.functions_txt.is_file())
        self.assertTrue(r.ctx.plan_json.is_file())
        self.assertEqual(r.last()["applied_plan_sha256"], r.last()["plan_sha256"])
        self.assertTrue(any("cache (import" in line for line in out))
        self.assertTrue(list(r.ctx.logs.glob("*-rebuild.log")))
        self.assertIn("stage plan", list(r.ctx.logs.glob("*-rebuild.log"))[0].read_text())
        self.assertFalse((r.proj / hl.REBUILD_DIR).exists())

    def test_existing_project_needs_yes_and_cache_is_reused(self):
        r = self.repo()
        cli.run_rebuild(r.ctx, out=lambda s: None)
        out = []
        rec = cli.run_rebuild(r.ctx, out=out.append)
        self.assertEqual(rec["status"], "failed")
        self.assertIn("--yes", rec["message"])
        self.assertEqual(len(r.fake.calls), 2)
        rec = cli.run_rebuild(r.ctx, yes=True, out=out.append)
        self.assertEqual(rec["status"], "ok", rec["message"])
        self.assertEqual([c[0] for c in r.fake.calls], ["cache", "apply", "apply"])
        self.assertTrue((r.proj / "civ5re.prev.gpr").exists())
        self.assertTrue(any("no import" in line for line in out))
        rec = cli.run_rebuild(r.ctx, yes=True, no_cache=True, out=out.append)
        self.assertEqual([c[0] for c in r.fake.calls][-2:], ["cache", "apply"])

    def test_open_project_blocks_rebuild(self):
        r = self.repo()
        (r.proj / "civ5re.gpr").write_text("g")
        (r.proj / "civ5re.rep").mkdir()
        (r.proj / "civ5re.lock").write_text("")
        rec = cli.run_rebuild(r.ctx, yes=True, out=lambda s: None)
        self.assertEqual(rec["status"], "failed")
        self.assertIn("lock", rec["message"])

    def test_divergence_keeps_the_working_project(self):
        def drift(state):
            state["funcs"][0]["name"] = "wrong"
        r = self.repo(drift=drift)
        (r.proj / "civ5re.gpr").write_text("old")
        (r.proj / "civ5re.rep").mkdir()
        out = []
        rec = cli.run_rebuild(r.ctx, yes=True, out=out.append)
        self.assertEqual(rec["status"], "conflicts")
        self.assertEqual((r.proj / "civ5re.gpr").read_text(), "old")
        self.assertTrue((r.proj / hl.REBUILD_DIR / "civ5re.gpr").exists())
        self.assertTrue(any("untouched" in line for line in out))

    def test_non_idempotent_apply_keeps_the_working_project(self):
        r = self.repo()
        r._factory(r.ctx)
        orig = r.fake.apply

        def apply(project_dir, **kw):
            res = orig(project_dir, **kw)
            plan = json.loads(r.ctx.plan_json.read_text())
            r.fake._write("apply-report.dry.json", report_for(plan, True, changes=2))
            return res
        r.fake.apply = apply
        rec = cli.run_rebuild(r.ctx, out=lambda s: None)
        self.assertEqual(rec["status"], "failed")
        self.assertIn("idempotent", rec["message"])
        self.assertFalse(hl.project_exists(r.proj))

    def test_without_typedb_only_the_cache_and_function_list_are_built(self):
        r = self.repo()

        def no_typedb(ctx):
            raise cli.InputError("the typedb module is not available")
        r.ctx.ensure_inputs = no_typedb
        r.ctx.build.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(DATA / "binfacts.json", r.ctx.build / "binfacts.json")
        rec = cli.run_rebuild(r.ctx, out=lambda s: None)
        self.assertEqual(rec["status"], "failed")
        self.assertEqual(rec["message"], "typedb not available: cache ready, plan/apply skipped")
        self.assertEqual([c[0] for c in r.fake.calls], ["cache"])
        self.assertTrue(r.ctx.functions_txt.is_file())
        self.assertFalse(r.ctx.plan_json.exists())
        self.assertEqual(cli.EXIT[rec["status"]], 2)

    def test_wrong_binary_is_refused(self):
        r = self.repo()
        (r.bins / "linux" / "Civ5XP").write_bytes(b"other")
        rec = cli.run_rebuild(r.ctx, out=lambda s: None)
        self.assertEqual(rec["status"], "failed")
        self.assertIn("bins.lock", rec["message"])
        self.assertIsNone(r.fake)


@unittest.skipUnless(shutil.which("git") and (shutil.which("sh") or shutil.which("bash")), "git and sh are required")
class PostCommitHook(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.repo = Path(self.tmp.name) / "r"
        (self.repo / ".githooks").mkdir(parents=True)
        shutil.copyfile(REPO_ROOT / ".githooks" / "post-commit", self.repo / ".githooks" / "post-commit")
        git(self.repo, "init", "-q")
        git(self.repo, "config", "core.hooksPath", ".githooks")
        self.sh = shutil.which("sh") or shutil.which("bash")

    def commit(self):
        (self.repo / "a.txt").write_text("x\n")
        git(self.repo, "add", "a.txt")
        p = git(self.repo, "commit", "-q", "-m", "c")
        n = git(self.repo, "rev-list", "--count", "HEAD").stdout.strip()
        return p, n

    def hook(self):
        return subprocess.run([self.sh, ".githooks/post-commit"], cwd=self.repo, capture_output=True, text=True)

    def test_failing_apply_leaves_the_commit_and_exits_0(self):
        venv = self.repo / ".venv" / "bin"
        venv.mkdir(parents=True)
        (venv / "python").write_text("#!/bin/sh\necho \"$@\" > \"$(dirname \"$0\")/args.txt\"\nexit 3\n", newline="\n")
        p, n = self.commit()
        self.assertEqual(p.returncode, 0, p.stderr)
        self.assertEqual(n, "1")
        h = self.hook()
        self.assertEqual(h.returncode, 0)
        self.assertIn("ghidra apply failed", h.stderr)
        args = (venv / "args.txt").read_text()
        self.assertIn("-m civ5re ghidra apply --hook", args)

    def test_no_venv_exits_0(self):
        p, n = self.commit()
        self.assertEqual(n, "1")
        h = self.hook()
        self.assertEqual(h.returncode, 0)
        self.assertIn(".venv not found", h.stderr)

    def test_hook_file_is_lf_and_ends_with_exit_0(self):
        raw = (REPO_ROOT / ".githooks" / "post-commit").read_bytes()
        self.assertNotIn(b"\r", raw)
        self.assertTrue(raw.startswith(b"#!/bin/sh\n"))
        self.assertTrue(raw.rstrip().endswith(b"exit 0"))


if __name__ == "__main__":
    unittest.main()
