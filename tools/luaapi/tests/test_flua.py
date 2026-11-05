import sys
import unittest
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parents[2]))

from luaapi import flua, grammar  # noqa: E402
from luaapi.tests import synth  # noqa: E402


def fits(mapped, lua_text, is_return=False):
    return flua.compatible(mapped, grammar.parse_type(lua_text), is_return=is_return)


class Mapping(unittest.TestCase):
    def setUp(self):
        self.m = flua.Mapper(shapes={"FGXVector2": "Vector2"}, classes={"ControlBase": "ControlBase"})

    def test_table(self):
        cases = {
            "bool": "boolean", "int": "integer", "unsigned int": "integer", "unsigned long": "integer",
            "char": "integer", "float": "number", "double": "number", "const char *": "string",
            "char *": "string", "const wchar_t *": "string", "FStringA": "string", "FStringW": "string",
            "FLua::String": "string", "FLua::Table": "table", "FLua::Function": "function",
            "FLua::Value": "any", "StackValue": "any", "const FGXVector2 &": "Vector2",
            "ControlBase *": "ControlBase", "ControlBase &": "ControlBase",
        }
        for cpp, lua in cases.items():
            self.assertEqual(self.m.map(cpp).type, lua, cpp)

    def test_pointer_flag_and_void(self):
        self.assertTrue(self.m.map("ControlBase *").pointer)
        self.assertFalse(self.m.map("FGXVector2").pointer)
        self.assertTrue(self.m.map("void").void)
        self.assertIsNone(self.m.map("Mystery").type)
        self.assertIsNone(self.m.map("int *").type)

    def test_graph_resolves_typedef_and_enum(self):
        types = {"uint32": {"kind": "typedef", "name": "uint32", "of": "unsigned int"},
                 "unsigned int": {"kind": "builtin", "name": "unsigned int", "signed": False, "size": 4},
                 "Mode": {"kind": "enum", "key": "Mode", "size": 4},
                 "FStringA": {"kind": "typedef", "name": "FStringA", "of": "Str"},
                 "Str": {"kind": "record", "key": "Str", "size": 4}}
        m = flua.Mapper(types)
        self.assertEqual(m.map("uint32").type, "integer")
        self.assertEqual(m.map("Mode").type, "integer")
        self.assertEqual(m.map("FStringA").type, "string")

    def test_graph_of_synthetic_db(self):
        m = flua.Mapper(synth.TYPES, shapes={"FGXVector2": "Vector2"})
        self.assertEqual(m.map("const FGXVector2 &").type, "Vector2")
        self.assertEqual(m.map("bool").type, "boolean")


class Compatibility(unittest.TestCase):
    def test_integer_fits_number_param(self):
        self.assertTrue(fits(flua.Mapped("number", False), "integer"))
        self.assertFalse(fits(flua.Mapped("integer", False), "number"))
        self.assertTrue(fits(flua.Mapped("integer", False), "number", is_return=True))

    def test_nullable_only_for_pointers(self):
        self.assertTrue(fits(flua.Mapped("ControlBase", True), "ControlBase?"))
        self.assertTrue(fits(flua.Mapped("ControlBase", True), "ControlBase"))
        self.assertFalse(fits(flua.Mapped("boolean", False), "boolean?"))

    def test_other_kinds(self):
        self.assertTrue(fits(flua.Mapped("table", False), "string[]"))
        self.assertTrue(fits(flua.Mapped("function", False), "fun(a: integer)"))
        self.assertTrue(fits(flua.Mapped("any", False), "string"))
        self.assertTrue(fits(flua.Mapped("string", False), "any"))
        self.assertTrue(fits(flua.Mapped("string", False), "integer|string"))
        self.assertFalse(fits(flua.Mapped("string", False), "boolean"))
        self.assertTrue(fits(flua.Mapped("Vector2", False), "Vector2"))
        self.assertFalse(fits(flua.Mapped("Vector2", False), "integer"))
        self.assertTrue(fits(flua.Mapped(None, False), "integer"))


if __name__ == "__main__":
    unittest.main()
