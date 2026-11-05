/// origin guess
#pragma once
#include <steam.h>

/// size ?
class SteamServerClientList {
public:
    /// 0x08d5e9a2
    SteamServerClientList();
    /// 0x08d5e9b4
    ~SteamServerClientList();
    /// 0x08d5ea06
    UNK_RET Add(long a1, CSteamID a2, unsigned char const* a3, unsigned int a4);
    /// 0x08d5eadc
    UNK_RET Find(long a1) const;
    /// 0x08d5eafe
    UNK_RET Remove(long a1);
    /// 0x08d5eb6e
    UNK_RET Remove(CSteamID a1);
    /// 0x08d5ebf2
    UNK_RET Find(CSteamID a1) const;
};

namespace Protocol {
/// 0x08d61578
UNK_RET setTimeoutCheck(bool a1);
}

namespace ServerKeyMap {
/// 0x08d6107c
UNK_RET Add(char const* a1, char const* a2);
/// 0x08d6117c
UNK_RET FindValue(char const* a1);
/// 0x08d6123a
UNK_RET FindKey(char const* a1);
}
