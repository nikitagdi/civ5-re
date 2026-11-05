/// origin guess
#pragma once

/// size ?
class cvEventChannelManager {
public:
    /// 0x08b7a6ec
    cvEventChannelManager();
    /// 0x08b7a6f8
    ~cvEventChannelManager();
    /// 0x08b7a6fa
    UNK_RET ReserveChannel();
    /// 0x08b7a71e
    UNK_RET ReleaseChannel(unsigned int a1);
};
