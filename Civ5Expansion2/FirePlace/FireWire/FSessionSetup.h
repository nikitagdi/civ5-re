/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FSessionSetup;

/// size ?
/// vtable 0x0a303fc0
class FSessionSetup {
public:
    /// 0x08d505c8
    FSessionSetup();
    /// 0x08d50618
    FSessionSetup(FSessionSetup const& a1);
    /// 0x08d508ea
    /// deleting 0x08d508c2
    virtual ~FSessionSetup();
    /// 0x08d506cc
    UNK_RET SetGameParam(unsigned int a1, unsigned int a2, FStringA const& a3);
    /// 0x08d507d4
    UNK_RET operator=(FSessionSetup const& a1);
    /// 0x08d5099a
    UNK_RET GetGameParamType(unsigned int a1) const;
    /// 0x08d509bc
    UNK_RET GetGameParamValue(unsigned int a1) const;
    /// 0x08d509f8
    UNK_RET GetGameParamName(unsigned int a1) const;
    /// 0x08d50aae
    UNK_RET GetGameParamValueAtIndex(unsigned int a1) const;
    /// 0x08d50ad2
    UNK_RET GetGameParamNameAtIndex(unsigned int a1) const;
};
