/// origin guess
#pragma once
#include "FirePlace/FireWire/FSocket.h"

class FBroadcastAddress;
class FInetAddress;
class FInetHostAddress;

/// size ?
/// vtable 0x0a304770
class FUDPSocket : public FSocket {
public:
    /// 0x08d5ab50
    FUDPSocket();
    /// 0x08d5abb8
    FUDPSocket(FInetAddress const& a1, unsigned short a2);
    /// 0x08d5b884
    /// deleting 0x08d5acfc
    virtual ~FUDPSocket();
    /// 0x08d5b8ce
    virtual UNK_RET Connect(FInetHostAddress const& a1, unsigned short a2, int a3);
    /// 0x08d5b8d4
    virtual UNK_RET Disconnect();
    /// 0x08d5adf8
    virtual UNK_RET GetPeer(unsigned short* a1);
    /// 0x08d5b916
    virtual UNK_RET Peek(unsigned char* a1, unsigned int a2);
    /// 0x08d5b8da
    virtual UNK_RET Receive(unsigned char* a1, unsigned int a2);
    /// 0x08d5ae88
    virtual UNK_RET Send(unsigned char const* a1, unsigned int a2);
    /// 0x08d5ad24
    virtual UNK_RET SetPeer(FInetHostAddress const& a1, unsigned short a2);
    /// 0x08d5ad8e
    virtual UNK_RET SetPeer(FBroadcastAddress const& a1, unsigned short a2);
    /// 0x08d5a5b2
    UNK_RET Receive(unsigned char* a1, unsigned int a2, FInetHostAddress* a3, unsigned short* a4);
};
