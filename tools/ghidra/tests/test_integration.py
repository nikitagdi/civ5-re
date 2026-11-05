"""Integration test on the t.cpp fixture with a real headless Ghidra (CIV5RE_GHIDRA_TESTS=1, ghidra_dir set).

Runs the civ5re ghidra chain (rebuild -> dry run -> paramid -> apply) in a temporary repository copy and checks
the decompiler output. Target: under 2 minutes.
"""
import hashlib
import json
import os
import shutil
import subprocess
import tempfile
import time
import unittest
from pathlib import Path
from unittest import mock

from civ5re import config
from ghidra import cli, headless

HERE = Path(__file__).resolve().parent
FIXTURE = HERE / "fixture"
REPO = HERE.parents[2]
GHIDRA_DIR = None
if os.environ.get("CIV5RE_GHIDRA_TESTS") == "1":
    try:
        GHIDRA_DIR = config.load().path("ghidra_dir")
    except config.ConfigError:
        GHIDRA_DIR = None


def sha(p):
    return hashlib.sha256(Path(p).read_bytes()).hexdigest()


def block(text, name):
    """Decompiler output of one function from the probe dump."""
    out, on = [], False
    for line in text.splitlines():
        if line.startswith("=== "):
            on = line.split(" ", 2)[2] == name
            continue
        if on:
            out.append(line)
    return "\n".join(out)


@unittest.skipUnless(GHIDRA_DIR and GHIDRA_DIR.is_dir(), "set CIV5RE_GHIDRA_TESTS=1 and ghidra_dir")
class FixtureChain(unittest.TestCase):

    def setUp(self):
        self.tmp = Path(tempfile.mkdtemp(prefix="civ5re-ghidra-"))
        if not os.environ.get("CIV5RE_GHIDRA_KEEP"):
            self.addCleanup(shutil.rmtree, self.tmp, True)
        repo = self.tmp / "repo"
        (repo / "tools/ghidra/java").mkdir(parents=True)
        for f in (REPO / "tools/ghidra/java").glob("*.java"):
            shutil.copy(f, repo / "tools/ghidra/java" / f.name)
        shutil.copy(REPO / "tools/ghidra/analysis.properties", repo / "tools/ghidra/analysis.properties")
        (repo / "bin").mkdir()
        shutil.copy(FIXTURE / "t.elf", repo / "bin/Civ5XP")
        elf = FIXTURE / "t.elf"
        (repo / "bins.lock").write_text(
            f'[[file]]\npath = "Civ5XP"\nrole = "target"\nsha256 = "{sha(elf)}"\nsize = {elf.stat().st_size}\n'
            f'origin = "fixture"\n', encoding="utf-8")
        (repo / "build").mkdir()
        for f in ("typedb.json", "binfacts.json"):
            shutil.copy(FIXTURE / f, repo / "build" / f)
        env = {"CIV5RE_GHIDRA_DIR": str(GHIDRA_DIR), "CIV5RE_GHIDRA_PROJECT_DIR": str(self.tmp / "proj"),
               "CIV5RE_BIN_DIR": str(repo / "bin"), "CIV5RE_GHIDRA_HEAP": "2G"}
        self.ctx = cli.Ctx(config.load(root=repo, environ=env), ensure_inputs=lambda ctx: None,
                           )
        self.log = []

    def out(self, s):
        self.log.append(s)

    def probe(self, *args):
        exe = headless.analyze_headless(GHIDRA_DIR)
        argv = [str(exe), str(self.tmp / "proj"), headless.PROJECT, "-process", headless.PROGRAM, "-noanalysis",
                "-scriptPath", str(FIXTURE), "-postScript", str(FIXTURE / "Civ5reTestProbe.java"), *args]
        if args[0] == "--decompile":
            argv.append("-readOnly")
        p = subprocess.run(argv, capture_output=True, text=True, errors="replace", stdin=subprocess.DEVNULL)
        self.assertEqual(p.returncode, 0, p.stdout[-3000:])

    def decompile(self):
        dst = self.tmp / "dec.txt"
        if dst.exists():
            dst.unlink()
        self.probe("--decompile", str(dst))
        return dst.read_text(encoding="utf-8")

    def report(self, dry=False):
        p = self.ctx.dry_report_json if dry else self.ctx.report_json
        return json.loads(p.read_text(encoding="utf-8"))

    def state(self):
        return json.loads(self.ctx.state_json.read_text(encoding="utf-8"))

    def apply(self, dry=False):
        rec = cli.run_apply(self.ctx, dry_run=dry, out=self.out)
        self.assertEqual(rec["status"], "ok", "\n".join(self.log))
        return rec

    def test_chain(self):
        t0 = time.monotonic()
        times = {}
        rec = cli.run_rebuild(self.ctx, yes=True, out=self.out)
        times["rebuild"] = time.monotonic() - t0
        self.assertEqual(rec["status"], "ok", "\n".join(self.log))
        self.assertEqual(rec["divergences"], 0)
        funcs = self.ctx.functions_txt.read_text(encoding="utf-8").splitlines()
        self.assertTrue(funcs[0] == "# civ5re ghidra-functions 2")
        self.assertIn(f"binary_sha256={sha(FIXTURE / 't.elf')}", funcs[1])
        rows = {ln.split("\t")[0]: ln.split("\t") for ln in funcs[3:]}
        self.assertEqual(rows["0x08049800"][3], "r=ECX,EDX")
        self.assertEqual(rows["0x08049720"][3], "-")
        self.assertEqual(rows["0x08049800"][1], "0x0804980e")

        st0 = sha(self.ctx.state_json)
        rec = self.apply(dry=True)
        self.assertEqual(rec["changes"], 0, self.report(dry=True)["entries"])
        self.assertEqual(sha(self.ctx.state_json), st0, "--dry-run changed state.json")

        t1 = time.monotonic()
        dec = self.decompile()
        times["decompile"] = time.monotonic() - t1
        self.assertIn("this->m_iExtra", block(dec, "Derived::Update"))
        self.assertIn("(*p->_vptr->Update)(p,dt)", block(dec, "CallUpdate"))
        self.assertIn("(*p->_vptr->Extra)(p,3,4)", block(dec, "CallExtra"))
        self.assertIn("Vec2 * Derived::GetPos(Vec2 *__return_storage_ptr__,Derived *this)", dec)
        hidden = block(dec, "hidden_func")
        self.assertIn("d->m_iExtra", hidden)
        self.assertNotIn("in_ECX", hidden)
        self.assertNotIn("in_EDX", hidden)
        self.assertIn("Derived::Sum(Derived *this,int param_1,int param_2)", dec)
        self.assertIn("this->_vptr = &Derived_vtbl;", block(dec, "Derived::Derived"))
        self.assertIn("int UsePos(int param_1,int param_2)", dec)

        t1 = time.monotonic()
        self.assertEqual(cli.cmd_paramid(None, self.ctx, out=self.out), 0, "\n".join(self.log))
        times["paramid"] = time.monotonic() - t1
        self.assertIn("int UsePos(Derived *param_1,int param_2)", self.decompile())
        rec = self.apply(dry=True)
        self.assertEqual(rec["changes"], 0, self.report(dry=True)["entries"])

        self.probe("--rename", "0x08049770:MyPeek")
        rec = self.apply()
        manual = [e for e in self.report()["entries"] if e["kind"] == "manual"]
        self.assertEqual(len(manual), 1, self.report()["entries"])
        self.assertIn("0x08049770", manual[0]["id"])
        self.assertIn("MyPeek", manual[0]["message"])
        peek = [f for f in self.state()["funcs"] if f["addr"] == "0x08049770"][0]
        self.assertEqual((peek["name"], peek["name_source"]), ("Peek", "IMPORTED"))

        db_path = self.ctx.typedb_json
        db = json.loads(db_path.read_text(encoding="utf-8"))
        db["records"] = [r for r in db["records"] if r["name"] != "Removed"]
        db["types"].pop("Removed")
        db_path.write_text(json.dumps(db, indent=1, sort_keys=True) + "\n", encoding="utf-8", newline="\n")
        self.assertTrue(any(t["id"] == "/civ5re/Removed" for t in self.state()["types"]))
        rec = self.apply()
        self.assertEqual(rec["changes"], 1, self.report()["entries"])
        self.assertFalse(any(t["id"] == "/civ5re/Removed" for t in self.state()["types"]))
        self.assertTrue(any(e["kind"] == "stale" and e["id"] == "/civ5re/Removed" for e in self.report()["entries"]))
        rec = self.apply(dry=True)
        self.assertEqual(rec["changes"], 0)

        times["total"] = time.monotonic() - t0
        print("\n[ghidra integration] " + " ".join(f"{k}={v:.1f}s" for k, v in times.items()))
        self.assertLess(times["total"], 120)


if __name__ == "__main__":
    unittest.main()
