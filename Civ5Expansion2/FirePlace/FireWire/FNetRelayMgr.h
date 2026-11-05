/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"

class FNetMessage;

/// size ?
class FNetRelayMgr {
public:
    /// 0x08d4f754
    FNetRelayMgr(int a1);
    /// 0x08d4f782
    ~FNetRelayMgr();
    /// 0x08d4f796
    UNK_RET AddRelay(int a1, int a2);
    /// 0x08d4f83a
    UNK_RET GetSession() const;
    /// 0x08d4f86a
    UNK_RET AddRelayTarget(int a1, int a2);
    /// 0x08d4f8e6
    UNK_RET IsRelayTarget(int a1, int a2) const;
    /// 0x08d4f952
    UNK_RET RemoveRelays(int a1);
    /// 0x08d4fac6
    UNK_RET UseRelay(int a1) const;
    /// 0x08d4fb1c
    UNK_RET IsRelayed(int a1) const;
    /// 0x08d4fb64
    UNK_RET CheckRelaySend(FNetMessage& a1, int a2) const;
    /// 0x08d4fbf0
    UNK_RET CheckRelaySend(FDataStream& a1, int a2) const;
    /// 0x08d4fcc4
    UNK_RET RelayMessage(FNetMessage& a1, int a2, bool a3);
    /// 0x08d4fd68
    UNK_RET ProcessRelayedMessage(FNetMessage& a1);
};
