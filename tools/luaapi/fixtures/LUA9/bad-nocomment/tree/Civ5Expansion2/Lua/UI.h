/// origin guess
#pragma once

struct lua_State;

class cvLuaUI {
public:
    /// 0x08000010
    /// lua none
    static int lGetX(lua_State* L);
    // Separated from its marker by a blank line.

    /// 0x08000011
    /// lua none
    static int lGetY(lua_State* L);
};
