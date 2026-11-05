import contextlib
import io
import unittest
from unittest import mock

from binfacts import cli
from binfacts.tests import support
from civ5re import __main__ as dispatch


def run(*argv):
    out, err = io.StringIO(), io.StringIO()
    facts = support.load_expected()
    with mock.patch.object(cli, "_facts", return_value=(facts, "fresh", 0.0)), \
            contextlib.redirect_stdout(out), contextlib.redirect_stderr(err):
        code = dispatch.main(["binfacts", *argv])
    return code, out.getvalue(), err.getvalue()


class Commands(unittest.TestCase):
    def test_registered_with_the_dispatcher(self):
        self.assertEqual(dispatch.MODULES[0], "binfacts")
        args = dispatch.build_parser().parse_args(["binfacts", "--force"])
        self.assertTrue(args.force)
        args = dispatch.build_parser().parse_args(["binfacts", "vtable", "ns::Shape"])
        self.assertEqual((args.sub, args.cls), ("vtable", "ns::Shape"))

    def test_build_summary(self):
        code, out, _ = run()
        self.assertEqual(code, 0)
        self.assertIn("fresh", out)
        self.assertIn("denominator:", out)
        self.assertIn("= 88 symbols", out)

    def test_bytes_arguments(self):
        args = dispatch.build_parser().parse_args(["binfacts", "bytes", "0x0a30f834", "16", "--as", "ptr"])
        self.assertEqual((args.sub, args.addr, args.len, args.as_), ("bytes", "0x0a30f834", 16, "ptr"))

    def test_find(self):
        code, out, err = run("find", r"Shape::(touch|area)")
        self.assertEqual(code, 0)
        lines = out.strip().splitlines()
        self.assertEqual(len(lines), 1)
        self.assertRegex(lines[0], r"^0x[0-9a-f]{8} +\d+ method +firaxis +ns::Shape::touch\(\)$")
        self.assertIn("1 symbols", err)

    def test_find_without_matches_and_bad_regex(self):
        self.assertEqual(run("find", "no_such_symbol_xyz")[0], 1)
        code, _out, err = run("find", "(")
        self.assertEqual(code, 2)
        self.assertIn("bad regex", err)

    def test_vtable_slots_by_group(self):
        code, out, _ = run("vtable", "ns::Mix")
        self.assertEqual(code, 0)
        self.assertIn("## ns::Mix  vtable _ZTVN2ns3MixE", out)
        self.assertIn("group 1: offset_to_top -8", out)
        self.assertRegex(out, r"\[0\] func +0x[0-9a-f]{8} firaxis +ns::Mix::~Mix\(\)")
        self.assertRegex(out, r"\[2\] thunk +0x[0-9a-f]{8} firaxis +non-virtual thunk to ns::Mix::b\(\)")

    def test_vtable_pure_and_import_slots(self):
        _code, out, _ = run("vtable", "ns::Shape")
        self.assertRegex(out, r"\[2\] pure +0x[0-9a-f]{8} - +__cxa_pure_virtual")
        _code, out, _ = run("vtable", "ns::UsesExt")
        self.assertRegex(out, r"\[0\] import ")

    def test_vtable_matches_a_class_name_part(self):
        code, out, _ = run("vtable", "Square")
        self.assertEqual(code, 0)
        self.assertIn("## ns::Square", out)
        self.assertEqual(run("vtable", "NoSuchClass")[0], 1)

    def test_report(self):
        code, out, _ = run("report")
        self.assertEqual(code, 0)
        for text in ("unclassified 0", "groups (symbols):", "firaxis: top prefixes", "coverage denominator",
                     "covered by class:", "vtables with slots without a symbol name"):
            self.assertIn(text, out)
        self.assertRegex(out, r"\n +\d+  ns\n")


if __name__ == "__main__":
    unittest.main()
