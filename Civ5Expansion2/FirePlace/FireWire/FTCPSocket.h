/// origin guess
#pragma once
#include "FirePlace/FireWire/FSocket.h"

class FInetAddress;
class FInetHostAddress;

/// size ?
/// vtable 0x0a3047f0
class FTCPSocket : public FSocket {
public:
    /// 0x08d5aed6
    FTCPSocket(FInetAddress const& a1, unsigned short a2, int a3);
    /// absent
    virtual ~FTCPSocket();
    /// 0x08d5b9a2
    virtual UNK_RET Connect(FInetHostAddress const& a1, unsigned short a2, int a3);
    /// 0x08d5b9a8
    virtual UNK_RET Disconnect();
    /// 0x08d5b9b6
    virtual UNK_RET Peek(unsigned char* a1, unsigned int a2);
    /// 0x08d5b9b2
    virtual UNK_RET Receive(unsigned char* a1, unsigned int a2);
    /// 0x08d5b9ae
    virtual UNK_RET Send(unsigned char const* a1, unsigned int a2);
    /// 0x08d5b9ba
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08d5b048
    UNK_RET Reject();
};
