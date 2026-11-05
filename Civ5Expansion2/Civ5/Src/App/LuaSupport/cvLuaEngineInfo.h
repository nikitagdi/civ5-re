/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a3007a0
class cvLuaEngineInfo {
public:
    /// 0x08b77890
    cvLuaEngineInfo(char const* a1);
    /// 0x089f6268
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08b779dc
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08b77962
    virtual UNK_RET GetDataRow(char const* a1, int a2, FStringA& a3, unsigned int a4);
    /// 0x08b779d8
    virtual UNK_RET GetDataRow(unsigned int a1, int a2, FStringA& a3, unsigned int a4);
};
