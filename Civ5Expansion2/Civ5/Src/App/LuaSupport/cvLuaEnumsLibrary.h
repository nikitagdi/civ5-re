/// origin guess
#pragma once
#include <lua.h>

class cvLuaContentManagerEnums;
class cvLuaUIEnumsLibrary;

/// size ?
template <class T0>
class cvLuaEnumsLibrary {
public:
    /// 0x0876703a cvLuaEnumsLibrary<cvLuaContentManagerEnums>
    /// 0x086c6c9e cvLuaEnumsLibrary<cvLuaUIEnumsLibrary>
    static int pRegister(lua_State* a1);
    /// 0x087671de cvLuaEnumsLibrary<cvLuaContentManagerEnums>
    /// 0x0897a4f8 cvLuaEnumsLibrary<cvLuaUIEnumsLibrary>
    UNK_RET EnumStart(lua_State* a1, char const* a2);
};
