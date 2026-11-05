"""Rules A1-A11, V1-V3, L1-L3: unit tests next to the fixtures (selftest runs the fixtures themselves)."""
import json
import re
import unittest
from collections import Counter, defaultdict
from pathlib import Path

from checks import plugins, report, selftest
from checks.api import InputError
from checks.context import Settings, parse_ghidra
from checks.rules import a_symbols as A
from checks.rules import l_layout, v_vtables

IDS = "A1 A2 A3 A4 A5 A6 A7 A8 A9 A10 A11 V1 V2 V3 L1 L2 L3".split()
REAL = Path(__file__).resolve().parents[3] / "build" / "binfacts.json"


def world():
    return json.loads((selftest.WORLD / "binfacts.json").read_text(encoding="utf-8"))


class Ctx:
    """The part of Context that the rules read."""

    def __init__(self, db, bf=None, ghidra=None, export_coverage="report"):
        self.typedb, self.binfacts, self.ghidra, self.export_coverage = db, bf, ghidra, export_coverage


def fn(name, mangled, addr=None, base=None, deleting=None, thunks=(), variants=None, kind="function", path="a.h", line=1,
       **markers):
    mk = {"addr": addr, "base": base, "deleting": deleting, "thunks": list(thunks), "inline": False, "pure": False,
          "absent": False, "fastcc": False, "lua": []}
    mk.update(markers)
    return {"key": mangled, "qualified": name, "name": name.split("::")[-1], "scope_kind": "namespace", "kind": kind,
            "file": path, "line": line, "mangled": mangled, "variants": variants, "linkage": "external",
            "markers": mk, "instances": []}


def wildcard(sym):
    i = sym["mangled"].rindex(sym["variant"])
    return sym["mangled"][:i] + sym["variant"][0] + "*" + sym["mangled"][i + 2:]


def declare_all(bf):
    """A typedb dict that declares every symbol of the coverage denominator with its binary address."""
    funcs, globals_, records, ents = [], [], [], defaultdict(dict)
    thunks = []
    for s in bf["symbols"]:
        if s["group"] != "firaxis":
            continue
        k, a = s["kind"], s["addr"]
        if k in ("function", "method", "c"):
            funcs.append(fn(s["demangled"], s["mangled"], addr=a))
        elif k in ("ctor", "dtor"):
            ents[wildcard(s)][s["variant"]] = (s, a)
        elif k == "thunk":
            thunks.append(a)
        elif k == "vtable":
            records.append({"name": s["scope"], "origin": "tree", "vtable_marker": a, "instance_markers": []})
        elif k == "data":
            globals_.append({"mangled": s["mangled"], "qualified": s["mangled"], "addr_marker": a, "file": "a.h", "line": 1})
    for key, vs in ents.items():
        first, second = ("C1", "C2") if "C*" in key else ("D1", "D2")
        names = {v: key.replace(first[0] + "*", v) for v in ((first, second) if first == "C1" else (first, second, "D0"))}
        a1, a2 = (vs.get(first) or (None, None))[1], (vs.get(second) or (None, None))[1]
        d0 = (vs.get("D0") or (None, None))[1]
        funcs.append(fn(key, names[first], addr=a1, base=a2 if a2 != a1 else None, deleting=d0, variants=names,
                        kind="ctor" if first == "C1" else "dtor"))
    if thunks:
        funcs.append(fn("Thunked", "_ZThunked", addr="0x08000000", thunks=thunks))
    return {"records": records, "functions": funcs, "globals": globals_}


class Coverage(unittest.TestCase):
    def setUp(self):
        self.bf = world()
        self.db = declare_all(self.bf)

    def test_a10_denominator_is_binfacts_stats_and_report_agrees(self):
        den = self.bf["stats"]["denominator"]
        per, missing = A.coverage(self.db, self.bf)
        self.assertEqual({k: t for k, (_c, t) in per.items()}, den["by_kind"])
        self.assertEqual(sum(t for _c, t in per.values()), den["total"])
        self.assertEqual(missing, [])
        rep = report.coverage(self.db, self.bf)
        self.assertEqual(rep["denominator_total"], den["total"])
        self.assertEqual(rep["total"], den["total"])
        self.assertEqual({k: v["total"] for k, v in rep["by_kind"].items()}, den["by_kind"])
        self.assertEqual(report.compute(self.db, self.bf)["export_coverage"]["denominator_total"], den["total"])

    def test_a10_clean_tree_has_no_finding(self):
        self.assertEqual(A.A10().run(Ctx(self.db, self.bf)), [])

    def test_a10_one_missing_declaration_report_is_warn_required_is_error(self):
        self.db["globals"].clear()
        total = self.bf["stats"]["denominator"]["total"]
        (f,) = A.A10().run(Ctx(self.db, self.bf, export_coverage="report"))
        self.assertEqual((f.severity, f.message), ("warn", f"not declared 1 of {total}"))
        (f,) = A.A10().run(Ctx(self.db, self.bf, export_coverage="required"))
        self.assertEqual((f.severity, f.message), ("error", f"not declared 1 of {total}"))
        self.assertIn("data g_cvCounter 0x0b000000", f.detail)

    def test_a10_wrong_address_does_not_count(self):
        for f in self.db["functions"]:
            if f["mangled"] == "_Z11cvFreeUtilsi":
                f["markers"]["addr"] = "0x0834a298"
        (f,) = A.A10().run(Ctx(self.db, self.bf))
        self.assertIn("function _Z11cvFreeUtilsi 0x0834a290", f.detail)

    def test_a10_c1_eq_c2_is_covered_by_one_marker(self):
        ctor = next(f for f in self.db["functions"] if f["mangled"] == "_ZN6cvBaseC1Ev")
        self.assertIsNone(ctor["markers"]["base"])
        _per, missing = A.coverage(self.db, self.bf)
        self.assertNotIn("_ZN6cvBaseC2Ev", [m[1] for m in missing])

    def test_a10_refuses_stats_that_disagree_with_the_symbols(self):
        self.bf["stats"]["denominator"]["total"] += 1
        with self.assertRaises(InputError):
            A.A10().run(Ctx(self.db, self.bf))

    def test_a10_ignores_other_groups(self):
        self.assertEqual(sum(1 for s in self.bf["symbols"] if s["group"] != "firaxis"), 3)
        per, _ = A.coverage({"records": [], "functions": [], "globals": []}, self.bf)
        self.assertEqual(sum(t for _c, t in per.values()), self.bf["stats"]["denominator"]["total"])


def entity_categories(bf, group="firaxis"):
    ents = defaultdict(dict)
    for s in bf["symbols"]:
        if s["group"] == group and s["variant"]:
            ents[wildcard(s)][s["variant"]] = s["addr"]
    out = Counter()
    for vs in ents.values():
        for a, b, n in (("C1", "C2", "c"), ("D1", "D2", "d")):
            if a in vs and b in vs:
                out[f"{n}1_{'eq' if vs[a] == vs[b] else 'ne'}_{n}2"] += 1
            elif a in vs:
                out[f"{n}1_only"] += 1
            elif b in vs:
                out[f"{n}2_only"] += 1
        out["has_d0"] += "D0" in vs
    return out


def alias_counts(bf):
    b = A.Bin(bf)
    multi = [n for n in ({a: [s.name for s in b.funcs_at(a)] for a in b.at}).values() if len(n) > 1]
    return len(multi), sum(len(n) for n in multi)


class BinfactsStats(unittest.TestCase):
    def check(self, bf):
        want = bf["stats"]["variants"]["firaxis"]["entities"]
        got = entity_categories(bf)
        self.assertEqual({k: got.get(k, 0) for k in want}, want)
        self.assertEqual(alias_counts(bf), (bf["stats"]["aliases"]["addresses"], bf["stats"]["aliases"]["names"]))

    def test_world_variants_and_aliases(self):
        bf = world()
        self.check(bf)
        for key in ("c1_eq_c2", "c1_only", "c2_only", "d1_eq_d2", "d1_ne_d2", "d1_only", "d2_only", "has_d0"):
            self.assertGreater(bf["stats"]["variants"]["firaxis"]["entities"][key], 0, key)
        merged = [s["mangled"] for s in bf["symbols"] if s["addr"] == "0x0834a270"]
        self.assertEqual(sorted(merged), sorted(["_ZN7FSocketD1Ev", "_ZN10FUDPSocketD1Ev"]))

    @unittest.skipUnless(REAL.is_file(), "build/binfacts.json is not present")
    def test_real_variants_and_aliases(self):
        self.check(json.loads(REAL.read_text(encoding="utf-8")))


class Thunks(unittest.TestCase):
    def test_target(self):
        t = A.thunk_target
        self.assertEqual(t("_ZThn4_N15cvUnitVisSystem5FlushEv"), "_ZN15cvUnitVisSystem5FlushEv")
        self.assertEqual(t("_ZTv0_n12_N8VDiamond1fEv"), "_ZN8VDiamond1fEv")
        self.assertEqual(t("_ZThn8_N6cvLeafD0Ev"), "_ZN6cvLeafD0Ev")
        self.assertEqual(t("_ZTch0_h4_N1D1fEv"), "_ZN1D1fEv")
        self.assertEqual(t("_ZTcv0_n12_v0_n16_N1D1fEv"), "_ZN1D1fEv")
        self.assertIsNone(t("_ZN6cvBase4InitEv"))

    def test_world_thunks_resolve_to_world_symbols(self):
        b = A.Bin(world())
        names = {s.name for s in b.syms}
        self.assertTrue(b.thunk_syms)
        for target in b.thunk_syms:
            self.assertIn(target, names)


class FastccAndLists(unittest.TestCase):
    LIST = ("# civ5re ghidra-functions 2\n# binary_sha256=0 ghidra=12 cache_key=x export=1 functions=2\n"
            "0x08350100\t0x08350140\tFUN_08350100\t\n0x08350200\t0x08350220\tFUN_08350200\tr=ECX,EDX\n")

    def run_a7(self, addr, fastcc, ghidra):
        db = {"functions": [fn("Fun_%08x" % addr, "_Z12Fun_%08xi" % addr, addr="0x%08x" % addr, fastcc=fastcc)]}
        return A.A7().run(Ctx(db, world(), ghidra=ghidra))

    def test_without_a_list_fastcc_and_unknown_starts_warn(self):
        (f,) = self.run_a7(0x08350100, False, None)
        self.assertEqual(f.severity, "warn")
        self.assertIn("civ5re ghidra functions", f.message)
        (f,) = self.run_a7(0x08350000, True, None)
        self.assertEqual(f.severity, "warn")
        self.assertEqual(self.run_a7(0x08350000, False, None), [])

    def test_with_a_list_the_same_cases_are_errors_or_clean(self):
        g = parse_ghidra(self.LIST)
        self.assertEqual(self.run_a7(0x08350200, True, g), [])
        self.assertEqual(self.run_a7(0x08350100, False, g), [])
        (f,) = self.run_a7(0x08350100, True, g)
        self.assertEqual(f.severity, "error")
        (f,) = self.run_a7(0x08350200, False, g)
        self.assertEqual(f.severity, "error")
        (f,) = self.run_a7(0x08350300, False, g)
        self.assertEqual(f.severity, "error")

    def test_fastcc_on_an_exported_address_is_an_error_with_or_without_a_list(self):
        for g in (None, parse_ghidra(self.LIST)):
            (f,) = self.run_a7(0x0834a290, True, g)
            self.assertEqual(f.severity, "error")


class LayoutHelpers(unittest.TestCase):
    def rec(self, name, size, fields, accessed_from=None, marker="0x10"):
        return {"name": name, "origin": "tree", "file": "a.h", "line": 1, "size": size, "size_marker": marker,
                "fields": fields, "accessed_from": accessed_from, "template": None}

    def test_nested_anonymous_unions_add_every_level(self):
        inner = self.rec("O::(anon union L5)", 4, [dict(name="x", offset=0, offset_marker="0x14", bit_offset=None,
                                                         bit_size=None, line=7)],
                         accessed_from={"record": "O::(anon struct L4)", "offset": 4})
        mid = self.rec("O::(anon struct L4)", 8, [], accessed_from={"record": "O", "offset": 0x10})
        outer = self.rec("O", 0x20, [])
        db = {"records": [outer, mid, inner]}
        self.assertEqual(l_layout.L2().run(Ctx(db)), [])
        inner["fields"][0]["offset_marker"] = "0x10"
        (f,) = l_layout.L2().run(Ctx(db))
        self.assertIn("clang +0x14", f.message)

    def test_bit_field_uses_the_byte_of_its_first_bit(self):
        r = self.rec("B", 8, [dict(name="f", offset=4, offset_marker="0x6", bit_offset=51, bit_size=3, line=3)])
        self.assertEqual(l_layout.L2().run(Ctx({"records": [r]})), [])
        r["fields"][0]["offset_marker"] = "0x4"
        self.assertEqual(len(l_layout.L2().run(Ctx({"records": [r]}))), 1)

    def test_unknown_size_is_not_an_error_and_patterns_are_skipped(self):
        r = self.rec("U", 8, [], marker="?")
        pat = self.rec("P", None, [], marker="0x99")
        self.assertEqual(l_layout.L1().run(Ctx({"records": [r, pat]})), [])
        r["size_marker"] = "0x4"
        self.assertEqual(len(l_layout.L1().run(Ctx({"records": [r, pat]}))), 1)

    def test_l3_warns_when_the_declared_size_is_below_the_binary_evidence(self):
        bf = {"sizes": {"C": {"min": 0x4c, "from": ["0x08b94170"]}},
              "symbols": [{"addr": "0x08b94170", "kind": "ctor"}]}
        r = self.rec("C", 0x3c, [], marker="0x3c")
        (f,) = l_layout.L3().run(Ctx({"records": [r]}, bf))
        self.assertEqual(f.severity, "warn")
        self.assertIn("size 0x3c, binary uses at least 0x4c (ctor 0x08b94170)", f.message)
        r["size_marker"] = "0x4c"
        self.assertEqual(l_layout.L3().run(Ctx({"records": [r]}, bf)), [])
        r["size_marker"] = "?"
        self.assertEqual(l_layout.L3().run(Ctx({"records": [r]}, bf)), [])    # `size ?` declares nothing
        self.assertEqual(l_layout.L3().run(Ctx({"records": [r]}, {"symbols": []})), [])

    def test_rtti_kind(self):
        b = lambda **kw: dict(dict(name="X", offset=0, virtual=False, access="public"), **kw)  # noqa: E731
        self.assertEqual(v_vtables._rtti_kind([]), "class")
        self.assertEqual(v_vtables._rtti_kind([b()]), "si")
        self.assertEqual(v_vtables._rtti_kind([b(offset=4)]), "vmi")
        self.assertEqual(v_vtables._rtti_kind([b(virtual=True)]), "vmi")
        self.assertEqual(v_vtables._rtti_kind([b(access="private")]), "vmi")
        self.assertEqual(v_vtables._rtti_kind([b(), b(offset=4)]), "vmi")


class AliasAndAddressRules(unittest.TestCase):
    def test_a8_allows_only_names_binfacts_puts_on_one_address(self):
        bf = world()
        a = fn("FSocket::~FSocket", "_ZN7FSocketD1Ev", addr="0x0834a270", path="a.h", line=1)
        b = fn("FUDPSocket::~FUDPSocket", "_ZN10FUDPSocketD1Ev", addr="0x0834a270", path="b.h", line=2)
        self.assertEqual(A.A8().run(Ctx({"functions": [a, b], "globals": []}, bf)), [])
        c = fn("Other", "_Z5Otheri", addr="0x0834a270", path="c.h", line=3)
        (f,) = A.A8().run(Ctx({"functions": [a, b, c], "globals": []}, bf))
        self.assertEqual((f.path, f.line), ("c.h", 3))

    def test_a8_same_function_twice_is_one_entity(self):
        f = fn("C::C", "_ZN1CC1Ev", addr="0x08350000", base="0x08350000", variants={"C1": "_ZN1CC1Ev", "C2": "_ZN1CC2Ev"})
        self.assertEqual(A.A8().run(Ctx({"functions": [f], "globals": []}, world())), [])

    def test_a9_global_sizes_and_spans(self):
        bf = world()
        g = {"qualified": "g_cvCounter", "mangled": "g_cvCounter", "addr_marker": "0x0b000000", "size": 8,
             "file": "g.h", "line": 4}
        (f,) = A.A9().run(Ctx({"globals": [g]}, bf))
        self.assertIn("sizeof 8, symbol has 4", f.message)
        g["size"] = 4
        self.assertEqual(A.A9().run(Ctx({"globals": [g]}, bf)), [])


class RuleCatalog(unittest.TestCase):
    def test_all_rules_are_registered_with_fixtures(self):
        loaded = plugins.load_builtin(Path(selftest.CHECKS_DIR).parents[1])
        ids = {r.id for r in loaded.rules}
        self.assertLessEqual(set(IDS), ids)
        for rid in IDS:
            rule = next(r for r in loaded.rules if r.id == rid)
            cases = [c.name for c in selftest.discover_cases(plugins.fixture_dir(rule, None, selftest.CHECKS_DIR))]
            self.assertTrue(any(c.startswith("ok-") for c in cases), rid)
            self.assertTrue(any(c.startswith("bad-") for c in cases), rid)
            self.assertEqual(rule.deep, rid in ("A5", "V1", "V2", "V3", "L1", "L2", "L3"), rid)

    def test_a10_is_always_shown_and_a8_is_whole_tree(self):
        by = {r.id: r for r in plugins.load_builtin(Path(selftest.CHECKS_DIR).parents[1]).rules}
        self.assertEqual((by["A10"].staged_kind, by["A8"].staged_kind), ("always", "tree"))


class FixturesRun(unittest.TestCase):
    def test_selftest_of_these_rules(self):
        from civ5re import config
        from checks.tests.test_runner import only_loaded
        cfg = config.load()
        try:
            cfg.tool("clang++")
        except Exception as e:  # noqa: BLE001 - any config problem means no LLVM
            self.skipTest(f"LLVM is not available: {e}")
        if not (cfg.root / "third_party" / "pch.h").is_file():
            self.skipTest("third_party/ is not seeded yet")
        rules = [r for r in plugins.load_builtin(cfg.root).rules if r.id in IDS]
        res, errors = selftest.run(Settings(repo=cfg.root, cfg=cfg, selftest=False), only_loaded(rules), only=IDS)
        self.assertEqual((res["failed"], errors), ([], []))
        self.assertGreaterEqual(res["fixtures"], 40)


if __name__ == "__main__":
    unittest.main()
