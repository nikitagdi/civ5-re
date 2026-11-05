import unittest

from seed import luaentry


def sym(name, scope="", params="lua_State*", kind="function", group="firaxis", mangled=None):
    return {"name": name, "scope": scope, "params": params, "kind": kind, "group": group,
            "mangled": mangled or f"_Z{scope}{name}", "addr": "0x08000000", "demangled": f"{scope}::{name}"}


class ClassifyTests(unittest.TestCase):
    def test_l_function_of_a_class(self):
        self.assertEqual(luaentry.classify(sym("lGetCredits", "cvLuaInterfaceBuddy")), "l")
        self.assertEqual(luaentry.classify(sym("lFoo", "A::B::C", kind="method")), "l")

    def test_template_member(self):
        self.assertEqual(luaentry.classify(sym("lAdd", "CvLuaEvent<EventTemplate<Event_Int2Type<99>>>")), "template")

    def test_thunk_has_no_scope(self):
        self.assertEqual(luaentry.classify(sym("LuaThunk_ControlBase_SetHide")), "thunk")
        self.assertIsNone(luaentry.classify(sym("LuaToString_Foo")))
        self.assertIsNone(luaentry.classify(sym("LuaThunk_Scoped", "Ns")))

    def test_not_an_entry_point(self):
        self.assertIsNone(luaentry.classify(sym("Update", "Foo")))
        self.assertIsNone(luaentry.classify(sym("lower", "Foo", params="int")))
        self.assertIsNone(luaentry.classify(sym("lFoo", "Foo", group="aspyr")))
        self.assertIsNone(luaentry.classify(sym("lFoo", "Foo", kind="ctor")))
        self.assertIsNone(luaentry.classify(sym("l1", "Foo")))
        self.assertIsNone(luaentry.classify(sym("lFoo")))


class EntriesTests(unittest.TestCase):
    def setUp(self):
        self.bf = {"symbols": [
            sym("lA", "Foo", mangled="_a"),
            sym("lB", "Foo", mangled="_b", kind="method"),
            sym("lAdd", "T<1>", mangled="_t1"),
            sym("lAdd", "T<2>", mangled="_t2"),
            sym("lGet", "T<2>", mangled="_t3"),
            sym("LuaThunk_X", mangled="_th"),
            sym("Other", "Foo"),
        ]}

    def test_counts_one_entry_per_template_member(self):
        self.assertEqual(luaentry.counts(self.bf), {"l": 2, "template": 2, "thunk": 1, "total": 5})

    def test_entries_are_sorted_and_deterministic(self):
        keys = [(e["kind"], e["key"]) for e in luaentry.entries(self.bf)]
        self.assertEqual(keys, sorted(keys))
        self.assertIn(("template", "T::lAdd"), keys)

    def test_index(self):
        idx = luaentry.Index(self.bf)
        self.assertTrue(idx.is_entry(mangled="_a"))
        self.assertTrue(idx.is_entry(mangled="_th"))
        self.assertTrue(idx.is_entry(scope="T", name="lAdd"))
        self.assertFalse(idx.is_entry(scope="T", name="lOther"))
        self.assertFalse(idx.is_entry(mangled="_nope"))


class RealBinaryTests(unittest.TestCase):
    def test_spec_counts(self):
        from binfacts import api
        try:
            bf = api.load()
        except Exception as e:  # noqa: BLE001 - no binary and no fresh cache
            self.skipTest(f"binfacts unavailable: {e}")
        self.assertEqual(luaentry.counts(bf), {"l": 847, "template": 10, "thunk": 313, "total": 1170})


if __name__ == "__main__":
    unittest.main()
