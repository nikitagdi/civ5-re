/// origin guess
#pragma once
#include "FirePlace/FireWorks/FNetLobbyIFace.h"
#include "FirePlace/FireWorks/FString.h"
#include <steam.h>

/// size ?
/// vtable 0x0a304b00
class FNetLobbySteamBase : public FNetLobbyIFace {
public:
    /// 0x08d4ae8a
    /// deleting 0x08d61052
    virtual ~FNetLobbySteamBase();
    /// 0x08d60914
    virtual UNK_RET ConnectionAttempt(long a1, unsigned char const* a2, unsigned int a3);
    /// pure
    virtual UNK_RET GetSteamID() const = 0;
    /// 0x08739a08
    virtual UNK_RET GetHostSteamID(long a1) const;
    /// 0x08739a1c
    virtual UNK_RET GetInviteID(CSteamID a1);
    /// 0x08739a22
    virtual UNK_RET GetRoomID(CSteamID a1) const;
    /// 0x08739a28
    virtual UNK_RET JoinServer(FStringA& a1);
};
