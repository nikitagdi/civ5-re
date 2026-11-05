/// origin guess
#pragma once
#include <steam.h>

class FSteamPacket;

/// size ?
class FSteamPacket {
public:
    /// 0x08d6bcda
    FSteamPacket(CSteamID a1);
    /// 0x08d6bcfe
    FSteamPacket(unsigned char const* a1, unsigned char const* a2, CSteamID const& a3);
    /// 0x08d6bd40
    FSteamPacket();
    /// 0x08d6bd68
    ~FSteamPacket();
    /// 0x08d6b660
    UNK_RET payload();
    /// 0x08d6b666
    UNK_RET payload() const;
    /// 0x08d6b66c
    UNK_RET steamID() const;
    /// 0x08d6bd98
    UNK_RET operator=(FSteamPacket const& a1);
    /// 0x08d6bdde
    UNK_RET steamID();
};
