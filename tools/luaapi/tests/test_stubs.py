import json
import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from luaapi import emit_stubs, grammar, model  # noqa: E402
from luaapi.tests import synth  # noqa: E402

EXPECTED = synth.FIXTURES / "stubs"


def stubs():
    return emit_stubs.generate(model.build(synth.typedb(), synth.binfacts()))


class Reference(unittest.TestCase):
    def test_matches_reference(self):
        files = stubs()
        if synth.updating():
            for rel, text in files.items():
                p = EXPECTED / rel
                p.parent.mkdir(parents=True, exist_ok=True)
                p.write_bytes(text.encode("utf-8"))
        have = {p.relative_to(EXPECTED).as_posix() for p in EXPECTED.rglob("*") if p.is_file()}
        self.assertEqual(sorted(files), sorted(have))
        for rel, text in files.items():
            self.assertEqual(text, (EXPECTED / rel).read_bytes().decode("utf-8"), rel)

    def test_deterministic(self):
        self.assertEqual(stubs(), stubs())


class Shape(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.files = stubs()

    def test_every_lua_file_starts_with_meta(self):
        for rel, text in self.files.items():
            if rel.endswith(".lua"):
                self.assertTrue(text.startswith("---@meta\n-- generated, do not edit\n"), rel)

    def test_layout(self):
        self.assertEqual(sorted(self.files), [
            ".luarc.json", "_enums.lua", "_globals.lua", "_types.lua", "classes/AlphaAnim.lua",
            "classes/ControlBase.lua", "classes/Event.lua", "events.lua", "tables/UI.lua"])
        rc = json.loads(self.files[".luarc.json"])
        self.assertEqual(rc, {"runtime": {"version": "Lua 5.1"}, "workspace": {"library": ["."]}})

    def test_overload_and_return_and_doc(self):
        ui = self.files["tables/UI.lua"]
        self.assertIn("--- Head of the active player's selected group.\n---@return Unit?\n"
                      "function UI.GetHeadSelectedUnit() end", ui)
        self.assertIn("---@overload fun(name: string, flag: boolean)\nfunction UI.Open(name) end", ui)
        self.assertNotIn("lHidden", ui)
        self.assertNotIn("Later", ui)

    def test_event_arguments_are_substituted(self):
        ev = self.files["events.lua"]
        self.assertIn("---@class Event.SerialEventUnitCreated : Event", ev)
        self.assertIn("---@field Add fun(self: Event.SerialEventUnitCreated, handler: fun(playerID: integer, "
                      "unitID: integer))", ev)
        self.assertIn("---@field __call fun(self: Event.SerialEventStart)", ev)
        self.assertIn("---@field SerialEventStart Event.SerialEventStart", ev)

    def test_method_and_inherited_owner(self):
        self.assertIn("function AlphaAnim:SetSize(size) end", self.files["classes/AlphaAnim.lua"])
        self.assertIn("---@class AlphaAnim : ControlBase", self.files["classes/AlphaAnim.lua"])
        self.assertIn("---@param size Vector2", self.files["classes/ControlBase.lua"])

    def test_globals_and_enums(self):
        self.assertIn("---@type table<string, ControlBase>\nControls = nil", self.files["_globals.lua"])
        self.assertIn("---@enum Keys\nKeys = {\n    VK_A = 65,\n    VK_RETURN = 13,\n}", self.files["_enums.lua"])
        self.assertIn("---@field x number", self.files["_types.lua"])
        self.assertIn("-- Lekmod: Lua/CvLuaUnit.h", self.files["_types.lua"])


class TypeSpelling(unittest.TestCase):
    def test_lt(self):
        cases = {"integer": "integer", "Unit?": "Unit?", "(A|B)[]": "(A|B)[]", "table<string, integer[]>":
                 "table<string, integer[]>", "{x: number, y?: number}": "{ x: number, y?: number }",
                 "fun(a: integer) -> boolean, string": "fun(a: integer): boolean, string", "A|B?": "A|B?",
                 "(A|B)?": "(A|B)?"}
        for src, want in cases.items():
            self.assertEqual(emit_stubs.lt(grammar.parse_type(src)), want, src)

    def test_reserved_names_are_renamed(self):
        self.assertEqual(emit_stubs.ident("end"), "end_")
        self.assertEqual(emit_stubs.ident("size"), "size")


if __name__ == "__main__":
    unittest.main()
