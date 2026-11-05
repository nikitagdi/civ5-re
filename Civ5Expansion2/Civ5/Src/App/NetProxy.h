/// origin binary
#pragma once
#include "FirePlace/FireWorks/FMemoryStream.h"
#include "FirePlace/FireWorks/FString.h"
#include <memory>
#include <steam.h>
#include <string>
#include <vector>

class FNetMessage;
class FNetSteamServerInitParams;
class FSessionSetup;
class HotJoinState;
class KickReason;
class LinearEventDispatcher;
class LobbyType;
class LocalMachineContainer;
class NetProxyExtendedErrorType;
class NetProxyInitParams;
class NullEventClass;
class ServerType;
class SteamServerInviteData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class NetProxy {
public:
    /// 0x0872dbca
    NetProxy(NetProxyInitParams const* a1);
    /// 0x0872e360
    ~NetProxy();
    /// 0x0872e080
    UNK_RET Init(NetProxyInitParams const* a1);
    /// 0x0872e5a8
    UNK_RET Uninit();
    /// 0x0872e5f0
    UNK_RET GetDefaultServerParameters(FNetSteamServerInitParams* a1) const;
    /// 0x0872e720
    UNK_RET SetNetIDs();
    /// 0x0872e8e4
    UNK_RET UninitSession(bool a1);
    /// 0x0872ec6e
    UNK_RET UninitLobby();
    /// 0x0872ecc0
    UNK_RET UninitProfile();
    /// 0x0872ed12
    UNK_RET ResetSession(bool a1);
    /// 0x0872ed3c
    UNK_RET ResetProfile();
    /// 0x0872ed64
    UNK_RET InitProfile();
    /// 0x0872ee0c
    UNK_RET ResetLobby();
    /// 0x0872ee20
    UNK_RET Host(ServerType a1);
    /// 0x0872f9fc
    UNK_RET InitSession();
    /// 0x0872fae8
    UNK_RET GetSessionData(FSessionSetup* a1);
    /// 0x0872fb8a
    UNK_RET GetLocalPlayerID() const;
    /// 0x0872fbbc
    UNK_RET JoinGame(FStringA a1);
    /// 0x0872fe20
    UNK_RET IsHost() const;
    /// 0x0872fe80
    UNK_RET BroadcastMessage(FNetMessage& a1) const;
    /// 0x0872feb8
    UNK_RET Update(float a1);
    /// 0x0872ff3c
    UNK_RET InitLobby(LobbyType a1);
    /// 0x08730076
    UNK_RET JoinGame(long a1);
    /// 0x08730132
    UNK_RET FinishJoinGame(long a1);
    /// 0x08730a5a
    UNK_RET SetLastExtendedError(NetProxyExtendedErrorType a1);
    /// 0x08730a6a
    UNK_RET SetLastExtendedErrorData(FStringA const& a1);
    /// 0x08730aaa
    UNK_RET GetSessionData(long a1, FSessionSetup* a2);
    /// 0x08730aac
    UNK_RET LaunchMultiplayerGame();
    /// 0x087310a8
    UNK_RET IsGameLaunching() const;
    /// 0x087310b4
    UNK_RET Flush();
    /// 0x08731138
    UNK_RET GetPlayer(int a1);
    /// 0x0873118c
    UNK_RET LaunchGame();
    /// 0x08731262
    UNK_RET ActivateGame(bool a1);
    /// 0x08731294
    UNK_RET GetUserNameA();
    /// 0x0873141a
    UNK_RET BroadcastLANHost(FStringA a1, int a2);
    /// 0x0873154a
    UNK_RET FinishBroadcastHost(FStringA a1, int a2, bool a3);
    /// 0x08731bec
    UNK_RET FindLANGames();
    /// 0x08731c30
    UNK_RET FinishFindGames();
    /// 0x08731c90
    UNK_RET BroadcastINetHost(FStringA a1, int a2, bool a3);
    /// 0x08731e40
    UNK_RET BroadcastINetServer(FStringA a1, int a2, bool a3);
    /// 0x08731f70
    UNK_RET FindINetGames();
    /// 0x087320ba
    UNK_RET ProcessSteamLobbyInvitation(CSteamID a1, CSteamID a2);
    /// 0x0873238a
    UNK_RET ProcessSteamServerInvitation(EventTemplate<Event_Int2Type<138>, SteamServerInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08732512
    UNK_RET AddTurnReminder(int a1);
    /// 0x08732590
    UNK_RET FlushTurnReminders();
    /// 0x08732724
    UNK_RET SendTurnReminderSteamInvite(int a1);
    /// 0x087328cc
    UNK_RET SendTurnReminderEmails();
    /// 0x0873353c
    UNK_RET GameLoadedTurnReminder();
    /// 0x0873370e
    UNK_RET CheckForNetworkDisconnect();
    /// 0x0873377a
    UNK_RET ProfileLogin(FStringA const& a1, FStringA const& a2);
    /// 0x087337cc
    UNK_RET IsProfileConnected(bool a1);
    /// 0x08733806
    UNK_RET GetProfileDetails(int a1);
    /// 0x08733844
    UNK_RET UpdateHotJoin();
    /// 0x08734442
    UNK_RET CheckPingTimes();
    /// 0x0873448c
    UNK_RET SendMessageA(FNetMessage& a1, int a2) const;
    /// 0x087344ce
    UNK_RET HandleMessage(FNetMessage& a1);
    /// 0x087344fa
    UNK_RET HandleDisconnect(bool a1);
    /// 0x087345a0
    UNK_RET HandleAbandon(KickReason a1);
    /// 0x0873463e
    UNK_RET HandleLastPlayer();
    /// 0x08734860
    UNK_RET GetTurnsActive() const;
    /// 0x0873488e
    UNK_RET TransferFile(FStringA const& a1, int a2) const;
    /// 0x087348ee
    UNK_RET TransferStream(FMemoryStream* a1, int a2) const;
    /// 0x0873494e
    UNK_RET DataSent(unsigned int a1, int a2, bool a3);
    /// 0x08734950
    UNK_RET IncomingData(int a1, int a2, int a3);
    /// 0x08734a4c
    UNK_RET DataReceived(unsigned int a1, int a2, bool a3);
    /// 0x08734a4e
    UNK_RET GetReceivedData(unsigned int a1);
    /// 0x08734aa2
    UNK_RET RemoveTransfer(unsigned int a1);
    /// 0x08734af2
    UNK_RET SetSyncing(bool a1);
    /// 0x08734b24
    UNK_RET GetSyncing() const;
    /// 0x08734b52
    UNK_RET ActivatePlayer(int a1, bool a2);
    /// 0x08734b8e
    UNK_RET SetSyncPaused(bool a1);
    /// 0x08734bc0
    UNK_RET GetSyncPaused() const;
    /// 0x08734bee
    UNK_RET HasMessagesQueued() const;
    /// 0x08734c20
    UNK_RET HandleNewPlayerHotJoin(int a1);
    /// 0x08734d5c
    UNK_RET AddDeferredHotJoinPlayer(int a1);
    /// 0x08734e06
    UNK_RET KickPlayer(int a1, KickReason a2);
    /// 0x08734e56
    UNK_RET PlayerRegistered(int a1);
    /// 0x08735132
    UNK_RET SetPlayerKicked(int a1, bool a2);
    /// 0x0873514c
    UNK_RET SetPlayerRegisterComplete(int a1, bool a2);
    /// 0x08735166
    UNK_RET SetPlayerDesiredSlot(int a1, int a2);
    /// 0x08735180
    UNK_RET SetPlayerCachedSwapSlot(int a1, int a2);
    /// 0x0873519a
    UNK_RET IsPlayerRegisterComplete(int a1) const;
    /// 0x087351ba
    UNK_RET PlayersSwapped(int a1, int a2);
    /// 0x08735810
    UNK_RET IsPlayerKicked(int a1);
    /// 0x08735830
    UNK_RET PlayerLeft(int a1);
    /// 0x08735d1e
    UNK_RET PlayerRequestingConnection(int a1, FStringA const& a2);
    /// 0x08735f0e
    UNK_RET GetNewPlayerID(FStringA const& a1, bool a2);
    /// 0x087365fc
    UNK_RET PlayerConnected(int a1);
    /// 0x08736cac
    UNK_RET GetHostPlayerID() const;
    /// 0x08736cde
    UNK_RET GetNumPlayers(bool a1) const;
    /// 0x08736d2a
    UNK_RET GetPlayer(int a1) const;
    /// 0x08736d7e
    UNK_RET CloseConnection(int a1);
    /// 0x08736dae
    UNK_RET ForceResync();
    /// 0x08736e64
    UNK_RET RegisterRequiredDLC();
    /// 0x08736e94
    UNK_RET EndGame();
    /// 0x08736eb8
    UNK_RET GetLastCompletedCommTurn() const;
    /// 0x08736ee2
    UNK_RET GetTargetCommTurn() const;
    /// 0x08736f10
    UNK_RET GetPlayerCommTurn(int a1) const;
    /// 0x08736f52
    UNK_RET ResyncGame();
    /// 0x08736f94
    UNK_RET SetTempKeepAlive(float a1);
    /// 0x0873700e
    UNK_RET ProcessLaunch();
    /// 0x08737132
    UNK_RET CheckKeepAliveTime(int a1);
    /// 0x0873717c
    UNK_RET IsMultiplayer() const;
    /// 0x08737188
    UNK_RET waitingForRemotePlayers() const;
    /// 0x08737194
    UNK_RET waitingForRemotePlayers(bool a1);
    /// 0x087371dc
    UNK_RET IsMultiplayer(bool a1);
    /// 0x087371ec
    UNK_RET NetMessageDebugLog(std::string const& a1) const;
    /// 0x0873725c
    UNK_RET IsLoggingEnabledA() const;
    /// 0x0873728c
    UNK_RET HasReconnectCache() const;
    /// 0x087372a2
    UNK_RET Reconnect();
    /// 0x0873740e
    UNK_RET UpdateCount() const;
    /// 0x0873741a
    UNK_RET HasPendingHotJoins() const;
    /// 0x08737432
    UNK_RET HasDeferredHotJoins() const;
    /// 0x0873744a
    UNK_RET SetHotJoinState(HotJoinState a1);
    /// 0x08737462
    UNK_RET StartHotJoin(std::vector<int, std::allocator<int> > const& a1);
    /// 0x087375dc
    UNK_RET OnHotJoinStart(std::vector<int, std::allocator<int> > const& a1);
    /// 0x08737720
    UNK_RET OnHotJoinComplete();
    /// 0x08737968
    UNK_RET IsEveryoneConnected() const;
    /// 0x087379ce
    UNK_RET IsDeferredHotJoinerReady();
    /// 0x08737a76
    UNK_RET SetAssetsHashString(FStringA const& a1);
    /// 0x08737ab4
    UNK_RET GetAssetHashString() const;
    /// 0x08737abc
    UNK_RET PlayerStatusChanged(int a1);
    /// 0x08737abe
    UNK_RET SetLastExtendedErrorData(char const* a1);
    /// 0x08737b0a
    UNK_RET ClearLastExtendedError();
    /// 0x08737b1a
    UNK_RET GetPingTime(int a1);
    /// 0x08737b54
    UNK_RET SendPlayerDesiredSlot(int a1);
    /// 0x08737c18
    UNK_RET GetPlayerDesiredSlot(int a1) const;
    /// 0x08737c30
    UNK_RET GetDebugLogLevel() const;
    /// 0x08737c3c
    UNK_RET SetDebugLogLevel(int a1);
    /// 0x08737c4c
    UNK_RET HostMigrated(int a1, int a2);
    /// 0x08738190
    UNK_RET PlayerValidHostCandidate(int a1);
    /// 0x0873830e
    UNK_RET GetLANNickName();
    /// 0x0873836a
    UNK_RET IsPlayerHotJoining(int a1);
    /// 0x087383c0
    UNK_RET GetPlayerCachedSwapSlot(int a1) const;
    /// 0x087383d8
    UNK_RET UpdatePlayerDesiredSlot();
    /// 0x08738576
    UNK_RET SetServerParameters(FNetSteamServerInitParams const& a1);
    /// 0x0873864e
    UNK_RET GetServerParameters() const;
    /// 0x08738658
    UNK_RET GetDefaultInitParams();
    /// 0x0a3e1870
    static UNK_DATA ms_pInst[0x4];
};

extern "C" {
    /// 0x0872e5ef
    UNK_RET SteamAPIDebugTextHook(UNK_PARAMS);
    /// 0x0a3e1874
    extern UNK_DATA g_bDisableSteamHack[0x1];
}
