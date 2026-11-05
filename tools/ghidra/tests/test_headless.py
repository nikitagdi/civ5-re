import json
import multiprocessing
import os
import tempfile
import time
import unittest
from pathlib import Path

from ghidra import headless as hl


def fake_ghidra(root, revision="REV1", version="12.1.2"):
    g = Path(root) / "ghidra"
    (g / "Ghidra").mkdir(parents=True)
    (g / "support").mkdir()
    (g / "Ghidra" / "application.properties").write_text(
        f"application.name=Ghidra\napplication.version={version}\napplication.revision.ghidra={revision}\n")
    (g / "support" / ("analyzeHeadless.bat" if os.name == "nt" else "analyzeHeadless")).write_text("")
    return g


def fake_repo(root):
    r = Path(root) / "repo"
    (r / "tools" / "ghidra" / "java").mkdir(parents=True)
    (r / "tools" / "ghidra" / "analysis.properties").write_text("a = 1\n")
    (r / "tools" / "ghidra" / "java" / "Civ5reOptions.java").write_text("class Civ5reOptions {}\n")
    return r


class Recorder:
    def __init__(self, code=0):
        self.calls = []
        self.code = code

    def __call__(self, argv, env, logfile):
        self.calls.append((argv, env))
        logfile.write("headless output\n")
        return self.code


class CacheKey(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.g = fake_ghidra(self.tmp.name)
        self.r = fake_repo(self.tmp.name)

    def key(self, sha="a" * 64):
        return hl.cache_key(sha, self.g, self.r)

    def test_stable(self):
        self.assertEqual(self.key(), self.key())
        self.assertEqual(len(hl.key12(self.key())), 12)

    def test_typedb_is_not_part_of_the_key(self):
        (self.r / "build").mkdir()
        (self.r / "build" / "typedb.json").write_text("{}")
        before = self.key()
        (self.r / "build" / "typedb.json").write_text('{"records": [1, 2, 3]}')
        (self.r / "Civ5Expansion2").mkdir()
        (self.r / "Civ5Expansion2" / "x.h").write_text("struct X {};")
        self.assertEqual(self.key(), before)

    def test_inputs_that_change_the_key(self):
        base = self.key()
        self.assertNotEqual(self.key("b" * 64), base)
        (self.r / "tools" / "ghidra" / "analysis.properties").write_text("a = 2\n")
        k2 = self.key()
        self.assertNotEqual(k2, base)
        (self.r / "tools" / "ghidra" / "java" / "Civ5reOptions.java").write_text("class Civ5reOptions { int x; }\n")
        k3 = self.key()
        self.assertNotEqual(k3, k2)
        fake_ghidra_rev = self.g / "Ghidra" / "application.properties"
        fake_ghidra_rev.write_text("application.version=12.1.2\napplication.revision.ghidra=REV2\n")
        self.assertNotEqual(self.key(), k3)

    def test_seeds_are_part_of_the_key(self):
        self.assertNotEqual(hl.cache_key("a" * 64, self.g, self.r, "0x08048000\n"), self.key())

    def test_function_seeds(self):
        bf = {"vtables": [{"groups": [{"slots": [
                  {"kind": "func", "target": "0x08000020"}, {"kind": "thunk", "target": "0x08000010"},
                  {"kind": "pure", "target": "0x08000099"}, {"kind": "import", "target": "0x08000098"},
                  {"kind": "func", "target": "0x08000020"}]}]}],
              "symbols": [{"type": "func", "size": 4, "section": ".text", "addr": "0x08000030"},
                          {"type": "func", "size": 0, "section": ".text", "addr": "0x08000040"},
                          {"type": "func", "size": 4, "section": "UND", "addr": "0x00000000"},
                          {"type": "object", "size": 4, "section": ".data", "addr": "0x08000050"}]}
        self.assertEqual(hl.function_seeds(bf).split("\n")[1:-1], ["0x08000010", "0x08000020", "0x08000030"])

    def test_ghidra_version_is_pinned(self):
        self.assertEqual(hl.check_ghidra(self.g)["version"], "12.1.2")
        other = fake_ghidra(Path(self.tmp.name) / "o", version="11.2.1")
        with self.assertRaises(hl.HeadlessError):
            hl.check_ghidra(other)


class Running(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.g = fake_ghidra(self.tmp.name)
        self.r = fake_repo(self.tmp.name)
        self.rec = Recorder()
        ticks = iter(range(0, 1000, 5))
        self.h = hl.Headless(self.g, "16G", self.r, Path(self.tmp.name) / "logs", runner=self.rec,
                             clock=lambda: next(ticks))

    def test_heap_goes_through_the_environment(self):
        res = self.h.apply(Path(self.tmp.name) / "proj")
        env = self.rec.calls[0][1]
        self.assertEqual(env["GHIDRA_HEADLESS_MAXMEM"], "16G")
        self.assertEqual(env["CIV5RE_REPO"], str(self.r))
        self.assertTrue(res.ok)

    def test_log_has_stage_time(self):
        res = self.h.apply(Path(self.tmp.name) / "proj")
        text = res.log.read_text()
        self.assertIn("== stage apply", text)
        self.assertIn("finished in 5.0 s, exit 0", text)
        self.assertEqual(res.seconds, 5)

    def test_failure_exit_code(self):
        self.rec.code = 3
        res = self.h.apply(Path(self.tmp.name) / "proj")
        self.assertFalse(res.ok)
        self.assertIn("exit 3", res.log.read_text())

    def test_apply_args(self):
        a = [str(x) for x in self.h.apply_args("P", "civ5re")]
        self.assertEqual(a[:6], ["P", "civ5re", "-process", "Civ5XP", "-noanalysis", "-postScript"])
        self.assertTrue(a[6].endswith("Civ5reApply.java"))
        self.assertTrue(os.path.isabs(a[6]))
        self.assertIn(f"--repo={self.r}", a)
        self.assertNotIn("-readOnly", a)
        d = [str(x) for x in self.h.apply_args("P", "civ5re", dry_run=True)]
        self.assertIn("-readOnly", d)
        self.assertEqual(d[-1], "--dry-run")
        both = [str(x) for x in self.h.apply_args("P", "civ5re", both=True)]
        self.assertEqual(both.count("-postScript"), 2)
        self.assertNotIn("-readOnly", both)
        self.assertEqual(both[-1], "--dry-run")
        pa = [str(x) for x in self.h.apply_args("P", "civ5re", prune_all=True)]
        self.assertIn("--prune-all", pa)

    def test_cache_args(self):
        a = [str(x) for x in self.h.cache_args("P", "abc123abc123", "C:/bin/Civ5XP", "out.txt", True)]
        self.assertEqual(a[1], "pristine-abc123abc123")
        self.assertIn("-import", a)
        self.assertIn("-overwrite", a)
        self.assertEqual(a[a.index("-processor") + 1], "x86:LE:32:default")
        self.assertEqual(a[a.index("-cspec") + 1], "gcc")
        self.assertLess(a.index("-preScript"), a.index("-postScript"))
        self.assertNotIn("-noanalysis", a)
        self.assertIn("--out=out.txt", a)
        self.assertIn("--cache-key=abc123abc123", a)
        self.assertNotIn("-overwrite", [str(x) for x in self.h.cache_args("P", "k", "b", "o", False)])

    def test_script_path_precedes_scripts(self):
        self.h.apply(Path(self.tmp.name) / "proj")
        argv = self.rec.calls[0][0]
        i = argv.index("-scriptPath")
        self.assertEqual(argv[i + 1], str(self.r / "tools" / "ghidra" / "java"))
        self.assertLess(i, argv.index("-postScript"))
        self.assertFalse(hl.REBUILD_DIR.startswith("."))

    def test_export_is_read_only(self):
        self.h.export_functions(Path(self.tmp.name) / "proj", "k12k12k12k12")
        argv = self.rec.calls[0][0]
        self.assertIn("-readOnly", argv)
        self.assertIn("-noanalysis", argv)


class PicCalls(unittest.TestCase):
    def test_offsets_of_call_next_pop(self):
        code = b"\x55" + b"\xe8\x00\x00\x00\x00\x5b" + b"\x90" + b"\xe8\x00\x00\x00\x00\x58" + b"\xe8\x00\x00\x00\x00\x90" + \
            b"\xe8\x01\x00\x00\x00\x5b"
        self.assertEqual(hl.pic_call_offsets(code), [1, 8])

    def test_overlapping_and_edge_matches(self):
        self.assertEqual(hl.pic_call_offsets(b"\xe8\x00\x00\x00\x00\xe8\x00\x00\x00\x00\x5f"), [5])
        self.assertEqual(hl.pic_call_offsets(b"\xe8\x00\x00\x00\x00"), [])

    def test_the_options_script_scans_for_the_same_bytes(self):
        src = (Path(__file__).resolve().parents[1] / "java" / "Civ5reOptions.java").read_text(encoding="utf-8")
        self.assertIn("!= 0xe8", src)
        self.assertIn("(data[i + 5] & 0xf8) != 0x58", src)
        self.assertIn("FlowOverride.BRANCH", src)
        self.assertLess(src.index("picCalls();"), src.index("seedFunctions(seeds);"))


class Projects(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.d = Path(self.tmp.name)

    def make(self, d, name, marker):
        d.mkdir(parents=True, exist_ok=True)
        (d / f"{name}.gpr").write_text(marker)
        (d / f"{name}.rep").mkdir(exist_ok=True)
        (d / f"{name}.rep" / "data.txt").write_text(marker)

    def test_lock_detection(self):
        self.make(self.d, "civ5re", "x")
        self.assertTrue(hl.project_exists(self.d))
        self.assertFalse(hl.is_open(self.d))
        (self.d / "civ5re.lock").write_text("")
        self.assertTrue(hl.is_open(self.d))

    def test_copy_project_renames(self):
        self.make(self.d / "cache", "pristine-k", "cache")
        hl.copy_project(self.d / "cache", "pristine-k", self.d / hl.REBUILD_DIR, "civ5re")
        self.assertEqual((self.d / hl.REBUILD_DIR / "civ5re.gpr").read_text(), "cache")
        self.assertEqual((self.d / hl.REBUILD_DIR / "civ5re.rep" / "data.txt").read_text(), "cache")
        self.assertTrue(hl.project_exists(self.d / "cache", "pristine-k"))
        with self.assertRaises(hl.HeadlessError):
            hl.copy_project(self.d, "nope", self.d / "x", "y")

    def test_replace_keeps_one_previous(self):
        self.make(self.d, "civ5re", "v1")
        self.make(self.d / hl.REBUILD_DIR, "civ5re", "v2")
        hl.replace_project(self.d, {"cache_key": "k" * 64})
        self.assertEqual((self.d / "civ5re.gpr").read_text(), "v2")
        self.assertEqual((self.d / "civ5re.prev.gpr").read_text(), "v1")
        self.assertEqual((self.d / "civ5re.prev.rep" / "data.txt").read_text(), "v1")
        self.assertFalse((self.d / hl.REBUILD_DIR).exists())
        self.assertEqual(hl.read_meta(self.d)["cache_key"], "k" * 64)
        self.make(self.d / hl.REBUILD_DIR, "civ5re", "v3")
        hl.replace_project(self.d, {"cache_key": "j" * 64})
        self.assertEqual((self.d / "civ5re.prev.gpr").read_text(), "v2")
        self.assertEqual((self.d / "civ5re.gpr").read_text(), "v3")
        self.assertEqual(json.loads((self.d / "civ5re.json").read_text())["cache_key"], "j" * 64)

    def test_replace_without_current_project(self):
        self.make(self.d / hl.REBUILD_DIR, "civ5re", "v1")
        hl.replace_project(self.d, {})
        self.assertEqual((self.d / "civ5re.gpr").read_text(), "v1")
        self.assertFalse(hl.project_files(self.d, "civ5re.prev")[0].exists())

    def test_replace_refuses_an_open_project(self):
        self.make(self.d, "civ5re", "v1")
        self.make(self.d / hl.REBUILD_DIR, "civ5re", "v2")
        (self.d / "civ5re.lock").write_text("")
        with self.assertRaises(hl.HeadlessError):
            hl.replace_project(self.d, {})
        self.assertEqual((self.d / "civ5re.gpr").read_text(), "v1")
        self.assertTrue((self.d / hl.REBUILD_DIR / "civ5re.gpr").exists())

    def test_cache_paths(self):
        self.assertEqual(hl.cache_functions(self.d, "abc").name, "pristine-abc.functions.txt")
        self.assertFalse(hl.cache_exists(self.d, "abc"))
        self.make(self.d / "cache", "pristine-abc", "c")
        self.assertTrue(hl.cache_exists(self.d, "abc"))


if __name__ == "__main__":
    unittest.main()


def _hold_lock(path, hold, log):
    from ghidra import projlock
    with projlock.project_lock(path, timeout=30, out=lambda s: None, step=0.05):
        with open(log, "a") as f:
            f.write(f"in {time.time()}\n")
        time.sleep(hold)
        with open(log, "a") as f:
            f.write(f"out {time.time()}\n")


class ProjectLock(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.path = Path(self.tmp.name) / "ghidra" / "headless.lock"

    def test_parallel_processes_are_serialized(self):
        log = Path(self.tmp.name) / "events.txt"
        ctx = multiprocessing.get_context("spawn")
        procs = [ctx.Process(target=_hold_lock, args=(self.path, 0.5, log)) for _ in range(2)]
        for p in procs:
            p.start()
        for p in procs:
            p.join(60)
            self.assertEqual(p.exitcode, 0)
        kinds = [line.split()[0] for line in log.read_text().splitlines()]
        self.assertEqual(kinds, ["in", "out", "in", "out"])

    def test_reentrant_and_records_pid(self):
        from ghidra import projlock
        with projlock.project_lock(self.path, timeout=1):
            with projlock.project_lock(self.path, timeout=1):
                self.assertEqual(projlock.holder(self.path), os.getpid())

    def test_waits_once_then_times_out(self):
        from ghidra import projlock
        log = Path(self.tmp.name) / "events.txt"
        p = multiprocessing.get_context("spawn").Process(target=_hold_lock, args=(self.path, 3, log))
        p.start()
        self.addCleanup(p.join, 60)
        while not log.exists():
            time.sleep(0.05)
        said = []
        with self.assertRaises(projlock.LockTimeout):
            with projlock.project_lock(self.path, timeout=0.5, out=said.append, step=0.05):
                pass
        self.assertEqual(said, [f"waiting for project lock (held by pid {p.pid})"])


class LockedRetry(unittest.TestCase):
    LOCKED = ("ERROR Abort due to Headless analyzer error: ghidra.framework.store.LockException: "
              "Unable to lock project! E:/x/civ5re\n")

    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self.tmp.cleanup)
        self.g = fake_ghidra(self.tmp.name)
        self.r = fake_repo(self.tmp.name)
        self.sleeps = []

    def headless(self, runner):
        return hl.Headless(self.g, "4G", self.r, Path(self.tmp.name) / "logs", runner=runner,
                           sleep=self.sleeps.append)

    def test_detection(self):
        self.assertTrue(hl.project_locked(self.LOCKED))
        self.assertFalse(hl.project_locked("ERROR SCRIPT ERROR in Civ5reDecomp.java\n"))

    def test_retries_with_backoff_until_unlocked(self):
        codes = iter([1, 1, 0])

        def runner(argv, env, logfile):
            code = next(codes)
            logfile.write(self.LOCKED if code else "ok\n")
            return code

        res = self.headless(runner).decomp(Path(self.tmp.name) / "proj", "req", "out", None)
        self.assertTrue(res.ok)
        self.assertEqual(self.sleeps, [hl.LOCK_BACKOFF, hl.LOCK_BACKOFF * 2])

    def test_other_failures_are_not_retried(self):
        def runner(argv, env, logfile):
            logfile.write("ERROR something else\n")
            return 1

        res = self.headless(runner).decomp(Path(self.tmp.name) / "proj", "req", "out", None)
        self.assertEqual(res.code, 1)
        self.assertEqual(self.sleeps, [])

    def test_gives_up_after_the_last_retry(self):
        def runner(argv, env, logfile):
            logfile.write(self.LOCKED)
            return 1

        res = self.headless(runner).decomp(Path(self.tmp.name) / "proj", "req", "out", None)
        self.assertFalse(res.ok)
        self.assertEqual(len(self.sleeps), hl.LOCK_RETRIES)
