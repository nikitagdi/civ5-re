import shutil
import tempfile
import unittest
from pathlib import Path

from seed import env, inputs


class FwdTests(unittest.TestCase):
    def test_forward_declarations_by_header(self):
        foreign = [("CSteamID", "class", "fwd/steam.h"), ("granny::model", "struct", "fwd/granny.h"),
                   ("granny::file_writer_seek_type", "enum", "fwd/granny.h"), ("Json::Value", "class", "fwd/jsoncpp.h"),
                   ("SDL_Event", "union", "fwd/sdl.h"), ("lua_State", "struct", "lua-5.1.4/include/lua.h"),
                   ("_GUID", "struct", "compat/Windows.h")]
        files = env.fwd_files(foreign)
        self.assertEqual(sorted(files), ["third_party/fwd/granny.h", "third_party/fwd/jsoncpp.h",
                                         "third_party/fwd/sdl.h", "third_party/fwd/steam.h"])
        text = files["third_party/fwd/granny.h"].decode()
        self.assertTrue(text.startswith("#pragma once\n"))
        self.assertIn("namespace granny {\nenum file_writer_seek_type : int;\nstruct model;\n}", text)
        self.assertIn("namespace Json {\nclass Value;\n}", files["third_party/fwd/jsoncpp.h"].decode())
        self.assertIn("union SDL_Event;", files["third_party/fwd/sdl.h"].decode())

    def test_real_foreign_list_parses(self):
        foreign = env.load_foreign()
        self.assertGreater(len(foreign), 80)
        files = env.fwd_files(foreign)
        self.assertIn("third_party/fwd/granny.h", files)
        self.assertNotIn("third_party/compat/Windows.h", files)

    def test_bad_line_is_error(self):
        d = Path(tempfile.mkdtemp())
        self.addCleanup(shutil.rmtree, d, True)
        p = d / "f.txt"
        p.write_text("X\tbogus\tfwd/x.h\n", encoding="utf-8")
        with self.assertRaises(env.EnvError):
            env.load_foreign(p)


class EnvironmentFileTests(unittest.TestCase):
    def test_include_dirs_order(self):
        self.assertEqual(env.include_dirs_text().split(), [
            "compat", "fwd", "lua-5.1.4/include", "libcxx/v1", "<clang>",
            "libc/include/x86_64-linux-gnu", "libc/include"])

    def test_pch_starts_with_the_prelude(self):
        text = env.pch_text({"vector", "string", "list"})
        lines = text.split("\n")
        self.assertEqual(lines[:2], ['#include "compat/civ5re_prelude.h"', '#include "compat/Windows.h"'])
        self.assertEqual(lines[2:5], ["#include <list>", "#include <string>", "#include <vector>"])

    def test_sdk_system_includes_are_libcxx_files(self):
        libcxx = {"third_party/libcxx/v1/vector": b"", "third_party/libcxx/v1/string": b""}
        sdk = {"a.h": '#include <vector>\n#include <Windows.h>\n  #  include <string>\n#include "x.h"\n'}
        self.assertEqual(env.sdk_system_includes(sdk, libcxx), {"vector", "string"})

    def test_compat_files_are_the_templates(self):
        self.assertEqual(sorted(inputs.compat_files()), [
            "compat/Windows.h", "compat/civ5re_prelude.h", "compat/gnu/stubs-32.h", "compat/hash_map", "compat/hash_set",
            "compat/sqlite3.h", "compat/tbb.h"])


class ClosureTests(unittest.TestCase):
    def test_parse_deps(self):
        text = "a.o: /x/a.cpp /usr/include/stdio.h \\\n /usr/include/my\\ file.h \\\r\n /b.h\n"
        self.assertEqual(env.parse_deps(text), ["/x/a.cpp", "/usr/include/stdio.h", "/usr/include/my file.h", "/b.h"])

    def test_libc_args_point_at_the_sources(self):
        tp = Path("C:/w/third_party")
        args = ["-nostdinc", "-isystem", "C:/w/third_party/libc/include/x86_64-linux-gnu",
                "-isystem", "C:/w/third_party/libc/include", "-isystem", "C:/w/third_party/compat"]
        got = env.libc_args(args, tp, "//wsl.localhost/U/usr/include")
        self.assertEqual(got[2], "//wsl.localhost/U/usr/include/x86_64-linux-gnu")
        self.assertEqual(got[4], "//wsl.localhost/U/usr/include")
        self.assertEqual(got[6], "C:/w/third_party/compat")

    def test_classify_maps_to_third_party_paths(self):
        src = "//wsl.localhost/U/usr/include"
        deps = [f"{src}/stdio.h", f"{src}/x86_64-linux-gnu/bits/types.h", "C:/w/third_party/compat/Windows.h",
                "C:/w/Civ5Expansion2/A.h", "C:/llvm/lib/clang/22/include/stddef.h"]
        got = env.classify_libc(deps, "C:/w/third_party", src, "C:/w/Civ5Expansion2", "C:/llvm/lib/clang/22/include")
        self.assertEqual(sorted(got), ["third_party/libc/include/stdio.h",
                                       "third_party/libc/include/x86_64-linux-gnu/bits/types.h"])

    def test_classify_rejects_strangers(self):
        with self.assertRaises(env.EnvError):
            env.classify_libc(["D:/elsewhere/x.h"], "C:/w/third_party", "//wsl.localhost/U/usr/include",
                              "C:/w/Civ5Expansion2", "C:/llvm/include")

    def test_foreign_libc_roots(self):
        foreign = [("_IO_FILE", "struct", "libc/include/x86_64-linux-gnu/bits/types/struct_FILE.h"),
                   ("in_addr", "struct", "libc/include/netinet/in.h"), ("X", "class", "fwd/x.h")]
        self.assertEqual(env.foreign_libc_roots(foreign), ["bits/types/struct_FILE.h", "netinet/in.h"])


class ManifestTests(unittest.TestCase):
    def test_rows(self):
        lock = inputs.load_lock()
        files = {"third_party/pch.h": b"x\n", "third_party/libcxx/v1/vector": b"v\n",
                 "third_party/libc/include/stdio.h": b"s\n",
                 "third_party/libc/include/x86_64-linux-gnu/bits/a.h": b"a\n",
                 "third_party/lua-5.1.4/include/lua.h": b"l\n", "third_party/compat/Windows.h": b"w\n",
                 "third_party/MANIFEST.txt": b"ignored"}
        rows = [r.split("\t") for r in env.manifest_text(files, lock).strip().split("\n")[1:]]
        by_path = {r[0]: r for r in rows}
        self.assertNotIn("MANIFEST.txt", by_path)
        self.assertEqual(by_path["pch.h"][2:], ["seed generated", "-", "-"])
        self.assertEqual(by_path["libcxx/v1/vector"][2:], ["WSL libc++", "18", "usr/lib/llvm-18/include/c++/v1/vector"])
        self.assertEqual(by_path["libc/include/stdio.h"][4], "usr/include/stdio.h")
        self.assertEqual(by_path["libc/include/x86_64-linux-gnu/bits/a.h"][4], "usr/include/x86_64-linux-gnu/bits/a.h")
        self.assertEqual(by_path["lua-5.1.4/include/lua.h"][4], "SDKs/Lua/5.1/include/lua.h")
        self.assertEqual(by_path["compat/Windows.h"][2], "seed template")
        self.assertEqual(rows, sorted(rows, key=lambda r: r[0]))
        self.assertEqual(by_path["pch.h"][1], env.sha256(b"x\n"))


class WritePlanTests(unittest.TestCase):
    def setUp(self):
        self.root = Path(tempfile.mkdtemp())
        self.addCleanup(shutil.rmtree, self.root, True)

    def test_missing_identical_different(self):
        (self.root / "third_party").mkdir()
        (self.root / "third_party" / "same.h").write_bytes(b"a\r\nb\r\n")
        (self.root / "third_party" / "other.h").write_bytes(b"old\n")
        files = {"third_party/same.h": b"a\nb\n", "third_party/other.h": b"new\n", "third_party/new.h": b"n\n"}
        writes, same, diff = env.plan(self.root, files)
        self.assertEqual((writes, same, diff), (["third_party/new.h"], ["third_party/same.h"], ["third_party/other.h"]))

    def test_redeclared_class_in_another_file(self):
        tree = self.root / "Civ5Expansion2"
        (tree / "Sdk").mkdir(parents=True)
        (tree / "Sdk" / "Mine.h").write_text("/// origin binary\nclass FFastVector {\n};\nclass Other;\n", encoding="utf-8")
        (tree / "Sdk" / "Pilot.h").write_text("class cvUnitVisSystem : public Base {};\n", encoding="utf-8")
        files = {"Civ5Expansion2/Sdk/Sdk.h": b"struct FFastVector\n{\n};\nenum class E : int {};\n"}
        self.assertEqual(env.redeclared(self.root, files), [("Civ5Expansion2/Sdk/Mine.h", "FFastVector")])
        self.assertEqual(env.redeclared(self.root, {"Civ5Expansion2/Sdk/Mine.h": b""} | files), [])


if __name__ == "__main__":
    unittest.main()
