/// origin guess
#pragma once

struct lua_State;

/// lua table UI
class cvLuaUI {
public:
    /// 0x08000010
    /// lua UI.GetX() -> integer
    static int lGetX(lua_State* L);
    /// 0x08000011
    /// lua UI.GetX() -> integer
    static int lGetY(lua_State* L);
};
