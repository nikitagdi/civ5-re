/// origin guess
#pragma once
#include "FirePlace/FireWorks/FNetLobbySteamBase.h"
#include "FirePlace/FireWorks/FString.h"

class FNetLobbyCallbacksIFace;
class FNetSteamServerInitParams;
class FSessionSetup;
class GSPolicyResponse_t;
class LobbyType;
class SteamServerConnectFailure_t;
class SteamServersConnected_t;
class SteamServersDisconnected_t;
class ValidateAuthTicketResponse_t;

/// size ?
/// vtable 0x0a304870
class FNetSteamServer : public FNetLobbySteamBase {
public:
    /// 0x08d5ec2a
    FNetSteamServer(int a1, FNetLobbyCallbacksIFace* a2, LobbyType a3);
    /// 0x08d5f3fa
    /// deleting 0x08d5f3d2
    virtual ~FNetSteamServer();
    /// 0x08d5fc84
    virtual UNK_RET BroadcastGame(FStringA const& a1, int a2, bool a3);
    /// 0x08d5fcf2
    virtual UNK_RET BrowseGames(FSessionSetup const* a1, int a2);
    /// 0x08d5fe80
    virtual UNK_RET ConnectionClosed(long a1);
    /// 0x08d5fe00
    virtual UNK_RET GetCurrentGameMaxPlayers() const;
    /// 0x08d5fd6a
    virtual UNK_RET GetCurrentGameName() const;
    /// 0x08d5fe0c
    virtual UNK_RET GetNumServers() const;
    /// 0x08d5fdb8
    virtual UNK_RET GetRoomID() const;
    /// 0x087398d4
    virtual UNK_RET GetServerCustomInt(long a1, FStringA const& a2) const;
    /// 0x087398d8
    virtual UNK_RET GetServerCustomString(long a1, FStringA const& a2) const;
    /// 0x08739872
    virtual UNK_RET GetServerGameName(long a1) const;
    /// 0x08d5fe10
    virtual UNK_RET GetServerIDAtIndex(int a1) const;
    /// 0x087398bc
    virtual UNK_RET GetServerLocked(long a1) const;
    /// 0x087398c8
    virtual UNK_RET GetServerMaxPlayers(long a1) const;
    /// 0x087398c4
    virtual UNK_RET GetServerNumPlayers(long a1) const;
    /// 0x087398c0
    virtual UNK_RET GetServerPing(long a1) const;
    /// 0x087399b6
    virtual UNK_RET GetServerPrivateAddress(long a1) const;
    /// 0x087398cc
    virtual UNK_RET GetServerPrivateSlots(long a1) const;
    /// 0x0873996c
    virtual UNK_RET GetServerPublicAddress(long a1) const;
    /// 0x08d5fe16
    virtual UNK_RET GetServerTransportType(long a1) const;
    /// 0x08d5fe78
    virtual UNK_RET GetServerType(long a1) const;
    /// 0x08d5fe2e
    virtual UNK_RET GetSteamID() const;
    /// 0x08d5f624
    virtual UNK_RET Init();
    /// 0x08d5fcf8
    virtual UNK_RET JoinRoom(long a1);
    /// 0x08d5fd56
    virtual UNK_RET LeaveRoom();
    /// 0x08d5fe1c
    virtual UNK_RET MigrateHost(bool a1);
    /// 0x08d5faf2
    virtual UNK_RET RegisterServerIntKey(FStringA const& a1, int a2);
    /// 0x08d5fb70
    virtual UNK_RET RegisterServerStringKey(FStringA const& a1, FStringA const& a2);
    /// 0x08d5fdea
    virtual UNK_RET SetCurrentGameMaxPlayers(int a1);
    /// 0x08d5fbe6
    virtual UNK_RET Shutdown();
    /// 0x08d5f9dc
    virtual UNK_RET Update();
    /// 0x08d5fa68
    virtual UNK_RET UpdateGameDetails();
    /// 0x087398d0
    virtual UNK_RET vf_38(UNK_PARAMS);
    /// 0x08739922
    virtual UNK_RET vf_52(UNK_PARAMS);
    /// 0x08739a00
    virtual UNK_RET vf_57(UNK_PARAMS);
    /// 0x08739a04
    virtual UNK_RET vf_58(UNK_PARAMS);
    /// 0x08d5fdc4
    virtual UNK_RET GetPublicIPAddress() const;
    /// 0x08d5fe7e
    virtual UNK_RET UpdateServerKeys();
    /// 0x08d5e5cc
    UNK_RET OnAuthTicketRequest(long a1, void const* a2, unsigned int a3);
    /// 0x08d5eec8
    UNK_RET OnSteamServersConnected(SteamServersConnected_t* a1);
    /// 0x08d5ef6e
    UNK_RET OnSteamServersConnectFailure(SteamServerConnectFailure_t* a1);
    /// 0x08d5efda
    UNK_RET OnSteamServersDisconnected(SteamServersDisconnected_t* a1);
    /// 0x08d5f026
    UNK_RET OnPolicyResponse(GSPolicyResponse_t* a1);
    /// 0x08d5f0d4
    UNK_RET OnValidateAuthTicketResponse(ValidateAuthTicketResponse_t* a1);
    /// 0x08d5f62a
    UNK_RET Init(FNetSteamServerInitParams const& a1);
    /// 0x08d5f706
    UNK_RET InitSteamGameServer();
    /// 0x08d5f9f0
    UNK_RET StartClientAuthorization(long a1, unsigned char const* a2, unsigned int a3);
};
