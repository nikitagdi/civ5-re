import copy
import unittest

from ghidra import plan as pm, verify as vf
from ghidra.tests.helpers import build, report_for, state_from_plan

R = "/civ5re/"


class Verify(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.plan = build()
        cls.good = state_from_plan(cls.plan)

    def state(self):
        return copy.deepcopy(self.good)

    def kinds(self, st):
        return sorted({d["kind"] for d in vf.compare(self.plan, st)})

    def func(self, st, addr):
        return next(f for f in st["funcs"] if f["addr"] == addr)

    def type_(self, st, tid):
        return next(t for t in st["types"] if t["id"] == tid)

    def test_clean(self):
        self.assertEqual(vf.compare(self.plan, self.good), [])
        self.assertEqual(vf.verify(self.plan, self.good), [])

    def test_state_of_another_plan_is_refused(self):
        st = self.state()
        st["plan_sha256"] = "0" * 64
        with self.assertRaises(vf.VerifyError):
            vf.verify(self.plan, st)

    # --- functions
    def test_function_missing_and_extra(self):
        st = self.state()
        st["funcs"] = [f for f in st["funcs"] if f["addr"] != "0x08000240"]
        st["funcs"].append(dict(self.func(self.good, "0x08000240"), addr="0x09999999"))
        self.assertEqual(self.kinds(st), ["function.extra", "function.missing"])

    def test_function_not_owned_outside_plan_is_not_extra(self):
        st = self.state()
        st["funcs"].append(dict(self.func(self.good, "0x08000240"), addr="0x09999999", owned=False))
        self.assertEqual(self.kinds(st), [])

    def test_function_name_and_scope_and_scope_kind(self):
        st = self.state()
        self.func(st, "0x08000240")["name"] = "Upd"
        self.func(st, "0x08000250")["scope"] = ["Other"]
        self.func(st, "0x08000270")["scope_kinds"] = ["namespace"]
        self.assertEqual(self.kinds(st), ["function.name", "function.scope", "function.scope-kind"])

    def test_function_cc_and_storage(self):
        st = self.state()
        self.func(st, "0x08000240")["cc"] = "__cdecl"
        self.func(st, "0x08000280")["custom"] = False
        self.func(st, "0x08000500")["params"][0]["storage"] = {"reg": "EDX"}
        self.assertEqual(self.kinds(st), ["function.cc", "function.storage"])

    def test_function_return_params_varargs_source(self):
        st = self.state()
        self.func(st, "0x08000240")["ret"] = {"b": "uint"}
        self.func(st, "0x08000250")["params"][0]["type"] = {"b": "uint"}
        self.func(st, "0x08000270")["varargs"] = True
        self.func(st, "0x08000140")["sig_source"] = "USER_DEFINED"
        self.func(st, "0x08000130")["name_source"] = "DEFAULT"
        self.assertEqual(self.kinds(st), ["function.params", "function.return", "function.source",
                                          "function.varargs"])

    def test_auto_this_and_retptr(self):
        st = self.state()
        self.func(st, "0x08000240")["auto_this"] = {"p": {"b": "int"}}
        self.func(st, "0x08000260")["auto_retptr"] = False
        self.assertEqual(self.kinds(st), ["function.params"])
        self.assertEqual(len(vf.compare(self.plan, st)), 2)

    def test_partial_function_without_cc_leaves_cc_and_storage_to_param_id(self):
        pf = {"addr": "0x08000999", "scope": [], "name": "f", "cc": "", "custom": False, "ret": {"b": "undefined"},
              "partial": True, "this_type": None, "params": None, "auto_retptr": False, "varargs": False,
              "ret_storage": None}
        sf = {"exists": True, "name": "f", "scope": [], "scope_kinds": [], "cc": "__cdecl", "custom": True,
              "ret": {"b": "undefined"}, "name_source": "IMPORTED"}
        out = []
        vf._compare_function(pf, sf, {}, out)
        self.assertEqual(out, [])
        out = []
        vf._compare_function(dict(pf, partial=False, params=[]), dict(sf, params=[], sig_source="IMPORTED"), {}, out)
        self.assertEqual([d["kind"] for d in out], ["function.cc"])

    def test_partial_thiscall_stub_with_custom_storage_and_no_this_diverges(self):
        # What apply must repair: Param ID leaves custom storage and no typed `this` on a stub that ignores `this`.
        pf = {"addr": "0x08000998", "scope": ["C"], "name": "vf_1", "cc": "__thiscall", "custom": False,
              "ret": {"b": "undefined"}, "partial": True, "this_type": {"p": {"t": R + "C"}}, "params": None,
              "auto_retptr": False, "varargs": False, "ret_storage": None, "drop_this": True, "scope_kinds": ["class"]}
        sf = {"exists": True, "name": "vf_1", "scope": ["C"], "scope_kinds": ["class"], "cc": "__thiscall",
              "custom": True, "ret": {"b": "undefined"}, "name_source": "IMPORTED", "auto_this": None}
        out = []
        vf._compare_function(pf, sf, {("C",): "class"}, out)
        self.assertEqual(sorted(d["kind"] for d in out), ["function.cc", "function.params"])
        out = []
        vf._compare_function(pf, dict(sf, custom=False, auto_this={"p": {"t": R + "C"}}), {("C",): "class"}, out)
        self.assertEqual(out, [])

    def test_partial_signature_compares_only_name_scope_cc_return(self):
        st = self.state()
        f = self.func(st, "0x080002a0")
        f["params"] = [{"name": "param_2", "type": {"b": "int"}, "storage": None}]
        f["sig_source"] = "ANALYSIS"
        f["varargs"] = True
        self.assertEqual(self.kinds(st), [])
        f["ret"] = {"b": "int"}
        self.assertEqual(self.kinds(st), ["function.return"])

    # --- types
    def test_type_missing_extra_kind(self):
        st = self.state()
        st["types"] = [t for t in st["types"] if t["id"] != R + "Side"]
        st["types"].append({"id": R + "Stale", "kind": "struct", "size": 4, "desc": None, "fields": []})
        self.type_(st, R + "Mode")["kind"] = "typedef"
        self.assertEqual(self.kinds(st), ["type.extra", "type.kind", "type.missing"])

    def test_type_layout(self):
        st = self.state()
        self.type_(st, R + "Derived")["size"] = 24
        self.type_(st, R + "Opaque")["desc"] = None
        d = self.type_(st, R + "Vec2")
        d["fields"][1]["off"] = 6
        self.assertEqual(self.kinds(st), ["type.desc", "type.field-offset", "type.length"])

    def test_type_field_name_type_bit_missing_extra(self):
        st = self.state()
        v = self.type_(st, R + "Vec2")
        v["fields"][0]["name"] = "xx"
        p = self.type_(st, R + "Pod")
        p["fields"][0]["type"] = {"b": "uint"}
        bits = self.type_(st, R + "Bits")
        bits["fields"][1]["bit"] = {"size": 5, "shift": 4}
        del self.type_(st, R + "Derived")["fields"][2]
        self.type_(st, R + "Mix")["fields"].append({"off": 14, "name": "zz", "type": {"b": "char"},
                                                    "comment": None, "bit": None})
        kinds = self.kinds(st)
        for k in ("type.field-name", "type.field-type", "type.field-missing", "type.field-extra"):
            self.assertIn(k, kinds)
        self.assertTrue({"type.field-bit", "type.field-missing"} & set(kinds))

    def test_type_values(self):
        st = self.state()
        self.type_(st, R + "Mode")["values"][0][1] = 7
        self.type_(st, R + "BOOL")["to"] = {"b": "uint"}
        self.type_(st, R + "Derived_vfn/Update")["ret"] = {"b": "void"}
        divs = vf.compare(self.plan, st)
        self.assertEqual({d["kind"] for d in divs}, {"type.value"})
        self.assertEqual(len(divs), 3)

    # --- data and labels
    def test_data(self):
        st = self.state()
        st["data"] = [d for d in st["data"] if d["addr"] != "0x0a100000"]
        st["data"].append({"addr": "0x0b000000", "exists": True, "owned": True, "name": "x", "scope": [],
                           "type": {"b": "int"}, "size": 4})
        d = next(x for x in st["data"] if x["addr"] == "0x0a100200")
        d["name"] = "other"
        d = next(x for x in st["data"] if x["addr"] == "0x0a000208")
        d["type"] = {"b": "int"}
        d["size"] = 4
        self.assertEqual(self.kinds(st), ["data.extra", "data.length", "data.missing", "data.name", "data.type"])

    def test_labels(self):
        st = self.state()
        st["labels"] = []
        st["labels"].append({"addr": "0x0c000000", "exists": True, "owned": True, "name": "stale", "scope": []})
        self.assertEqual(self.kinds(st), ["label.extra", "label.missing"])

    def test_body_must_reach_the_symbol_end_unless_skipped(self):
        plan = build(funcs=self.cut_funcs())
        st = state_from_plan(plan)
        self.assertEqual(vf.compare(plan, st), [])
        st["bodies"][0]["end"] = "0x08000805"
        self.assertEqual([d["kind"] for d in vf.compare(plan, st)], ["function.body"])
        st["bodies"][0]["status"] = "skipped: the body ends at 0x08000805 but not after a call"
        self.assertEqual(vf.compare(plan, st), [])
        st["bodies"][0]["status"] = "bogus"
        self.assertEqual([d["kind"] for d in vf.compare(plan, st)], ["function.body"])
        del st["bodies"][0]
        self.assertEqual([d["kind"] for d in vf.compare(plan, st)], ["function.body"])

    @staticmethod
    def cut_funcs():
        from ghidra.tests.helpers import fixture
        funcs = fixture()[2]
        funcs[0x08000800] = (0x08000805, "FUN_08000800", "-")
        funcs[0x08000805] = (0x08000810, "FUN_08000805", "-")
        return funcs

    def test_all_kinds_covered_by_tests(self):
        covered = {"type.missing", "type.extra", "type.kind", "type.length", "type.desc", "type.field-missing",
                   "type.field-extra", "type.field-name", "type.field-offset", "type.field-type",
                   "type.field-bit", "type.value", "function.missing", "function.extra", "function.name",
                   "function.scope", "function.scope-kind", "function.cc", "function.storage",
                   "function.return", "function.params", "function.varargs", "function.source", "function.body", "data.missing",
                   "data.extra", "data.name", "data.type", "data.length", "label.missing", "label.extra"}
        self.assertEqual(covered, set(vf.KINDS))

    def test_summary_and_format(self):
        st = self.state()
        self.func(st, "0x08000240")["name"] = "Upd"
        divs = vf.compare(self.plan, st)
        self.assertEqual(vf.summary(divs), {"function.name": 1})
        self.assertIn("0x08000240", vf.format_divergences(divs)[0])

    def test_report_check(self):
        sha = self.plan["header"]["plan_sha256"]
        self.assertEqual(vf.check_report(report_for(self.plan, False, 3, 1), sha), (3, 1, None))
        self.assertEqual(vf.check_report(report_for(self.plan, True, aborted="stale"), sha)[2], "stale")
        with self.assertRaises(vf.VerifyError):
            vf.check_report(report_for(self.plan, False), "0" * 64)


if __name__ == "__main__":
    unittest.main()
