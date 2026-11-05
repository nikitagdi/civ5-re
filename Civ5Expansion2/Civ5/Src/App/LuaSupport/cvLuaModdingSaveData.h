/// origin guess
#pragma once
#include <lua.h>

/// size ?
/// vtable 0x0a2ffd58
class cvLuaModdingSaveData {
public:
    /// 0x089ebcf4
    cvLuaModdingSaveData();
    /// 0x089ebd5a
    /// deleting 0x089ebd24
    virtual ~cvLuaModdingSaveData();
    /// 0x089ebd8a
    UNK_RET GetDatabase();
    /// 0x089ebd92
    static int OpenSaveData(lua_State* a1);
    /// 0x089ebf80
    UNK_RET PushMethods(lua_State* a1, int a2);
    /// 0x089ec140
    /// lua ?
    static int lDestroy(lua_State* a1);
    /// 0x089ec172
    /// lua ?
    static int lGetValue(lua_State* a1);
    /// 0x089ec34a
    /// lua ?
    static int lSetValue(lua_State* a1);
    /// 0x089ec558
    /// lua ?
    static int lQuery(lua_State* a1);
    /// 0x089ec6e2
    /// lua ?
    static int lIterateQuery(lua_State* a1);
};
