/// origin guess
#pragma once
#include "Civ5/Src/App/ISteamMatchmakingPingResponse.h"
#include "Civ5/Src/App/ISteamMatchmakingRulesResponse.h"
#include "Civ5/Src/App/ISteamMatchmakingServerListResponse.h"
#include "FirePlace/FireWorks/FNetLobbyGameListUpdateData.h"
#include "FirePlace/FireWorks/FNetLobbySteamBase.h"
#include "FirePlace/FireWorks/FString.h"
#include <steam.h>

class FNetLobbyCallbacksIFace;
class FNetLobbySteam;
class FSessionSetup;
class FSteamGameServer;
class LobbyCreated_t;
class LobbyDataUpdate_t;
class LobbyEnter_t;
class LobbyMatchList_t;
class LobbyType;
class gameserveritem_t;
enum EMatchMakingServerResponse : int;

/// size ?
/// vtable 0x0a303a80
class FNetLobbySteam : public FNetLobbySteamBase {
public:
    class GameListUpdateData;
    class ServerBrowserCallbacks;
    class ServerPingCallbacks;
    class ServerRulesCallbacks;
    /// size ?
    /// vtable 0x0a303d28
    class GameListUpdateData : public FNetLobbyGameListUpdateData {
    public:
        /// 0x08d484d8
        virtual UNK_RET vf_00(UNK_PARAMS);
        /// 0x08d484c4
        virtual UNK_RET vf_01(UNK_PARAMS);
    };
    enum LIST_TYPE : int;
    enum SEARCH_TYPE : int;
    /// size ?
    /// vtable 0x0a303bb0
    class ServerBrowserCallbacks : public ISteamMatchmakingServerListResponse {
    public:
        /// 0x08d46d0a
        virtual UNK_RET ServerResponded(void* a1, int a2);
        /// 0x08d46f1e
        virtual UNK_RET ServerFailedToRespond(void* a1, int a2);
        /// 0x08d470ea
        virtual UNK_RET RefreshComplete(void* a1, EMatchMakingServerResponse a2);
        /// 0x08d43d50
        UNK_RET Init(FNetLobbySteam* a1, FNetLobbySteam::SEARCH_TYPE a2);
        /// 0x08d44984
        UNK_RET Stop();
    };
    /// size ?
    /// vtable 0x0a303be4
    class ServerPingCallbacks : public ISteamMatchmakingPingResponse {
    public:
        /// 0x08d47376
        virtual UNK_RET ServerResponded(gameserveritem_t& a1);
        /// 0x08d474c8
        virtual UNK_RET ServerFailedToRespond();
        /// 0x08d43d6a
        UNK_RET Init(FNetLobbySteam* a1, FNetLobbySteam::SEARCH_TYPE a2);
        /// 0x08d4721e
        UNK_RET Reset();
        /// 0x08d47232
        UNK_RET PingServer(unsigned int a1, int a2);
    };
    /// size ?
    /// vtable 0x0a303b90
    class ServerRulesCallbacks : public ISteamMatchmakingRulesResponse {
    public:
        /// 0x08d47182
        ~ServerRulesCallbacks();
        /// 0x08d471b6
        virtual UNK_RET RulesResponded(char const* a1, char const* a2);
        /// 0x08d471fe
        virtual UNK_RET RulesFailedToRespond();
        /// 0x08d4720e
        virtual UNK_RET RulesRefreshComplete();
        /// 0x08d449c0
        UNK_RET Stop();
        /// 0x08d46a8e
        UNK_RET Start();
    };
    /// 0x08d43548
    FNetLobbySteam(int a1, FNetLobbyCallbacksIFace* a2, LobbyType a3);
    /// 0x08d43ae6
    /// deleting 0x08d43abe
    virtual ~FNetLobbySteam();
    /// 0x08d4513a
    virtual UNK_RET AcceptInvitation(long a1);
    /// 0x08d441c4
    virtual UNK_RET BroadcastGame(FStringA const& a1, int a2, bool a3);
    /// 0x08d4445a
    virtual UNK_RET BrowseGames(FSessionSetup const* a1, int a2);
    /// 0x08d476b8
    virtual UNK_RET ConnectionAccepted(long a1, unsigned char const* a2, unsigned int a3);
    /// 0x08d47650
    virtual UNK_RET ConnectionClosed(long a1);
    /// 0x08d4671a
    virtual UNK_RET GetCurrentGameMaxPlayers() const;
    /// 0x08d45378
    virtual UNK_RET GetCurrentGameName() const;
    /// 0x08d45a9e
    virtual UNK_RET GetHostSteamID(long a1) const;
    /// 0x08d4520a
    virtual UNK_RET GetInviteID(CSteamID a1);
    /// 0x08d4547e
    virtual UNK_RET GetNumServers() const;
    /// 0x08d45440
    virtual UNK_RET GetRoomID() const;
    /// 0x08d462c4
    virtual UNK_RET GetRoomID(CSteamID a1) const;
    /// 0x08d46054
    virtual UNK_RET GetServerCustomInt(long a1, FStringA const& a2) const;
    /// 0x08d46144
    virtual UNK_RET GetServerCustomString(long a1, FStringA const& a2) const;
    /// 0x08d454ee
    virtual UNK_RET GetServerGameName(long a1) const;
    /// 0x08d454a2
    virtual UNK_RET GetServerIDAtIndex(int a1) const;
    /// 0x08d4562c
    virtual UNK_RET GetServerLocked(long a1) const;
    /// 0x08d457aa
    virtual UNK_RET GetServerMaxPlayers(long a1) const;
    /// 0x08d456fc
    virtual UNK_RET GetServerNumPlayers(long a1) const;
    /// 0x08d4568e
    virtual UNK_RET GetServerPing(long a1) const;
    /// 0x08d45a2c
    virtual UNK_RET GetServerPrivateAddress(long a1) const;
    /// 0x08d4585c
    virtual UNK_RET GetServerPrivateSlots(long a1) const;
    /// 0x08d4589c
    virtual UNK_RET GetServerPublicAddress(long a1) const;
    /// 0x08739444
    virtual UNK_RET GetServerTransportType(long a1) const;
    /// 0x08d47602
    virtual UNK_RET GetServerType(long a1) const;
    /// 0x08d462b0
    virtual UNK_RET GetSteamID() const;
    /// 0x08d43d00
    virtual UNK_RET Init();
    /// 0x08d449fc
    virtual UNK_RET IsBrowsing() const;
    /// 0x08d441be
    virtual UNK_RET IsLoggedIn() const;
    /// 0x08d44a1c
    virtual UNK_RET JoinRoom(long a1);
    /// 0x08d47874
    virtual UNK_RET JoinServer(FStringA& a1);
    /// 0x08d45048
    virtual UNK_RET LeaveRoom();
    /// 0x08d45b6c
    virtual UNK_RET MigrateHost(bool a1);
    /// 0x08d46052
    virtual UNK_RET RegisterServerIntKey(FStringA const& a1, int a2);
    /// 0x08d4677e
    virtual UNK_RET RegisterServerStringKey(FStringA const& a1, FStringA const& a2);
    /// 0x08d466b8
    virtual UNK_RET SetCurrentGameMaxPlayers(int a1);
    /// 0x08d43f80
    virtual UNK_RET Shutdown();
    /// 0x08d450d0
    virtual UNK_RET StartGame();
    /// 0x08d44926
    virtual UNK_RET StopBrowsing();
    /// 0x08d43d7e
    virtual UNK_RET Update();
    /// 0x08d46bc2
    virtual UNK_RET AddServerToServerBrowser(FNetLobbySteam::SEARCH_TYPE a1, FSteamGameServer* a2);
    /// 0x08d438ee
    UNK_RET OnLobbyDataUpdatedCallback(LobbyDataUpdate_t* a1);
    /// 0x08d43da0
    UNK_RET RemoveCompletedServerRulesQueries();
    /// 0x08d4402e
    UNK_RET StopAllBrowsing();
    /// 0x08d4408e
    UNK_RET ClearServers(FNetLobbySteam::SEARCH_TYPE a1);
    /// 0x08d44158
    UNK_RET SetSteamServerAuthTicket(unsigned int a1);
    /// 0x08d4429c
    UNK_RET OnLobbyCreated(LobbyCreated_t* a1, bool a2);
    /// 0x08d444ea
    UNK_RET RequestLobbies();
    /// 0x08d44640
    UNK_RET RequestServers(FNetLobbySteam::SEARCH_TYPE a1);
    /// 0x08d44b20
    UNK_RET IsLobbyID(long a1) const;
    /// 0x08d44ba0
    UNK_RET GetSteamID(long a1) const;
    /// 0x08d44c82
    UNK_RET OnLobbyEntered(LobbyEnter_t* a1, bool a2);
    /// 0x08d44f0e
    UNK_RET GetServerFromRoomID(long a1) const;
    /// 0x08d44f8a
    UNK_RET HandleDedicatedJoinLobby(CSteamID const& a1);
    /// 0x08d452f6
    UNK_RET GetInviteSteamID(long a1) const;
    /// 0x08d45358
    UNK_RET IsInviteID(long a1) const;
    /// 0x08d4536e
    UNK_RET GetRoomIndex(long a1) const;
    /// 0x08d4629e
    UNK_RET GetRoomListType(long a1) const;
    /// 0x08d462a6
    UNK_RET GetRoomSearchType(long a1) const;
    /// 0x08d463ec
    UNK_RET OnLobbyMatchListCallback(LobbyMatchList_t* a1, bool a2);
    /// 0x08d46856
    UNK_RET GetCurrentMembers() const;
    /// 0x08d468b6
    UNK_RET IsRoomIDValid(long a1) const;
    /// 0x08d468c2
    UNK_RET IsServerID(long a1) const;
    /// 0x08d46962
    UNK_RET GetBrowserFilters();
    /// 0x08d4696a
    UNK_RET GetBrowserFilters() const;
    /// 0x08d46972
    UNK_RET StartServerRulesQuery(FNetLobbySteam::SEARCH_TYPE a1, FSteamGameServer* a2);
    /// 0x08d46ad8
    UNK_RET RemoveServerRulesQuery(FSteamGameServer* a1);
    /// 0x08d46b58
    UNK_RET RemoveFromPendingList(FNetLobbySteam::SEARCH_TYPE a1, FSteamGameServer* a2);
    /// 0x08d4764e
    UNK_RET OnAuthTicketResponse(long a1, unsigned int a2);
    /// 0x08d47860
    UNK_RET SetServerReportType(FNetLobbySteam::LIST_TYPE a1, FNetLobbySteam::SEARCH_TYPE a2);
};
