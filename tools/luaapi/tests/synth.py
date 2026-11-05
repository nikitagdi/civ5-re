"""Synthetic typedb.json / binfacts.json dicts for the model and stub tests."""
from __future__ import annotations

import os
from pathlib import Path

FIXTURES = Path(__file__).resolve().parent / "fixtures"
FILE = "Civ5Expansion2/Lua/UI.h"

TYPES = {
    "lua_State": {"kind": "record", "key": "lua_State", "size": None},
    "lua_State *": {"kind": "pointer", "to": "lua_State", "size": 4},
    "bool": {"kind": "builtin", "name": "bool", "signed": None, "size": 1},
    "int": {"kind": "builtin", "name": "int", "signed": True, "size": 4},
    "void": {"kind": "builtin", "name": "void", "signed": None, "size": None},
    "FGXVector2": {"kind": "record", "key": "FGXVector2", "size": 8},
    "const FGXVector2": {"kind": "cv", "of": "FGXVector2", "const": True, "volatile": False},
    "const FGXVector2 &": {"kind": "ref", "to": "const FGXVector2", "rvalue": False, "size": 4},
    "UnitCreatedData": {"kind": "record", "key": "UnitCreatedData", "size": 8},
    "UnitCreatedData &": {"kind": "ref", "to": "UnitCreatedData", "rvalue": False, "size": 4},
}


def lua(*texts, line=1):
    return {"lua": [{"file": FILE, "line": line + i, "text": t} for i, t in enumerate(texts)]}


def comment(text, line=1):
    return {"line": line, "text": text}


def fn(qualified, params=(), ret="void", texts=(), line=10, mangled=None, cmt=None, scope="", scope_kind="namespace",
       addr=None, instances=()):
    name = qualified.rsplit("::", 1)[-1]
    key = mangled or (f"{qualified}@{FILE}:{line}" if instances else f"_Z_{qualified.replace('::', '_')}")
    m = lua(*texts, line=line - len(texts))
    m.update({"addr": addr, "base": None, "deleting": None, "thunks": [], "inline": False, "pure": False,
              "absent": False, "fastcc": False})
    return {"key": key, "qualified": qualified, "name": name, "scope": scope, "scope_kind": scope_kind,
            "file": FILE, "line": line, "kind": "method" if scope else "function",
            "mangled": None if instances else key, "variants": {}, "static": False, "return": ret,
            "params": [{"name": n, "type": t} for n, t in params], "markers": m,
            "instances": list(instances), "redecls": [], "comment": comment(cmt, line - len(texts) - 1) if cmt else None}


def rec(name, texts=(), bases=(), template=None, cmt=None, line=5):
    return {"name": name, "kind": "class", "file": FILE, "line": line, "bases": [{"name": b, "access": "public"} for b in bases],
            "fields": [], "markers": lua(*texts, line=line - len(texts)), "template": template,
            "comment": comment(cmt, line - len(texts) - 1) if cmt else None}


def typedb():
    th = lambda n, m: fn(n, [("L", "lua_State *")], "int", (), 80, m, addr="0x08000000")
    return {
        "schema": "civ5re.typedb/1", "ok": True, "types": TYPES, "diagnostics": [],
        "records": [
            rec("cvLuaInterfaceBuddy", ["table UI"], line=20),
            rec("ControlBase", ["class ControlBase"], line=30),
            rec("AlphaAnimCtl", ["class AlphaAnim : ControlBase xml AlphaAnim, Alpha"], bases=["ControlBase"], line=40),
            rec("FGXVector2", ["shape Vector2 {x: number, y: number}"], line=50),
            rec("ControlHolder", ["global Controls: table<string, ControlBase>"], line=60),
            rec("CvLuaEvent", ["class Event"], template={"args": None, "pattern": "CvLuaEvent"}, line=70),
            rec("Holder", [], line=75),
        ],
        "functions": [
            fn("cvLuaInterfaceBuddy::lGetHeadSelectedUnit", [("L", "lua_State *")], "int",
               ["UI.GetHeadSelectedUnit() -> Unit?"], 22, "_ZN19cvLuaInterfaceBuddy20lGetHeadSelectedUnitEP9lua_State",
               "Head of the active player's selected group.", "cvLuaInterfaceBuddy", "class", "0x089468e6"),
            fn("cvLuaInterfaceBuddy::lOpen", [("L", "lua_State *")], "int",
               ["UI.Open(name: string)", "UI.Open(name: string, flag: boolean)"], 25,
               "_ZN19cvLuaInterfaceBuddy5lOpenEP9lua_State", None, "cvLuaInterfaceBuddy", "class", "0x089468e7"),
            fn("cvLuaInterfaceBuddy::lHidden", [("L", "lua_State *")], "int", ["none"], 28,
               "_ZN19cvLuaInterfaceBuddy7lHiddenEP9lua_State", "Registered but never reachable from Lua.",
               "cvLuaInterfaceBuddy", "class", "0x089468e8"),
            fn("cvLuaInterfaceBuddy::lLater", [("L", "lua_State *")], "int", ["?"], 29,
               "_ZN19cvLuaInterfaceBuddy6lLaterEP9lua_State", None, "cvLuaInterfaceBuddy", "class", "0x089468e9"),
            fn("ControlBase::SetHide", [("hide", "bool")], "void", ["ControlBase:SetHide(hide: boolean)"], 32,
               "_ZN11ControlBase7SetHideEb", None, "ControlBase", "class", "0x08691916"),
            fn("ControlBase::SetSize", [("size", "const FGXVector2 &")], "void",
               ["AlphaAnim:SetSize(size: Vector2)", "ControlBase:SetSize(size: Vector2)"], 36,
               "_ZN11ControlBase7SetSizeERK10FGXVector2", "Resizes the control.", "ControlBase", "class", "0x08691a00"),
            th("LuaThunk_ControlBase_SetHide", "_Z28LuaThunk_ControlBase_SetHideP9lua_State"),
            th("LuaThunk_ControlBase_SetSize", "_Z28LuaThunk_ControlBase_SetSizeP9lua_State"),
            fn("CvLuaEvent::lAdd", [("L", "lua_State *")], "int", ["Event:Add(handler: fun(EventArgs))"], 72,
               None, None, "CvLuaEvent", "class", None, instances=[{"spec": "x", "mangled": "_ZFAKE"}]),
            fn("CvLuaEvent::lMetaCall", [("L", "lua_State *")], "int", ["Event:__call(EventArgs)"], 74,
               None, None, "CvLuaEvent", "class", None, instances=[{"spec": "x", "mangled": "_ZFAKE2"}]),
            fn("LuaSupport::ToLua", [("L", "lua_State *"), ("d", "UnitCreatedData &")], "void",
               ["push (playerID: integer, unitID: integer)"], 90, "_ZN10LuaSupport5ToLuaEP9lua_StateR15UnitCreatedData"),
            fn("LuaSupport::FromLua", [("L", "lua_State *"), ("d", "UnitCreatedData &")], "void",
               ["pull (playerID: integer, unitID: integer)"], 95, "_ZN10LuaSupport7FromLuaEP9lua_StateR15UnitCreatedData"),
        ],
        "enums": [{"qualified": "Keys", "file": FILE, "line": 100, "values": [{"name": "VK_A", "value": 65},
                  {"name": "VK_RETURN", "value": 13}], "markers": lua("enum Keys", line=99),
                  "comment": comment("Virtual keys.", 98)}],
        "typedefs": [
            {"qualified": "EvUnitCreated", "file": FILE, "line": 110, "markers": lua("event Events.SerialEventUnitCreated", line=109),
             "type": "EventTemplate<Event_Int2Type<3>,UnitCreatedData,LinearEventDispatcher,SerialContainer,NullEventClass>",
             "comment": None},
            {"qualified": "EvStart", "file": FILE, "line": 112, "markers": lua("event Events.SerialEventStart", line=111),
             "type": "EventTemplate<Event_Int2Type<4>,NullEventClass,LinearEventDispatcher,NonSerialContainer,NullEventClass>",
             "comment": comment("Fires once.", 110)},
        ],
    }


def _sym(mangled, name, scope="", addr="0x08000000", params="lua_State*"):
    return {"mangled": mangled, "demangled": f"{scope + '::' if scope else ''}{name}({params})", "addr": addr,
            "size": 8, "type": "func", "bind": "weak" if "<" in scope else "global", "section": ".text",
            "kind": "method" if scope else "function", "variant": None, "scope": scope,
            "scope_is_class": True if scope else None, "name": name, "params": params, "cv": None, "group": "firaxis"}


def ev_scope(id_, payload, cont):
    return f"CvLuaEvent<EventTemplate<Event_Int2Type<{id_}>, {payload}, LinearEventDispatcher, {cont}, NullEventClass>>"


def binfacts():
    syms = [
        _sym("_ZN19cvLuaInterfaceBuddy20lGetHeadSelectedUnitEP9lua_State", "lGetHeadSelectedUnit", "cvLuaInterfaceBuddy", "0x089468e6"),
        _sym("_ZN19cvLuaInterfaceBuddy5lOpenEP9lua_State", "lOpen", "cvLuaInterfaceBuddy", "0x089468e7"),
        _sym("_ZN19cvLuaInterfaceBuddy7lHiddenEP9lua_State", "lHidden", "cvLuaInterfaceBuddy", "0x089468e8"),
        _sym("_ZN19cvLuaInterfaceBuddy6lLaterEP9lua_State", "lLater", "cvLuaInterfaceBuddy", "0x089468e9"),
        _sym("_ZN19cvLuaInterfaceBuddy6lOtherEP9lua_State", "lOther", "cvLuaInterfaceBuddy", "0x089468ea"),
        _sym("_Z28LuaThunk_ControlBase_SetHideP9lua_State", "LuaThunk_ControlBase_SetHide", "", "0x08693aaf"),
        _sym("_Z28LuaThunk_ControlBase_SetSizeP9lua_State", "LuaThunk_ControlBase_SetSize", "", "0x08693b00"),
        _sym("_Z24LuaThunk_FooBar_SomethingP9lua_State", "LuaThunk_FooBar_Something", "", "0x08693c00"),
        _sym("_ZLuaToString", "LuaToString_ControlBase_SetHide", "", "0x08693d00"),
    ]
    for i, (id_, p, c) in enumerate(((3, "UnitCreatedData", "SerialContainer"), (4, "NullEventClass", "NonSerialContainer"),
                                     (9, "NullEventClass", "LocalMachineContainer"))):
        sc = ev_scope(id_, p, c)
        syms.append(_sym(f"_ZFAKEAdd{i}", "lAdd", sc, f"0x08a7{i:04x}"))
        syms.append(_sym(f"_ZFAKECall{i}", "lMetaCall", sc, f"0x08a8{i:04x}"))
        syms.append(_sym(f"_ZFAKEHandle{i}", "lDestroy", sc + "::Handle", f"0x08a9{i:04x}"))
        syms.append(_sym(f"_ZFAKEReg{i}", "RegisterMeta", sc, f"0x08aa{i:04x}"))
    syms.append(_sym("_ZToLua", "ToLua", "LuaSupport", "0x08b00000", "lua_State*, UnitCreatedData&"))
    syms.append(_sym("_ZFromLua", "FromLua", "LuaSupport", "0x08b00010", "lua_State*, UnitCreatedData&"))
    return {"schema": 1, "binary": {"sha256": "ab" * 32}, "symbols": syms}


def updating() -> bool:
    return os.environ.get("LUAAPI_UPDATE") == "1"
