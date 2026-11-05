/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"
#include <steam.h>

class FPendingConnection;

/// size ?
class FPendingConnQueue {
public:
    /// 0x08d65c56
    ~FPendingConnQueue();
    /// 0x08d65cae
    UNK_RET Pop(bool a1);
    /// 0x08d65d30
    UNK_RET AddConnection(int a1, bool a2, FStringA const& a3, FStringA const& a4, char const* a5, unsigned int a6);
    /// 0x08d65f54
    UNK_RET AddConnection(int a1, bool a2, CSteamID a3, unsigned long long a4, char const* a5, unsigned int a6);
    /// 0x08d6601c
    UNK_RET AddHostConnection(FStringA const& a1, char const* a2, unsigned int a3);
    /// 0x08d66216
    UNK_RET AddHostConnection(CSteamID a1, unsigned long long a2, char const* a3, unsigned int a4);
    /// 0x08d66284
    UNK_RET ConnectionFailureCallback(long a1);
    /// 0x08d662ea
    UNK_RET Front() const;
    /// 0x08d662fc
    UNK_RET Update();
    /// 0x08d663be
    UNK_RET AttemptPublicAddress(FPendingConnection* a1, bool a2);
    /// 0x08d6652e
    UNK_RET AttemptPrivateAddress(FPendingConnection* a1);
    /// 0x08d6668c
    UNK_RET ReportFailure();
    /// 0x08d66732
    UNK_RET AttemptSteamID(FPendingConnection* a1);
};
