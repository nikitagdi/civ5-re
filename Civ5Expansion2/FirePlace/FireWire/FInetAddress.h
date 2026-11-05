/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FInetAddress;
class in_addr;

/// size ?
/// vtable 0x0a304688
class FInetAddress {
public:
    /// 0x08d57a28
    FInetAddress();
    /// 0x08d57b48
    FInetAddress(FStringA const& a1);
    /// 0x08d5808c
    FInetAddress(in_addr a1);
    /// 0x08d5814c
    FInetAddress(unsigned int a1, unsigned short a2);
    /// 0x08d58214
    FInetAddress(FInetAddress const& a1);
    /// 0x08d58f0a
    /// deleting 0x08d582d4
    virtual ~FInetAddress();
    /// 0x08d57ac0
    UNK_RET operator=(unsigned long a1);
    /// 0x08d57d30
    UNK_RET SetAddress(FStringA const& a1);
    /// 0x08d582fc
    UNK_RET IsPrivateIPAddr(FStringA const& a1);
    /// 0x08d58340
    UNK_RET IsPrivateIPAddr(FInetAddress const& a1);
    /// 0x08d583b8
    UNK_RET GetAddressUINT() const;
    /// 0x08d583ca
    UNK_RET GetHostname() const;
    /// 0x08d584d8
    UNK_RET GetFullAddress() const;
    /// 0x08d58674
    UNK_RET GetIPAddress() const;
    /// 0x08d5870c
    UNK_RET GetPort() const;
    /// 0x08d58726
    UNK_RET GetAddress() const;
    /// 0x08d58746
    UNK_RET GetAddress(unsigned int a1) const;
    /// 0x08d5877c
    UNK_RET GetAddressUINT(unsigned int a1) const;
    /// 0x08d5879e
    UNK_RET SetIPAddress(FStringA const& a1);
    /// 0x08d587e6
    UNK_RET IsInetAddress() const;
    /// 0x08d58802
    UNK_RET IsEmpty() const;
    /// 0x08d58848
    UNK_RET operator==(FInetAddress const& a1) const;
    /// 0x08d588fe
    UNK_RET operator=(FStringA const& a1);
    /// 0x08d58a7e
    UNK_RET operator=(in_addr a1);
    /// 0x08d58b08
    UNK_RET operator=(FInetAddress const& a1);
    /// 0x08d58bac
    UNK_RET operator!=(FInetAddress const& a1) const;
};
