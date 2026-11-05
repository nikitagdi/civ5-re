import hashlib
import os
import shutil
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path

REPO = Path(__file__).resolve().parents[3]
GIT = shutil.which("git")


def snapshot(root):
    snap = {}
    for p in sorted(Path(root).rglob("*")):
        if p.is_file() and ".git" not in p.relative_to(root).parts[:1] and "__pycache__" not in p.parts:
            snap[str(p.relative_to(root))] = (hashlib.sha256(p.read_bytes()).hexdigest(), p.stat().st_mtime_ns)
    return snap


@unittest.skipUnless(GIT, "git not found")
class BootstrapOnClone(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls._tmp = tempfile.TemporaryDirectory()
        cls.root = Path(cls._tmp.name) / "clone"
        cls.root.mkdir()
        subprocess.run([GIT, "init", "-q"], cwd=cls.root, check=True)
        shutil.copy(REPO / "civ5re.cfg", cls.root)
        shutil.copy(REPO / "bins.lock", cls.root)
        shutil.copytree(REPO / "tools" / "civ5re", cls.root / "tools" / "civ5re",
                        ignore=shutil.ignore_patterns("__pycache__", "tests"))
        shutil.copy(REPO / "tools" / "bootstrap.py", cls.root / "tools" / "bootstrap.py")
        cls.llvm = Path(cls._tmp.name) / "llvm"
        (cls.llvm / "bin").mkdir(parents=True)
        suffix = ".exe" if os.name == "nt" else ""
        for tool in ("llvm-cxxfilt", "clang++", "ld.lld"):
            (cls.llvm / "bin" / (tool + suffix)).write_text("")
        cls.project = Path(cls._tmp.name) / "project"
        cls.project.mkdir()
        cls.bins = Path(cls._tmp.name) / "bins"
        cls.bins.mkdir()

    @classmethod
    def tearDownClass(cls):
        cls._tmp.cleanup()

    def env(self, **extra):
        env = {k: v for k, v in os.environ.items() if not k.startswith("CIV5RE_")}
        env.update(extra)
        return env

    def run_bootstrap(self, **extra):
        return subprocess.run([sys.executable, str(self.root / "tools" / "bootstrap.py")], cwd=self.root,
                              env=self.env(**extra), capture_output=True, text=True)

    def hooks_path(self):
        return subprocess.run([GIT, "config", "--get", "core.hooksPath"], cwd=self.root,
                              capture_output=True, text=True).stdout.strip()

    def test_clone_flow(self):
        res = self.run_bootstrap(CIV5RE_LLVM_DIR=str(self.llvm), CIV5RE_BIN_DIR=str(self.bins))
        self.assertEqual(res.returncode, 1, res.stdout + res.stderr)
        self.assertIn("ghidra_project_dir", res.stdout)
        self.assertIn("CIV5RE_GHIDRA_PROJECT_DIR", res.stdout)
        self.assertEqual(self.hooks_path(), ".githooks")
        self.assertIn("pre-commit: not installed", res.stdout)
        template = self.root / "civ5re.local.cfg"
        text = template.read_text(encoding="utf-8")
        for key in ("llvm_dir", "ghidra_dir", "ghidra_project_dir", "bin_dir", "game_dir",
                    "lekmod_dir", "out"):
            self.assertRegex(text, rf"(?m)^{key} = ")
        self.assertTrue((self.root / ".venv").is_dir())
        pths = list((self.root / ".venv").rglob("civ5re.pth"))
        self.assertEqual(len(pths), 1)
        self.assertEqual(pths[0].read_text(encoding="utf-8").strip(), str(self.root / "tools"))

        env = dict(CIV5RE_LLVM_DIR=str(self.llvm), CIV5RE_BIN_DIR=str(self.bins),
                   CIV5RE_GHIDRA_PROJECT_DIR=str(self.project))
        res = self.run_bootstrap(**env)
        self.assertEqual(res.returncode, 0, res.stdout + res.stderr)
        self.assertIn("warning:", res.stdout)
        self.assertEqual(template.read_text(encoding="utf-8"), text)

        before = snapshot(self.root)
        again = self.run_bootstrap(**env)
        self.assertEqual(again.returncode, 0, again.stdout + again.stderr)
        self.assertNotIn("changed:", again.stdout)
        self.assertEqual(snapshot(self.root), before)
        self.assertEqual(self.hooks_path(), ".githooks")

    def test_existing_local_cfg_is_not_touched(self):
        # runs after test_clone_flow alphabetically; only needs the clone to exist
        local = self.root / "civ5re.local.cfg"
        local.write_text("# mine\n", encoding="utf-8")
        res = self.run_bootstrap(CIV5RE_LLVM_DIR=str(self.llvm), CIV5RE_GHIDRA_PROJECT_DIR=str(self.project))
        self.assertEqual(res.returncode, 0, res.stdout + res.stderr)
        self.assertEqual(local.read_text(encoding="utf-8"), "# mine\n")

    def test_zz_bad_config_and_bins_lock_mismatch(self):
        local = self.root / "civ5re.local.cfg"
        local.write_text("bogus = 1\n", encoding="utf-8")
        res = self.run_bootstrap(CIV5RE_LLVM_DIR=str(self.llvm))
        self.assertEqual(res.returncode, 1)
        self.assertIn("unknown key bogus", res.stdout)

        local.write_text("", encoding="utf-8")
        target = self.bins / "linux_1.0.3.279"
        target.mkdir(exist_ok=True)
        (target / "Civ5XP").write_bytes(b"not the real binary")
        res = self.run_bootstrap(CIV5RE_LLVM_DIR=str(self.llvm), CIV5RE_BIN_DIR=str(self.bins),
                                 CIV5RE_GHIDRA_PROJECT_DIR=str(self.project))
        self.assertEqual(res.returncode, 1)
        self.assertIn("error: linux_1.0.3.279/Civ5XP: size", res.stdout)


if __name__ == "__main__":
    unittest.main()
