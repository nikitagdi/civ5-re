/// origin guess
#pragma once
#include "FirePlace/FireWire/FSocket.h"

class FInetHostAddress;
class FTCPConnection;
class FTCPSocket;

/// size ?
/// vtable 0x0a3047a0
class FTCPConnection : public FSocket {
public:
    /// 0x08d5b098
    FTCPConnection();
    /// 0x08d5b0e6
    FTCPConnection(FInetHostAddress const& a1, unsigned short a2, int a3, unsigned long a4);
    /// 0x08d5b19a
    FTCPConnection(FTCPSocket& a1, int a2, unsigned long a3);
    /// 0x08d5b304
    FTCPConnection(FTCPConnection const& a1);
    /// absent
    virtual ~FTCPConnection();
    /// 0x08d5b352
    virtual UNK_RET Connect(FInetHostAddress const& a1, unsigned short a2, int a3);
    /// 0x08d5b620
    virtual UNK_RET Disconnect();
    /// 0x08d5b7a2
    virtual UNK_RET Peek(unsigned char* a1, unsigned int a2);
    /// 0x08d5b71a
    virtual UNK_RET Receive(unsigned char* a1, unsigned int a2);
    /// 0x08d5b67e
    virtual UNK_RET Send(unsigned char const* a1, unsigned int a2);
    /// 0x08d5b23e
    UNK_RET Accept(FTCPSocket& a1, int a2);
};
