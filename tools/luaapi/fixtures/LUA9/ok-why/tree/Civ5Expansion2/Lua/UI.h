/// origin guess
#pragma once

struct lua_State;

class cvLuaUI {
public:
    // The exe registers this name but never calls it.
    /// 0x08000010
    /// lua none
    static int lGetX(lua_State* L);
};
