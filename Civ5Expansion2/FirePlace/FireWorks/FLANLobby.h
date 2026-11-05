/// origin guess
#pragma once
#include "FirePlace/FireWorks/FNetLobbyIFace.h"
#include "FirePlace/FireWorks/FString.h"

class FNetLobbyCallbacksIFace;
class FSessionSetup;
class LANServer_t;
class LobbyType;

/// size ?
/// vtable 0x0a303d50
class FLANLobby : public FNetLobbyIFace {
public:
    /// 0x08d4853c
    FLANLobby(int a1, FNetLobbyCallbacksIFace* a2, LobbyType a3);
    /// 0x08d48632
    /// deleting 0x08d4860a
    virtual ~FLANLobby();
    /// 0x08d490e8
    virtual UNK_RET BroadcastGame(FStringA const& a1, int a2, bool a3);
    /// 0x08d49218
    virtual UNK_RET BrowseGames(FSessionSetup const* a1, int a2);
    /// 0x08d49b02
    virtual UNK_RET ConnectionAttempt(long a1, unsigned char const* a2, unsigned int a3);
    /// 0x08d499f8
    virtual UNK_RET GetCurrentGameMaxPlayers() const;
    /// 0x08d4957c
    virtual UNK_RET GetCurrentGameName() const;
    /// 0x08d495d0
    virtual UNK_RET GetNumServers() const;
    /// 0x08d495c8
    virtual UNK_RET GetRoomID() const;
    /// 0x08d49a50
    virtual UNK_RET GetServerCustomString(long a1, FStringA const& a2) const;
    /// 0x08d4966a
    virtual UNK_RET GetServerGameName(long a1) const;
    /// 0x08d495e4
    virtual UNK_RET GetServerIDAtIndex(int a1) const;
    /// 0x08d49738
    virtual UNK_RET GetServerMaxPlayers(long a1) const;
    /// 0x08d49708
    virtual UNK_RET GetServerNumPlayers(long a1) const;
    /// 0x08d4976c
    virtual UNK_RET GetServerPrivateAddress(long a1) const;
    /// 0x08d4960a
    virtual UNK_RET GetServerType(long a1) const;
    /// 0x08d486b0
    virtual UNK_RET Init();
    /// 0x08d49400
    virtual UNK_RET JoinRoom(long a1);
    /// 0x08d4953e
    virtual UNK_RET LeaveRoom();
    /// 0x08d4980a
    virtual UNK_RET MigrateHost(bool a1);
    /// 0x08d49a00
    virtual UNK_RET RegisterServerStringKey(FStringA const& a1, FStringA const& a2);
    /// 0x08d499ec
    virtual UNK_RET SetCurrentGameMaxPlayers(int a1);
    /// 0x08d4907e
    virtual UNK_RET Shutdown();
    /// 0x08d49576
    virtual UNK_RET StartGame();
    /// 0x08d493a0
    virtual UNK_RET StopBrowsing();
    /// 0x08d486e8
    virtual UNK_RET Update();
    /// 0x08d48fd8
    UNK_RET AddServer(LANServer_t const& a1);
    /// 0x08d490a6
    UNK_RET StopBroadcasting();
    /// 0x08d4933a
    UNK_RET ClearServerList();
    /// 0x08d49510
    UNK_RET GetServer(long a1);
    /// 0x08d4963c
    UNK_RET GetServer(long a1) const;
};
