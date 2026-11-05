/// origin binary
#pragma once
#include "FirePlace/FireWorks/FString.h"

/// size ?
class FCSVFile {
public:
    /// 0x08d87ed4
    FCSVFile(bool a1, bool a2, bool a3);
    /// 0x08d87f18
    ~FCSVFile();
    /// 0x08d87f2c
    UNK_RET Clear();
    /// 0x08d87f76
    UNK_RET DeallocateStringIndex();
    /// 0x08d87fb8
    UNK_RET Initialize(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d88040
    UNK_RET AllocateStringIndex();
    /// 0x08d880f2
    char const* Get(unsigned int a1, unsigned int a2) const;
    /// 0x08d88160
    UNK_RET GetStringIndex(unsigned int a1, unsigned int a2) const;
    /// 0x08d881a2
    bool GetFloat(unsigned int a1, unsigned int a2, float& a3) const;
    /// 0x08d88206
    bool GetInt(unsigned int a1, unsigned int a2, int& a3) const;
    /// 0x08d8826a
    UNK_RET Set(unsigned int a1, unsigned int a2, char const* a3);
    /// 0x08d8847a
    UNK_RET ReallocateStringIndex(unsigned int a1, unsigned int a2);
    /// 0x08d88574
    UNK_RET SetStringIndex(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d885b6
    UNK_RET GrowFileText(unsigned int a1);
    /// 0x08d88844
    UNK_RET Set(unsigned int a1, unsigned int a2, float a3);
    /// 0x08d888ac
    UNK_RET Set(unsigned int a1, unsigned int a2, int a3);
    /// 0x08d88910
    UNK_RET Set(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d88974
    UNK_RET Load(char const* a1);
    /// 0x08d88a36
    UNK_RET LoadFromString(FStringA const& a1, char a2);
    /// 0x08d88d7a
    UNK_RET Save(char const* a1) const;
    /// 0x08d88fb2
    UNK_RET GetIndex(unsigned int a1, unsigned int a2) const;
};
