/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a305388
class FNetStatRecord {
public:
    /// 0x08d76870
    FNetStatRecord(int a1);
    /// 0x08d76912
    /// deleting 0x08d768ea
    virtual ~FNetStatRecord();
    /// 0x08d769be
    UNK_RET SetPlatformID(unsigned int a1, unsigned char* a2);
    /// 0x08d76a10
    UNK_RET SetColumnValue(int a1, int a2);
    /// 0x08d76a94
    UNK_RET SetColumnValue(int a1, double a2);
    /// 0x08d76b1c
    UNK_RET SetColumnValue(int a1, FStringA const& a2);
    /// 0x08d76b9a
    UNK_RET GetIntValue(int a1) const;
    /// 0x08d76bf6
    UNK_RET GetStatField(int a1) const;
    /// 0x08d76c42
    UNK_RET GetFloatValue(int a1) const;
    /// 0x08d76cae
    UNK_RET GetStringValue(int a1) const;
    /// 0x08d76d90
    UNK_RET GetNumFields() const;
};
