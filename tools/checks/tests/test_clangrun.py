import sys
import unittest
from unittest import mock

from checks import clangrun
from checks.api import InputError
from checks.context import Context

from .helpers import TmpCase, write

FAKE = r'''
import sys
if "--version" in sys.argv:
    print("fake clang 1.0")
    sys.exit(0)
text = sys.stdin.read()
import os, uuid
os.makedirs(sys.argv[1], exist_ok=True)
open(os.path.join(sys.argv[1], uuid.uuid4().hex), "w").close()   # one file per call: no shared-append race
if "BAD" in text:
    sys.stderr.write("E:\\proj\\Civ5Expansion2\\X.h:3:7: error: unknown type name 'BAD'\n")
    sys.stderr.write("E:\\proj\\Civ5Expansion2\\X.h:3:7: warning: ignored\n")
    sys.exit(1)
'''


class ClangRun(TmpCase):
    def setUp(self):
        super().setUp()
        write(self.dir, {"fake.py": FAKE, "Civ5Expansion2/A.h": "int a;\n", "Civ5Expansion2/B.h": "BAD\n"})
        self.log = self.dir / "calls"
        s = self.settings("root", root=self.dir, clang_prefix=[sys.executable, str(self.dir / "fake.py"), str(self.log)],
                          jobs=2)
        self.ctx = Context(s)
        patcher = mock.patch.object(Context, "clang_args", lambda self, mode="alone": ["-Ifoo"])
        patcher.start()
        self.addCleanup(patcher.stop)

    def calls(self):
        return len(list(self.log.iterdir())) if self.log.exists() else 0

    def jobs(self):
        return [clangrun.Job("A", '#include "A.h"\n', ["Civ5Expansion2/A.h"]),
                clangrun.Job("B", '#include "B.h"\n// BAD\n',["Civ5Expansion2/B.h"])]

    def test_results_and_cache(self):
        r = clangrun.run(self.ctx, self.jobs())
        self.assertTrue(r["A"].ok)
        self.assertFalse(r["B"].ok)
        d = r["B"].errors[0]
        self.assertEqual((d.line, d.col, d.message), (3, 7, "unknown type name 'BAD'"))
        self.assertEqual(len(r["B"].errors), 1)
        self.assertEqual(self.calls(), 2)
        self.assertFalse(r["A"].cached)
        again = clangrun.run(Context(self.ctx.settings), self.jobs())
        self.assertEqual(self.calls(), 2)
        self.assertTrue(again["A"].cached and again["B"].cached)
        self.assertEqual(again["B"].errors, r["B"].errors)

    def test_key_changes_with_contents_flags_and_version(self):
        j = clangrun.Job("A", "t", ["Civ5Expansion2/A.h"])
        k = clangrun.cache_key("v1", ["-a"], "tp", j, self.ctx.read)
        self.assertEqual(k, clangrun.cache_key("v1", ["-a"], "tp", j, self.ctx.read))
        self.assertNotEqual(k, clangrun.cache_key("v2", ["-a"], "tp", j, self.ctx.read))
        self.assertNotEqual(k, clangrun.cache_key("v1", ["-b"], "tp", j, self.ctx.read))
        self.assertNotEqual(k, clangrun.cache_key("v1", ["-a"], "tp2", j, self.ctx.read))
        self.assertNotEqual(k, clangrun.cache_key("v1", ["-a"], "tp", clangrun.Job("A", "u", j.closure), self.ctx.read))
        write(self.dir, {"Civ5Expansion2/A.h": "int b;\n"})
        self.assertNotEqual(k, clangrun.cache_key("v1", ["-a"], "tp", j, Context(self.ctx.settings).read))

    def test_edit_invalidates_only_that_job(self):
        clangrun.run(self.ctx, self.jobs())
        write(self.dir, {"Civ5Expansion2/A.h": "int c;\n"})
        clangrun.run(Context(self.ctx.settings), self.jobs())
        self.assertEqual(self.calls(), 3)

    def test_missing_clang_is_an_input_error(self):
        s = self.settings("root", root=self.dir)
        with self.assertRaises(InputError) as cm:
            clangrun.run(Context(s), self.jobs())
        self.assertIn(cm.exception.name, ("clang", "llvm_dir"))

    def test_parse_diagnostics(self):
        err = "In file included from a.cpp:1:\nC:/r/Civ5Expansion2/X.h:40:3: fatal error: 'q.h' file not found\nnote: x\n"
        d = clangrun.parse_diagnostics(err, "C:\\r")
        self.assertEqual([(x.path, x.line, x.col) for x in d], [("Civ5Expansion2/X.h", 40, 3)])


if __name__ == "__main__":
    unittest.main()
