/// origin guess
#pragma once
#include <unordered_map>

class FNetPlatformDetailsSteam;
class FNetPlayer;
class KickReason;

/// size ?
class FNetPlayerManager {
public:
    /// 0x08d4d450
    FNetPlayerManager(int a1);
    /// 0x08d4d4c4
    ~FNetPlayerManager();
    /// 0x08d4d534
    UNK_RET Update();
    /// 0x08d4d6f8
    UNK_RET GetSession() const;
    /// 0x08d4d728
    UNK_RET GetPlayer(int a1) const;
    /// 0x08d4d774
    UNK_RET IsAlive(int a1) const;
    /// 0x08d4d83a
    UNK_RET IsHost() const;
    /// 0x08d4d894
    UNK_RET KickPlayer(int a1, KickReason a2);
    /// 0x08d4d960
    UNK_RET CreateNetPlayer(long a1, FNetPlatformDetailsSteam const* a2, int a3);
    /// 0x08d4ddc2
    UNK_RET GetNumPlayers() const;
    /// 0x08d4ddca
    UNK_RET CreateLocalPlayer(int a1);
    /// 0x08d4de30
    UNK_RET RemoveNetPlayer(int a1);
    /// 0x08d4e15c
    UNK_RET GetPlayer(long a1) const;
    /// 0x08d4e1a8
    UNK_RET EnumeratePlayers(std::__hash_map_iterator<std::__hash_iterator<std::__hash_node<std::__hash_value_type<int, FNetPlayer*>, void*>*> >& a1, std::__hash_map_iterator<std::__hash_iterator<std::__hash_node<std::__hash_value_type<int, FNetPlayer*>, void*>*> >& a2);
    /// 0x08d4e1c0
    UNK_RET AreAllRegistered() const;
    /// 0x08d4e1e8
    UNK_RET AreAllValidated() const;
    /// 0x08d4e238
    UNK_RET AreAllCheckedIn(unsigned int a1) const;
    /// 0x08d4e28e
    UNK_RET SetKeepAliveTime(float a1);
    /// 0x08d4e29a
    UNK_RET MigrateHost(int a1);
    /// 0x08d4e3a4
    UNK_RET CompressPlayers();
    /// 0x08d4e488
    UNK_RET SwapPlayers(int a1, int a2);
    /// 0x08d4e5f2
    UNK_RET ChangePlayerID(int a1, int a2);
    /// 0x08d4e714
    UNK_RET IsResponsive(int a1) const;
};
