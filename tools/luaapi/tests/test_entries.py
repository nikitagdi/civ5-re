import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from luaapi import entries, model  # noqa: E402
from luaapi.tests import synth  # noqa: E402


def classify(db, facts, paths=None):
    scan = model.Scan(db)
    eps = entries.entry_points(facts)
    res = entries.classify(eps, find=scan.find_decl, kinds=scan.kinds_of,
                           paths=set(scan.paths) if paths is None else paths, classes=scan.cxx_classes)
    return eps, res


class ThunkPath(unittest.TestCase):
    def test_longest_prefix_class(self):
        classes = {"ControlBase": "ControlBase", "Control": "Short"}
        self.assertEqual(entries.thunk_path("LuaThunk_ControlBase_SetHide", classes), "ControlBase:SetHide")
        self.assertEqual(entries.thunk_path("LuaThunk_Control_Base_SetHide", classes), "Short:Base_SetHide")

    def test_class_with_underscore_wins(self):
        classes = {"Foo": "Foo", "Foo_Bar": "FooBar"}
        self.assertEqual(entries.thunk_path("LuaThunk_Foo_Bar_Baz", classes), "FooBar:Baz")

    def test_global_with_underscore(self):
        self.assertEqual(entries.thunk_path("LuaThunk_Get_Thing", {"ControlBase": "ControlBase"}), "Get_Thing")
        self.assertEqual(entries.thunk_path("LuaThunk_LookUpControl", {}), "LookUpControl")


class EntryPoints(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.facts = synth.binfacts()
        cls.eps = entries.entry_points(cls.facts)

    def by_kind(self, kind):
        return [e for e in self.eps if e.kind == kind]

    def test_one_entry_point_per_kind(self):
        self.assertEqual([len(self.by_kind(k)) for k in entries.KINDS], [5, 3, 3])

    def test_template_members_are_grouped(self):
        names = {e.key: e for e in self.by_kind("template")}
        self.assertEqual(sorted(names), ["CvLuaEvent::Handle::lDestroy", "CvLuaEvent::lAdd",
                                         "CvLuaEvent::lMetaCall"])
        self.assertEqual(len(names["CvLuaEvent::lAdd"].symbols), 3)

    def test_not_entry_points(self):
        names = {e.name for e in self.eps}
        self.assertNotIn("RegisterMeta", names)
        self.assertNotIn("LuaToString_ControlBase_SetHide", names)
        self.assertNotIn("ToLua", names)

    def test_group_and_params_filter(self):
        facts = {"symbols": [dict(s, group="aspyr") for s in self.facts["symbols"]]}
        self.assertEqual(entries.entry_points(facts), [])

    def test_l_candidate_by_name(self):
        def s(m, name, scope, addr):
            return {"mangled": m, "name": name, "scope": scope, "params": "lua_State*", "group": "firaxis",
                    "addr": addr}
        facts = {"symbols": [
            s("a", "lSetText", "TextControl", "0x1"), s("b", "LuaThunk_TextControl_SetText", "", "0x2"),
            s("c", "lLookUpControl", "LuaContext", "0x3"), s("d", "LuaThunk_LookUpControl", "", "0x4"),
            s("e", "LuaThunk_Other", "", "0x5")]}
        eps = entries.entry_points(facts)
        self.assertEqual(entries.thunk_l_candidates(eps), {"b": "a", "d": "c"})


class Classify(unittest.TestCase):
    def test_one_status_each(self):
        eps, res = classify(synth.typedb(), synth.binfacts())
        st = {e.name: res[e.key][0] for e in eps}
        self.assertEqual(st["lGetHeadSelectedUnit"], "marked")
        self.assertEqual(st["lHidden"], "none")
        self.assertEqual(st["lLater"], "unknown")
        self.assertEqual(st["lOther"], "undeclared")
        self.assertEqual(st["LuaThunk_ControlBase_SetHide"], "marked")
        self.assertEqual(st["LuaThunk_FooBar_Something"], "undeclared")
        self.assertEqual(st["lAdd"], "marked")
        self.assertEqual(entries.count_statuses(res)["undeclared"], 3)

    def test_unmarked(self):
        db = synth.typedb()
        for f in db["functions"]:
            if f["name"] == "lLater":
                f["markers"]["lua"] = []
        eps, res = classify(db, synth.binfacts())
        self.assertEqual(res[next(e.key for e in eps if e.name == "lLater")][0], "unmarked")

    def test_thunk_with_question_mark(self):
        db = synth.typedb()
        for f in db["functions"]:
            if f["name"] == "LuaThunk_ControlBase_SetHide":
                f["markers"]["lua"] = [{"file": synth.FILE, "line": 1, "text": "?"}]
        eps, res = classify(db, synth.binfacts(), paths=set())
        self.assertEqual(res[next(e.key for e in eps if e.name == "LuaThunk_ControlBase_SetHide")][0], "unknown")


if __name__ == "__main__":
    unittest.main()
