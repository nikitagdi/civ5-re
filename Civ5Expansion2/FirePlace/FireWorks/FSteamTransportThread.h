/// origin guess
#pragma once
#include "FirePlace/FireWorks/FThread.h"
#include <steam.h>

/// size ?
/// vtable 0x0a304fd0
class FSteamTransportThread : public FThread {
public:
    class SourceType;
    /// 0x08d6ab84
    FSteamTransportThread(FSteamTransportThread::SourceType a1);
    /// 0x08d6ad3e
    /// deleting 0x08d6ad16
    virtual ~FSteamTransportThread();
    /// 0x08d6ae22
    virtual UNK_RET Run();
    /// 0x08d6b736
    UNK_RET getPacketQueue();
    /// 0x08d6b7dc
    UNK_RET send(CSteamID a1, unsigned char const* a2, unsigned int a3);
    /// 0x08d6b91c
    UNK_RET addConnection(CSteamID a1);
    /// 0x08d6b9a6
    UNK_RET removeConnection(CSteamID a1);
    /// 0x08d6ba12
    UNK_RET timeouts() const;
    /// 0x08d6bbe6
    UNK_RET EnablePings(bool a1);
    /// 0x08d6bc00
    UNK_RET SetPingFrequency(unsigned int a1);
    /// 0x08d6bc1a
    UNK_RET GetPingTime(CSteamID a1, unsigned int* a2) const;
    /// 0x08d6bc98
    UNK_RET CheckPingTimes();
    /// 0x08d6bde6
    UNK_RET RunSynchronous();
};
