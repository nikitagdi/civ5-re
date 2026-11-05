"""seed env into temporary roots on the real inputs (about 45 s of mostly clang work).

The module sorts last on purpose: importing it (test discovery imports every module before the first test runs)
starts the two full runs in a background thread, the other test modules run meanwhile, and the first test here
waits for the result. Skipped without Lekmod, the WSL sources, LLVM or a binfacts cache.
"""
import json
import shutil
import tempfile
import threading
import unittest
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

from seed import env


class _Prepared:
    """What the background thread produces (or why it could not)."""
    skip = None
    error = None
    lekmod = None
    roots = ()
    lines = ()
    rcs = ()
    files = ()


_state = _Prepared()
_done = threading.Event()


def generated(root):
    return {p.relative_to(root).as_posix(): p.read_bytes() for p in root.rglob("*")
            if p.is_file() and "build" not in p.relative_to(root).parts}


def _inputs():
    from civ5re import config
    try:
        cfg = config.load()
        lekmod = cfg.path("seed", "lekmod_dir")
        libcxx = cfg.path("seed", "libcxx_src")
        libc = cfg.path("seed", "libc_src")
        from typedb import compile as cc
        cc.tools(cfg)
        from binfacts import api
        api.load()
    except Exception as e:  # noqa: BLE001 - any missing input means skip
        raise unittest.SkipTest(f"inputs unavailable: {e}") from None
    for name, p in (("lekmod_dir", lekmod and lekmod / ".git"), ("libcxx_src", libcxx), ("libc_src", libc)):
        if not p:
            raise unittest.SkipTest(f"[seed] {name} is not set in civ5re.local.cfg")
        if not Path(p).exists():
            raise unittest.SkipTest(f"missing {p}")
    return lekmod, libcxx, libc


def _prepare():
    try:
        lekmod, libcxx, libc = _inputs()
    except unittest.SkipTest as e:
        _state.skip = str(e)
        _done.set()
        return
    except BaseException as e:  # noqa: BLE001 - reported by the first test
        _state.error = e
        _done.set()
        return
    try:
        # Two independent full runs overlap in time; the second replaces a sequential repeat on the same root and
        # is compared with the first.
        roots = [Path(tempfile.mkdtemp(prefix="seed-env-")) for _ in range(2)]
        _state.roots = roots
        lines = [[], []]

        def run(i):
            rc = env.run(roots[i], lekmod, libcxx, libc, out=lines[i].append)
            return rc, (generated(roots[i]) if rc == 0 else {})
        with ThreadPoolExecutor(2) as ex:
            results = list(ex.map(run, range(2)))
        _state.lines = lines
        _state.rcs = [rc for rc, _ in results]
        _state.files = [files for _, files in results]
    except BaseException as e:  # noqa: BLE001 - reported by the first test
        _state.error = e
    finally:
        _done.set()


def _start():
    t = threading.Thread(target=_prepare, name="seed-env-live", daemon=True)
    t.start()
    return t


_thread = _start()


class IntegrationTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        _done.wait()
        if _state.skip:
            raise unittest.SkipTest(_state.skip)
        for r in _state.roots:
            cls.addClassCleanup(shutil.rmtree, r, True)
        if _state.error is not None:
            raise _state.error
        cls.roots, cls.lines, cls.rcs, cls.files = _state.roots, _state.lines, _state.rcs, _state.files

    def test_independent_runs_agree_and_match_the_plan(self):
        for rc, lines in zip(self.rcs, self.lines):
            self.assertEqual(rc, 0, "\n".join(lines))
        a, b = self.files
        self.assertEqual(a, b)
        # what a repeat on the written tree would see: nothing to write, nothing different
        writes, same, diff = env.plan(self.roots[0], b)
        self.assertEqual((writes, diff), ([], []))
        self.assertEqual(len(same), len(b))
        self.assertEqual(len(b), json.loads((self.roots[0] / "build" / "seed-env-report.json")
                                            .read_text(encoding="utf-8"))["files"])

    def test_env_output(self):
        root, lines = self.roots[0], self.lines[0]
        self.assertEqual(self.rcs[0], 0, "\n".join(lines))
        summary = json.loads((root / "build" / "seed-env-report.json").read_text(encoding="utf-8"))
        self.assertEqual(summary["different"], 0)
        self.assertEqual(summary["typedb_probe_failed"], [])
        sdk_files = [p for p in (root / "Civ5Expansion2").rglob("*") if p.is_file()]
        self.assertEqual(len(sdk_files), 98)
        first = [p.read_text(encoding="utf-8").split("\n")[0] for p in sdk_files]
        self.assertEqual(first.count("/// origin binary"), 67)
        self.assertEqual(first.count("/// origin guess"), 31)
        tp = root / "third_party"
        self.assertEqual((tp / "include_dirs.txt").read_text(encoding="utf-8"), env.include_dirs_text())
        self.assertTrue((tp / "pch.h").read_text(encoding="utf-8").startswith('#include "compat/civ5re_prelude.h"'))
        manifest = (tp / "MANIFEST.txt").read_text(encoding="utf-8")
        self.assertIn("\npch.h\t", manifest)
        self.assertIn("\ninclude_dirs.txt\t", manifest)
        from typedb import api as typedb_api
        out = typedb_api.build_db(root / "Civ5Expansion2", root / "build", markers="strict",
                                  third_party=root / "third_party")
        strict = json.loads(Path(out).read_text(encoding="utf-8"))
        self.assertEqual([d for d in strict["diagnostics"] if d["severity"] == "error"], [])
        self.assertTrue(strict["ok"])


if __name__ == "__main__":
    unittest.main()
