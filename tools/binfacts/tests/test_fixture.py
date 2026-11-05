import json
import struct
import tempfile
import unittest

from binfacts import build, ehframe, groups, rtti, symbols, vtables
from binfacts.elf import Elf, R_386_COPY, R_386_JUMP_SLOT
from binfacts.tests import support
from binfacts.tests.support import FIXTURES, FIXTURE_ELF, TableDemangler
from civ5re import config

LLVM = support.llvm_bin()
needs_llvm = unittest.skipUnless(LLVM, "LLVM is not available: demangling, groups and the reference are skipped")


def sha256_line():
    return (FIXTURES / "fixture.elf.sha256").read_text(encoding="utf-8").splitlines()[0].split()[0]


class FixtureElf(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.elf = Elf.open(FIXTURE_ELF)
        cls.defined = cls.elf.defined_symbols()

    def test_sha256_matches_the_recorded_one(self):
        self.assertEqual(build.sha256_bytes(FIXTURE_ELF.read_bytes()), sha256_line())

    def test_image_layout(self):
        self.assertEqual(self.elf.e_type, 2)
        self.assertIsNotNone(self.elf.section(".eh_frame_hdr"))
        self.assertIsNone(self.elf.section(".interp"))

    def test_relocations(self):
        kinds = [r[0] for r in self.elf.relocs.values()]
        self.assertEqual(kinds.count(R_386_COPY), 4)
        self.assertEqual(kinds.count(R_386_JUMP_SLOT), len(self.elf.plt_stubs()))

    def test_plt_and_canonical_address(self):
        stubs = {n: a for a, n in self.elf.plt_stubs()}
        for name in ("__cxa_pure_virtual", "ext_init", "ext_hook", "__cxa_guard_acquire"):
            self.assertIn(name, stubs)
        canonical = {s.name: s.value for s in self.elf.plt_imports()}
        self.assertEqual(canonical["__cxa_pure_virtual"], stubs["__cxa_pure_virtual"])

    def test_eh_frame_agrees_with_eh_frame_hdr(self):
        eh = self.elf.section(".eh_frame")
        fdes = ehframe.parse(self.elf.section_data(".eh_frame"), eh.addr)
        count, = struct.unpack_from("<I", self.elf.section_data(".eh_frame_hdr"), 8)
        self.assertEqual(len(fdes), count)
        self.assertGreater(len(fdes), 30)
        text = self.elf.section(".text")
        for start, size in fdes:
            self.assertTrue(text.addr <= start < text.addr + text.size)
            self.assertGreater(size, 0)

    def test_symbol_counts(self):
        self.assertEqual(len(self.defined), 126)
        self.assertEqual(sum(1 for s in self.defined if s.type == 2), 80)


class FixtureStructure(unittest.TestCase):
    """RTTI and vtables, with names from the committed reference so that LLVM is not needed."""

    @classmethod
    def setUpClass(cls):
        cls.elf = Elf.open(FIXTURE_ELF)
        defined = cls.elf.defined_symbols()
        dem = TableDemangler()
        roots = vtables.typeinfo_roots(cls.elf, defined)
        cls.typeinfo = rtti.parse(cls.elf, defined, roots, dem)
        cls.by_class = {t.class_: t for t in cls.typeinfo}
        cls.vtables = {(v["class"], v["construction"]): v for v in vtables.build(cls.elf, defined, cls.typeinfo, dem)}

    def test_typeinfo_kinds(self):
        kinds = {t.class_: t.kind for t in self.typeinfo}
        self.assertEqual(kinds["ns::Shape"], "class")
        self.assertEqual(kinds["ns::Square"], "si")
        self.assertEqual(kinds["ns::Mix"], "vmi")
        self.assertEqual(kinds["ns::DV"], "vmi")
        self.assertEqual(kinds["ns::DD"], "si")

    def test_bases(self):
        b = self.by_class["ns::Mix"].bases
        self.assertEqual([(x["class"], x["offset"], x["virtual"], x["public"]) for x in b],
                         [("ns::M1", 0, False, True), ("ns::M2", 8, False, True)])
        v = self.by_class["ns::DV"].bases[0]
        self.assertEqual((v["class"], v["virtual"], v["offset"] < 0), ("ns::VB", True, True))

    def test_imported_base_has_a_class_and_no_object(self):
        base = self.by_class["ns::UsesExt"].bases[0]
        self.assertEqual((base["class"], base["typeinfo"]), ("ExtBase", "_ZTI7ExtBase"))
        self.assertNotIn("ExtBase", self.by_class)

    def test_vmi_flags_and_si_without_flags(self):
        self.assertEqual(self.by_class["ns::Mix"].flags, 0)
        self.assertIsNone(self.by_class["ns::Square"].flags)

    def test_every_vtable_group_points_to_the_class_typeinfo(self):
        by_addr = {t.addr: t for t in self.typeinfo}
        for (cls, _construction), v in self.vtables.items():
            for g in v["groups"]:
                self.assertEqual(by_addr[g["typeinfo"]].class_, cls)

    def test_vtable_inventory(self):
        self.assertEqual(len(self.vtables), 12)
        self.assertEqual(sum(1 for (_c, cons) in self.vtables if cons), 1)
        self.assertNotIn("__cxxabiv1::__class_type_info", [c for c, _ in self.vtables])

    def test_pure_and_slot_kinds(self):
        slots = self.vtables[("ns::Shape", False)]["groups"][0]["slots"]
        self.assertEqual([s["kind"] for s in slots], ["func", "func", "pure", "func"])
        self.assertEqual(slots[2]["symbols"], ["__cxa_pure_virtual"])

    def test_secondary_group_and_non_virtual_thunks(self):
        g = self.vtables[("ns::Mix", False)]["groups"]
        self.assertEqual([x["offset_to_top"] for x in g], [0, -8])
        self.assertEqual([s["kind"] for s in g[1]["slots"]], ["thunk"] * 3)
        self.assertTrue(g[1]["slots"][0]["symbols"][0].startswith("_ZThn8_"))

    def test_unexported_secondary_slots_become_decoded_thunks(self):
        # the fixture thunks are inlined bodies; patch one into `addl $-8, 4(%esp); jmp Mix::b` in a copy
        elf = Elf(FIXTURE_ELF.read_bytes())
        by_name = {s.name: s for s in elf.defined_symbols()}
        thunk, dest = by_name["_ZThn8_N2ns3Mix1bEv"], by_name["_ZN2ns3Mix1bEv"]
        off = next(o + thunk.value - b for b, o, fsz, _m in elf.loads if b <= thunk.value < b + fsz)
        code = b"\x83\x44\x24\x04\xf8\xe9" + struct.pack("<i", dest.value - (thunk.value + 10))
        data = bytearray(elf.data)
        data[off:off + len(code)] = code
        elf = Elf(bytes(data))
        defined = [s for s in elf.defined_symbols() if not s.name.startswith(("_ZTh", "_ZTv"))]
        dem = TableDemangler()
        ti = rtti.parse(elf, defined, vtables.typeinfo_roots(elf, defined), dem)
        mix = {(v["class"], v["construction"]): v for v in vtables.build(elf, defined, ti, dem)}[("ns::Mix", False)]
        slots = mix["groups"][1]["slots"]
        self.assertEqual([s["kind"] for s in slots], ["func", "func", "thunk"])
        self.assertEqual([s["symbols"] for s in slots], [[]] * 3)
        self.assertEqual([s["target"] for s in slots], [s["target"] for s in self.vtables[("ns::Mix", False)]["groups"][1]["slots"]])
        decoded = [s for s in slots if "thunk_target" in s]
        self.assertEqual([(s["thunk_target"], s["adjust"]) for s in decoded], [(dest.value, -8)])
        self.assertNotIn("thunk_target", mix["groups"][0]["slots"][0])

    def test_virtual_base_group_header(self):
        v = self.vtables[("ns::DV", False)]
        g = v["groups"]
        self.assertEqual(len(g), 2)
        self.assertEqual(g[0]["vbase_offsets"], [8])
        self.assertEqual(g[1]["offset_to_top"], -8)
        self.assertEqual(g[1]["vbase_offsets"], [-8, -8])
        self.assertTrue(any(s["symbols"][0].startswith("_ZTv0_") for s in g[1]["slots"]))
        self.assertEqual(g[0]["address_point"], v["addr"] + 12)

    def test_construction_vtable(self):
        v = self.vtables[("ns::DV", True)]
        self.assertTrue(v["construction"])
        self.assertTrue(v["symbol"].startswith("_ZTC"))
        self.assertEqual(len(v["groups"]), 2)

    def test_import_slots(self):
        slots = self.vtables[("ns::UsesExt", False)]["groups"][0]["slots"]
        self.assertEqual([s["kind"] for s in slots], ["import", "func", "import", "func"])

    def test_zero_slot_is_a_parse_error(self):
        original = Elf(FIXTURE_ELF.read_bytes())
        sym = next(s for s in original.defined_symbols() if s.name == "_ZTVN2ns5ShapeE")
        va = sym.value + 8
        off = next(o + va - b for b, o, fsz, _m in original.loads if b <= va < b + fsz)
        data = bytearray(original.data)
        data[off:off + 4] = b"\0\0\0\0"
        broken = Elf(bytes(data))
        broken_sym = next(s for s in broken.defined_symbols() if s.name == "_ZTVN2ns5ShapeE")
        # a zeroed slot reads as an offset word, so the error is reported at the next slot
        with self.assertRaisesRegex(Exception, "zero or unrecognized word at 0x%08x" % (va + 4)):
            vtables.parse_one(broken, broken_sym)


@needs_llvm
class FixtureWithLlvm(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.dem = symbols.Demangler(config.load().tool("llvm-cxxfilt"))
        cls.rules = groups.load_rules(FIXTURES / "groups.cfg")
        cls.data = FIXTURE_ELF.read_bytes()
        cls.facts = build.extract(cls.data, "fixture.elf", build.sha256_bytes(cls.data), cls.dem, cls.rules, "")

    def test_matches_the_reference(self):
        got = json.loads(build.dumps(self.facts))
        want = support.load_expected()
        got.pop("tool")
        want.pop("tool")
        self.assertEqual(got, want)

    def test_output_is_deterministic(self):
        again = build.extract(self.data, "fixture.elf", build.sha256_bytes(self.data), self.dem, self.rules, "")
        self.assertEqual(build.dumps(self.facts), build.dumps(again))

    def test_rebuild_gives_the_recorded_sha256(self):
        from binfacts.tests.fixtures import build_fixture
        recorded = (FIXTURES / "fixture.elf.sha256").read_text(encoding="utf-8").splitlines()[1].lstrip("# ")
        if build_fixture.clang_version(LLVM) != recorded:
            self.skipTest("another clang than the one that built the fixture")
        with tempfile.TemporaryDirectory() as tmp:
            elf = build_fixture.build(LLVM, tmp)
            self.assertEqual(build_fixture.sha256_of(elf), sha256_line())

    def test_shapes_and_values(self):
        syms = {s["mangled"]: s for s in self.facts["symbols"]}
        s = syms["_ZNK2ns3VecclEi"]
        self.assertEqual((s["kind"], s["scope"], s["name"], s["params"], s["cv"]),
                         ("method", "ns::Vec", "operator()", "int", "const"))
        s = syms["_ZNK2ns3VecltERKS0_"]
        self.assertEqual((s["name"], s["params"]), ("operator<", "ns::Vec const&"))
        s = syms["_ZN2ns4Stat4bumpEv"]
        self.assertEqual((s["kind"], s["scope_is_class"]), ("function", None))
        s = syms["_ZN2ns4Stat5countE"]
        self.assertEqual((s["kind"], s["scope"]), ("data", "ns::Stat"))
        s = syms["_ZGVZN2ns7counterEvE1c"]
        self.assertEqual((s["kind"], s["bind"]), ("guard", "weak"))
        self.assertEqual(syms["lua_version"]["group"], "lua")
        self.assertEqual(syms["_start"]["group"], "crt")
        self.assertEqual(syms["_ZNK2ns9AslHelper3runEv"]["group"], "aspyr")
        s = syms["_ZN2ns4tmaxIiEET_S1_S1_"]
        self.assertEqual((s["kind"], s["scope"], s["name"], s["params"]), ("function", "ns", "tmax<int>", "int, int"))
        box = syms["_ZN2ns3BoxIiEC1Ei"]
        self.assertEqual((box["bind"], box["scope"]), ("weak", "ns::Box<int>"))

    def test_ctor_and_dtor_variants(self):
        by = {}
        for s in self.facts["symbols"]:
            if s["variant"]:
                by.setdefault(s["scope"], {})[s["variant"]] = s["addr"]
        shape = by["ns::Shape"]
        self.assertIn("C2", shape)
        self.assertNotIn("C1", shape)
        self.assertIn("D0", shape)
        sq = by["ns::Square"]
        self.assertEqual(sq["C1"], sq["C2"])
        dv = by["ns::DV"]
        self.assertNotEqual(dv["C1"], dv["C2"])
        self.assertNotEqual(dv["D1"], dv["D2"])

    def test_scope_is_class_evidence(self):
        syms = {s["mangled"]: s for s in self.facts["symbols"]}
        self.assertTrue(syms["_ZN2ns5Shape5touchEv"]["scope_is_class"])
        self.assertIsNone(syms["_ZN2ns11use_counterEv"]["scope_is_class"])
        self.assertIsNone(syms["fixture_c_func"]["scope_is_class"])

    def test_stats(self):
        st = self.facts["stats"]
        self.assertEqual(set(st), {"groups", "kinds", "variants", "aliases", "denominator"})
        self.assertEqual(sum(st["groups"].values()), len(self.facts["symbols"]))
        v = st["variants"]["firaxis"]
        self.assertEqual(v["symbols"], {"C1": 7, "C2": 8, "C3": 0, "D0": 10, "D1": 9, "D2": 9})
        self.assertEqual(v["entities"], {"c1_eq_c2": 5, "c1_ne_c2": 2, "c1_only": 0, "c2_only": 1,
                                         "d1_eq_d2": 7, "d1_ne_d2": 2, "d1_only": 0, "d2_only": 0, "has_d0": 10})
        d = st["denominator"]
        self.assertEqual(d["total"], sum(d["by_kind"].values()))
        self.assertEqual(set(d["by_kind"]), {"function", "method", "ctor", "dtor", "data", "thunk", "vtable", "c"})
        self.assertEqual(d["covered_by_class"], {"typeinfo": 10, "typeinfo_name": 10, "guard": 1})
        self.assertEqual(st["aliases"], {"addresses": 13, "names": 28})
        self.assertEqual(st["kinds"]["firaxis"]["ctor"], d["by_kind"]["ctor"])

    def test_sizes_are_lower_bounds_from_structors(self):
        sizes = self.facts["sizes"]
        self.assertEqual(sizes["ns::Shape"]["min"], 8)      # vptr + id
        self.assertEqual(sizes["ns::Square"]["min"], 12)    # + side
        self.assertEqual(set(sizes["ns::Square"]), {"min", "from"})
        self.assertEqual(sizes["ns::Square"]["from"], ["0x00403770"])

    def test_alias_count_matches_symbols(self):
        by_addr = {}
        for s in self.facts["symbols"]:
            if s["type"] == "func":
                by_addr.setdefault(s["addr"], []).append(s["mangled"])
        multi = [v for v in by_addr.values() if len(v) > 1]
        self.assertEqual(len(multi), self.facts["stats"]["aliases"]["addresses"])

    def test_arrays_are_sorted(self):
        for key in ("symbols", "functions_eh", "plt", "typeinfo", "vtables"):
            addrs = [x["addr"] for x in self.facts[key]]
            self.assertEqual(addrs, sorted(addrs), key)
        syms = [(s["addr"], s["mangled"]) for s in self.facts["symbols"]]
        self.assertEqual(syms, sorted(syms))

    def test_invariants(self):
        for s in self.facts["symbols"]:
            if s["kind"] in ("ctor", "dtor"):
                self.assertIsNotNone(s["variant"])
            else:
                self.assertIsNone(s["variant"])
        self.assertTrue(any(s["group"] == "firaxis" for s in self.facts["symbols"]))
        self.assertTrue(all(s["group"] in groups.GROUPS for s in self.facts["symbols"]))


if __name__ == "__main__":
    unittest.main()
