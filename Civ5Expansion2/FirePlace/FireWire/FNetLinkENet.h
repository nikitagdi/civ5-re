/// origin guess
#pragma once
#include <steam.h>

class FInetAddress;

/// size ?
class FNetLinkENet {
public:
    /// 0x08d57090
    FNetLinkENet(long a1);
    /// 0x08d57108
    UNK_RET ResetLink(long a1);
    /// 0x08d57214
    UNK_RET SetAddress(FInetAddress const& a1, bool a2);
    /// 0x08d57274
    UNK_RET SetSteamID(CSteamID a1, bool a2);
};
