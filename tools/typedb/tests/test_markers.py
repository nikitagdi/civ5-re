import unittest
from pathlib import Path

from typedb.markers import scan

FIX = Path(__file__).parent / "fixtures" / "markers"


def one(body):
    return scan("// x\n/// " + body + "\nint a;\n")


def marker(body):
    fs = one(body)
    assert not fs.diags, (body, fs.diags)
    (b,) = fs.blocks
    (m,) = b.markers
    return m


def codes(text, **kw):
    return [d.code for d in scan(text, **kw).diags]


class ValidForms(unittest.TestCase):
    def test_origin_binary(self):
        fs = scan("/// origin binary\nint a;\n")
        self.assertEqual((fs.origin.kind, fs.origin.value, fs.origin.line), ("origin", "binary", 1))
        self.assertEqual(fs.diags, ())
        self.assertEqual(fs.blocks, ())

    def test_origin_guess(self):
        self.assertEqual(scan("/// origin guess\n").origin.value, "guess")

    def test_size_hex(self):
        m = marker("size 0x2c")
        self.assertEqual((m.kind, m.value, m.spec, m.fastcc, m.trailing), ("size", "0x2c", None, False, False))

    def test_size_unknown(self):
        self.assertEqual(marker("size ?").value, "?")

    def test_size_spec(self):
        self.assertEqual(marker("size 0x2c FFastVector<int>").spec, "FFastVector<int>")

    def test_size_unknown_spec(self):
        m = marker("size ? ns::Foo<char, 4>")
        self.assertEqual((m.value, m.spec), ("?", "ns::Foo<char,4>"))

    def test_vtable(self):
        m = marker("vtable 0x0a1b2c40")
        self.assertEqual((m.kind, m.value), ("vtable", "0x0a1b2c40"))

    def test_vtable_unknown(self):
        self.assertEqual(marker("vtable ?").value, "?")

    def test_vtable_spec(self):
        self.assertEqual(marker("vtable 0x0a1b2c40 Foo<int>").spec, "Foo<int>")

    def test_vtable_unknown_spec(self):
        m = marker("vtable ? Foo<int>")
        self.assertEqual((m.value, m.spec), ("?", "Foo<int>"))

    def test_addr(self):
        m = marker("0x0834a1c0")
        self.assertEqual((m.kind, m.value, m.fastcc, m.spec), ("addr", "0x0834a1c0", False, None))

    def test_addr_fastcc(self):
        m = marker("0x0834a1c0 fastcc")
        self.assertEqual((m.fastcc, m.spec), (True, None))

    def test_addr_spec(self):
        self.assertEqual(marker("0x0834a1c0 Foo<int>").spec, "Foo<int>")

    def test_addr_fastcc_spec(self):
        m = marker("0x0834a1c0 fastcc Foo<int>")
        self.assertEqual((m.fastcc, m.spec), (True, "Foo<int>"))

    def test_base_address(self):
        m = marker("base 0x0834a1c0")
        self.assertEqual((m.kind, m.value), ("base", "0x0834a1c0"))

    def test_base_lone(self):
        m = marker("base")
        self.assertEqual((m.kind, m.value, m.spec), ("base", None, None))

    def test_base_lone_spec(self):
        m = marker("base Foo<int>")
        self.assertEqual((m.value, m.spec), (None, "Foo<int>"))

    def test_base_spec(self):
        m = marker("base 0x0834a1c0 Foo<int>")
        self.assertEqual((m.value, m.spec), ("0x0834a1c0", "Foo<int>"))

    def test_deleting(self):
        self.assertEqual(marker("deleting 0x0834a1c0").kind, "deleting")

    def test_deleting_spec(self):
        self.assertEqual(marker("deleting 0x0834a1c0 Foo<int>").spec, "Foo<int>")

    def test_thunk(self):
        self.assertEqual(marker("thunk 0x0834a1c0").kind, "thunk")

    def test_thunk_spec(self):
        self.assertEqual(marker("thunk 0x0834a1c0 Foo<int>").spec, "Foo<int>")

    def test_inline(self):
        m = marker("inline")
        self.assertEqual((m.kind, m.value), ("inline", None))

    def test_pure(self):
        self.assertEqual(marker("pure").kind, "pure")

    def test_absent(self):
        self.assertEqual(marker("absent").kind, "absent")

    def test_lua(self):
        m = marker("lua Foo.Bar(a, b)  = x")
        self.assertEqual((m.kind, m.value), ("lua", "Foo.Bar(a, b)  = x"))

    def test_lua_question(self):
        self.assertEqual(marker("lua ?").value, "?")

    def test_trailing_spaces_ignored(self):
        self.assertEqual(marker("size 0x2c   ").value, "0x2c")

    def test_trailing_tab_and_cr_ignored(self):
        fs = scan("// x\r\n/// pure\t\r\nint a;\r\n")
        self.assertEqual(fs.diags, ())
        self.assertEqual(fs.blocks[0].markers[0].kind, "pure")

    def test_indented_marker_col(self):
        fs = scan("\t/// pure\nint a;")
        self.assertEqual(fs.blocks[0].markers[0].col, 2)
        self.assertEqual(fs.diags, ())

    def test_trailing_offset(self):
        fs = scan("int m_a; ///< +0x10\n")
        m = fs.trailing[1]
        self.assertEqual((m.kind, m.value, m.trailing, m.col), ("offset", "0x10", True, 10))
        self.assertEqual(fs.diags, ())
        self.assertEqual(fs.blocks, ())

    def test_trailing_offset_large(self):
        self.assertEqual(scan("int a; ///< +0x1a2b\n").trailing[1].value, "0x1a2b")

    def test_size_uppercase_hex_rejected(self):
        self.assertEqual(codes("/// size 0x2C\n"), ["marker-syntax"])


class SpecNormalization(unittest.TestCase):
    def spec(self, s):
        return marker("size 0x1 " + s).spec

    def test_spaces_removed(self):
        self.assertEqual(self.spec("Foo < int , 4 >"), "Foo<int,4>")

    def test_unsigned_int_kept(self):
        self.assertEqual(self.spec("Foo<unsigned int>"), "Foo<unsigned int>")

    def test_pointer(self):
        self.assertEqual(self.spec("Foo<cvUnitVis *>"), "Foo<cvUnitVis*>")

    def test_nested(self):
        self.assertEqual(self.spec("A<B<int> >"), "A<B<int>>")

    def test_qualified(self):
        self.assertEqual(self.spec("ns :: Foo<char, 4>"), "ns::Foo<char,4>")

    def test_const_pointer(self):
        self.assertEqual(self.spec("Foo<const char *>"), "Foo<const char*>")

    def test_parenthesized_gt(self):
        self.assertEqual(self.spec("Foo<(1>2)>"), "Foo<(1>2)>")


class InvalidForms(unittest.TestCase):
    def syn(self, body):
        fs = one(body)
        self.assertEqual([d.code for d in fs.diags], ["marker-syntax"], body)
        self.assertEqual(fs.blocks, ())
        return fs.diags[0]

    def test_unknown_word(self):
        self.syn("frobnicate")

    def test_address_seven_digits(self):
        self.assertIn("8", self.syn("0x834a1c0").message)

    def test_address_nine_digits(self):
        self.syn("0x00834a1c0")

    def test_address_uppercase(self):
        self.assertIn("lowercase", self.syn("0x0834A1C0").message)

    def test_address_uppercase_x(self):
        self.syn("0X0834a1c0")

    def test_vtable_short(self):
        self.syn("vtable 0x1234")

    def test_vtable_uppercase(self):
        self.syn("vtable 0x0A1B2C40")

    def test_deleting_no_address(self):
        self.syn("deleting")

    def test_thunk_no_address(self):
        self.syn("thunk")

    def test_size_no_value(self):
        self.syn("size")

    def test_size_bad_value(self):
        self.syn("size 2c")

    def test_size_decimal(self):
        self.syn("size 44")

    def test_origin_bad_value(self):
        self.assertEqual(codes("/// origin maybe\n"), ["marker-syntax"])

    def test_origin_no_value(self):
        self.assertEqual(codes("/// origin\n"), ["marker-syntax"])

    def test_inline_with_argument(self):
        self.syn("inline Foo<int>")

    def test_pure_with_argument(self):
        self.syn("pure 0x0834a1c0")

    def test_absent_with_argument(self):
        self.syn("absent x")

    def test_lua_without_text(self):
        self.syn("lua")

    def test_lua_blank_text(self):
        self.assertEqual(codes("/// lua   \n"), ["marker-syntax"])

    def test_four_slashes(self):
        self.assertEqual(codes("//// size 0x2c\n"), ["marker-syntax"])

    def test_five_slashes_separator(self):
        self.assertEqual(codes("///// ------\n"), ["marker-syntax"])

    def test_empty_triple_slash(self):
        self.assertEqual(codes("///\nint a;"), ["marker-syntax"])

    def test_triple_slash_spaces_only(self):
        self.assertEqual(codes("///   \n"), ["marker-syntax"])

    def test_no_space(self):
        self.assertEqual(codes("///size 0x2c\n"), ["marker-syntax"])

    def test_two_spaces(self):
        self.assertEqual(codes("///  size 0x2c\n"), ["marker-syntax"])

    def test_tab_after_slashes(self):
        self.assertEqual(codes("///\tsize 0x2c\n"), ["marker-syntax"])

    def test_double_space_in_body(self):
        self.syn("size  0x2c")

    def test_unbalanced_spec(self):
        self.syn("size 0x2c Foo<int")

    def test_unbalanced_spec_extra_close(self):
        self.syn("size 0x2c Foo<int>>")

    def test_spec_trailing_text(self):
        self.syn("size 0x2c Foo<int> bar")

    def test_spec_not_template_id(self):
        self.syn("size 0x2c Foo")

    def test_fastcc_after_spec(self):
        self.syn("0x0834a1c0 Foo<int> fastcc")

    def test_fastcc_on_base(self):
        self.syn("base 0x0834a1c0 fastcc")

    def test_fastcc_on_thunk(self):
        self.syn("thunk 0x0834a1c0 fastcc")

    def test_addr_trailing_garbage(self):
        self.syn("0x0834a1c0 junk")

    def test_trailing_offset_bad(self):
        fs = scan("int a; ///< 0x10\n")
        self.assertEqual([d.code for d in fs.diags], ["marker-syntax"])
        self.assertEqual(fs.trailing, {})

    def test_trailing_offset_uppercase(self):
        self.assertEqual(codes("int a; ///< +0x1A\n"), ["marker-syntax"])

    def test_trailing_offset_no_space(self):
        self.assertEqual(codes("int a; ///<+0x10\n"), ["marker-syntax"])

    def test_trailing_offset_no_plus(self):
        self.assertEqual(codes("int a; ///< +10\n"), ["marker-syntax"])

    def test_diag_position(self):
        d = scan("int a;\n  /// bogus\n").diags[0]
        self.assertEqual((d.line, d.col), (2, 3))


class Misplaced(unittest.TestCase):
    def test_after_code(self):
        self.assertEqual(codes("int a; /// size 0x2c\n"), ["marker-misplaced"])

    def test_trailing_on_own_line(self):
        self.assertEqual(codes("///< +0x10\n"), ["marker-misplaced"])

    def test_origin_not_on_line_one(self):
        fs = scan("int a;\n/// origin binary\n")
        self.assertEqual([d.code for d in fs.diags], ["marker-misplaced"])
        self.assertIsNone(fs.origin)
        self.assertEqual(fs.blocks, ())

    def test_origin_not_in_block(self):
        fs = scan("/// origin binary\n/// size 0x2c\nclass A {};\n")
        self.assertEqual(len(fs.blocks), 1)
        self.assertEqual(fs.blocks[0].first_line, 2)


class Blocks(unittest.TestCase):
    def test_consecutive_lines(self):
        fs = scan("/// 0x0834a1c0\n/// base 0x0834a200\n/// deleting 0x0834a300\nvoid f();\n")
        (b,) = fs.blocks
        self.assertEqual((b.first_line, b.last_line, len(b.markers)), (1, 3, 3))

    def test_blank_line_splits(self):
        fs = scan("/// pure\n\n/// inline\nvoid f();\n")
        self.assertEqual([(b.first_line, b.last_line) for b in fs.blocks], [(1, 1), (3, 3)])

    def test_plain_comment_splits(self):
        self.assertEqual(len(scan("/// pure\n// note\n/// inline\nvoid f();\n").blocks), 2)

    def test_invalid_line_keeps_extent(self):
        fs = scan("/// pure\n/// bogus\n/// 0x0834a1c0\nvoid f();\n")
        (b,) = fs.blocks
        self.assertEqual((b.first_line, b.last_line, len(b.markers)), (1, 3, 2))

    def test_all_invalid_no_block(self):
        self.assertEqual(scan("/// bogus\nvoid f();\n").blocks, ())

    def test_multiple_thunks(self):
        fs = scan("/// thunk 0x0834a1c0\n/// thunk 0x0834a1d0\nvoid f();\n")
        self.assertEqual([m.kind for m in fs.blocks[0].markers], ["thunk", "thunk"])

    def test_marker_line_numbers(self):
        fs = scan("a;\n/// pure\n/// lua x\nb;\n")
        self.assertEqual([m.line for m in fs.blocks[0].markers], [2, 3])

    def test_fixture_clean(self):
        fs = scan((FIX / "clean.h").read_text())
        self.assertEqual(fs.diags, ())
        self.assertEqual(fs.origin.value, "binary")
        self.assertEqual([b.first_line for b in fs.blocks], [3, 8, 12])
        self.assertEqual(fs.trailing[11].value, "0x10")
        self.assertEqual(fs.comments[3], (2, "A cell of the map."))
        self.assertEqual(fs.comments[8], (7, "Returns the owner."))


class NearMarker(unittest.TestCase):
    def test_address(self):
        self.assertEqual(codes("// 0x0834a1c0\nvoid f();\n"), ["near-marker"])

    def test_pure(self):
        self.assertEqual(codes("// pure\n"), ["near-marker"])

    def test_absent(self):
        self.assertEqual(codes("//absent\n"), ["near-marker"])

    def test_inline(self):
        self.assertEqual(codes("// inline\n"), ["near-marker"])

    def test_size(self):
        self.assertEqual(codes("// size 0x2c\n"), ["near-marker"])

    def test_offset(self):
        self.assertEqual(codes("int a; //< +0x10\n"), ["near-marker"])

    def test_offset_spaced(self):
        self.assertEqual(codes("int a; // < +0x10\n"), ["near-marker"])

    def test_origin(self):
        self.assertEqual(codes("// origin binary\n"), ["near-marker"])

    def test_after_code(self):
        self.assertEqual(codes("void f(); // 0x0834a1c0\n"), ["near-marker"])

    def test_prose_not_caught(self):
        self.assertEqual(codes("// size of the cell\nint a;\n"), [])

    def test_prose_not_caught_far_from_block(self):
        self.assertEqual(codes("// size of the cell\n\n/// pure\nvoid f();\n"), [])

    def test_prose_pure_virtual(self):
        self.assertEqual(codes("// pure virtual interface\nint a;\n"), [])

    def test_adjacent_above_block_caught(self):
        fs = scan("// 0x0834a1c0 is the address\n/// size 0x2c\nclass A {};\n")
        self.assertEqual([(d.code, d.line) for d in fs.diags], [("near-marker", 1)])

    def test_adjacent_above_block_keyword_caught(self):
        self.assertEqual(codes("// size of the cell\n/// size 0x2c\nclass A {};\n"), ["near-marker"])

    def test_adjacent_below_block_caught(self):
        fs = scan("/// size 0x2c\n// vtable stuff\nclass A {};\n")
        self.assertEqual([(d.code, d.line) for d in fs.diags], [("near-marker", 2)])

    def test_not_double_reported(self):
        self.assertEqual(codes("// pure\n/// size 0x2c\nclass A {};\n"), ["near-marker"])

    def test_non_adjacent_not_caught(self):
        self.assertEqual(codes("// 0x1234 stuff\n\n/// size 0x2c\nclass A {};\n"), [])

    def test_non_keyword_above_block_fine(self):
        self.assertEqual(codes("// The cell.\n/// size 0x2c\nclass A {};\n"), [])

    def test_keyword_prefix_word_not_caught(self):
        self.assertEqual(codes("// sizeof matters\n/// size 0x2c\nclass A {};\n"), [])

    def test_block_comment_not_near(self):
        self.assertEqual(codes("/* 0x0834a1c0 */\n/* pure */\nint a;\n"), [])


class Lexer(unittest.TestCase):
    def test_marker_in_string(self):
        self.assertEqual(codes('const char *s = "/// size 0x2c";\n'), [])

    def test_near_in_string(self):
        self.assertEqual(codes('const char *s = "// pure";\n'), [])

    def test_marker_in_block_comment(self):
        fs = scan("/*\n/// size 0x2c\n// pure\n*/\nint a;\n")
        self.assertEqual((fs.blocks, fs.diags), ((), ()))

    def test_block_comment_address_not_near(self):
        self.assertEqual(codes("/* 0x0834a1c0 */ int a;\n"), [])

    def test_slashes_inside_block_comment_line(self):
        self.assertEqual(codes("/* // pure */ int a;\n"), [])

    def test_escaped_quote_in_string(self):
        self.assertEqual(codes('const char *s = "a\\"b"; /// size 0x2c\n'), ["marker-misplaced"])

    def test_char_literal_quote(self):
        self.assertEqual(codes("char c = '\"'; // pure\n"), ["near-marker"])

    def test_char_literal_slash(self):
        self.assertEqual(codes("char c = '/'; char d = '/';\n"), [])

    def test_raw_string(self):
        self.assertEqual(codes('auto s = R"x(\n/// bogus\n// pure\n)x";\n'), [])

    def test_raw_string_with_quote_and_paren(self):
        self.assertEqual(codes('auto s = R"x( ") /// bogus )x"; // plain\n'), [])

    def test_raw_string_prefixes(self):
        for p in ("LR", "uR", "UR", "u8R"):
            self.assertEqual(codes('auto s = ' + p + '"(/// bogus)";\n'), [], p)

    def test_raw_string_then_code(self):
        self.assertEqual(codes('auto s = R"(a)"; /// size 0x2c\n'), ["marker-misplaced"])

    def test_identifier_ending_in_r_not_raw(self):
        self.assertEqual(codes('auto s = fooR"(a)"; /// size 0x2c\n'), ["marker-misplaced"])

    def test_line_continuation_in_string(self):
        self.assertEqual(codes('const char *s = "a\\\n/// bogus";\n'), [])

    def test_line_continuation_in_line_comment(self):
        fs = scan("// text \\\n/// size 0x2c\nint a;\n")
        self.assertEqual((fs.blocks, fs.diags), ((), ()))

    def test_unterminated_char_ends_at_newline(self):
        self.assertEqual(codes("#error don't\n/// bogus\n"), ["marker-syntax"])

    def test_line_numbers_after_block_comment(self):
        fs = scan("/* a\nb */ /// pure\n")
        self.assertEqual([(d.code, d.line) for d in fs.diags], [("marker-misplaced", 2)])

    def test_line_numbers_after_raw_string(self):
        fs = scan('auto s = R"(\n\n)";\n/// bogus\n')
        self.assertEqual(fs.diags[0].line, 4)

    def test_bom_stripped(self):
        self.assertEqual(scan("﻿/// origin binary\n").origin.col, 1)


class CommentExtraction(unittest.TestCase):
    def test_above_block(self):
        fs = scan("// The cell.\n/// size 0x2c\nclass A {};\n")
        self.assertEqual(fs.comments[2], (1, "The cell."))

    def test_without_block(self):
        fs = scan("// First line\n//   indented\nvoid f();\n")
        self.assertEqual(fs.comments[3], (1, "First line\n  indented"))

    def test_blank_line_breaks(self):
        fs = scan("// far\n\nvoid f();\n")
        self.assertNotIn(3, fs.comments)

    def test_only_nearest_run(self):
        fs = scan("// one\n\n// two\n// three\nvoid f();\n")
        self.assertEqual(fs.comments[5], (3, "two\nthree"))

    def test_triple_slash_excluded(self):
        fs = scan("// a\n/// pure\n// b\nvoid f();\n")
        self.assertEqual(fs.comments[2], (1, "a"))
        self.assertEqual(fs.comments[4], (3, "b"))

    def test_block_comment_excluded(self):
        self.assertEqual(scan("/* c */\nvoid f();\n").comments, {})

    def test_block_comment_breaks_run(self):
        fs = scan("// a\n/* c */\n// b\nvoid f();\n")
        self.assertEqual(fs.comments[4], (3, "b"))

    def test_trailing_comment_excluded(self):
        self.assertEqual(scan("int a; // t\nvoid f();\n").comments, {})

    def test_empty_comment_line(self):
        self.assertEqual(scan("// a\n//\n// b\nvoid f();\n").comments[4], (1, "a\n\nb"))

    def test_indented_comment(self):
        self.assertEqual(scan("class A {\n\t// doc\n\tvoid f();\n};\n").comments[3], (2, "doc"))

    def test_trailing_whitespace_stripped(self):
        self.assertEqual(scan("// a  \nvoid f();\n").comments[2], (1, "a"))

    def test_string_slashes_not_comment(self):
        self.assertEqual(scan('const char *s = "// no";\nvoid f();\n').comments, {})

    def test_comment_in_block_not_counted(self):
        self.assertEqual(scan("/*\n// inner\n*/\nvoid f();\n").comments, {})

    def test_near_marker_line_still_in_run(self):
        self.assertEqual(scan("// 0x0834a1c0\n/// pure\nvoid f();\n").comments[2], (1, "0x0834a1c0"))


class OffMode(unittest.TestCase):
    def test_nothing_reported(self):
        text = "/// origin binary\n/// bogus\n// pure\nint a; ///< +0x1\nint b; /// x\n"
        fs = scan(text, strict=False)
        self.assertEqual((fs.blocks, fs.trailing, fs.diags, fs.origin), ((), {}, (), None))

    def test_triple_slash_is_comment(self):
        fs = scan("/// \\brief doc\n/// more\nvoid f();\n", strict=False)
        self.assertEqual(fs.comments[3], (1, "\\brief doc\nmore"))

    def test_doxygen_fixture(self):
        text = (FIX / "doxygen.h").read_text()
        fs = scan(text, strict=False)
        self.assertEqual(fs.diags, ())
        self.assertEqual(fs.comments[3][0], 1)
        self.assertGreater(len(scan(text).diags), 0)

    def test_mixed_double_and_triple(self):
        fs = scan("// a\n/// b\nvoid f();\n", strict=False)
        self.assertEqual(fs.comments[3], (1, "a\nb"))


if __name__ == "__main__":
    unittest.main()
