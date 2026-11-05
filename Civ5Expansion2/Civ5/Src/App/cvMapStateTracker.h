/// origin guess
#pragma once
#include <lua.h>

class FGXInt2;
class HEXVec_i;

/// size ?
class cvMapStateTracker {
public:
    /// 0x0893f73c
    cvMapStateTracker();
    /// 0x0893f790
    ~cvMapStateTracker();
    /// 0x0893f7b6
    UNK_RET SetAll(unsigned char a1);
    /// 0x0893f7f0
    UNK_RET Set(HEXVec_i const& a1, unsigned char a2);
    /// 0x0893f84c
    UNK_RET Startup(FGXInt2 const& a1, unsigned char a2);
    /// 0x0893f906
    UNK_RET Shutdown();
    /// 0x0893f908
    UNK_RET StartupGame(FGXInt2 const& a1, unsigned char a2);
    /// 0x0893f9c2
    UNK_RET ShutdownGame();
};

namespace cvFLuaLibrary {
/// 0x0893fb48
int Register(lua_State* a1);
/// 0x0893fb86
int pRegister(lua_State* a1);
/// 0x0893fc5a
int pCreateFLuaData(lua_State* a1);
/// 0x0893fd90
int pRegisterFLuaExposures(lua_State* a1);
}

namespace cvLuaArtInfoLibrary {
/// 0x0893feec
int Register(lua_State* a1);
/// 0x0893ff2a
int pRegister(lua_State* a1);
/// 0x0893ffc4
UNK_RET RegisterListener();
/// 0x08940002
int pRegisterListener(lua_State* a1);
}
