import json
import sys
import unittest
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from luaapi import entries, model  # noqa: E402

BINFACTS = Path(__file__).resolve().parents[3] / "build" / "binfacts.json"


@unittest.skipUnless(BINFACTS.is_file(), "build/binfacts.json is missing: run `civ5re binfacts`")
class RealBinary(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.facts = json.loads(BINFACTS.read_text(encoding="utf-8"))
        cls.eps = entries.entry_points(cls.facts)

    def kind(self, k):
        return [e for e in self.eps if e.kind == k]

    def test_entry_point_counts(self):
        ls = self.kind("l")
        self.assertEqual(len(ls), 847)
        self.assertEqual(len({e.scope for e in ls}), 33)
        tm = self.kind("template")
        self.assertEqual(len(tm), 10)
        self.assertEqual(sum(len(e.symbols) for e in tm), 2560)
        self.assertEqual(len(self.kind("thunk")), 313)
        self.assertEqual(len(self.eps), 1170)
        self.assertEqual(sum(len(e.symbols) for e in self.eps), 847 + 2560 + 313)

    def test_scope_sizes(self):
        by = Counter(e.scope for e in self.kind("l"))
        self.assertEqual(by["cvLuaInterfaceBuddy"], 207)
        self.assertEqual(by["cvLuaOptionsManager"], 198)
        self.assertEqual(by["cvLuaPreGame"], 104)

    def test_template_members(self):
        keys = sorted(e.key for e in self.kind("template"))
        self.assertIn("CvLuaEvent::Handle::lDestroy", keys)
        self.assertEqual(sum(k.startswith("CvLuaEvent::") for k in keys), 10)

    def test_event_instantiations(self):
        self.assertEqual(model.count_instantiations(self.facts), 256)
        inst = model.instantiation_events(self.facts)
        self.assertEqual(Counter(c for c, _ in inst), {"serial": 133, "local": 98, "nonserial": 24, "sequencer": 1})

    def test_marshallers(self):
        syms = [s for s in self.facts["symbols"] if s["group"] == "firaxis" and s["scope"] == "LuaSupport"]
        self.assertEqual(Counter(s["name"] for s in syms), {"ToLua": 137, "FromLua": 137})

    def test_thunks_with_an_l_candidate(self):
        self.assertEqual(len(entries.thunk_l_candidates(self.eps)), 14)

    def test_all_lua_state_symbols(self):
        n = sum(1 for s in self.facts["symbols"] if s["params"] == "lua_State*")
        self.assertEqual(n, 4943)


if __name__ == "__main__":
    unittest.main()
