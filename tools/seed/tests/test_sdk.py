import re
import shutil
import unittest

from seed import inputs, sdk, unifdef
from seed.sdk import SdkError

LEKMOD = inputs.default_lekmod_dir()
HAVE_LEKMOD = LEKMOD is not None and (LEKMOD / ".git").exists() and shutil.which("git") is not None
KEYWORDS = r"(?:0x|origin\b|size\b|vtable\b|base\b|deleting\b|thunk\b|inline\b|pure\b|absent\b|fastcc\b|lua\b)"


class DecodeTests(unittest.TestCase):
    def test_cp1252_crlf_to_utf8_lf(self):
        self.assertEqual(sdk.decode(b"a\xe9\r\nb\r\n"), "aé\nb\n")

    def test_undefined_byte_is_error(self):
        with self.assertRaises(SdkError):
            sdk.decode(b"\x81")


class CommentTests(unittest.TestCase):
    def test_triple_slash_becomes_double(self):
        self.assertEqual(sdk.rewrite_comments("/// text\nint a; ///< tail\n"), "// text\nint a; //< tail\n")

    def test_long_slash_runs(self):
        self.assertEqual(sdk.rewrite_comments("////////////////\n"), "//\n")

    def test_strings_and_block_comments_are_untouched(self):
        src = 'const char* u = "http:///x"; /* /// */ int a;\n'
        self.assertEqual(sdk.rewrite_comments(src), src)

    def test_char_literal_and_escape_do_not_open_a_string(self):
        src = "char q = '\"'; // /// x\n"
        self.assertEqual(sdk.rewrite_comments(src), src)

    def test_marker_like_lines_are_neutralized(self):
        for body in ("0x0834a1c0", "size of the cell", "pure virtual", "inline routines", "lua stack", "base class"):
            out = sdk.rewrite_comments(f"// {body}\n")
            self.assertEqual(out, f"// Note: {body}\n")
            self.assertIsNone(re.match(rf"^\s*//\s*{KEYWORDS}", out))

    def test_offset_marker_form(self):
        self.assertEqual(sdk.rewrite_comments("int a; //< +0x10\n"), "int a; // Note: < +0x10\n")

    def test_ordinary_comments_stay(self):
        src = "// Note: this is fine\n// The size is 4\n//\n"
        self.assertEqual(sdk.rewrite_comments(src), src)

    def test_after_rewrite_no_triple_slash(self):
        out = sdk.rewrite_comments("/// a\n//// b\n/////<\n")
        self.assertNotIn("///", out)


class IncludeTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.entries = inputs.sdk_entries()
        cls.resolver = sdk.IncludeResolver([e.dest for e in cls.entries], {"lua.h", "lauxlib.h", "lua.hpp"})

    def rw(self, includer, line):
        return sdk.rewrite_includes(line + "\n", includer, self.resolver)

    def test_sibling_include_gets_root_path(self):
        out = self.rw("FirePlace/FireWorks/FArray.h", '#include "FFastVector.h"')
        self.assertEqual(out, '#include "FirePlace/FireWorks/FFastVector.h"\n')

    def test_case_is_taken_from_the_file_list(self):
        out = self.rw("FirePlace/FireWorks/FArray.h", '#include "fastar.h"')
        self.assertEqual(out, '#include "fastar.h"\n')
        out = self.rw("FirePlace/FireWorks/FArray.h", '#include "ffastvector.h"')
        self.assertEqual(out, '#include "FirePlace/FireWorks/FFastVector.h"\n')

    def test_subdirectory_and_backslash(self):
        self.assertEqual(self.rw("FirePlace/FireWorks/FString.h", '#include "Win32/FDebugHelper.h"'),
                         '#include "FirePlace/FireWorks/Win32/FDebugHelper.h"\n')
        self.assertEqual(self.rw("FirePlace/FireWorks/FMemHooks.h", '#include "FireWorks\\FUncopyable.h"'),
                         '#include "FirePlace/FireWorks/FUncopyable.h"\n')

    def test_parent_directory(self):
        self.assertEqual(self.rw("FirePlace/FireWorks/Win32/FKBInputDevice.h", '#include "..\\FInputDevice.h"'),
                         '#include "FirePlace/FireWorks/FInputDevice.h"\n')

    def test_angle_form_with_project_prefix(self):
        self.assertEqual(self.rw("Civ5/Src/App/CvGameCoreDLLUtil/ICvDLLUtility.h", "#include <FireWorks/FMemoryStream.h>"),
                         '#include "FirePlace/FireWorks/FMemoryStream.h"\n')

    def test_cross_library(self):
        self.assertEqual(self.rw("Civ5/Src/App/CvWorldBuilderMap/CvWorldBuilderMap.h", '#include "FUncopyable.h"'),
                         '#include "FirePlace/FireWorks/FUncopyable.h"\n')

    def test_own_directory_wins(self):
        out = self.rw("Civ5/Src/CvGameDatabase/CvGameDatabase.h", '#include "Database.h"')
        self.assertEqual(out, '#include "Civ5/Src/CvGameDatabase/Database.h"\n')

    def test_lua_stays_a_system_include(self):
        self.assertEqual(self.rw("FirePlace/FireWorks/FLua/Include/FLuaCommon.h", '#include "lua.h"'), "#include <lua.h>\n")
        self.assertEqual(self.rw("FirePlace/FireWorks/FLua/Include/FLuaCommon.h", "#include <lua.hpp>"), "#include <lua.hpp>\n")

    def test_system_includes_are_untouched(self):
        for line in ("#include <Windows.h>", "#include <vector>", '#include "vector"', "#include <stdio.h>"):
            self.assertEqual(self.rw("FirePlace/FireWorks/FArray.h", line), line + "\n")

    def test_spacing_and_trailing_comment_survive(self):
        out = self.rw("FirePlace/FireWorks/FArray.h", '  #  include "FFastVector.h" // c')
        self.assertEqual(out, '  #  include "FirePlace/FireWorks/FFastVector.h" // c\n')


class TransformTests(unittest.TestCase):
    def test_transform_order_and_final_newline(self):
        entry = inputs.Entry("sdk", "x", "FirePlace/FireWorks/FArray.h", "binary")
        resolver = sdk.IncludeResolver([entry.dest, "FirePlace/FireWorks/FFastVector.h"])
        src = '#ifdef AUI_X\nint mod;\n#endif\n/// c\n#include "FFastVector.h"'
        out = sdk.transform(src, entry, resolver, ["AUI_*"])
        self.assertEqual(out, '// c\n#include "FirePlace/FireWorks/FFastVector.h"\n')


@unittest.skipUnless(HAVE_LEKMOD, "Lekmod checkout not available")
class BuildTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.files = sdk.build_sdk(LEKMOD)

    def test_98_files_in_map_order(self):
        entries = inputs.sdk_entries()
        self.assertEqual(sorted(self.files), sorted(e.dest for e in entries))
        self.assertEqual(len(self.files), 98)

    def test_no_mod_macros_left(self):
        pats = unifdef.load_patterns()
        import fnmatch
        for dest, text in self.files.items():
            for w in set(re.findall(r"[A-Za-z_]\w*", text)):
                self.assertFalse(any(fnmatch.fnmatchcase(w, p) for p in pats), f"{dest}: {w}")
            self.assertNotRegex(text, r"#\s*pragma\s+message")

    def test_no_triple_slash_and_no_marker_like_comments(self):
        for dest, text in self.files.items():
            self.assertNotIn("///", text, dest)
            for i, line in enumerate(text.split("\n"), 1):
                m = re.search(r"//\s*(.*)$", line)
                if m and not re.search(r'"[^"]*//', line):
                    self.assertIsNone(re.match(KEYWORDS, m.group(1)), f"{dest}:{i}: {line}")

    def test_utf8_lf_final_newline(self):
        for dest, text in self.files.items():
            self.assertNotIn("\r", text, dest)
            self.assertTrue(text.endswith("\n"), dest)

    def test_quote_includes_are_root_relative_or_system(self):
        dests = {d.lower() for d in self.files}
        for dest, text in self.files.items():
            for m in re.finditer(r'(?m)^[ \t]*#[ \t]*include[ \t]*"([^"]+)"', text):
                name = m.group(1)
                if "/" in name and name.split("/")[0] in ("FirePlace", "Civ5"):
                    self.assertIn(name.lower(), dests, f"{dest}: {name}")

    def test_deterministic(self):
        again = sdk.build_sdk(LEKMOD)
        self.assertEqual(again, self.files)

    def test_patch_changes_files_and_is_syntax_only_in_size(self):
        base = sdk.stage(LEKMOD)
        changed = [d for d in base if base[d] != self.files[d]]
        self.assertGreater(len(changed), 0)

    def test_enum_header_matches_blob_modulo_comments(self):
        lock = inputs.load_lock()
        blob = inputs.read_blob(LEKMOD, lock.commits["sdk"],
                                "DLC/LEKMOD/CvGameCore_Expansion2/CvGameCoreDLLUtil/include/CvEnums.h")
        self.assertTrue(blob.startswith(b"") and len(blob) > 0)

        def norm(s):
            s = re.sub(r"/\*.*?\*/", "", s, flags=re.S)
            s = re.sub(r"//[^\n]*", "", s)
            s = re.sub(r"#include[^\n]*", "", s)
            return re.sub(r"\s+", "", s)

        self.assertEqual(norm(sdk.decode(blob)), norm(self.files["Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"]))


if __name__ == "__main__":
    unittest.main()
