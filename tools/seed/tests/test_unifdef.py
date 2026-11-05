import unittest

from seed import unifdef
from seed.unifdef import UnifdefError, process

PATS = ["AUI_*", "LEKMOD*", "INGAME_HOTKEY_MANAGER", "REPLAY_MESSAGE_EXTENDED"]


def run(src):
    return process(src, PATS)


class UnifdefTests(unittest.TestCase):
    def test_ifdef_mod_macro_is_dropped(self):
        self.assertEqual(run("a\n#ifdef AUI_X\nb\n#endif\nc"), "a\nc")

    def test_ifdef_else_keeps_else_branch(self):
        self.assertEqual(run("#ifdef AUI_X\nb\n#else\nc\n#endif\nd"), "c\nd")

    def test_ifndef_mod_macro_keeps_body(self):
        self.assertEqual(run("#ifndef AUI_Q\ng\n#endif"), "g")

    def test_defined_and_not_defined(self):
        self.assertEqual(run("#if defined(AUI_X) && !defined(T)\nd\n#endif"), "")

    def test_negated_mod_macro_simplifies_to_remaining(self):
        out = run("#if defined(Z) && !defined(AUI_Y)\nf\n#endif")
        self.assertEqual(out, "#if defined(Z)\nf\n#endif")

    def test_or_with_mod_macro(self):
        out = run("#if defined(AUI_W) || defined(K)\nh\n#else\ni\n#endif")
        self.assertEqual(out, "#if defined(K)\nh\n#else\ni\n#endif")

    def test_elif_chain(self):
        src = "#if defined(AUI_X)\na\n#elif defined(Q)\nb\n#else\nc\n#endif"
        self.assertEqual(run(src), "#if defined(Q)\nb\n#else\nc\n#endif")

    def test_elif_becomes_else_when_true(self):
        src = "#if defined(P)\na\n#elif !defined(AUI_X)\nb\n#endif"
        self.assertEqual(run(src), "#if defined(P)\na\n#else\nb\n#endif")

    def test_untouched_conditions_are_verbatim(self):
        src = "#  if defined( FOO )  // c\nx\n#endif"
        self.assertEqual(run(src), src)

    def test_nested_inside_dropped_region(self):
        src = "#ifdef AUI_X\n#if A\nx\n#else\ny\n#endif\n#endif\nz"
        self.assertEqual(run(src), "z")

    def test_pragma_message_removed(self):
        self.assertEqual(run('a\n#pragma message("x")\nb'), "a\nb")

    def test_unresolvable_is_error(self):
        with self.assertRaises(UnifdefError):
            run("#if AUI_X > 3\nq\n#endif")
        with self.assertRaises(UnifdefError):
            run("#if (AUI_X + 1)\nq\n#endif")

    def test_leftover_mod_macro_is_error(self):
        with self.assertRaises(UnifdefError):
            run("int AUI_VALUE;")

    def test_unterminated_is_error(self):
        with self.assertRaises(UnifdefError):
            run("#ifdef AUI_X\n")

    def test_patterns_file_loads(self):
        pats = unifdef.load_patterns()
        self.assertIn("AUI_*", pats)
        self.assertIn("REPLAY_MESSAGE_EXTENDED", pats)


if __name__ == "__main__":
    unittest.main()
