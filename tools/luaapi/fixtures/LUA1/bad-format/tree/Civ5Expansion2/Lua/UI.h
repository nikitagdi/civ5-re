/// origin guess
#pragma once

struct lua_State;

/// lua table UI
class cvLuaUI {
public:
    /// 0x08000010
    /// lua UI.GetX(a: integer,b: string)
    static int lGetX(lua_State* L);
    /// 0x08000011
    /// lua UI.GetY(a: integer) ->Unit
    static int lGetY(lua_State* L);
    /// 0x08000012
    /// lua UI.GetZ(
    static int lGetZ(lua_State* L);
};
