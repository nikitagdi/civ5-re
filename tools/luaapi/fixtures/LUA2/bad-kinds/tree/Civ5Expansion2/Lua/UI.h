/// origin guess
#pragma once

struct lua_State;

/// 0x08000010
/// lua table UI
void lFree(lua_State* L);

/// 0x08000011
/// lua event Events.X
void lEvent(lua_State* L);

/// lua UI.Run()
class Holder {};

/// lua enum Mode
typedef int Mode;

/// 0x08000012
/// lua push (a: integer)
void FromLua(lua_State* L, Holder& h);

/// 0x08000013
/// lua pull (a: integer)
void ToLua(lua_State* L, Holder& h);
