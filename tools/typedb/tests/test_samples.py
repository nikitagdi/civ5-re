"""clang_ast / llvm_ir / typegraph / variants on saved samples (no clang needed)."""
import unittest

from typedb import clang_ast, llvm_ir, variants
from typedb.build import _linkage, _unify_structor
from typedb.typegraph import TypeGraph, parse_thunk

from .util import FIXTURES

SAMPLES = FIXTURES / "samples"


def tree_map(p):
    return ("Civ5Expansion2/" + p.replace("\\", "/").split("src/")[-1], "tree") if "s.h" in p else (p, "other")


class AstSample(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        root = clang_ast.load_json(SAMPLES / "ast_small.json")
        cls.model = clang_ast.extract(root, tree_map)
        cls.by = {}
        for d in cls.model.decls:
            if d.qualified:
                cls.by.setdefault(d.qualified, d)

    def test_delta_lines_after_pruned_bodies(self):
        # `int f() { return x + 1; }` has a body (pruned); the next field still gets its own line.
        self.assertEqual(self.by["S::f"].line, 3)
        self.assertTrue(self.by["S::f"].has_body)
        self.assertEqual(self.by["S::y"].line, 4)
        self.assertEqual(self.by["P"].line, 6)
        self.assertEqual(self.by["kConst"].line, 8)
        self.assertEqual(self.by["Outer::sm"].line, 11)
        self.assertTrue(all(d.file == "Civ5Expansion2/s.h" for d in self.model.decls))

    def test_macro_expansion_loc(self):
        a, b = self.by["S::a"], self.by["S::b"]
        self.assertTrue(a.macro and b.macro)
        self.assertEqual((a.line, b.line), (2, 2))
        self.assertFalse(self.by["S::x"].macro)

    def test_definition_data_and_mangled(self):
        self.assertTrue(self.by["S"].node["definitionData"].get("canPassInRegisters"))
        self.assertFalse(self.by["P"].node["definitionData"].get("canPassInRegisters", False))
        self.assertEqual(self.by["sfun"].node["mangledName"], "_ZL4sfuni")
        self.assertEqual(self.by["kConst"].node["mangledName"], "_ZL6kConst")
        self.assertEqual(self.by["(anonymous namespace)::anon"].node["mangledName"], "_ZN12_GLOBAL__N_14anonEi")
        self.assertEqual(self.by["cstat"].node["mangledName"], "_ZL5cstati")
        self.assertEqual(self.by["Outer::sm"].node["mangledName"], "_ZN5Outer2smE")
        self.assertTrue(self.by["cstat"].extern_c)

    def test_template_pattern_and_instantiation(self):
        v = self.by["V"]
        self.assertEqual((v.ent, v.template), ("record_template", "pattern"))
        self.assertEqual(v.instantiations, [])
        push = self.by["V::push"]
        self.assertTrue(push.in_pattern)
        self.assertNotIn("mangledName", push.node)

    def test_linkage_from_mangled(self):
        self.assertEqual(_linkage("_ZL4sfuni"), "internal")
        self.assertEqual(_linkage("_ZL6kConst"), "internal")
        self.assertEqual(_linkage("_ZL5cstati"), "internal")
        self.assertEqual(_linkage("_ZN12_GLOBAL__N_17anonfunEi"), "internal")
        self.assertEqual(_linkage("_ZN5Outer2smE"), "external")
        self.assertEqual(_linkage("_ZN4FLua7DetailsL12DescribeTypeIbEEPKcv"), "internal")
        self.assertEqual(_linkage("_ZN4FLua7Details12DescribeTypeIbEEPKcv"), "external")
        # an `L` inside template arguments (literals) is not the entity's own
        self.assertEqual(_linkage("_ZN15FGlobalFacilityI15CvNexusListener19FNewInstanceBuilderIS0_L10eMPoolType26ELi0EEE"
                                  "13sm_pkInstanceE"), "external")
        self.assertEqual(_linkage("_ZNK7FEndian5CountIcEEiT_NS_11EEndiannessE"), "external")
        self.assertEqual(_linkage("FSBAAllocate"), "external")

    def test_parse_diagnostics(self):
        d = clang_ast.parse_diagnostics("C:/x/a.h:4:7: error: unknown type name 'Q'\nnote: foo\n")
        self.assertEqual(d, [("C:/x/a.h", 4, 7, "error", "unknown type name 'Q'")])


class IrSample(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.mod = llvm_ir.parse((SAMPLES / "ir_small.ll").read_text(encoding="utf-8"))
        cls.tg = TypeGraph(cls.mod, lambda p: (p, "other"))

    def test_quoted_symbols(self):
        self.assertEqual(llvm_ir.ptr_target(self.mod.globals["pf"]), "odd$name")
        self.assertEqual(llvm_ir.unquote_symbol('@"\\01weird"'), "\x01weird")

    def test_overload_by_id_alias(self):
        g = self.mod.globals["_ZN3Gfx11__typedb_f1E"]
        self.assertEqual(llvm_ir.ptr_target(g), "_ZN3Gfx4LoadEfPNS_3TexE")

    def test_size_probe(self):
        self.assertEqual(llvm_ir.int_array(self.mod.globals["__typedb_s1"]), [12, 4, 12])

    def test_vtable_groups(self):
        groups = llvm_ir.vtable_arrays(self.mod.globals["_ZTV11__typedb_P1"])
        self.assertEqual(len(groups), 2)
        self.assertEqual(groups[0][:2], [("int", 0), ("sym", "_ZTI11__typedb_P1")])
        self.assertEqual(groups[1][0], ("int", -8))
        self.assertEqual(groups[1][2], ("sym", "_ZThn8_N1D7OnEventEi"))
        self.assertEqual(llvm_ir.typeinfo_base(self.mod.globals["_ZTI11__typedb_P1"]), "_ZTI1D")

    def test_di_records_and_spelling(self):
        d = self.tg.records["D"]
        self.assertEqual(d.get("size"), 96)
        fields, bases, methods = self.tg.record_layout(d)
        self.assertEqual([(b[0], b[1]) for b in bases], [("B", 0), ("E", 64)])
        self.assertTrue(self.tg.dynamic(d))
        g = self.mod.globals["_ZN3Gfx11__typedb_f1E"]
        var = self.tg.node(self.mod.meta[g.dbg].get("var"))
        tid = self.tg.tid(var.get("type"))
        # the probe's `__typedb_id<…>` alias is transparent: the pointer points at the function type itself
        self.assertEqual(self.tg.types[tid]["to"], "void (float, Gfx::Tex *)")
        self.assertEqual(self.tg.types["void (float, Gfx::Tex *)"],
                         {"kind": "function", "ret": "void", "params": ["float", "Gfx::Tex *"], "variadic": False})

    def test_thunk_names(self):
        self.assertEqual(parse_thunk("_ZThn4_N9FireWorks15cvUnitVisSystem7OnEventEi"),
                         ("_ZN9FireWorks15cvUnitVisSystem7OnEventEi", {"nv": -4, "vcall": None}, "_ZThn4_"))
        self.assertEqual(parse_thunk("_ZTv0_n12_N8VDiamond1fEv"),
                         ("_ZN8VDiamond1fEv", {"nv": 0, "vcall": -12}, "_ZTv0_n12_"))
        self.assertEqual(parse_thunk("_ZTch0_h4_N1A1fEv")[0], "_ZN1A1fEv")
        self.assertIsNone(parse_thunk("_ZN1A1fEv"))


class Variants(unittest.TestCase):
    def test_c1foo_trap(self):
        dem = {"_ZN5C1FooC1Ev": "C1Foo::C1Foo()", "_ZN5C2FooC1Ev": "C2Foo::C2Foo()", "_ZN5C1FooC2Ev": "C1Foo::C1Foo()"}
        self.assertEqual(variants.candidates("_ZN5C1FooC1Ev", "C1", "C2"), ["_ZN5C2FooC1Ev", "_ZN5C1FooC2Ev"])
        self.assertEqual(variants.resolve("_ZN5C1FooC1Ev", "ctor", False, dem),
                         ({"C1": "_ZN5C1FooC1Ev", "C2": "_ZN5C1FooC2Ev"}, None))

    def test_no_candidate_is_internal(self):
        v, err = variants.resolve("asm_ctor", "ctor", False, {"asm_ctor": "asm_ctor"})
        self.assertIsNotNone(err)

    def test_d0_only_for_virtual(self):
        dem = {"_ZN1AD1Ev": "A::~A()", "_ZN1AD2Ev": "A::~A()", "_ZN1AD0Ev": "A::~A()"}
        self.assertEqual(variants.resolve("_ZN1AD1Ev", "dtor", False, dem)[0]["D0"], None)
        self.assertEqual(variants.resolve("_ZN1AD1Ev", "dtor", True, dem)[0]["D0"], "_ZN1AD0Ev")

    def test_di_structor_names(self):
        self.assertEqual(_unify_structor("_ZN5C4FooC4Ev", {"_ZN5C4FooC1Ev"}), "_ZN5C4FooC1Ev")
        self.assertEqual(_unify_structor("_ZN1AD4Ev"), "_ZN1AD1Ev")


if __name__ == "__main__":
    unittest.main()
