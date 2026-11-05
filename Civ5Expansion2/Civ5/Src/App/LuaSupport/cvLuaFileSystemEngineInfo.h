/// origin guess
#pragma once
#include "Civ5/Src/App/LuaSupport/cvLuaEngineInfo.h"
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a2ffd90
class cvLuaFileSystemEngineInfo : public cvLuaEngineInfo {
public:
    /// 0x089f61a0
    cvLuaFileSystemEngineInfo();
    /// 0x089f61d8
    virtual UNK_RET GetDataRow(unsigned int a1, int a2, FStringA& a3, unsigned int a4);
    /// 0x089f6298
    virtual UNK_RET vf_01(UNK_PARAMS);
};
