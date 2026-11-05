import contextlib
import io
import sys
import tempfile
import unittest
from pathlib import Path

from civ5re import __main__ as dispatch
from civ5re import placeholders as ph


class FakeCli:
    def __init__(self, *commands):
        self.commands = commands

    def register(self, subparsers):
        for name in self.commands:
            p = subparsers.add_parser(name)
            p.set_defaults(func=lambda args, n=name: 0)


class Dispatcher(unittest.TestCase):
    def test_modules_registered(self):
        parser = dispatch.build_parser(["a", "b"], import_cli=lambda m: FakeCli(m))
        self.assertEqual(parser.parse_args(["a"]).command, "a")
        self.assertEqual(parser.parse_args(["b"]).command, "b")

    def test_duplicate_command_is_startup_error(self):
        clis = {"a": FakeCli("same"), "b": FakeCli("same")}
        with self.assertRaisesRegex(dispatch.DispatchError, "duplicate command: same"):
            dispatch.build_parser(["a", "b"], import_cli=clis.get)

    def test_missing_package_is_skipped(self):
        parser = dispatch.build_parser(["no_such_package_xyz"])
        self.assertIsNone(parser.parse_args([]).command)

    def test_import_error_inside_existing_package_propagates(self):
        with tempfile.TemporaryDirectory() as d:
            pkg = Path(d) / "civ5re_tmp_pkg"
            pkg.mkdir()
            (pkg / "__init__.py").write_text("")
            (pkg / "cli.py").write_text("import civ5re_missing_dependency\n")
            sys.path.insert(0, d)
            self.addCleanup(sys.path.remove, d)
            self.addCleanup(sys.modules.pop, "civ5re_tmp_pkg", None)
            with self.assertRaises(ModuleNotFoundError):
                dispatch._import_cli("civ5re_tmp_pkg")

    def test_binfacts_registered(self):
        self.assertEqual(dispatch.MODULES[0], "binfacts")
        args = dispatch.build_parser().parse_args(["binfacts", "find", "x"])
        self.assertEqual((args.command, args.sub, args.regex), ("binfacts", "find", "x"))
        self.assertTrue(callable(args.func))

    def test_main_without_command_prints_help(self):
        out = io.StringIO()
        with contextlib.redirect_stdout(out):
            self.assertEqual(dispatch.main([]), 2)
        self.assertIn("binfacts", out.getvalue())

    def test_config_error_is_reported_without_traceback(self):
        from civ5re.config import ConfigError

        class Boom:
            @staticmethod
            def register(subparsers):
                def boom(args):
                    raise ConfigError("bad config")
                subparsers.add_parser("a").set_defaults(func=boom)
        self.addCleanup(setattr, dispatch, "MODULES", dispatch.MODULES)
        self.addCleanup(setattr, dispatch, "_import_cli", dispatch._import_cli)
        dispatch.MODULES = ["a"]
        dispatch._import_cli = lambda m: Boom
        err = io.StringIO()
        with contextlib.redirect_stderr(err):
            self.assertEqual(dispatch.main(["a"]), 2)
        self.assertEqual(err.getvalue().strip(), "civ5re: bad config")


class Placeholders(unittest.TestCase):
    def test_classify(self):
        cases = {
            "Fun_0834a1c0": "fun", "g_0834a1c0": "global", "Class_0a1b2c40": "class",
            "field_14": "field", "field_0": "field", "unk_14": "unk", "vf_12": "vf", "vf_105": "vf",
            "a1": "param", "a12": "param", "UNK_RET": "unk_ret", "UNK_PARAMS": "unk_params",
            "UNK_DATA": "unk_data",
        }
        for name, kind in cases.items():
            self.assertEqual(ph.classify(name).kind, kind, name)

    def test_values(self):
        self.assertEqual(ph.classify("Fun_0834a1c0").value, "0834a1c0")
        self.assertEqual(ph.classify("vf_07").value, "07")
        self.assertEqual(ph.classify("a3").value, "3")

    def test_non_placeholders(self):
        for name in ("Fun_834a1c0", "Fun_0834A1C0", "field_014", "field_0x14", "vf_7", "a0", "a01",
                     "m_iCount", "FUN_0834a1c0", "unk_", "UNK_RETURN", "g_0834a1c0x"):
            self.assertIsNone(ph.classify(name), name)

    def test_ghidra_style(self):
        for name in ("FUN_0834a1c0", "DAT_0834a1c0", "field_0x14", "PTR_0834a1c0"):
            self.assertTrue(ph.is_ghidra_style(name), name)
        for name in ("Fun_0834a1c0", "field_14", "FUNCTION"):
            self.assertFalse(ph.is_ghidra_style(name), name)

    def test_kinds_and_builders_round_trip(self):
        self.assertEqual(len(ph.kinds()), len(set(ph.kinds())))
        self.assertEqual(ph.classify(ph.fun_name(0x834a1c0)).kind, "fun")
        self.assertEqual(ph.classify(ph.global_name(0x8)).value, "00000008")
        self.assertEqual(ph.classify(ph.class_name(0xa1b2c40)).kind, "class")
        self.assertEqual(ph.field_name(0x14), "field_14")
        self.assertEqual(ph.classify(ph.unk_name(0)).kind, "unk")
        self.assertEqual(ph.vf_name(3), "vf_03")
        self.assertEqual(ph.classify(ph.vf_name(123)).value, "123")
        self.assertEqual(ph.param_name(2), "a2")


if __name__ == "__main__":
    unittest.main()
