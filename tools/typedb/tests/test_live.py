"""Live tests on the fixture tree with the fixture third_party (skipped without LLVM)."""
import io
import json
import shutil
import unittest
from contextlib import redirect_stdout, redirect_stderr
from unittest import mock

from typedb import api, schema
from typedb.build import build_db

from .util import THIRD_PARTY, built, fixture_tree, needs_llvm, tmp_root


def _rec(db, name):
    return next(r for r in db["records"] if r["name"] == name)


def _fn(db, key):
    return next(f for f in db["functions"] if f["key"] == key)


def _slots(group):
    return [(s["kind"], s["symbol"]) for s in group["slots"]]


@needs_llvm
class MainFixture(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.path, cls.db = built("main")

    def test_clean_and_valid(self):
        self.assertEqual(self.db["diagnostics"], [])
        self.assertTrue(self.db["ok"])
        self.assertEqual(schema.validate(self.db), [])
        self.assertEqual(self.db["schema"], "civ5re.typedb/1")

    def test_multiple_inheritance_vtable(self):
        r = _rec(self.db, "FireWorks::cvUnitVisSystem")
        self.assertEqual((r["size"], r["align"], r["dsize"]), (0x3c, 4, 0x3c))
        self.assertEqual(r["vtable_symbol"], "_ZTVN9FireWorks15cvUnitVisSystemE")
        self.assertEqual(r["typeinfo_symbol"], "_ZTIN9FireWorks15cvUnitVisSystemE")
        g0, g1 = r["vtable"]["groups"]
        self.assertEqual([s["kind"] for s in g0["slots"]], ["D1", "D0", "method", "method", "method"])
        self.assertEqual([s["symbol"] for s in g0["slots"]][2:], [
            "_ZN9FireWorks12cvSystemBase6UpdateEv", "_ZNK9FireWorks12cvSystemBase4PrioEv",
            "_ZN9FireWorks15cvUnitVisSystem7OnEventEi"])
        self.assertEqual(g0["slots"][0]["symbol"], "_ZN9FireWorks15cvUnitVisSystemD1Ev")
        self.assertEqual(g0["slots"][1]["symbol"], "_ZN9FireWorks15cvUnitVisSystemD0Ev")
        self.assertEqual(g0["slots"][0]["function"], "_ZN9FireWorks15cvUnitVisSystemD1Ev")
        self.assertEqual((g0["subobject"], g0["offset_to_top"]), ("FireWorks::cvUnitVisSystem", 0))
        self.assertEqual((g1["subobject"], g1["offset_to_top"], g1["vbase_offsets"]),
                         ("FireWorks::IEventListener", -4, []))
        self.assertEqual(g1["slots"], [{"kind": "method", "function": "_ZN9FireWorks15cvUnitVisSystem7OnEventEi",
                                        "symbol": "_ZThn4_N9FireWorks15cvUnitVisSystem7OnEventEi",
                                        "thunk": {"nv": -4, "vcall": None}}])
        self.assertEqual(r["vptrs"], [0, 4])
        self.assertEqual([(b["name"], b["offset"], b["virtual"]) for b in r["bases"]],
                         [("FireWorks::cvSystemBase", 0, False), ("FireWorks::IEventListener", 4, False)])

    def test_pure_slot(self):
        g = _rec(self.db, "FireWorks::IEventListener")["vtable"]["groups"][0]
        self.assertEqual(g["slots"], [{"kind": "pure", "function": "_ZN9FireWorks14IEventListener7OnEventEi",
                                       "symbol": "__cxa_pure_virtual", "thunk": None}])

    def test_virtual_diamond(self):
        r = _rec(self.db, "VDiamond")
        self.assertEqual((r["size"], r["align"], r["dsize"]), (44, 4, 44))
        groups = r["vtable"]["groups"]
        self.assertEqual([(g["subobject"], g["offset_to_top"], g["vbase_offsets"]) for g in groups],
                         [("VDiamond", 0, [36]), ("VRight", -8, [28]), ("VBase", -36, [-36])])
        self.assertEqual(groups[2]["slots"][0]["symbol"], "_ZTv0_n12_N8VDiamond1fEv")
        self.assertEqual(groups[2]["slots"][0]["thunk"], {"nv": 0, "vcall": -12})
        self.assertEqual(r["vptrs"], [0, 8, 36])
        left = _rec(self.db, "VLeft")
        self.assertEqual(left["bases"], [{"name": "VBase", "offset": 8, "virtual": True, "access": "public"}])

    def test_template_instances(self):
        fn = _fn(self.db, "FFastVector::push_back@Civ5Expansion2/FirePlace/FireWorks/FFastVector.h:10")
        self.assertIsNone(fn["mangled"])
        self.assertIsNone(fn["markers"]["addr"])
        inst = fn["instances"][0]
        self.assertEqual((inst["spec"], inst["addr"], inst["mangled"]),
                         ("FFastVector<int>", "0x08100000", "_ZN11FFastVectorIiE9push_backERKi"))
        self.assertEqual(inst["params"], [{"name": "v", "type": "const int &"}])
        pattern = _rec(self.db, "FFastVector")
        self.assertEqual((pattern["size_marker"], pattern["instance_markers"]), ("0x8", []))
        self.assertEqual(_rec(self.db, "FFastVector<int>")["size_marker"], "0x8")
        self.assertEqual(_rec(self.db, "FFastVector<FireWorks::cvUnitVis*>")["template"],
                         {"pattern": "FFastVector", "args": ["FireWorks::cvUnitVis*"]})
        maker = _rec(self.db, "FTMaker")
        self.assertIsNone(maker["size_marker"])
        self.assertEqual(maker["instance_markers"], [
            {"spec": "FTMaker<int>", "kind": "size", "value": "0x8"},
            {"spec": "FTMaker<int>", "kind": "vtable", "value": "0x0a000100"}])
        mi = _rec(self.db, "FTMaker<int>")
        self.assertEqual((mi["size_marker"], mi["vtable_marker"], mi["vtable_symbol"]),
                         ("0x8", "0x0a000100", "_ZTV7FTMakerIiE"))
        dtor = next(f for f in self.db["functions"] if f["key"].startswith("FTMaker::~FTMaker"))
        self.assertEqual(dtor["instances"][0]["variants"],
                         {"D1": "_ZN7FTMakerIiED1Ev", "D2": "_ZN7FTMakerIiED2Ev", "D0": "_ZN7FTMakerIiED0Ev"})
        tw = next(f for f in self.db["functions"] if f["name"] == "Twice")
        self.assertEqual(tw["instances"][0]["mangled"], "_Z5TwiceIiET_S0_")

    def test_structor_variants(self):
        self.assertEqual(_fn(self.db, "_ZN5C1FooC1Ev")["variants"], {"C1": "_ZN5C1FooC1Ev", "C2": "_ZN5C1FooC2Ev"})
        d = _fn(self.db, "_ZN5C1FooD1Ev")
        self.assertEqual(d["variants"], {"D1": "_ZN5C1FooD1Ev", "D2": "_ZN5C1FooD2Ev", "D0": None})
        self.assertEqual((d["markers"]["addr"], d["markers"]["base"]), (None, "0x08130080"))
        self.assertEqual(_fn(self.db, "_ZN5VDtorD1Ev")["variants"]["D0"], "_ZN5VDtorD0Ev")

    def test_abi_facts(self):
        self.assertTrue(_fn(self.db, "_ZNK9FireWorks15cvUnitVisSystem6GetPosEv")["return_indirect"])
        self.assertTrue(_fn(self.db, "_Z7MakeVecff")["return_indirect"])
        self.assertFalse(_fn(self.db, "_ZN9FireWorks15cvUnitVisSystem5CountEii")["return_indirect"])
        self.assertTrue(_rec(self.db, "PodPair")["trivial_for_calls"])
        self.assertFalse(_rec(self.db, "CopyPair")["trivial_for_calls"])
        self.assertEqual(_fn(self.db, "_ZL12StaticHelperi")["linkage"], "internal")
        self.assertEqual(_fn(self.db, "_ZN12_GLOBAL__N_110AnonHelperEi")["linkage"], "internal")
        self.assertEqual(_fn(self.db, "_Z7MakeVecff")["linkage"], "external")
        k = next(g for g in self.db["globals"] if g["qualified"] == "kConst")
        self.assertEqual((k["linkage"], k["inline_marker"], k["mangled"]), ("internal", True, "_ZL6kConst"))
        m = _fn(self.db, "_ZN9FireWorks15cvUnitVisSystem5CountEii")
        self.assertTrue(m["markers"]["fastcc"])
        self.assertTrue(m["static"])
        self.assertIsNone(m["this"])
        self.assertEqual(_fn(self.db, "_ZNK9FireWorks15cvUnitVisSystem6GetPosEv")["this"],
                         "const FireWorks::cvUnitVisSystem *")

    def test_unknown_placeholders(self):
        g = next(g for g in self.db["globals"] if g["qualified"] == "g_0a100000")
        self.assertEqual((g["type"], g["size"]), ("UNK_DATA[16]", 16))
        t = self.db["types"]["UNK_DATA[16]"]
        self.assertEqual((t["kind"], t["of"], t["count"]), ("array", "UNK_DATA", 16))
        self.assertEqual(self.db["types"]["UNK_DATA"], {"kind": "unknown", "name": "UNK_DATA", "size": 1})
        self.assertEqual(self.db["types"]["UNK_RET"], {"kind": "unknown", "name": "UNK_RET", "size": None})
        f = _fn(self.db, "_Z12Fun_08140080z")
        self.assertEqual((f["params_unknown"], f["variadic"], f["params"], f["return"]), (True, False, [], "UNK_RET"))
        p = _fn(self.db, "_Z6PrintfPKcz")
        self.assertEqual((p["params_unknown"], p["variadic"]), (False, True))
        self.assertTrue(_fn(self.db, "FSBAAllocate")["params_unknown"])
        ph = self.db["stats"]["placeholders"]
        self.assertEqual((ph["unk_data"], ph["unk_params"], ph["unk_ret"], ph["lua ?"]), (1, 2, 3, 1))
        self.assertEqual((ph["fun"], ph["unk"], ph["global"]), (2, 1, 1))

    def test_fields_bitfields_union_macro(self):
        r = _rec(self.db, "FireWorks::cvUnitVisSystem")
        f = {x["name"]: x for x in r["fields"]}
        self.assertEqual((f["m_bFlagB"]["bit_offset"], f["m_bFlagB"]["bit_size"], f["m_bFlagB"]["offset"]), (177, 3, 0x16))
        self.assertEqual(f["m_sKind"]["offset_marker"], "0x14")
        self.assertEqual(f["unk_28"]["type"], "char[20]")
        u = _rec(self.db, "FireWorks::cvUnitVisSystem::(anon union L67)")
        self.assertEqual(u["accessed_from"], {"record": "FireWorks::cvUnitVisSystem", "offset": 0x18})
        self.assertEqual([(x["name"], x["offset"], x["offset_marker"]) for x in u["fields"]],
                         [("m_iRaw", 0, "0x18"), ("m_fRaw", 0, "0x18")])
        self.assertEqual(_fn(self.db, "_ZNK9FireWorks15cvUnitVisSystem8GetFlagsEv")["markers"]["addr"], "0x0834a280")
        vec = _rec(self.db, "std::__1::vector<int>")
        self.assertEqual((vec["origin"], vec["size"], vec["file"]), ("third_party", 12, "third_party/libcxx/v1/vector"))

    def test_lua_comment_enum_typedef(self):
        r = _rec(self.db, "FireWorks::cvUnitVisSystem")
        self.assertEqual(r["markers"]["lua"][0]["text"], "class UnitVisSystem xml")
        self.assertEqual(r["comment"]["text"], "Gamecore events reach it through EventSystem, not direct calls.")
        e = self.db["enums"][0]
        self.assertEqual(e["values"], [{"name": "US_IDLE", "value": 0}, {"name": "US_MOVING", "value": 4},
                                       {"name": "US_DEAD", "value": 5}])
        self.assertEqual(e["size"], 4)
        t = self.db["typedefs"][0]
        self.assertEqual((t["qualified"], t["type"]), ("UnitCallback", "int (*)(int, void *)"))
        self.assertEqual(self.db["types"]["int (*)(int, void *)"]["kind"], "pointer")

    def test_cpp_defs(self):
        defs = {(d["line"], d["qualified"]): d for d in self.db["cpp_defs"]}
        ctor = defs[(10, "FireWorks::cvUnitVisSystem::cvUnitVisSystem")]
        self.assertEqual((ctor["function"], ctor["markers"]["addr"]), ("_ZN9FireWorks15cvUnitVisSystemC1Ev", "0x0834a000"))
        pb = defs[(18, "FFastVector::push_back")]
        self.assertEqual(pb["instances"], [{"spec": "FFastVector<int>", "addr": "0x08100000", "base": None,
                                            "fastcc": False}])
        self.assertTrue(_fn(self.db, "_ZN9FireWorks15cvUnitVisSystem7OnEventEi")["has_body"])

    def test_section_11_experiments(self):
        # gnu++11 defines linux/unix/i386: otherwise Abi.h stops with #error and the fixture is not clean.
        self.assertEqual([(r["size"], r["align"], r["dsize"]) for r in (_rec(self.db, "TailA"), _rec(self.db, "TailB"))],
                         [(12, 4, 9), (12, 4, 10)])
        self.assertEqual({f["name"]: f["offset"] for f in _rec(self.db, "TailB")["fields"]}, {"d": 9})
        sealed = _rec(self.db, "Sealed")  # private destructor: the probe works under -fno-access-control
        self.assertEqual([s["kind"] for s in sealed["vtable"]["groups"][0]["slots"]], ["D1", "D0"])
        load = _fn(self.db, "_ZN3Gfx4LoadEfPNS_3TexE")
        self.assertEqual([p["type"] for p in load["params"]], ["float", "Gfx::Tex *"])
        self.assertEqual(_fn(self.db, "_ZN3Gfx4LoadEi")["params"], [{"name": "a1", "type": "int"}])

    def test_redecl_in_inl(self):
        f = _fn(self.db, "_ZNK9FireWorks15cvUnitVisSystem8GetTwiceEv")
        self.assertTrue(f["markers"]["inline"])
        self.assertEqual([(r["file"], r["line"], r["markers"]["inline"]) for r in f["redecls"]],
                         [("Civ5Expansion2/Civ5/Src/App/Graphics/cvUnitVisSystem.inl", 6, True)])
        self.assertTrue(f["has_body"])
        inl = next(x for x in self.db["files"] if x["kind"] == "inl")
        self.assertEqual(inl["origin"], "guess")

    def test_index(self):
        ix = api.index(self.db)
        self.assertEqual(ix.mangled("_ZN9FireWorks15cvUnitVisSystemC2Ev")["key"], "_ZN9FireWorks15cvUnitVisSystemC1Ev")
        self.assertEqual(ix.mangled("_ZN11FFastVectorIiE9push_backERKi")["name"], "push_back")
        self.assertTrue(ix.at("Civ5Expansion2/FirePlace/FireWorks/VDiamond.h", 26))
        self.assertEqual(api.load(self.path, require_ok=True)["schema"], "civ5re.typedb/1")


@needs_llvm
class Determinism(unittest.TestCase):
    def test_two_runs_identical_and_cli_equal(self):
        a = build_db(fixture_tree("main"), tmp_root() / "det-a", third_party=THIRD_PARTY)
        b = build_db(fixture_tree("main"), tmp_root() / "det-b", third_party=THIRD_PARTY, force=True)
        from civ5re.__main__ import main
        out = io.StringIO()
        with redirect_stdout(out), redirect_stderr(io.StringIO()):
            rc = main(["typedb", "--tree", str(fixture_tree("main").parent), "--build", str(tmp_root() / "det-c"),
                       "--third-party", str(THIRD_PARTY)])
        self.assertEqual(rc, 0)
        c = tmp_root() / "det-c" / "typedb.json"
        self.assertEqual(a.read_bytes(), b.read_bytes())
        self.assertEqual(a.read_bytes(), c.read_bytes())
        self.assertNotIn(b"\r\n", a.read_bytes())
        self.assertNotIn(str(tmp_root()).replace("\\", "/").encode(), a.read_bytes())

    def test_cache_does_not_invoke_clang(self):
        build = tmp_root() / "cache"
        first = build_db(fixture_tree("clean"), build, third_party=THIRD_PARTY)
        data = first.read_bytes()
        with mock.patch("typedb.compile.run", side_effect=AssertionError("clang invoked")), \
                mock.patch("subprocess.run", side_effect=AssertionError("process started")):
            again = build_db(fixture_tree("clean"), build, third_party=THIRD_PARTY)
        self.assertEqual(again.read_bytes(), data)


@needs_llvm
class Diagnostics(unittest.TestCase):
    CODES = ("marker-syntax", "near-marker", "marker-orphan", "marker-ambiguous", "marker-misplaced",
             "marker-duplicate", "marker-missing", "marker-conflict", "marker-unsupported", "marker-macro",
             "cpp-undeclared", "file-unreached", "probe-failed", "clang-error", "internal")

    def test_each_code_has_its_fixture(self):
        from typedb.attach import CODES
        self.assertEqual(set(CODES), set(self.CODES))
        for code in self.CODES:
            with self.subTest(code=code):
                _p, db = built(code)
                codes = {d["code"] for d in db["diagnostics"]}
                self.assertIn(code, codes)
                self.assertFalse(db["ok"])
                self.assertTrue(all(d["file"].startswith("Civ5Expansion2/") for d in db["diagnostics"]))

    def test_clean_fixture(self):
        _p, db = built("clean")
        self.assertEqual(db["diagnostics"], [])
        self.assertTrue(db["ok"])

    def test_exit_codes(self):
        from civ5re.__main__ import main
        with redirect_stdout(io.StringIO()), redirect_stderr(io.StringIO()):
            rc1 = main(["typedb", "--tree", str(fixture_tree("marker-missing").parent), "--build",
                        str(tmp_root() / "exit1"), "--third-party", str(THIRD_PARTY)])
            broken = tmp_root() / "broken_tp"
            shutil.copytree(THIRD_PARTY, broken, dirs_exist_ok=True)
            (broken / "include_dirs.txt").write_text("compat\nnope\n<clang>\n", encoding="utf-8")
            rc2 = main(["typedb", "--tree", str(fixture_tree("clean").parent), "--build",
                        str(tmp_root() / "exit2"), "--third-party", str(broken)])
        self.assertEqual((rc1, rc2), (1, 2))

    def test_clang_error_skips_pass_b_but_keeps_declarations(self):
        _p, db = built("clang-error")
        self.assertEqual(db["types"], {})
        fn = db["functions"][0]
        self.assertEqual(fn["markers"]["addr"], "0x08000000")


@needs_llvm
class MarkersOff(unittest.TestCase):
    def test_doxygen_off_has_no_marker_codes_and_same_layout(self):
        _p, off = built("doxygen", "off")
        _p, strict = built("doxygen", "strict")
        self.assertEqual(off["markers"], "off")
        self.assertEqual([d for d in off["diagnostics"]
                          if d["code"].startswith("marker-") or d["code"] == "near-marker"], [])
        self.assertTrue(off["ok"])
        self.assertTrue(any(d["code"] == "marker-syntax" for d in strict["diagnostics"]))
        self.assertEqual([(r["name"], r["size"], r["fields"]) for r in off["records"]],
                         [(r["name"], r["size"], [dict(f, offset_marker=None) for f in r["fields"]])
                          for r in strict["records"]])
        self.assertEqual(off["files"][0]["origin"], None)
        self.assertEqual(off["records"][0]["size"], 8)

    def test_main_off_layouts_equal_strict(self):
        _p, off = built("main", "off")
        _p, strict = built("main")
        self.assertEqual(off["diagnostics"], [])
        # Instantiations named only by SPEC markers are not probed when markers are not read.
        lay = lambda db: {r["name"]: (r["size"], r["dsize"], r["vtable"]) for r in db["records"]}  # noqa: E731
        lo, ls = lay(off), lay(strict)
        self.assertEqual(set(ls) - set(lo), {"FFastVector<int>", "FTMaker<int>"})
        self.assertEqual(lo, {k: ls[k] for k in lo})
        self.assertTrue(all(f["markers"]["addr"] is None for f in off["functions"]))


if __name__ == "__main__":
    unittest.main()
