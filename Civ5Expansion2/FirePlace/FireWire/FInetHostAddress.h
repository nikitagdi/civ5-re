/// origin guess
#pragma once
#include "FirePlace/FireWire/FInetAddress.h"
#include "FirePlace/FireWorks/FString.h"

class FInetHostAddress;
class FInetMaskAddress;
class in_addr;

/// size ?
/// vtable 0x0a3046b0
class FInetHostAddress : public FInetAddress {
public:
    /// 0x08d58c56
    FInetHostAddress(in_addr a1);
    /// 0x08d58c94
    FInetHostAddress(FStringA a1);
    /// absent
    virtual ~FInetHostAddress();
    /// 0x08d58dc4
    UNK_RET operator&=(FInetMaskAddress const& a1);
};

/// 0x08d58e78
UNK_RET operator&(FInetHostAddress const& a1, FInetMaskAddress const& a2);
