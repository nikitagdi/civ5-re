import json
import unittest

from checks import report, selftest
from checks.context import parse_ghidra, parse_waivers
from checks.api import InputError


def world():
    return json.loads((selftest.WORLD / "binfacts.json").read_text(encoding="utf-8"))


DB = {
    "records": [
        {"name": "cvBase", "origin": "tree", "file": "Civ5Expansion2/Civ5/Src/App/Graphics/cvBase.h", "size_marker": "0x20",
         "vtable_marker": "0x0a1b2c00", "fields": [{"name": "m_iCount"}, {"name": "field_4"}, {"name": "unk_8"}],
         "typeinfo_symbol": "_ZTI6cvBase", "instance_markers": [], "markers": {"lua": []}},
        {"name": "Class_0a1b2c80", "origin": "tree", "file": "Civ5Expansion2/FirePlace/FireGrafix/x.h", "size_marker": "?",
         "vtable_marker": "?", "fields": [], "instance_markers": [{"spec": "A<int>", "kind": "size", "value": "?"}],
         "markers": {"lua": [{"text": "?"}]}},
        {"name": "std::vector<int>", "origin": "third_party", "file": "third_party/v", "size_marker": None, "fields": []},
    ],
    "functions": [
        {"name": "Update", "mangled": "_ZN6cvBase6UpdateEf", "file": "Civ5Expansion2/Civ5/Src/App/Graphics/cvBase.h",
         "line": 1, "return": "void", "params": [{"name": "dt"}], "params_unknown": False, "markers": {"addr": "0x0834a060"}},
        {"name": "Fun_0834a380", "mangled": "_ZN6cvBase4InitEv", "file": "Civ5Expansion2/Civ5/Src/App/Graphics/cvBase.h",
         "line": 2, "return": "UNK_RET", "params": [{"name": "a1"}], "params_unknown": True, "markers": {"addr": "0x0834a380"}},
        {"name": "~cvBase", "mangled": "_ZN6cvBaseD1Ev", "variants": {"D1": "_ZN6cvBaseD1Ev", "D2": "_ZN6cvBaseD2Ev",
                                                                       "D0": "_ZN6cvBaseD0Ev"},
         "file": "Civ5Expansion2/Civ5/Src/App/Graphics/cvBase.h", "line": 3, "return": "void", "params": [], "params_unknown": False,
         "markers": {"addr": "0x0834a030", "base": "0x0834a030", "deleting": "0x0834a040"}},
    ],
    "globals": [{"qualified": "g_0b000000", "mangled": "g_cvCounter", "type": "UNK_DATA [4]",
                 "file": "Civ5Expansion2/FirePlace/FireGrafix/x.h", "addr_marker": "0x0b000000"}],
}


class Report(unittest.TestCase):
    def test_folders(self):
        self.assertEqual(report.folder("Civ5Expansion2/Civ5/Src/App/Graphics/x.h"), "Civ5/Src/App/Graphics")
        self.assertEqual(report.folder("Civ5Expansion2/FirePlace/FireGrafix/a/b.h"), "FirePlace/FireGrafix")

    def test_placeholders(self):
        p = report.placeholders(DB)
        self.assertEqual(p["by_kind"], {"class": 1, "field": 1, "fun": 1, "global": 1, "lua ?": 1, "param": 1,
                                        "size ?": 2, "unk": 1, "unk_data": 1, "unk_params": 1, "unk_ret": 1,
                                        "vtable ?": 1})
        self.assertEqual(p["total"], sum(p["by_kind"].values()))
        self.assertEqual(sum(p["by_folder"].values()), p["total"])
        self.assertIn("FirePlace/FireGrafix", p["by_folder"])

    def test_coverage_uses_the_binfacts_denominator(self):
        c = report.coverage(DB, world())
        den = world()["stats"]["denominator"]
        self.assertEqual(c["denominator_total"], den["total"])
        self.assertEqual(c["total"], den["total"])
        self.assertEqual({k: v["total"] for k, v in c["by_kind"].items()}, den["by_kind"])
        self.assertEqual(c["by_kind"]["data"]["declared"], 1)
        self.assertEqual(c["by_kind"]["dtor"]["declared"], 3)       # D1, D2 via base, D0 via deleting
        self.assertEqual(c["by_kind"]["vtable"]["declared"], 1)
        self.assertEqual(c["covered_by_class"]["typeinfo"], {"covered": 1, "total": 3})

    def test_global_instances_count_as_declared(self):
        bf = {"stats": {"denominator": {"total": 2, "by_kind": {"data": 2}}},
              "symbols": [{"mangled": "_ZN1TIiE1xE", "addr": "0x09000000", "kind": "data", "group": "firaxis"},
                          {"mangled": "_ZN1TIjE1xE", "addr": "0x09000004", "kind": "data", "group": "firaxis"}]}
        db = {"records": [], "functions": [], "globals": [
            {"qualified": "T::x", "mangled": None, "instances": [{"spec": "T<int>", "addr": "0x09000000",
                                                                   "mangled": "_ZN1TIiE1xE"}]}]}
        c = report.coverage(db, bf)
        self.assertEqual(c["by_kind"]["data"]["declared"], 1)

    def test_zero_based_parameter_names_are_not_placeholders(self):
        fn = {"name": "Call", "file": "Civ5Expansion2/FirePlace/FireWorks/FLua/x.h", "line": 1, "return": "void",
              "params": [{"name": "a0"}, {"name": "a1"}], "markers": {}}
        self.assertNotIn("param", report.placeholders({"functions": [fn]})["by_kind"])
        fn["params"] = [{"name": "a1"}]
        self.assertEqual(report.placeholders({"functions": [fn]})["by_kind"]["param"], 1)

    def test_finished_functions_and_layouts(self):
        f = report.finished_functions(DB, world())
        self.assertEqual(f, {"done": 2, "total": 3})
        lay = report.layouts(DB)
        self.assertEqual(lay["classes_with_size"], {"done": 1, "total": 2})
        self.assertEqual(lay["fields_with_name"], {"done": 1, "total": 3})

    def test_compute_is_json_and_text(self):
        rep = report.compute(DB, world())
        json.dumps(rep)
        self.assertEqual(rep["schema"], "civ5re.report/1")
        self.assertIn("export coverage:", report.render_text(rep))


class Parsers(unittest.TestCase):
    def test_ghidra_list(self):
        g = parse_ghidra("# civ5re ghidra-functions 2\n# binary_sha256=AB ghidra=12 functions=2\n# start\tend\tname\tflags\n"
                         "0x08350100\t0x08350140\tFUN_a\t\n0x08350200\t0x08350220\tFUN_b\tr=ECX,EDX\n")
        self.assertEqual(g.sha256, "ab")
        self.assertTrue(g.fastcc(0x08350200))
        self.assertFalse(g.fastcc(0x08350100))
        self.assertEqual(g.containing(0x08350105)[2], "FUN_a")
        self.assertIsNone(g.containing(0x08350150))
        with self.assertRaises(InputError):
            parse_ghidra("# civ5re ghidra-functions 1\n")

    def test_waiver_lines(self):
        text = '[\n  {"rule": "A1", "key": "k", "reason": "r"},\n\n  {"rule": "B1",\n   "key": "k2"}\n]\n'
        w = parse_waivers(text)
        self.assertEqual([(x.rule, x.line, x.valid) for x in w], [("A1", 2, True), ("B1", 4, False)])
        for bad in ("{}", '[1]', '[{"rule": "A1"}]', "not json"):
            with self.assertRaises(InputError):
                parse_waivers(bad)


if __name__ == "__main__":
    unittest.main()
