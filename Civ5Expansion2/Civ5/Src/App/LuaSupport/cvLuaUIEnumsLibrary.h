/// origin guess
#pragma once
#include <Windows.h>
#include <lua.h>

class FGXVector4;

/// size ?
class cvLuaUIEnumsLibrary {
public:
    /// 0x08976188
    UNK_RET GetLibraryName();
    /// 0x0897619c
    static int RegisterEnums(lua_State* a1);
};

/// 0x0897a590
UNK_RET lua_tocolor(lua_State* a1, int a2);
/// 0x0897a69b
UNK_RET lua_pushcolor(lua_State* a1, FGXVector4 const& a2);
/// 0x0897a787
UNK_RET lua_pushFILETIME(lua_State* a1, _FILETIME const& a2);
/// 0x0897a7c4
UNK_RET lua_pushSYSTEMTIME(lua_State* a1, _SYSTEMTIME const& a2);
/// 0x0897a978
UNK_RET luaL_optbool(lua_State* a1, int a2, bool a3);
/// 0x0897aff4
/// lua ?
int LuaThunk_SetUnitActionCodeDebug(lua_State* a1);
/// 0x0897b0d5
UNK_RET SetUnitActionCodeDebug(int a1, int a2, float a3);
/// 0x0897b149
int LuaToString_SetUnitActionCodeDebug(lua_State* a1);

namespace cvLuaUtils {
/// 0x0897a9c4
UNK_RET ClearTable(lua_State* a1, int a2);
/// 0x0897aa54
UNK_RET CopyFields(lua_State* a1, int a2, char const** a3);
/// 0x0897aab4
UNK_RET CopyAllFields(lua_State* a1, int a2);
/// 0x0897ab48
int CreateReadOnlyProxy(lua_State* a1);
/// 0x0897ac3c
/// lua ?
int lReadOnlyError(lua_State* a1);
/// 0x0897ac6a
UNK_RET ExposeLibraryToThread(lua_State* a1, lua_State* a2, char const* a3, int (*a4)(lua_State*));
/// 0x0897acb8
int pExposeLibraryToThread(lua_State* a1);
/// 0x0897ada8
UNK_RET PushThreadAndEnvironment(lua_State* a1, lua_State* a2);
/// 0x0897ae48
int DumpCallStack(lua_State* a1);
}
