import json
import shutil
import tempfile
import unittest
from pathlib import Path

from binfacts import api, build
from binfacts.api import BinfactsError
from binfacts.tests import support
from binfacts.tests.support import FIXTURES, FIXTURE_ELF
from civ5re import config

LLVM = support.llvm_bin()


class Root:
    """A temporary repository root with bins.lock for the fixture and an optional cache."""

    def __init__(self, test, with_llvm=False, with_binary=False):
        self.dir = Path(tempfile.mkdtemp())
        test.addCleanup(shutil.rmtree, self.dir, True)
        data = FIXTURE_ELF.read_bytes()
        self.sha = build.sha256_bytes(data)
        (self.dir / "bins.lock").write_text(
            f'[[file]]\npath = "fixture.elf"\nrole = "target"\nsha256 = "{self.sha}"\nsize = {len(data)}\norigin = "test"\n',
            encoding="utf-8")
        env = {"CIV5RE_LLVM_DIR": str(LLVM.parent) if with_llvm else str(self.dir / "no-llvm")}
        if with_binary:
            env["CIV5RE_BIN_DIR"] = str(FIXTURES)
        self.cfg = config.load(root=self.dir, environ=env)
        self.path = build.out_path(self.cfg)

    def write_cache(self, **tool):
        facts = support.load_expected()
        cxx = symbols_version() if LLVM else "none"
        facts["tool"] = {"binfacts": build.MODULE_VERSION, "cxxfilt": cxx, "groups_cfg": build.groups_sha()}
        facts["tool"].update(tool)
        build.write(self.path, facts)
        return facts


def symbols_version():
    from binfacts import symbols
    return symbols.Demangler(config.load().tool("llvm-cxxfilt")).get_version()


class LoadByPath(unittest.TestCase):
    def test_reads_without_freshness_check(self):
        facts = api.load(FIXTURES / "expected.json")
        self.assertEqual(facts["schema"], api.SCHEMA)
        self.assertEqual(facts["tool"]["groups_cfg"], "")

    def test_schema_is_verified(self):
        d = Path(tempfile.mkdtemp())
        self.addCleanup(shutil.rmtree, d, True)
        p = d / "x.json"
        p.write_text(json.dumps({"schema": 99}), encoding="utf-8")
        with self.assertRaisesRegex(BinfactsError, "schema"):
            api.load(p)

    def test_missing_or_broken_file(self):
        with self.assertRaises(BinfactsError):
            api.load(FIXTURES / "nope.json")
        d = Path(tempfile.mkdtemp())
        self.addCleanup(shutil.rmtree, d, True)
        (d / "bad.json").write_text("{", encoding="utf-8")
        with self.assertRaises(BinfactsError):
            api.load(d / "bad.json")


class Cache(unittest.TestCase):
    def test_fresh_cache_is_returned_without_the_binary(self):
        r = Root(self)
        r.write_cache()
        facts, action = build.ensure(r.cfg)
        self.assertEqual(action, "fresh")
        self.assertEqual(facts["binary"]["name"], "fixture.elf")

    def test_stale_without_the_binary_is_an_error_with_the_reason(self):
        for tool, reason in (({"binfacts": "0"}, "module version"), ({"groups_cfg": "x"}, "groups.cfg")):
            r = Root(self)
            r.write_cache(**tool)
            with self.assertRaisesRegex(BinfactsError, reason):
                build.ensure(r.cfg)

    def test_other_binary_in_bins_lock_makes_the_cache_stale(self):
        r = Root(self)
        facts = r.write_cache()
        facts["binary"]["sha256"] = "0" * 64
        build.write(r.path, facts)
        with self.assertRaisesRegex(BinfactsError, "differs from bins.lock"):
            build.ensure(r.cfg)

    def test_missing_cache_without_binary(self):
        r = Root(self)
        with self.assertRaisesRegex(BinfactsError, "bin_dir"):
            build.ensure(r.cfg)

    @unittest.skipUnless(LLVM, "LLVM is not available")
    def test_stale_cache_is_rebuilt_when_binary_and_llvm_exist(self):
        r = Root(self, with_llvm=True, with_binary=True)
        r.write_cache(binfacts="0")
        facts, action = build.ensure(r.cfg)
        self.assertEqual(action, "built")
        self.assertEqual(facts["tool"]["binfacts"], build.MODULE_VERSION)
        self.assertEqual(build.ensure(r.cfg)[1], "fresh")

    @unittest.skipUnless(LLVM, "LLVM is not available")
    def test_other_cxxfilt_version_makes_the_cache_stale(self):
        r = Root(self, with_llvm=True, with_binary=True)
        r.write_cache(cxxfilt="LLVM 1")
        self.assertEqual(build.ensure(r.cfg)[1], "built")

    @unittest.skipUnless(LLVM, "LLVM is not available")
    def test_force_rebuilds_and_gives_identical_bytes(self):
        r = Root(self, with_llvm=True, with_binary=True)
        build.ensure(r.cfg, force=True)
        first = r.path.read_bytes()
        self.assertEqual(build.ensure(r.cfg)[1], "fresh")
        self.assertEqual(build.ensure(r.cfg, force=True)[1], "built")
        self.assertEqual(r.path.read_bytes(), first)

    @unittest.skipUnless(LLVM, "LLVM is not available")
    def test_binary_that_does_not_match_bins_lock_is_refused(self):
        r = Root(self, with_llvm=True, with_binary=True)
        text = (r.dir / "bins.lock").read_text(encoding="utf-8").replace(r.sha, "1" * 64)
        (r.dir / "bins.lock").write_text(text, encoding="utf-8")
        with self.assertRaisesRegex(BinfactsError, "sha256"):
            build.ensure(r.cfg, force=True)

    @unittest.skipUnless(LLVM, "LLVM is not available")
    def test_missing_llvm_cxxfilt_is_reported(self):
        r = Root(self, with_llvm=False, with_binary=True)
        with self.assertRaisesRegex(BinfactsError, "llvm-cxxfilt"):
            build.ensure(r.cfg, force=True)


if __name__ == "__main__":
    unittest.main()
