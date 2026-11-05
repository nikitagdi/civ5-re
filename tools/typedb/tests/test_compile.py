import shutil
import tempfile
import unittest
from pathlib import Path
from unittest import mock

from typedb import compile as cc

from .util import THIRD_PARTY

TREE = Path("C:/t/Civ5Expansion2") if Path("C:/").exists() else Path("/t/Civ5Expansion2")
BUILD = TREE.parent / "build"


def fake_env(tp=THIRD_PARTY):
    res = Path("/llvm/lib/clang/22/include")
    dirs = cc.read_include_dirs(tp.resolve(), res)
    return cc.Env(tp.resolve(), dirs, tp.resolve() / "pch.h", tp.resolve() / cc.PRELUDE,
                  Path("/llvm/bin/clang++"), Path("/llvm/bin/llvm-cxxfilt"), res)


def rel(args, env):
    return cc.relative_flags(args, env, tree=TREE, build=BUILD)


class ClangArgs(unittest.TestCase):
    def setUp(self):
        self.env = fake_env()
        common = ["--target=i386-pc-linux-gnu", "-std=gnu++11", "-nostdinc", "-nostdinc++",
                  "-isystem", "<third_party>/compat", "-isystem", "<third_party>/fwd",
                  "-isystem", "<third_party>/libcxx/v1", "-isystem", "<clang>",
                  "-ferror-limit=0", "-fno-color-diagnostics"]
        self.common = common

    def test_snapshot_per_mode(self):
        e = self.env
        self.assertEqual(rel(cc.clang_args("pch", e, build=BUILD), e),
                         self.common + ["-x", "c++-header", "<third_party>/pch.h", "-o", "<build>/third_party.pch"])
        self.assertEqual(rel(cc.clang_args("tree", e, tree=TREE, build=BUILD), e),
                         self.common + ["-include-pch", "<build>/third_party.pch", "-isystem", "<tree>"])
        self.assertEqual(rel(cc.clang_args("cpp", e, tree=TREE, build=BUILD), e),
                         self.common + ["-include-pch", "<build>/headers.pch", "-I", "<tree>"])
        self.assertEqual(rel(cc.clang_args("alone", e, tree=TREE), e),
                         self.common + ["-include", "<third_party>/compat/civ5re_prelude.h", "-I", "<tree>"])

    def test_pch_override(self):
        e = self.env
        pch = cc.work_dir(BUILD) / "probe.pch"
        self.assertEqual(rel(cc.clang_args("tree", e, tree=TREE, build=BUILD, pch=pch), e),
                         self.common + ["-include-pch", "<build>/probe.pch", "-isystem", "<tree>"])
        self.assertEqual(rel(cc.clang_args("cpp", e, tree=TREE, build=BUILD, pch=pch), e),
                         self.common + ["-include-pch", "<build>/probe.pch", "-I", "<tree>"])

    def test_no_ms_modes(self):
        for mode in ("tree", "cpp", "alone"):
            args = cc.clang_args(mode, self.env, tree=TREE, build=BUILD)
            self.assertFalse([a for a in args if a.startswith("-fms") or "_WIN32" in a or "_MSC_VER" in a])

    def test_unknown_mode(self):
        with self.assertRaises(ValueError):
            cc.clang_args("other", self.env, tree=TREE, build=BUILD)


class EnvFiles(unittest.TestCase):
    def setUp(self):
        self.tmp = Path(tempfile.mkdtemp())
        self.tp = self.tmp / "third_party"
        shutil.copytree(THIRD_PARTY, self.tp)
        self.addCleanup(shutil.rmtree, self.tmp, True)

    def _load(self):
        with mock.patch.object(cc, "tools", return_value=(Path("/x/clang++"), Path("/x/llvm-cxxfilt"))), \
                mock.patch.object(cc, "resource_include", return_value=Path("/x/lib/clang/22/include")):
            return cc.load_env(self.tp)

    def test_fixture_env_is_valid(self):
        env = self._load()
        self.assertEqual(env.include_dirs[-1], Path("/x/lib/clang/22/include"))
        self.assertEqual(len(env.include_dirs), 4)

    def test_broken_include_dirs(self):
        for text in ("compat\nmissing_dir\n<clang>\n", "compat\nfwd\n", "C:\\abs\n<clang>\n"):
            with self.subTest(text=text):
                (self.tp / "include_dirs.txt").write_text(text, encoding="utf-8")
                with self.assertRaises(cc.EnvError):
                    self._load()

    def test_comments_and_blank_lines_skipped(self):
        (self.tp / "include_dirs.txt").write_text("# c\n\ncompat\n<clang>\n", encoding="utf-8")
        self.assertEqual(len(self._load().include_dirs), 2)

    def test_prelude_not_first_in_pch(self):
        for text in ('#include <vector>\n#include "compat/civ5re_prelude.h"\n',
                     '#include "compat/civ5re_prelude.h"\nint x;\n', "// only a comment\n"):
            with self.subTest(text=text):
                (self.tp / "pch.h").write_text(text, encoding="utf-8")
                with self.assertRaises(cc.EnvError):
                    self._load()

    def test_missing_files(self):
        (self.tp / "pch.h").unlink()
        with self.assertRaises(cc.EnvError):
            self._load()

    def test_cli_exit_code_2(self):
        import io
        from contextlib import redirect_stderr
        from typedb import cli
        (self.tp / "pch.h").write_text("int x;\n", encoding="utf-8")
        args = mock.Mock(tree=str(self.tmp), build=str(self.tmp / "b"), third_party=str(self.tp), no_markers=False,
                         force=False, keep=False)
        (self.tmp / "Civ5Expansion2").mkdir()
        with redirect_stderr(io.StringIO()), \
                mock.patch.object(cc, "tools", return_value=(Path("/x/clang++"), Path("/x/llvm-cxxfilt"))), \
                mock.patch.object(cc, "resource_include", return_value=Path("/x/inc")):
            self.assertEqual(cli._run(args), 2)


class SinglePlaceOfFlags(unittest.TestCase):
    """typedb acceptance 4: checks and seed keep no clang flags of their own."""

    def test_no_flags_outside_typedb(self):
        tools = Path(__file__).resolve().parents[2]
        bad = []
        for module in ("checks", "seed", "ghidra", "luaapi"):
            for p in (tools / module).rglob("*.py") if (tools / module).is_dir() else ():
                if "tests" in p.parts or "prototype" in p.parts:
                    continue
                text = p.read_text(encoding="utf-8", errors="replace")
                for flag in ("--target=i386", "-nostdinc", "-std=gnu++", "-include-pch", "-fms-"):
                    if flag in text:
                        bad.append(f"{p.relative_to(tools)}: {flag}")
        self.assertEqual(bad, [], "use typedb.api.clang_args(mode) instead")


if __name__ == "__main__":
    unittest.main()
