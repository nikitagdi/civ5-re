import sys
import tempfile
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from luaapi import model, usage  # noqa: E402
from luaapi.tests import synth  # noqa: E402

LUA = """
-- UI.Commented() is not a call
--[[ UI.Block() ]]
local u = UI.GetHeadSelectedUnit()
UI:Open()
UI:Open(name)
Controls.Ok:SetHide(true)
Controls.Cancel:SetHide(false)
ContextPtr:SetInputHandler(f)
Events.SerialEventStart.Add(f)
Events.SerialEventStart(1, 2)
Events.SerialEventStart.CallImmediate()
LuaEvents.MyEvent.Add(f)
LuaEvents.MyEvent()
GameEvents.PlayerDoTurn.Add(f)
PreGame.Nope()
"""


class Scan(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        u = usage.Usage()
        u.add_text(LUA)
        cls.u = u.to_dict()

    def test_dot_and_colon(self):
        m = self.u["members"]
        self.assertEqual(m["UI"]["GetHeadSelectedUnit"], {"dot": 1, "colon": 0, "colon_args": 0})
        self.assertEqual(m["UI"]["Open"], {"dot": 0, "colon": 2, "colon_args": 1})
        self.assertEqual(m["ContextPtr"]["SetInputHandler"]["colon"], 1)
        self.assertNotIn("Commented", m["UI"])
        self.assertNotIn("Block", m["UI"])

    def test_controls_methods(self):
        self.assertEqual(self.u["controls"], {"SetHide": 2})

    def test_events(self):
        self.assertEqual(self.u["events"], {"SerialEventStart": {"Add": 1, "call": 1, "CallImmediate": 1}})
        self.assertEqual(self.u["luaevents"], {"MyEvent": 2})
        self.assertEqual(self.u["gameevents"], {"PlayerDoTurn": 1})

    def test_mismatches(self):
        doc = model.build(synth.typedb(), synth.binfacts())
        doc["functions"][0]["call"] = "dot"
        lines = usage.mismatches(doc, {"members": {"UI": {"Open": {"dot": 0, "colon": 2, "colon_args": 1},
                                                           "Nope": {"dot": 3, "colon": 0, "colon_args": 0}}}})
        self.assertEqual(lines, ["undeclared  UI.Nope  dot 3 colon 0",
                                 "colon-with-args  UI:Open  1 calls, declared with '.'"])

    def test_scan_dir(self):
        with tempfile.TemporaryDirectory() as d:
            (Path(d) / "Assets" / "UI").mkdir(parents=True)
            (Path(d) / "Assets" / "UI" / "a.lua").write_text("UI.A()\n", encoding="utf-8")
            (Path(d) / "Assets" / "UI" / "b.txt").write_text("UI.B()\n", encoding="utf-8")
            u = usage.scan_dir(d)
            self.assertEqual((u.files, sorted(u.members["UI"])), (1, ["A"]))
        self.assertEqual(usage.scan_dir("no-such-dir").files, 0)


class Game(unittest.TestCase):
    def test_real_game(self):
        from civ5re import config
        game = config.load().path("game_dir")
        if game is None or not (game / "Assets").is_dir():
            self.skipTest("game_dir is empty: usage tests need the game's Lua (set game_dir in civ5re.local.cfg)")
        files = usage.lua_files(game)
        self.assertEqual(len(files), 605)
        u = usage.scan_dir(game).to_dict()
        self.assertGreaterEqual(len(u["controls"]), 88)


if __name__ == "__main__":
    unittest.main()
