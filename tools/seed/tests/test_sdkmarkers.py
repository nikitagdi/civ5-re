import json
import re
import shutil
import tempfile
import unittest
from pathlib import Path

from seed import sdk, sdkmarkers as sm

FIXTURE_TP = Path(__file__).resolve().parents[2] / "typedb" / "tests" / "fixtures" / "third_party"


def llvm_missing():
    from typedb import compile as cc
    try:
        cc.tools()
    except Exception as e:  # noqa: BLE001
        return str(e)
    return None


class HelperTests(unittest.TestCase):
    def test_strip_args(self):
        self.assertEqual(sm.strip_args("A<int, B<C>>::D<x>::E"), "A::D::E")
        self.assertEqual(sm.strip_args("Plain::Name"), "Plain::Name")

    def test_split_top(self):
        self.assertEqual(sm.split_top("int, A<b, c>, void (*)(int, int)"), ["int", "A<b, c>", "void (*)(int, int)"])
        self.assertEqual(sm.split_top(""), [])
        self.assertEqual(sm.nparams(None), 0)
        self.assertEqual(sm.nparams("char const*, int"), 2)

    def test_decl_end_line(self):
        lines = ["union {", "  int a; // x;", "  char b;", "} m_u; // tail", "int next;"]
        self.assertEqual(sm.decl_end_line(lines, 1), 4)
        self.assertEqual(sm.decl_end_line(lines, 5), 5)
        self.assertEqual(sm.decl_end_line(['const char* s = ";";'], 1), 1)

    def test_line_comment_start(self):
        self.assertEqual(sm.line_comment_start("int a; // c"), 7)
        self.assertEqual(sm.line_comment_start('char* s = "//";'), -1)
        self.assertEqual(sm.line_comment_start("int a; /* // */ int b;"), -1)

    def test_trailing_marker_moves_the_old_comment_up(self):
        before, line = sm.put_trailing("\tint a; // the count", "///< +0x4")
        self.assertEqual(before, ["\t// the count"])
        self.assertEqual(line, "\tint a; ///< +0x4")
        self.assertEqual(sm.put_trailing("int a;", "///< +0x0"), ([], "int a; ///< +0x0"))

    def test_structor_lines(self):
        self.assertEqual(sm.structor_lines("0x08000010", "0x08000010", None),
                         [("addr", "/// 0x08000010")])
        self.assertEqual(sm.structor_lines("0x08000010", "0x08000020", "0x08000030"),
                         [("addr", "/// 0x08000010"), ("base", "/// base 0x08000020"),
                          ("deleting", "/// deleting 0x08000030")])
        self.assertEqual(sm.structor_lines(None, "0x08000020", None), [("base", "/// base 0x08000020")])
        self.assertEqual(sm.structor_lines("0x08000010", None, None, "Box<int>"),
                         [("addr", "/// 0x08000010 Box<int>")])
        self.assertEqual(sm.structor_lines(None, None, None), [])

    def test_no_code_kind(self):
        self.assertEqual(sm.no_code_kind({"pure": True, "has_body": False}), "pure")
        self.assertEqual(sm.no_code_kind({"pure": False, "has_body": True}), "inline")
        self.assertEqual(sm.no_code_kind({"pure": False, "has_body": False}), "absent")

    def test_insert_above_uses_original_numbering(self):
        text = "a\n\tb\nc"
        self.assertEqual(sm.insert_above(text, {1: ["/// size ?", "/// vtable ?"], 2: ["/// inline"]}),
                         "/// size ?\n/// vtable ?\na\n\t/// inline\n\tb\nc")

    def test_param_decl(self):
        self.assertEqual(sm.param_decl("char const*", 2), "char const* a2")
        self.assertEqual(sm.param_decl("void (*)(int, char)", 1), "void (*a1)(int, char)")
        self.assertEqual(sm.param_decl("int (Foo::*)(int) const", 3), "int (Foo::*a3)(int) const")
        self.assertEqual(sm.param_decl(
            "std::__1::basic_string<char, std::__1::char_traits<char>, std::__1::allocator<char>> const&", 1),
            "std::string const& a1")
        self.assertEqual(sm.param_decl("std::__1::vector<int, std::__1::allocator<int>>&", 1),
                         "std::vector<int, std::allocator<int>>& a1")

    def test_forward_decl(self):
        self.assertEqual(sm.forward_decl("Database::FXmlTraverser"), "namespace Database { class FXmlTraverser; }")
        self.assertEqual(sm.forward_decl("A::B::C"), "namespace A { namespace B { class C; }}")
        self.assertEqual(sm.forward_decl("Plain"), "class Plain; ")

    def test_entity_lines(self):
        ctor = {"kind": "ctor", "name": "S", "params": "int", "cv": None, "lua": False,
                "variants": {"C1": "0x08000010", "C2": "0x08000020"}, "size": 0}
        self.assertEqual(sm.entity_lines(ctor, "Ns::S"),
                         ["/// 0x08000010", "/// base 0x08000020", "S(int a1);"])
        dtor = {"kind": "dtor", "name": "S", "params": None, "cv": None, "lua": False,
                "variants": {"D1": "0x08000010", "D0": "0x08000030"}, "size": 0}
        self.assertEqual(sm.entity_lines(dtor, "S"), ["/// 0x08000010", "/// deleting 0x08000030", "virtual ~S();"])
        lua = {"kind": "function", "name": "lGo", "params": "lua_State*", "cv": None, "lua": True,
               "variants": {"-": "0x08000040"}, "size": 0}
        self.assertEqual(sm.entity_lines(lua, "S"), ["/// 0x08000040", "/// lua ?", "static int lGo(lua_State* a1);"])
        meth = {"kind": "method", "name": "Get", "params": "int, char const*", "cv": "const", "lua": False,
                "variants": {"-": "0x08000050"}, "size": 0}
        self.assertEqual(sm.entity_lines(meth, "S"), ["/// 0x08000050", "UNK_RET Get(int a1, char const* a2) const;"])
        data = {"kind": "data", "name": "s_x", "params": None, "cv": None, "lua": False,
                "variants": {"-": "0x08000060"}, "size": 16}
        self.assertEqual(sm.entity_lines(data, "S"), ["/// 0x08000060", "static UNK_DATA s_x[0x10];"])


class ApplyTests(unittest.TestCase):
    def test_origin_first_and_markers_sorted(self):
        e = sm.Edits()
        e.add("a.h", 2, "addr", "/// 0x08000010")
        e.add("a.h", 2, "size", "/// size 0x4")
        e.add("a.h", 2, "lua", "/// lua ?")
        e.field("a.h", 3, "///< +0x0")
        out = sm.apply_edits("#pragma once\n\tclass A\n\tint a; // note\n", "a.h", e, "guess")
        self.assertEqual(out.split("\n")[:7], [
            "/// origin guess", "#pragma once", "\t/// size 0x4", "\t/// 0x08000010", "\t/// lua ?", "\tclass A",
            "\t// note"])
        self.assertIn("\tint a; ///< +0x0", out)
        self.assertTrue(out.endswith("\n"))


DUMP = """*** Dumping AST Record Layout
         0 | class Box<int>
         0 |   (Box vtable pointer)
         4 |   int m_v
         8 |   union Box<int>::(anonymous at C:\\x\\Box.h:12:3)
         8 |     int i
         8 |     float f
        12 |   struct Box<int>::Inner m_in
        12 |     char c
         | [sizeof=16, align=4]

*** Dumping AST Record Layout
         0 | union Box<int>::(anonymous at C:\\x\\Box.h:12:3)
         0 |   int i
         0 |   float f
         | [sizeof=4, align=4]

*** Dumping AST Record Layout
         0 | struct Plain
         0 |   int x
"""


class LayoutDumpTests(unittest.TestCase):
    def test_blocks_flatten_nameless_anonymous_members(self):
        blocks = {b["raw"]: b["fields"] for b in sm.parse_layout_blocks(DUMP)}
        self.assertEqual(blocks["Box<int>"], {"m_v": 4, "i": 8, "f": 8, "m_in": 12})
        self.assertEqual(blocks["Box<int>::(anon union L12)"], {"i": 0, "f": 0})

    def test_dump_keeps_template_instances_only(self):
        d = sm.parse_layout_dump(DUMP)
        self.assertEqual(sorted(d), ["Box", "Box::(anon union L12)"])
        self.assertEqual(d["Box"]["i"], 8)

    def test_bit_field_offset_is_the_byte(self):
        d = sm.parse_layout_blocks("*** Dumping AST Record Layout\n  0 | struct S<int>\n  4:0-2 |   unsigned a\n")
        self.assertEqual(d[0]["fields"], {"a": 4})


class SourceScanTests(unittest.TestCase):
    TEXT = """#pragma once
#define M(x) { x \\
 }
namespace ns {
// class Fake { int a; };
template <typename T, int N = 3>
class Outer : public Base
{
  struct Inner { int a; };
  T m_t;
};
template <typename E, typename C>
class Part<std::vector<E>, C> {
  E m_e;
};
}
"""

    def test_scopes_at(self):
        s = sm.scopes_at(self.TEXT, 10)
        self.assertEqual([h[:9] for h in s], ["namespace", "template "])
        self.assertTrue(sm.scopes_at(self.TEXT, 1) == [])

    def test_class_head(self):
        c = sm.parse_class_head("template <typename E, typename C>\nclass Part<std::vector<E>, C> : public B")
        self.assertEqual((c["name"], c["spec"], len(c["params"])), ("Part", "std::vector<E>, C", 2))
        self.assertIsNone(sm.parse_class_head("namespace ns"))
        self.assertIsNone(sm.parse_class_head("template <class T> void f()"))

    def test_class_expression(self):
        self.assertEqual(sm.class_expression(self.TEXT, 10), "ns::Outer<int>")
        self.assertEqual(sm.class_expression(self.TEXT, 14), "ns::Part<std::vector<int>, int>")
        self.assertIsNone(sm.class_expression(self.TEXT, 1))

    def test_dummy_arguments(self):
        p = sm.template_params(["template <class T, uint N, bool B, Mode M, char* P, int D = 3>", "class X"], 1)
        self.assertEqual(sm.dummy_args(p), ["int", "((uint)1)", "((bool)1)", "((Mode)1)", "0"])
        self.assertIsNone(sm.dummy_args(["template <class> class TT"]))


@unittest.skipIf(llvm_missing() is not None, "LLVM not available")
class LiveTests(unittest.TestCase):
    """typedb (markers off) on a small tree, marked from a synthetic binfacts, then typedb strict."""

    HEADER = """#pragma once
class Base {
public:
	virtual ~Base();
	virtual int Get() const = 0;
	int m_a;
};

class Thing : public Base {
public:
	Thing();
	virtual ~Thing();
	virtual int Get() const;
	void Set(int v);
	int Twice() const { return m_a * 2; }
	static int lGo(lua_State*);
	int m_b; // trailing comment
	char m_c;
	union {
		int i;
		float f;
	} m_u;
};

template <class T> class Box {
public:
	T Get() const { return m_v; }
	T m_v;
	int m_n;
};

extern int g_count;
const int c_limit = 4;
"""

    @staticmethod
    def symbols():
        def s(mangled, addr, kind, name, scope="Thing", params=None, variant=None, cv=None, section=".text"):
            return {"mangled": mangled, "addr": addr, "kind": kind, "name": name, "scope": scope, "params": params,
                    "variant": variant, "cv": cv, "section": section, "group": "firaxis", "size": 4,
                    "demangled": f"{scope}::{name}"}
        return [
            s("_ZN5ThingC1Ev", "0x08000010", "ctor", "Thing", variant="C1"),
            s("_ZN5ThingC2Ev", "0x08000010", "ctor", "Thing", variant="C2"),
            s("_ZN5ThingD1Ev", "0x08000020", "dtor", "Thing", variant="D1"),
            s("_ZN5ThingD2Ev", "0x08000024", "dtor", "Thing", variant="D2"),
            s("_ZN5ThingD0Ev", "0x08000030", "dtor", "Thing", variant="D0"),
            s("_ZNK5Thing3GetEv", "0x08000040", "method", "Get", cv="const"),
            s("_ZN5Thing3SetEi", "0x08000050", "method", "Set", params="int"),
            s("_ZN5Thing3lGoEP9lua_State", "0x08000060", "function", "lGo", params="lua_State*"),
            s("_ZTV5Thing", "0x0a000000", "vtable", "vtable for Thing", section=".data.rel.ro"),
            s("g_count", "0x0b000000", "c", "g_count", scope="", section=".bss"),
        ]

    def test_marks_and_typedb_accepts(self):
        from typedb.build import build_db
        bf = {"symbols": self.symbols(), "vtables": []}
        root = Path(tempfile.mkdtemp(prefix="sdkmarkers-"))
        self.addCleanup(shutil.rmtree, root, True)
        tree = root / "Civ5Expansion2"
        dest = "Sdk/Thing.h"
        (tree / "Sdk").mkdir(parents=True)
        (tree / dest).write_text(self.HEADER, encoding="utf-8", newline="\n")
        # lua_State is not in the fixture environment
        text = "struct lua_State;\n" + self.HEADER
        (tree / dest).write_text(text, encoding="utf-8", newline="\n")
        out = build_db(tree, root / "build", markers="off", third_party=FIXTURE_TP)
        db = json.loads(out.read_text(encoding="utf-8"))
        self.assertEqual([d for d in db["diagnostics"] if d["severity"] == "error"], [], db["diagnostics"][:3])
        marked, report = sm.mark({dest: text}, db, bf, {dest: "guess"}, layouts={"Box": {"m_v": 0, "m_n": 4}})
        self.assertEqual(report["counts"]["lua"], 1)
        sdk.write_sdk(marked, tree)
        strict = json.loads(build_db(tree, root / "build", markers="strict", third_party=FIXTURE_TP)
                            .read_text(encoding="utf-8"))
        errors = [d for d in strict["diagnostics"] if d["severity"] == "error"]
        self.assertEqual(errors, [], errors[:5])
        body = marked[dest]
        for want in ("/// origin guess", "/// vtable 0x0a000000", "/// 0x08000010\n\tThing();",
                     "/// 0x08000020\n\t/// base 0x08000024\n\t/// deleting 0x08000030\n\tvirtual ~Thing();",
                     "/// 0x08000040\n\tvirtual int Get() const;", "/// pure\n\tvirtual int Get() const = 0;",
                     "/// inline\n\tint Twice() const", "/// 0x08000060\n\t/// lua ?\n\tstatic int lGo",
                     "// trailing comment\n\tint m_b; ///< +0x8", "} m_u; ///< +0x10"):
            self.assertIn(want, body)
        self.assertRegex(body, r"/// size \?\ntemplate <class T> class Box")
        self.assertRegex(body, r"/// 0x0b000000\nextern int g_count;")
        self.assertRegex(body, r"/// inline\nconst int c_limit")
        self.assertIn("///< +0x0", body.split("class Box")[1])

    def test_template_static_member_marked_above_its_definition(self):
        from typedb.build import build_db
        text = ("#pragma once\n"
                "template <typename T, unsigned int N>\n"
                "class Pool {\n"
                "public:\n"
                "\tstatic void* s_pTop;\n"
                "};\n"
                "\n"
                "template <typename T, unsigned int N>\n"
                "void* Pool<T, N>::s_pTop = 0;\n")
        sym = {"mangled": "_ZN4PoolIyLj8EE6s_pTopE", "addr": "0x10090688", "kind": "data", "name": "s_pTop",
               "scope": "Pool<unsigned long long, 8u>", "params": None, "variant": None, "cv": None,
               "section": ".bss", "group": "firaxis", "size": 4, "demangled": "Pool<unsigned long long, 8u>::s_pTop"}
        bf = {"symbols": [sym], "vtables": []}
        root = Path(tempfile.mkdtemp(prefix="sdkmarkers-"))
        self.addCleanup(shutil.rmtree, root, True)
        tree = root / "Civ5Expansion2"
        dest = "Sdk/Pool.h"
        (tree / "Sdk").mkdir(parents=True)
        (tree / dest).write_text(text, encoding="utf-8", newline="\n")
        db = json.loads(build_db(tree, root / "build", markers="off", third_party=FIXTURE_TP).read_text(encoding="utf-8"))
        marked, _report = sm.mark({dest: text}, db, bf, {dest: "guess"})
        self.assertIn("/// 0x10090688 Pool<unsigned long long, 8u>\ntemplate <typename T, unsigned int N>\n"
                      "void* Pool<T, N>::s_pTop = 0;", marked[dest])
        self.assertNotIn("/// 0x10090688", marked[dest].split("};")[0])
        sdk.write_sdk(marked, tree)
        strict = json.loads(build_db(tree, root / "build", markers="strict", third_party=FIXTURE_TP)
                            .read_text(encoding="utf-8"))
        self.assertEqual([d for d in strict["diagnostics"] if d["severity"] == "error"], [])
        (g,) = [g for g in strict["globals"] if g["qualified"] == "Pool::s_pTop"]
        self.assertEqual([(i["addr"], i["mangled"]) for i in g["instances"]],
                         [("0x10090688", "_ZN4PoolIyLj8EE6s_pTopE")])

    def test_exe_only_members_are_appended(self):
        from typedb.build import build_db
        bf = {"symbols": self.symbols() + [{
            "mangled": "_ZN5Thing5ExtraEPKc", "addr": "0x08000070", "kind": "method", "name": "Extra",
            "scope": "Thing", "params": "char const*", "variant": None, "cv": None, "section": ".text",
            "group": "firaxis", "size": 4, "demangled": "Thing::Extra(char const*)"}], "vtables": []}
        root = Path(tempfile.mkdtemp(prefix="sdkmarkers-"))
        self.addCleanup(shutil.rmtree, root, True)
        tree = root / "Civ5Expansion2"
        dest = "Sdk/Thing.h"
        text = "struct lua_State;\n" + self.HEADER
        (tree / "Sdk").mkdir(parents=True)
        (tree / dest).write_text(text, encoding="utf-8", newline="\n")
        db = json.loads(build_db(tree, root / "build", markers="off", third_party=FIXTURE_TP)
                        .read_text(encoding="utf-8"))
        report = {}
        out = sm.append_exe_members({dest: text}, db, bf, report)[dest]
        self.assertIn(sm.SEED_NOTE + "\npublic:\n\t/// 0x08000070\n\tUNK_RET Extra(char const* a1);\n};", out.replace(
            "\t" + sm.SEED_NOTE, sm.SEED_NOTE).replace("\n\tpublic:", "\npublic:"))
        self.assertEqual(report["exe_only"], [f"{dest} Thing::Extra(char const*)"])


class VtableTests(unittest.TestCase):
    def test_mismatch_is_reported_and_destructor_slots_are_exempt(self):
        rec = {"name": "A", "origin": "tree", "complete": True, "template": None, "vtable": {"groups": [{"slots": [
            {"kind": "D1", "symbol": "_ZN1AD1Ev"}, {"kind": "D0", "symbol": "_ZN1AD0Ev"},
            {"kind": "method", "symbol": "_ZN1A1fEv"}, {"kind": "pure", "symbol": "__cxa_pure_virtual"}]}]}}
        slots = [{"target": "0x1", "symbols": ["_ZN1BD1Ev"], "kind": "func"},
                 {"target": "0x2", "symbols": ["_ZN1BD0Ev"], "kind": "func"},
                 {"target": "0x3", "symbols": ["_ZN1A1fEv"], "kind": "func"},
                 {"target": "0x4", "symbols": [], "kind": "pure"}]
        bf = {"symbols": [], "vtables": [{"class": "A", "construction": False, "groups": [{"slots": slots}]}]}
        self.assertEqual(sm.vtable_mismatches({"records": [rec]}, bf), [])
        slots[2]["symbols"] = ["_ZN1A1gEv"]
        got = sm.vtable_mismatches({"records": [rec]}, bf)
        self.assertEqual(got, [("A", 2, ["_ZN1A1gEv"], "_ZN1A1fEv")])
        slots.pop()
        self.assertEqual(sm.vtable_mismatches({"records": [rec]}, bf)[0][:2], ("A", "count"))


if __name__ == "__main__":
    unittest.main()
