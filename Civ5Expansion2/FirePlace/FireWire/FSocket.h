/// origin guess
#pragma once

class FInetHostAddress;
class FSocket;

/// size ?
/// vtable 0x0a304740
class FSocket {
public:
    class Pending;
    class State;
    /// 0x08d5a07c
    FSocket(int a1, int a2, int a3);
    /// 0x08d5a152
    FSocket(unsigned int a1);
    /// 0x08d5a1e4
    FSocket(FSocket const& a1);
    /// 0x08d5b884
    /// deleting 0x08d5a3c0
    virtual ~FSocket();
    /// 0x08d5a472
    virtual UNK_RET GetPeer(unsigned short* a1);
    /// pure
    virtual UNK_RET Connect(FInetHostAddress const& a1, unsigned short a2, int a3) = 0;
    /// pure
    virtual UNK_RET Disconnect() = 0;
    /// pure
    virtual UNK_RET Send(unsigned char const* a1, unsigned int a2) = 0;
    /// pure
    virtual UNK_RET Receive(unsigned char* a1, unsigned int a2) = 0;
    /// pure
    virtual UNK_RET Peek(unsigned char* a1, unsigned int a2) = 0;
    /// 0x08d5a124
    UNK_RET Init();
    /// 0x08d5a284
    UNK_RET DupSocket(unsigned int a1, FSocket::State a2);
    /// 0x08d5a2e2
    UNK_RET operator=(FSocket const& a1);
    /// 0x08d5a33a
    UNK_RET Cleanup();
    /// 0x08d5a3e8
    UNK_RET GetLocal(unsigned short* a1) const;
    /// 0x08d5a4fc
    UNK_RET GetSender(unsigned short* a1) const;
    /// 0x08d5a690
    UNK_RET SetBlocking(bool a1);
    /// 0x08d5a6ee
    UNK_RET SetBroadcast(bool a1);
    /// 0x08d5a75a
    UNK_RET SetKeepAlive(bool a1);
    /// 0x08d5a7c4
    UNK_RET SetLinger(bool a1);
    /// 0x08d5a7de
    UNK_RET SetNoDelay(bool a1);
    /// 0x08d5a836
    UNK_RET SetSendBufSize(int a1);
    /// 0x08d5a8b0
    UNK_RET SetRecvBufSize(int a1);
    /// 0x08d5a92a
    UNK_RET SetRouting(bool a1);
    /// 0x08d5a994
    UNK_RET IsActive() const;
    /// 0x08d5a9a4
    UNK_RET IsConnected() const;
    /// 0x08d5a9b4
    UNK_RET IsPending(FSocket::Pending a1, unsigned long a2, int* a3);
    /// 0x08d5aadc
    UNK_RET ConnectError();
};
