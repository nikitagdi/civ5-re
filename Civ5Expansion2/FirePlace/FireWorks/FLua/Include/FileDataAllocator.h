/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFile.h"
#include <lua.h>

namespace FLua {
/// size ?
/// vtable 0x0a306b0c
class FileDataAllocator : public FIFile::Allocator {
public:
    /// 0x08dd98bc
    virtual byte * Allocate(unsigned int a1);
    /// 0x08dd9818
    virtual void Delete(unsigned char* a1);
    /// 0x100b2a50
    static UNK_DATA m_GlobalBuffer[0x100000];
    /// 0x0a313100
    static UNK_DATA m_gpGlobalBuffer[0x4];
};
}

/// 0x08dd9ad5
/// lua ?
int LuaThunk_TypeInfo(lua_State* a1);
/// 0x08dd9bcc
int LuaToString_TypeInfo(lua_State* a1);
/// 0x08dd9eed
/// lua ?
int LuaThunk_IsNull(lua_State* a1);
/// 0x08dd9fb9
int LuaToString_IsNull(lua_State* a1);
extern "C" {
    /// 0x101b2a68
    extern UNK_DATA FLuaCallStackBuffer[0x1000];
    /// 0x101b3a68
    extern UNK_DATA FLuaCallStackSaveActive[0x1];
    /// 0x0a318c7c
    extern UNK_DATA _g_ByteAlign[0x4];
    /// 0x0a318c8c
    extern UNK_DATA _g_LODInterpRate[0x4];
    /// 0x0a318c80
    extern UNK_DATA _g_PSystemGravity[0xc];
    /// 0x0a318ca0
    extern UNK_DATA _g_aDefaultBirthCB[0x30];
    /// 0x0a318c9c
    extern UNK_DATA _g_bLOD[0x4];
    /// 0x0a318c98
    extern UNK_DATA _g_fCoeffLOD[0x4];
    /// 0x0a318c94
    extern UNK_DATA _g_fFLOD[0x4];
    /// 0x0a318c90
    extern UNK_DATA _g_fILOD[0x4];
    /// 0x0a318cd0
    extern UNK_DATA _g_nThreadPriority[0x4];
}
