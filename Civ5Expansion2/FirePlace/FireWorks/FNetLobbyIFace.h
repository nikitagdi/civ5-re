/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FNetLobbyCallbacksIFace;
class FSessionSetup;
class LobbyType;

/// size ?
/// vtable 0x0a303e60
class FNetLobbyIFace {
public:
    /// 0x08d4ae10
    FNetLobbyIFace(int a1, FNetLobbyCallbacksIFace* a2, LobbyType a3);
    /// 0x08d4ae8a
    /// deleting 0x08d4ae50
    virtual ~FNetLobbyIFace() = 0;
    /// 0x08739326
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08d4afe6
    virtual UNK_RET Init();
    /// 0x08d4afec
    virtual UNK_RET Update();
    /// 0x08d4afee
    virtual UNK_RET Shutdown();
    /// 0x08d4aff0
    virtual UNK_RET ConnectionAttempt(long a1, unsigned char const* a2, unsigned int a3);
    /// 0x08739854
    virtual UNK_RET ConnectionAccepted(long a1, unsigned char const* a2, unsigned int a3);
    /// 0x08739332
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08739334
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x0873a2ce
    virtual UNK_RET ConnectionClosed(long a1);
    /// 0x08739336
    virtual UNK_RET vf_11(UNK_PARAMS);
    /// 0x0873933c
    virtual UNK_RET vf_12(UNK_PARAMS);
    /// 0x08739342
    virtual UNK_RET vf_13(UNK_PARAMS);
    /// 0x0873985a
    virtual UNK_RET IsLoggedIn() const;
    /// pure
    virtual UNK_RET BroadcastGame(FStringA const& a1, int a2, bool a3) = 0;
    /// pure
    virtual UNK_RET BrowseGames(FSessionSetup const* a1, int a2) = 0;
    /// 0x08739860
    virtual UNK_RET AcceptInvitation(long a1);
    /// 0x08739866
    virtual UNK_RET StopBrowsing();
    /// 0x08739868
    virtual UNK_RET IsBrowsing() const;
    /// 0x08d4aeee
    virtual UNK_RET UpdateGameDetails();
    /// pure
    virtual UNK_RET JoinRoom(long a1) = 0;
    /// pure
    virtual UNK_RET LeaveRoom() = 0;
    /// 0x08d4aff6
    virtual UNK_RET GetCurrentGameName() const;
    /// pure
    virtual UNK_RET GetRoomID() const = 0;
    /// pure
    virtual UNK_RET SetCurrentGameMaxPlayers(int a1) = 0;
    /// pure
    virtual UNK_RET GetCurrentGameMaxPlayers() const = 0;
    /// 0x08739344
    virtual UNK_RET vf_27(UNK_PARAMS);
    /// 0x0873986c
    virtual UNK_RET StartGame();
    /// 0x0873934a
    virtual UNK_RET vf_29(UNK_PARAMS);
    /// pure
    virtual UNK_RET GetNumServers() const = 0;
    /// pure
    virtual UNK_RET GetServerIDAtIndex(int a1) const = 0;
    /// pure
    virtual UNK_RET GetServerGameName(long a1) const = 0;
    /// 0x0873a3b8
    virtual UNK_RET GetServerLocked(long a1) const;
    /// 0x0873a3bc
    virtual UNK_RET GetServerPing(long a1) const;
    /// pure
    virtual UNK_RET GetServerNumPlayers(long a1) const = 0;
    /// pure
    virtual UNK_RET GetServerMaxPlayers(long a1) const = 0;
    /// 0x0873a3c0
    virtual UNK_RET GetServerPrivateSlots(long a1) const;
    /// 0x08739350
    virtual UNK_RET vf_38(UNK_PARAMS);
    /// 0x0873a3c4
    virtual UNK_RET GetServerCustomInt(long a1, FStringA const& a2) const;
    /// 0x08d4b040
    virtual UNK_RET GetServerCustomString(long a1, FStringA const& a2) const;
    /// 0x08739354
    virtual UNK_RET vf_41(UNK_PARAMS);
    /// 0x0873939e
    virtual UNK_RET vf_42(UNK_PARAMS);
    /// 0x087393a2
    virtual UNK_RET vf_43(UNK_PARAMS);
    /// 0x087393ec
    virtual UNK_RET vf_44(UNK_PARAMS);
    /// 0x0873a3c8
    virtual UNK_RET RegisterServerIntKey(FStringA const& a1, int a2);
    /// pure
    virtual UNK_RET RegisterServerStringKey(FStringA const& a1, FStringA const& a2) = 0;
    /// 0x087393f0
    virtual UNK_RET vf_47(UNK_PARAMS);
    /// 0x087393f2
    virtual UNK_RET vf_48(UNK_PARAMS);
    /// 0x087393f4
    virtual UNK_RET vf_49(UNK_PARAMS);
    /// 0x087393f6
    virtual UNK_RET vf_50(UNK_PARAMS);
    /// 0x087393f8
    virtual UNK_RET vf_51(UNK_PARAMS);
    /// 0x087393fa
    virtual UNK_RET vf_52(UNK_PARAMS);
    /// 0x0873a3ca
    virtual UNK_RET GetServerPublicAddress(long a1) const;
    /// 0x08d4b08a
    virtual UNK_RET GetServerPrivateAddress(long a1) const;
    /// 0x0873a414
    virtual UNK_RET GetServerTransportType(long a1) const;
    /// pure
    virtual UNK_RET GetServerType(long a1) const = 0;
    /// 0x0873944a
    virtual UNK_RET vf_57(UNK_PARAMS);
    /// 0x0873944e
    virtual UNK_RET vf_58(UNK_PARAMS);
    /// 0x08d4b0d4
    virtual UNK_RET MigrateHost(bool a1);
    /// 0x08d4aebe
    UNK_RET GetSession() const;
};
