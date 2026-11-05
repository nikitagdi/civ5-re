/// origin guess
#pragma once

class FNetPlatformDetailsSteam;

/// size ?
class FNetPlayer {
public:
    /// 0x08d4d274
    FNetPlayer(long a1, int a2, float a3);
    /// 0x08d4d2ba
    ~FNetPlayer();
    /// 0x08d4d2ea
    UNK_RET IsCheckedIn(unsigned int a1) const;
    /// 0x08d4d2fc
    UNK_RET GetLink();
    /// 0x08d4d330
    UNK_RET GetLink() const;
    /// 0x08d4d364
    UNK_RET SetPlatformDetails(FNetPlatformDetailsSteam const* a1);
    /// 0x08d4d3d2
    UNK_RET SetName(char const* a1);
    /// 0x08d4d402
    UNK_RET GetName() const;
    /// 0x08d4d424
    UNK_RET ShowDetails(int a1) const;
    /// 0x08d4d426
    UNK_RET GetPlayerID() const;
    /// 0x08d4d42e
    UNK_RET SetPlayerID(int a1);
    /// 0x08d4d43a
    UNK_RET MessagingSuspended(bool a1);
    /// 0x08d4d446
    UNK_RET MessagingSuspended() const;
};
