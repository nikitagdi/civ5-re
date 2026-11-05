import copy
import json
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from luaapi import emit_json, model  # noqa: E402
from luaapi.tests import synth  # noqa: E402

EXPECTED = synth.FIXTURES / "lua-api.expected.json"
INPUTS = {"typedb_sha256": "11" * 32, "binfacts_sha256": "ab" * 32}


def build(**kw):
    return model.build(synth.typedb(), synth.binfacts(), inputs=INPUTS, **kw)


class Reference(unittest.TestCase):
    def test_matches_reference(self):
        text = emit_json.dumps(build())
        if synth.updating():
            EXPECTED.parent.mkdir(parents=True, exist_ok=True)
            EXPECTED.write_bytes(text.encode("utf-8"))
        self.assertEqual(text, EXPECTED.read_bytes().decode("utf-8"))

    def test_repeated_run_is_byte_identical(self):
        self.assertEqual(emit_json.dumps(build()), emit_json.dumps(build()))

    def test_input_order_does_not_matter(self):
        db = synth.typedb()
        for k in ("functions", "records", "typedefs"):
            db[k] = list(reversed(db[k]))
        again = model.build(db, synth.binfacts(), inputs=INPUTS)
        self.assertEqual(emit_json.dumps(again), emit_json.dumps(build()))


class Content(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.doc = build()
        cls.fn = {}
        for f in cls.doc["functions"]:
            cls.fn.setdefault(f["path"], []).append(f)

    def test_ui_functions_and_doc(self):
        head = self.fn["UI.GetHeadSelectedUnit"][0]
        self.assertEqual(head["doc"], "Head of the active player's selected group.")
        self.assertEqual(head["bind"], "lib")
        self.assertEqual(head["call"], "dot")
        self.assertEqual(head["returns"][0]["type"], "Unit?")
        self.assertEqual(head["cxx"]["addr"], "0x089468e6")
        self.assertIsNone(self.fn["UI.Open"][0]["doc"])

    def test_overloads(self):
        self.assertEqual([f["overload"] for f in self.fn["UI.Open"]], [0, 1])
        self.assertEqual(len(self.fn["UI.Open"][1]["params"]), 2)

    def test_inherited_target_and_thunk(self):
        size = self.fn["AlphaAnim:SetSize"][0]
        self.assertEqual(size["cxx"]["qualified"], "ControlBase::SetSize")
        self.assertEqual(size["bind"], "flua")
        self.assertEqual(self.fn["ControlBase:SetSize"][0]["thunk"],
                         {"name": "LuaThunk_ControlBase_SetSize", "addr": "0x08693b00"})
        self.assertEqual(self.fn["ControlBase:SetHide"][0]["thunk"]["name"], "LuaThunk_ControlBase_SetHide")

    def test_template_members_are_event_bound(self):
        add = self.fn["Event:Add"][0]
        self.assertEqual(add["bind"], "event")
        self.assertTrue(add["cxx"]["template"])
        self.assertIsNone(add["cxx"]["mangled"])

    def test_events(self):
        ev = {e["name"]: e for e in self.doc["events"]}
        u = ev["SerialEventUnitCreated"]
        self.assertEqual((u["lib"], u["container"], u["id"], u["payload"]), ("Events", "serial", 3, "UnitCreatedData"))
        self.assertEqual([a["name"] for a in u["handler_args"]], ["playerID", "unitID"])
        self.assertEqual([a["name"] for a in u["call_args"]], ["playerID", "unitID"])
        self.assertTrue(u["binding"])
        s = ev["SerialEventStart"]
        self.assertIsNone(s["payload"])
        self.assertEqual((s["handler_args"], s["call_args"]), ([], []))
        self.assertEqual(s["doc"], "Fires once.")
        self.assertIsNone(u["doc"])

    def test_types_globals(self):
        t = {x["name"]: x for x in self.doc["types"]}
        self.assertEqual(t["Keys"]["values"][1], {"name": "VK_RETURN", "value": 13})
        self.assertEqual(t["Keys"]["doc"], "Virtual keys.")
        self.assertEqual((t["AlphaAnim"]["base"], t["AlphaAnim"]["xml"]), ("ControlBase", ["AlphaAnim", "Alpha"]))
        self.assertEqual(t["Vector2"]["kind"], "shape")
        self.assertEqual(t["UI"]["kind"], "table")
        self.assertEqual((t["Unit"]["kind"], t["Unit"]["origin"]), ("external", "gamecore"))
        g = self.doc["globals"][0]
        self.assertEqual((g["name"], g["type"], g["t"]["k"]), ("Controls", "table<string, ControlBase>", "map"))

    def test_coverage(self):
        c = self.doc["coverage"]
        self.assertFalse(c["partial"])
        self.assertEqual((c["unknown"], c["none"], c["undeclared"]), (1, 1, 3))
        self.assertEqual(c["events"], {"in_binary": 3, "declared": 2, "with_args": 2})
        self.assertEqual(c["payloads"], {"to_lua": 1, "push": 1, "from_lua": 1, "pull": 1})
        miss = {m["cxx"]: m for m in self.doc["missing"]}
        self.assertEqual(miss["cvLuaInterfaceBuddy::lOther"]["hint"], "UI.Other")
        self.assertEqual(miss["cvLuaInterfaceBuddy::lOther"]["status"], "undeclared")
        self.assertEqual(miss["cvLuaInterfaceBuddy::lLater"]["status"], "unknown")
        owners = {o["cxx_scope"]: o for o in c["by_owner"]}
        self.assertEqual(owners["cvLuaInterfaceBuddy"]["lua"], "UI")
        self.assertEqual(owners["cvLuaInterfaceBuddy"]["total"], 5)

    def test_diagnostics_are_rule_ids(self):
        self.assertEqual([d["rule"] for d in self.doc["diagnostics"]], [])

    def test_without_binfacts_is_partial(self):
        doc = model.build(synth.typedb())
        self.assertTrue(doc["coverage"]["partial"])
        self.assertEqual(doc["coverage"]["events"]["in_binary"], None)
        self.assertEqual(doc["inputs"]["binfacts_sha256"], None)
        self.assertEqual(len(doc["functions"]), len(self.doc["functions"]))
        self.assertGreater(doc["coverage"]["entry_points"], 5)

    def test_no_comment_means_null_doc_everywhere(self):
        db = synth.typedb()
        for coll in ("functions", "records", "enums", "typedefs"):
            for x in db[coll]:
                x["comment"] = None
        doc = model.build(db, synth.binfacts())
        self.assertTrue(all(f["doc"] is None for f in doc["functions"]))
        self.assertTrue(all(t["doc"] is None for t in doc["types"]))

    def test_usage_fills_game_uses(self):
        usage = {"members": {"UI": {"GetHeadSelectedUnit": {"dot": 5, "colon": 1, "colon_args": 0}}},
                 "controls": {"SetHide": 7}, "events": {"SerialEventStart": {"Add": 2, "call": 1}}}
        doc = build(usage=usage)
        fn = {f["path"]: f for f in doc["functions"]}
        self.assertEqual(fn["UI.GetHeadSelectedUnit"]["game_uses"], {"dot": 5, "method": 1})
        self.assertEqual(fn["ControlBase:SetHide"]["game_uses"], {"dot": 0, "method": 7})
        self.assertEqual({e["name"]: e["game_uses"] for e in doc["events"]}["SerialEventStart"],
                         {"add": 2, "call": 1, "call_immediate": 0})
        self.assertTrue(doc["inputs"]["game_scanned"])
        self.assertIsNone(self.doc["functions"][0]["game_uses"])


class Lint(unittest.TestCase):
    def lint(self, db, facts=None, only=None):
        return model.lint(model.Scan(db), facts, only)

    def test_clean_synthetic(self):
        self.assertEqual(self.lint(synth.typedb(), synth.binfacts()), [])

    def test_unmarked_entry_point_is_luas7(self):
        db = synth.typedb()
        for f in db["functions"]:
            if f["name"] == "lLater":
                f["markers"]["lua"] = []
        d = self.lint(db, synth.binfacts())
        self.assertEqual([(x.rule, x.severity) for x in d], [("LUA7", "error")])
        self.assertIn("lLater without /// lua", d[0].message)

    def test_rules_filter(self):
        db = copy.deepcopy(synth.typedb())
        db["functions"][0]["markers"]["lua"][0]["text"] = "UI.GetHeadSelectedUnit(a: integer,b: integer)"
        self.assertEqual([x.rule for x in self.lint(db, only={"LUA1"})], ["LUA1"])
        self.assertEqual(self.lint(db, only={"LUA9"}), [])


if __name__ == "__main__":
    unittest.main()
