/// origin guess
#pragma once

struct lua_State;

/// lua class Base
class Base {};

/// 0x08000020
/// lua Base:Run()
int LuaThunk_Base_Run(lua_State* L);

/// 0x08000021
/// lua ?
int LuaToString_Base_Run(lua_State* L);
