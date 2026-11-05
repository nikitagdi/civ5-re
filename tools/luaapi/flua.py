"""C++ types of FLua targets -> Lua types."""
from __future__ import annotations

import re
from typing import NamedTuple

from . import grammar

INTEGERS = frozenset((
    "char", "signed char", "unsigned char", "short", "unsigned short", "int", "unsigned int", "long",
    "unsigned long", "long long", "unsigned long long", "short int", "unsigned short int", "long int",
    "unsigned long int", "long long int", "unsigned long long int", "signed", "unsigned", "signed int",
    "wchar_t", "char16_t", "char32_t", "size_t", "int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t",
    "uint32_t", "int64_t", "uint64_t",
))
FLOATS = frozenset(("float", "double", "long double"))
STRINGS = frozenset(("FStringA", "FStringW", "FLua::String", "FString"))
SPECIAL = {"FLua::Table": "table", "FLua::Function": "function", "FLua::Value": "any", "StackValue": "any"}
CHARS = frozenset(("char", "wchar_t", "char16_t", "char32_t"))
_QUAL = re.compile(r"\b(const|volatile|struct|class|enum|union)\b")


class Mapped(NamedTuple):
    type: str | None   # Lua type, None when the C++ type is not in the table
    pointer: bool      # a pointer or reference to a Lua class (may be nil)
    void: bool = False
    cxx: str = ""      # the base C++ name after peeling


class Mapper:
    """types: typedb `types` graph (optional); shapes: C++ struct -> shape name; classes: C++ class -> Lua class."""

    def __init__(self, types: dict | None = None, shapes: dict | None = None, classes: dict | None = None):
        self.types = types or {}
        self.shapes = shapes or {"FGXVector2": "Vector2"}
        self.classes = classes or {}

    def peel(self, spelling: str):
        """(base name, kind, pointer depth, names seen on the way); kind is builtin|record|enum|unknown."""
        seen = []
        depth = 0
        tid = spelling
        for _ in range(64):
            node = self.types.get(tid)
            if node is None:
                break
            k = node["kind"]
            if k == "pointer":
                depth += 1
                tid = node["to"]
            elif k == "ref":
                tid = node["to"]
            elif k == "cv":
                tid = node["of"]
            elif k == "typedef":
                seen.append(node["name"])
                tid = node["of"]
            elif k == "builtin":
                return node["name"], "builtin", depth, seen
            elif k in ("record", "enum"):
                seen.append(node["key"])
                return node["key"], k, depth, seen
            else:
                return node.get("name") or tid, "unknown", depth, seen
        text = tid
        depth += text.count("*")
        text = text.replace("*", " ").replace("&", " ")
        text = " ".join(_QUAL.sub(" ", text).split())
        if text in INTEGERS or text in FLOATS or text in ("bool", "void"):
            return text, "builtin", depth, seen
        seen.append(text)
        return text, "unknown" if text not in self.classes and text not in self.shapes else "record", depth, seen

    def map(self, spelling: str) -> Mapped:
        base, kind, depth, seen = self.peel(spelling)
        if kind == "builtin":
            if base == "void" and depth == 0:
                return Mapped(None, False, True, base)
            if depth == 1 and base in CHARS:
                return Mapped("string", False, False, base)
            if depth:
                return Mapped(None, False, False, base)
            if base == "bool":
                return Mapped("boolean", False, False, base)
            if base in INTEGERS:
                return Mapped("integer", False, False, base)
            if base in FLOATS:
                return Mapped("number", False, False, base)
            return Mapped(None, False, False, base)
        if kind == "enum":
            return Mapped(None if depth else "integer", False, False, base)
        for n in seen:
            if n in STRINGS:
                return Mapped(None if depth else "string", False, False, n)
            if n in SPECIAL:
                return Mapped(None if depth else SPECIAL[n], False, False, n)
            if n in self.shapes:
                return Mapped(self.shapes[n], False, False, n)
            if n in self.classes:
                return Mapped(self.classes[n], bool(depth), False, n)
        return Mapped(None, False, False, base)


def _prim(t):
    return t["name"] if t["k"] == "prim" else None


def compatible(mapped: Mapped, lua_t: dict, *, is_return: bool = False) -> bool:
    """Whether the declared Lua type `lua_t` fits the Lua type of the C++ target."""
    if mapped.type is None:
        return True
    k = lua_t["k"]
    if k == "union":
        return any(compatible(mapped, it, is_return=is_return) for it in lua_t["items"])
    if k == "nullable":
        if not mapped.pointer and not (is_return and mapped.type in ("any", "table")):
            return False
        return compatible(mapped, lua_t["inner"], is_return=is_return)
    want = mapped.type
    if _prim(lua_t) == "any" or want == "any":
        return True
    if want == "integer":
        return _prim(lua_t) in (("integer", "number") if is_return else ("integer",))
    if want == "number":
        return _prim(lua_t) in (("number",) if is_return else ("number", "integer"))
    if want in ("boolean", "string"):
        return _prim(lua_t) == want
    if want == "table":
        return k in ("array", "map", "record") or _prim(lua_t) == "table"
    if want == "function":
        return k == "fun" or _prim(lua_t) == "function"
    return k == "name" and lua_t["name"] == want


def parse_lua_type(text: str) -> dict:
    return grammar.parse_type(text, strict=False)
