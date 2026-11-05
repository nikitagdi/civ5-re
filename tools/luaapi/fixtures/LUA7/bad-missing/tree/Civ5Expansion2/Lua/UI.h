/// origin guess
#pragma once

struct lua_State;

template <class T> class CvLuaEvent {
public:
    int lAdd(lua_State* L);
};

/// lua class Base
class Base {
public:
    /// 0x08000030
    /// lua Base:Run()
    void Run();
};

/// 0x08000040
/// lua ?
int LuaThunk_Base_Run(lua_State* L);

/// 0x08000041

int LuaThunk_Base_Stop(lua_State* L);

/// lua table UI
class cvLuaUI {
public:
    /// 0x08000010
    
    static int lGetX(lua_State* L);
    /// 0x08000011
    // Not exposed to Lua.
    /// lua none
    static int lGetY(lua_State* L);
};
