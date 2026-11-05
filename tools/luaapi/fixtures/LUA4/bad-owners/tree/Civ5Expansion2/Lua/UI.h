/// origin guess
#pragma once

struct lua_State;

/// lua table UI
class cvLuaUI {
public:
    /// 0x08000010
    /// lua PreGame.X()
    static int lX(lua_State* L);
};

class Plain {
public:
    /// 0x08000011
    /// lua UI.Y()
    static int lY(lua_State* L);
};
