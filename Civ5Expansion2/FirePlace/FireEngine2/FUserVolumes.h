/// origin binary
#pragma once

class FGameIniParser;

/// size ?
class FUserVolumes {
public:
    /// 0x08cdf97a
    FUserVolumes();
    /// 0x08cdf484
    UNK_RET RegisterUserVolume(char const* a1, char const* a2);
    /// 0x08cdf56c
    UNK_RET LoadAllVolumes(FGameIniParser* a1);
    /// 0x08cdf5e6
    UNK_RET LoadVolume(FGameIniParser* a1, char const* a2, char const* a3);
    /// 0x08cdf81e
    UNK_RET SaveAllVolumes(FGameIniParser* a1);
    /// 0x08cdf898
    UNK_RET SaveVolume(FGameIniParser* a1, char const* a2, char const* a3);
    /// 0x0d27acf8
    static UNK_DATA m_pInst[0x4];
};
