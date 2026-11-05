"""seed tree: demangler, class model, vtables, templates, placement, emitter, SDK SPEC lines (synthetic inputs)."""
import unittest

from seed import emit as E
from seed import env
from seed import mangle as M
from seed import model as MD
from seed import place as PL
from seed import sdkspec as SS
from seed import templates as TP
from seed import tree as T
from seed import vtables as VT


def sym(mangled, addr, kind=None, variant=None, cls=None, typ="func", size=16, bind="global", group="firaxis"):
    sp = M.Speller()
    scope = name = params = None
    cv = None
    if mangled.startswith("_Z") and not mangled.startswith(("_ZTV", "_ZTI", "_ZTS")):
        y = M.parse(mangled)
        if y.name is not None:
            scope = sp.name(y.name.scope()) if y.name.parts[:-1] else ""
            name = sp.part(y.name.last, y.name.parts[-2].name if len(y.name.parts) > 1 else None)
        if y.params is not None:
            params = sp.params(y.params, y.variadic)
            cv = "const" if y.const else None
    return {"mangled": mangled, "demangled": mangled, "addr": addr, "kind": kind or "method", "variant": variant,
            "scope": scope if scope is not None else "", "name": name or mangled, "params": params, "cv": cv,
            "scope_is_class": cls, "type": typ, "size": size, "section": ".text" if typ == "func" else ".bss",
            "bind": bind, "group": group}


def binfacts(symbols, typeinfo=(), vtables=()):
    kinds = {k: 0 for k in ("c", "ctor", "data", "dtor", "function", "method", "thunk", "vtable")}
    for s in symbols:
        if s["kind"] in kinds:
            kinds[s["kind"]] += 1
    return {"symbols": list(symbols), "typeinfo": list(typeinfo), "vtables": list(vtables),
            "stats": {"denominator": {"by_kind": kinds}}}


class FakeExisting:
    def __init__(self):
        self.records, self.all_records, self.kinds, self.fwd_names = {}, {}, {}, set()
        self.db = {"functions": []}
        self.root = None

    def declares_symbol(self, m):
        return False

    def kind(self, p):
        return None

    def file(self, p):
        return None

    def template_params(self, p):
        return None


class FakeForeign:
    def kind(self, p):
        return {("lua_State",): "struct"}.get(p)

    def header(self, p):
        return "lua-5.1.4/include/lua.h" if p == ("lua_State",) else None

    def include(self, p):
        return "<lua.h>" if p == ("lua_State",) else None


def build(bf, gfuncs=None):
    facts = MD.Facts(bf, gfuncs or {})
    facts.code = None
    ex, fo = FakeExisting(), FakeForeign()
    m = MD.build(facts, ex)
    MD.finish_structors(m)
    MD.classify(m, ex, fo)
    MD.apply_template_refs(m, ex)
    MD.template_template_params(m)
    VT.run(m, facts, ex)
    TP.generalize(m)
    MD.finish_members(m)
    return m, ex, fo


def text_of(m, ex, fo, units):
    rep = {"problems": [], "decisions": __import__("collections").defaultdict(list)}
    em = E.Emitter(m, ex, fo, rep)
    for u in em.units():
        em.file_of[u] = "/".join(u) + ".h"
    return "\n".join(em.file_text("/".join(u) + ".h", "guess", [u], {}) for u in units), rep


class MangleTests(unittest.TestCase):
    def spell(self, mangled):
        y = M.parse(mangled)
        sp = M.Speller()
        mp = {M.TParam(i): a for i, a in enumerate(y.targs or ())}
        params = sp.params(tuple(M.substitute(p, mp) for p in y.params), y.variadic)
        head = sp.name(y.name) + "(" + params + ")" + (" const" if y.const else "")
        return (sp.type(M.substitute(y.ret, mp)) + " " + head) if y.ret is not None else head

    def test_real_symbols(self):
        cases = {
            "_ZN12cvLuaPreGame14lGetFileHeaderEP9lua_State": "cvLuaPreGame::lGetFileHeader(lua_State*)",
            "_ZNK12FHttpSession9GetHandleEv": "FHttpSession::GetHandle() const",
            "_ZN9UIManager15AllocateControlI10UIString32EEPT_v": "UIString32* UIManager::AllocateControl<UIString32>()",
            "_ZN11FXmlElement6AssignERKNSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE":
                "FXmlElement::Assign(std::string const&)",
            "_ZN1A1fEPFiP9lua_StateE": "A::f(int (*)(lua_State*))",
            "_ZN1A1gEM1BFvvE": "A::g(void (B::*)())",
            "_ZN1A1hERA4_c": "A::h(char (&)[4])",
            "_ZN1AcviEv": "A::operator int()",
            "_ZN1AeqERKS_": "A::operator==(A const&)",
        }
        for m, want in cases.items():
            self.assertEqual(self.spell(m), want, m)

    def test_template_param_stays_symbolic(self):
        y = M.parse("_ZN9UIManager15AllocateControlI10UIString32EEPT_v")
        self.assertEqual(M.spell(y.ret, tparams=("T0",)), "T0*")

    def test_declarators(self):
        sp = M.Speller()
        fp = M.Ptr(M.Func(M.Builtin("int"), (MD.LUA_STATE,)))
        self.assertEqual(sp.type(fp, " a1"), "int (*a1)(lua_State*)")
        self.assertEqual(sp.type(M.CV(M.Ptr(M.Builtin("char")), True), " a1"), "char* const a1")

    def test_unsupported_raises(self):
        with self.assertRaises(M.MangleError):
            M.parse("_ZZN1A1fEvE1x")


class ModelTests(unittest.TestCase):
    def setUp(self):
        self.bf = binfacts([
            sym("_ZN5cvLua4lFooEP9lua_State", "0x08000010", "method", cls=True),
            sym("_ZN5cvLua3BarEi", "0x08000020", "method", cls=True),
            sym("_ZNK5cvLua3BazEv", "0x08000030", "method", cls=True),
            sym("_ZN7FEndian4SwapEj", "0x08000040", "function"),
            sym("_ZN4Mesh4DrawE10BlendTypes", "0x08000050", "method", cls=True),
            sym("frkPoolInit", "0x08000060", "c"),
            sym("g_Thing", "0x0a000000", "c", typ="object", size=12),
            sym("_ZN5cvLua7s_CacheE", "0x0a000010", "data", cls=True, typ="object", size=28),
        ])

    def test_kinds_and_members(self):
        m, ex, fo = build(self.bf)
        self.assertEqual(m.scopes[("cvLua",)].kind, "class")
        self.assertEqual(m.scopes[("FEndian",)].kind, "namespace")
        self.assertEqual(m.scopes[("BlendTypes",)].kind, "enum")
        fns = {f.part.name: f for f in m.scopes[("cvLua",)].fns}
        self.assertTrue(fns["lFoo"].static and fns["lFoo"].ret_text == "int")
        self.assertFalse(fns["Bar"].static)
        self.assertIn("cvLua::Bar", m.decisions["member_default"])
        self.assertIn("cvLua::lFoo", m.decisions["static_lua"])
        self.assertIn("BlendTypes", m.decisions["enum_by_name"])

    def test_emitted_text(self):
        m, ex, fo = build(self.bf)
        txt, _ = text_of(m, ex, fo, [("cvLua",)])
        self.assertIn("    /// 0x08000010\n    /// lua ?\n    static int lFoo(lua_State* a1);", txt)
        self.assertIn("UNK_RET Baz() const;", txt)
        self.assertIn("static UNK_DATA s_Cache[0x1c];", txt)
        self.assertIn("#include <lua.h>", txt)
        self.assertNotIn("guess", txt.replace("/// origin guess", ""))

    def test_extern_c(self):
        m, ex, fo = build(self.bf)
        em = E.Emitter(m, ex, fo, {"problems": [], "decisions": __import__("collections").defaultdict(list)})
        lines = em.free_lines((), {"fns": m.scopes[()].fns, "vars": m.scopes[()].vars})
        self.assertEqual(lines, ['extern "C" {', "    /// 0x08000060", "    UNK_RET frkPoolInit(UNK_PARAMS);",
                                 "    /// 0x0a000000", "    extern UNK_DATA g_Thing[0xc];", "}"])


def vt(cls, addr, groups):
    return {"addr": addr, "class": cls, "construction": False,
            "groups": [{"offset_to_top": o, "slots": [{"kind": k, "symbols": s, "target": t} for k, s, t in slots]}
                       for o, slots in groups]}


def ti(cls, symbol, bases=()):
    return {"addr": "0x0b000000", "class": cls, "symbol": symbol, "kind": "si" if bases else "class",
            "bases": [{"class": b, "offset": o, "public": True, "virtual": False, "typeinfo": bs}
                      for b, bs, o in bases]}


class VtableTests(unittest.TestCase):
    def setUp(self):
        syms = [
            sym("_ZN4BaseD1Ev", "0x08001000", "dtor", "D1", True), sym("_ZN4BaseD0Ev", "0x08001010", "dtor", "D0", True),
            sym("_ZN4Base1fEi", "0x08001020", "method", cls=True),
            sym("_ZN7DerivedD1Ev", "0x08002000", "dtor", "D1", True),
            sym("_ZN7DerivedD0Ev", "0x08002010", "dtor", "D0", True),
            sym("_ZN7Derived1fEi", "0x08002020", "method", cls=True),
            sym("_ZTV4Base", "0x0a100000", "vtable", typ="object"),
            sym("_ZTV7Derived", "0x0a100100", "vtable", typ="object"),
            sym("_ZTI4Base", "0x0b000000", "typeinfo", typ="object"),
            sym("_ZTI7Derived", "0x0b000010", "typeinfo", typ="object"),
        ]
        pure = ("pure", ["__cxa_pure_virtual"], "0x08468860")
        self.bf = binfacts(syms, [ti("Base", "_ZTI4Base"), ti("Derived", "_ZTI7Derived", [("Base", "_ZTI4Base", 0)])], [
            vt("Base", "0x0a100000", [(0, [("func", ["_ZN4BaseD1Ev"], "0x08001000"), ("func", ["_ZN4BaseD0Ev"], "0x08001010"),
                                           ("func", ["_ZN4Base1fEi"], "0x08001020"), pure])]),
            vt("Derived", "0x0a100100", [(0, [("func", ["_ZN7DerivedD1Ev"], "0x08002000"),
                                              ("func", ["_ZN7DerivedD0Ev"], "0x08002010"),
                                              ("func", ["_ZN7Derived1fEi"], "0x08002020"),
                                              ("func", [], "0x08002030"), ("func", [], "0x08002040")])]),
        ])

    def test_slots(self):
        m, ex, fo = build(self.bf, {0x08002030: "-", 0x08002040: "r=ECX"})
        txt, _ = text_of(m, ex, fo, [("Base",), ("Derived",)])
        self.assertIn("/// vtable 0x0a100000\nclass Base {", txt)
        self.assertIn("class Derived : public Base {", txt)
        self.assertIn("    /// pure\n    virtual UNK_RET vf_03(UNK_PARAMS) = 0;", txt)
        # the override of the pure slot takes the introducing slot's name; the new entry is vf_04 of Derived
        self.assertIn("    /// 0x08002030\n    virtual UNK_RET vf_03(UNK_PARAMS);", txt)
        self.assertIn("    /// 0x08002040 fastcc\n    virtual UNK_RET vf_04(UNK_PARAMS);", txt)
        self.assertIn("    /// 0x08002000\n    /// deleting 0x08002010\n    virtual ~Derived();", txt)
        d = txt.index("class Derived")
        self.assertLess(txt.index("vf_03(UNK_PARAMS);", d), txt.index("vf_04(UNK_PARAMS);", d))


class VtableChainTests(unittest.TestCase):
    """Classes without _ZTV between a base and a real class: their groups are synthesized from the real class."""

    def build_chain(self):
        syms = [
            sym("_ZN4BaseD1Ev", "0x08001000", "dtor", "D1", True), sym("_ZN4BaseD0Ev", "0x08001010", "dtor", "D0", True),
            sym("_ZN4Base1fEi", "0x08001020", "method", cls=True),
            sym("_ZN4Leaf1fEi", "0x08002020", "method", cls=True),
            sym("_ZTV4Base", "0x0a100000", "vtable", typ="object"),
            sym("_ZTV4Leaf", "0x0a100100", "vtable", typ="object"),
        ]
        names = [("Base", "_ZTI4Base"), ("Mid1", "_ZTI4Mid1"), ("Mid2", "_ZTI4Mid2"), ("Leaf", "_ZTI4Leaf")]
        tis = [ti(n, s, [(names[i - 1][0], names[i - 1][1], 0)] if i else ()) for i, (n, s) in enumerate(names)]
        syms += [sym(s, "0x0b0000%02x" % (16 * i), "typeinfo", typ="object") for i, (_, s) in enumerate(names)]
        bf = binfacts(syms, tis, [
            vt("Base", "0x0a100000", [(0, [("func", ["_ZN4BaseD1Ev"], "0x08001000"),
                                           ("func", ["_ZN4BaseD0Ev"], "0x08001010"),
                                           ("func", ["_ZN4Base1fEi"], "0x08001020")])]),
            # the destructors of Leaf have no symbols; Leaf::f is the only named slot
            vt("Leaf", "0x0a100100", [(0, [("func", [], "0x08002000"), ("func", [], "0x08002010"),
                                           ("func", ["_ZN4Leaf1fEi"], "0x08002020")])]),
        ])
        return build(bf)

    def test_synthesized_chain_keeps_base_slots(self):
        # Mid1 asks for its derived Mid2 while Mid2 needs Mid1's slot count: Mid2 must not cache an empty group
        m, ex, fo = self.build_chain()
        txt, _ = text_of(m, ex, fo, [("Leaf",)])
        self.assertNotIn("vf_00", txt)
        self.assertNotIn("vf_01", txt)
        self.assertIn("virtual ~Leaf();", txt)
        self.assertEqual(txt.count("virtual"), 2)

    def test_named_thunk_takes_a_primary_entry_and_derived_overrides_it(self):
        # Mid::g overrides only the secondary base's virtual; its body is inlined into the exported thunk, so no code
        # link exists. It takes the unnamed new primary entry (slot 1), and Leaf (declared first by name) overrides
        # that entry under the same name instead of adding a slot
        names = [("Prim", "_ZTI4Prim", []), ("Sec", "_ZTI3Sec", []),
                 ("Mid", "_ZTI3Mid", [("Prim", "_ZTI4Prim", 0), ("Sec", "_ZTI3Sec", 4)]),
                 ("Leaf", "_ZTI4Leaf", [("Mid", "_ZTI3Mid", 0)])]
        thunk = "_ZThn4_N3Mid1gEv"
        syms = [sym("_ZTV4Prim", "0x0a100000", "vtable", typ="object"), sym("_ZTV3Mid", "0x0a100100", "vtable", typ="object"),
                sym("_ZTV4Leaf", "0x0a100200", "vtable", typ="object"), sym(thunk, "0x08002100", "thunk", cls=True)]
        syms += [sym(s, "0x0b0000%02x" % (16 * i), "typeinfo", typ="object") for i, (_, s, _) in enumerate(names)]
        bf = binfacts(syms, [ti(n, s, b) for n, s, b in names], [
            vt("Prim", "0x0a100000", [(0, [("func", [], "0x08001000")])]),
            vt("Mid", "0x0a100100", [(0, [("func", [], "0x08001000"), ("func", [], "0x08002010")]),
                                     (-4, [("thunk", [thunk], "0x08002100")])]),
            vt("Leaf", "0x0a100200", [(0, [("func", [], "0x08001000"), ("func", [], "0x08003010")]),
                                      (-4, [("func", [], "0x08003100")])]),
        ])
        m, ex, fo = build(bf)
        txt, _ = text_of(m, ex, fo, [("Mid",), ("Leaf",)])
        mid = txt[txt.index("class Mid"):txt.index("class Leaf")]
        leaf = txt[txt.index("class Leaf"):]
        self.assertIn("    /// 0x08002010\n    /// thunk 0x08002100\n    virtual UNK_RET g();", mid)
        self.assertEqual(mid.count("virtual"), 1)
        self.assertIn("/// 0x08003010\n    virtual UNK_RET g();", leaf)
        self.assertEqual(leaf.count("virtual"), 1)

class ForeignVirtualTests(unittest.TestCase):
    HEADER = ("namespace tbb { class task { public: virtual ~task() {} virtual task* execute() = 0; "
              "virtual void note_affinity(unsigned short) {} int data; }; }\n")

    def test_virtuals_of_a_foreign_class(self):
        import tempfile
        from pathlib import Path
        from seed import existing as X
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / "compat").mkdir()
            (Path(d) / "compat" / "tbb.h").write_text(self.HEADER, encoding="utf-8")
            fo = X.Foreign(d)
        self.assertEqual(fo.virtuals[("tbb", "task")], [("~task", "", (), False), ("execute", "tbb::task*", (), False),
                                                       ("note_affinity", "void", ("unsigned short",), False)])

    def test_override_of_a_foreign_slot_is_named(self):
        class Fo(FakeForeign):
            virtuals = {("tbb", "task"): [("~task", "", (), False), ("execute", "tbb::task*", (), False),
                                          ("note_affinity", "void", ("unsigned short",), False)]}

            def kind(self, p):
                return "class" if p == ("tbb", "task") else super().kind(p)

        imp = ("import", ["_ZN3tbb4task13note_affinityEt"], "0x08467350")
        bf = binfacts([sym("_ZTV7JobTask", "0x0a100000", "vtable", typ="object"),
                       sym("_ZTI7JobTask", "0x0b000000", "typeinfo", typ="object")],
                      [ti("JobTask", "_ZTI7JobTask", [("tbb::task", "_ZTIN3tbb4taskE", 0)])],
                      [vt("JobTask", "0x0a100000", [(0, [("func", [], "0x08870b52"), ("func", [], "0x08dc291e"),
                                                         ("func", [], "0x08dc2932"), imp])])])
        facts = MD.Facts(bf, {})
        facts.code = None
        ex, fo = FakeExisting(), Fo()
        m = MD.build(facts, ex)
        MD.finish_structors(m)
        MD.classify(m, ex, fo)
        MD.apply_template_refs(m, ex)
        MD.template_template_params(m)
        VT.run(m, facts, ex, fo)
        MD.finish_members(m)
        txt, _ = text_of(m, ex, fo, [("JobTask",)])
        self.assertIn("    /// 0x08870b52\n    /// deleting 0x08dc291e\n    virtual ~JobTask();", txt)
        self.assertIn("    /// 0x08dc2932\n    virtual tbb::task* execute();", txt)
        self.assertNotIn("vf_", txt)


class LonePaddingTests(unittest.TestCase):
    def test_lone_base_behind_vptr_gets_a_body(self):
        syms = [sym("_ZTV5Child", "0x0a100000", "vtable", typ="object"),
                sym("_ZTI5Child", "0x0b000000", "typeinfo", typ="object"),
                sym("_ZTI4Flat", "0x0b000010", "typeinfo", typ="object")]
        bf = binfacts(syms, [ti("Flat", "_ZTI4Flat"), ti("Child", "_ZTI5Child", [("Flat", "_ZTI4Flat", 4)])],
                      [vt("Child", "0x0a100000", [(0, [("func", ["_ZN5ChildD1Ev"], "0x08002000"),
                                                       ("func", ["_ZN5ChildD0Ev"], "0x08002010")])])])
        bf["symbols"] += [sym("_ZN5ChildD1Ev", "0x08002000", "dtor", "D1", True),
                          sym("_ZN5ChildD0Ev", "0x08002010", "dtor", "D0", True)]
        m, ex, fo = build(bf)
        T.compute_pads(m, ex)
        self.assertEqual(m.scopes[("Flat",)].pad, (0, 4))
        self.assertIn("Flat +0x0[0x4]", m.decisions["padding_for_base_offset"])


class TemplateTests(unittest.TestCase):
    def test_generalized_member(self):
        bf = binfacts([
            sym("_ZN3FooIiE4pushERKi", "0x08003000", "function", bind="weak"),
            sym("_ZN3FooIfE4pushERKf", "0x08003100", "function", bind="weak"),
            sym("_ZN3FooIiE4sizeEi", "0x08003200", "function", bind="weak"),
            sym("_ZN3FooIfE4sizeEi", "0x08003300", "function", bind="weak"),
        ])
        m, ex, fo = build(bf)
        txt, _ = text_of(m, ex, fo, [("Foo",)])
        self.assertIn("/// 0x08003100 Foo<float>\n    /// 0x08003000 Foo<int>\n    UNK_RET push(T0 const& a1);", txt)
        self.assertIn("UNK_RET size(int a1);", txt)
        self.assertIn("template <class T0>\nclass Foo {", txt)


class PlaceTests(unittest.TestCase):
    def test_rules(self):
        ev = {"fxmlelement": [("FirePlace/FireWorks/FXmlElement.h", ".h")],
              "cvcity": [("Civ5/Src/App/Graphics/cvCity.cpp", ".cpp")],
              "fluafoo": [("FirePlace/FireWorks/FLua/Source/FLuaFoo.cpp", ".cpp")]}
        rules = [("UI*", "Civ5/Src/App/UI"), ("*", "Civ5/Src/App")]
        units = [{"key": ("FXmlElement",), "name": "FXmlElement", "qual": "FXmlElement", "addr": 10},
                 {"key": ("cvCity",), "name": "cvCity", "qual": "cvCity", "addr": 20},
                 {"key": ("FLuaFoo",), "name": "FLuaFoo", "qual": "FLuaFoo", "addr": None},
                 {"key": ("UIThing",), "name": "UIThing", "qual": "UIThing", "addr": None},
                 {"key": ("UITHING",), "name": "UITHING", "qual": "UITHING", "addr": None}]
        dec = __import__("collections").defaultdict(list)
        out = PL.place(units, ev, rules, dec)
        self.assertEqual(out[("FXmlElement",)], ("FirePlace/FireWorks/FXmlElement.h", "binary"))
        self.assertEqual(out[("cvCity",)], ("Civ5/Src/App/Graphics/cvCity.h", "guess"))
        self.assertEqual(out[("FLuaFoo",)], ("FirePlace/FireWorks/FLua/Include/FLuaFoo.h", "guess"))
        self.assertEqual(out[("UIThing",)][0].lower(), "civ5/src/app/ui/uithing.h")
        self.assertEqual(out[("UITHING",)], out[("UIThing",)])   # case collision: one file


class SafetyTests(unittest.TestCase):
    def test_env_accepts_tree_spec_lines(self):
        want = b"x\n/// inline\nvoid f();\n"
        have = b"x\n/// 0x08000000 Foo<int>\nvoid f();\n"
        self.assertTrue(env.tree_amended(have, want))
        self.assertFalse(env.tree_amended(b"x\n// changed\nvoid f();\n", want))

    def test_sdkspec_apply(self):
        text = "a\n    /// inline\n    void f();\n"
        self.assertEqual(SS.apply(text, {3: ["/// 0x08000000 Foo<int>"]}),
                         "a\n    /// 0x08000000 Foo<int>\n    void f();\n")

    def test_export_coverage_cfg(self):
        import tempfile
        from pathlib import Path
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "civ5re.cfg"
            p.write_text("[checks]\njobs = 0\n\n[seed]\nx = 1\n", encoding="utf-8")
            T.set_export_coverage(d)
            self.assertEqual(p.read_text(encoding="utf-8"),
                             "[checks]\njobs = 0\nexport_coverage = required\n\n[seed]\nx = 1\n")
            T.set_export_coverage(d)
            self.assertEqual(p.read_text(encoding="utf-8").count("export_coverage"), 1)


if __name__ == "__main__":
    unittest.main()
