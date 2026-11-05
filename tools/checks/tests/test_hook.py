"""The pre-commit hook against a temporary git repository with a stub `civ5re` (no real commit in this repo)."""
import os
import shutil
import subprocess
import sys
import unittest
from pathlib import Path

from .helpers import TmpCase, git, init_repo, write

REPO = Path(__file__).resolve().parents[3]
HOOK = REPO / ".githooks" / "pre-commit"
STUB = "import os, sys\nprint('stub civ5re ' + ' '.join(sys.argv[1:]))\nsys.exit(int(os.environ['STUB_CODE']))\n"


def find_sh():
    sh = shutil.which("sh")
    if sh:
        return sh
    g = shutil.which("git")
    if g:
        for rel in ("../../usr/bin/sh.exe", "../usr/bin/sh.exe"):
            p = (Path(g).parent / rel).resolve()
            if p.is_file():
                return str(p)
    return None


def link_dir(link, target):
    if os.name == "nt":
        r = subprocess.run(["cmd", "/c", "mklink", "/J", str(link), str(target)], capture_output=True)
        return r.returncode == 0
    os.symlink(target, link)
    return True


@unittest.skipUnless(HOOK.is_file() and find_sh(), "hook or sh is missing")
class PreCommitHook(TmpCase):
    def setUp(self):
        super().setUp()
        init_repo(self.dir)
        hook = self.dir / ".githooks" / "pre-commit"
        hook.parent.mkdir()
        hook.write_bytes(HOOK.read_bytes())
        hook.chmod(0o755)
        git(self.dir, "config", "core.hooksPath", ".githooks")
        write(self.dir, {"stub/civ5re/__init__.py": "", "stub/civ5re/__main__.py": STUB, "a.txt": "x\n"})
        git(self.dir, "add", "a.txt")

    def venv(self):
        if not link_dir(self.dir / ".venv", Path(sys.prefix)):
            self.skipTest("cannot link the interpreter directory")

    def commit(self, code):
        env = dict(os.environ, STUB_CODE=str(code), PYTHONPATH=str(self.dir / "stub"))
        env["PATH"] = os.path.dirname(find_sh()) + os.pathsep + env["PATH"]
        return subprocess.run(["git", "commit", "-m", "t"], cwd=self.dir, env=env, capture_output=True, text=True)

    def test_hook_file_is_lf_and_matches_the_spec(self):
        data = HOOK.read_bytes()
        self.assertNotIn(b"\r", data)
        self.assertTrue(data.startswith(b"#!/bin/sh\n"))
        self.assertIn(b"-m civ5re check --staged", data)

    def test_clean_run_lets_the_commit_through_with_staged_arguments(self):
        self.venv()
        r = self.commit(0)
        self.assertEqual(r.returncode, 0, r.stderr)
        self.assertIn("stub civ5re check --staged", r.stdout + r.stderr)
        self.assertIn("a.txt", git(self.dir, "show", "--stat", "--format=", "HEAD"))

    def test_errors_block_the_commit(self):
        self.venv()
        for code, text in ((1, "rules violated"), (2, "could not run"), (3, "selftest"), (9, "unexpected code 9")):
            r = self.commit(code)
            self.assertNotEqual(r.returncode, 0, code)
            self.assertIn(text, r.stderr)
        self.assertEqual(git(self.dir, "log", "--oneline").count("\n"), 1)     # only the init commit

    def test_missing_venv_blocks_with_a_bootstrap_hint(self):
        r = self.commit(0)
        self.assertNotEqual(r.returncode, 0)
        self.assertIn("bootstrap.py", r.stderr)


if __name__ == "__main__":
    unittest.main()
