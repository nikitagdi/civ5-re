/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a305378
class FNetStatField {
public:
    /// 0x08d76660
    FNetStatField(int a1);
    /// 0x08d766cc
    /// deleting 0x08d76698
    virtual ~FNetStatField();
    /// 0x08d766f0
    UNK_RET ResetValue();
    /// 0x08d7673a
    UNK_RET SetValue(int a1);
    /// 0x08d7676e
    UNK_RET SetValue(double a1);
    /// 0x08d767a2
    UNK_RET SetValue(FStringA const& a1);
    /// 0x08d76818
    UNK_RET GetIntValue() const;
    /// 0x08d7682c
    UNK_RET GetFloatValue() const;
    /// 0x08d76860
    UNK_RET GetStringValue() const;
};
