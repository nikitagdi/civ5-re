import subprocess
import tempfile
import unittest
from pathlib import Path

from checks.context import Settings

H = "/// origin binary\n#pragma once\n"


def write(root, files):
    for rel, text in files.items():
        p = Path(root) / rel
        p.parent.mkdir(parents=True, exist_ok=True)
        p.write_text(text, encoding="utf-8", newline="\n")


def git(root, *args):
    r = subprocess.run(["git", *args], cwd=root, capture_output=True, text=True)
    if r.returncode != 0:
        raise RuntimeError(f"git {' '.join(args)}: {r.stderr}")
    return r.stdout


def init_repo(root, files=None):
    git(root, "init", "-q")
    git(root, "config", "user.email", "t@example.com")
    git(root, "config", "user.name", "t")
    git(root, "config", "commit.gpgsign", "false")
    write(root, {"README.md": "x\n", **(files or {})})
    git(root, "add", "-A")
    git(root, "commit", "-q", "-m", "init")


class TmpCase(unittest.TestCase):
    def setUp(self):
        self._tmp = tempfile.TemporaryDirectory()
        self.addCleanup(self._tmp.cleanup)
        self.dir = Path(self._tmp.name).resolve()

    def settings(self, mode="full", **kw):
        kw.setdefault("selftest", False)
        return Settings(repo=self.dir, mode=mode, **kw)
