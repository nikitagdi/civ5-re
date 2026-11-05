/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FMemoryStream.h"
#include "FirePlace/FireWorks/FNetSessionIFace.h"
#include "FirePlace/FireWorks/FString.h"
#include <steam.h>

class FNetMessage;
class FNetPlatformDetailsSteam;
class FNetPlayer;
class FNetSessionCallbacksIFace;
class FSessionSetup;
class KickReason;
class TransportType;

/// size ?
/// vtable 0x0a305100
class FNetSession : public FNetSessionIFace {
public:
    /// 0x08d6f700
    FNetSession(int a1, FNetSessionCallbacksIFace* a2);
    /// 0x08d6f9aa
    /// deleting 0x08d6f982
    virtual ~FNetSession();
    /// 0x08d72572
    virtual UNK_RET Abandon(KickReason a1);
    /// 0x08d7447a
    virtual UNK_RET ActivatePlayer(int a1, bool a2);
    /// 0x08d743ee
    virtual UNK_RET ActivateSync(bool a1);
    /// 0x08d7227e
    virtual UNK_RET AddDelayedCloseConnection(int a1);
    /// 0x08d74742
    virtual UNK_RET AddSecurePlayer(FNetPlatformDetailsSteam const* a1);
    /// 0x08d74aca
    virtual UNK_RET AddTalker(int a1, FNetPlatformDetailsSteam const* a2);
    /// 0x08d710ce
    virtual UNK_RET BroadcastMessage(FNetMessage& a1, bool a2) const;
    /// 0x08d7068c
    virtual UNK_RET CheckConnectionAttempt(FNetPlatformDetailsSteam const* a1, FDataStream* a2, FDataStream* a3);
    /// 0x08d74d98
    virtual UNK_RET CheckPingTimes();
    /// 0x08d74648
    virtual UNK_RET CheckValidations();
    /// 0x08d7254a
    virtual UNK_RET Close();
    /// 0x08d722de
    virtual UNK_RET CloseConnection(int a1);
    /// 0x08d72cc8
    virtual UNK_RET CompressPlayers();
    /// 0x08d734a2
    virtual UNK_RET Connect(FStringA const& a1, bool a2, long& a3, char const* a4, unsigned int a5);
    /// 0x08d73852
    virtual UNK_RET Connect(CSteamID a1, bool a2, unsigned long long a3, long& a4, char const* a5, unsigned int a6);
    /// 0x08d725be
    virtual UNK_RET ConnectionClosed(FNetPlayer* a1);
    /// 0x08d73370
    virtual UNK_RET CreateOnlineComponents(bool a1);
    /// 0x08d724e4
    virtual UNK_RET EndGame();
    /// 0x08d74b8e
    virtual UNK_RET FlushQueue(int a1);
    /// 0x08d72a38
    virtual UNK_RET GetCommTurnTime() const;
    /// 0x08d73c1a
    virtual UNK_RET GetHostLocation() const;
    /// 0x08d72b76
    virtual UNK_RET GetHostPlayerID() const;
    /// 0x08d72a80
    virtual UNK_RET GetLastProcessedTurn() const;
    /// 0x08d73ac4
    virtual UNK_RET GetLocalAddress() const;
    /// 0x08d74a8a
    virtual UNK_RET GetLocalPlatformDetails();
    /// 0x08d72b3a
    virtual UNK_RET GetLocalPlayerID() const;
    /// 0x08d73bec
    virtual UNK_RET GetLocalPort() const;
    /// 0x08d73f26
    virtual UNK_RET GetLocalSteamID() const;
    /// 0x08d72afa
    virtual UNK_RET GetNumPlayers() const;
    /// 0x08d74d3e
    virtual UNK_RET GetPingTime(int a1);
    /// 0x08d72bb2
    virtual UNK_RET GetPlayer(int a1) const;
    /// 0x08d74a0a
    virtual UNK_RET GetSecureHostData() const;
    /// 0x08d74a4a
    virtual UNK_RET GetSecureHostDataLen() const;
    /// 0x08d747d6
    virtual UNK_RET GetSecureINetAddr(void const* a1) const;
    /// 0x08d73f6c
    virtual UNK_RET GetSteamID(long a1) const;
    /// 0x08d743ac
    virtual UNK_RET GetSyncPaused() const;
    /// 0x08d74330
    virtual UNK_RET GetSyncing() const;
    /// 0x08d72cb4
    virtual UNK_RET GetTransportType() const;
    /// 0x08d73a60
    virtual UNK_RET HandleConnectionFailure();
    /// 0x08d74222
    virtual UNK_RET HandleMessage(FNetMessage& a1);
    /// 0x08d745ea
    virtual UNK_RET HandleValidationResponse(int a1, unsigned int a2, unsigned char const* a3);
    /// 0x08d74d2e
    virtual UNK_RET HasMessagesQueued() const;
    /// 0x08d6fb52
    virtual UNK_RET Host(TransportType a1, int a2, FSessionSetup const* a3, int a4, bool a5);
    /// 0x08d72bfe
    virtual UNK_RET IsActive() const;
    /// 0x08d72c68
    virtual UNK_RET IsHost() const;
    /// 0x08d72c38
    virtual UNK_RET IsOnline() const;
    /// 0x08d72caa
    virtual UNK_RET IsServer() const;
    /// 0x08d6fde0
    virtual UNK_RET Join(int a1, FStringA const& a2, bool a3);
    /// 0x08d6ff9c
    virtual UNK_RET Join(int a1, CSteamID a2, bool a3);
    /// 0x08d70180
    virtual UNK_RET Join(int a1, long a2, FSessionSetup const* a3, int a4, bool a5);
    /// 0x08d73a90
    virtual UNK_RET JoinRefusedResponse(long a1);
    /// 0x08d73fba
    virtual UNK_RET MessageReceived(FNetMessage& a1, bool a2);
    /// 0x08d74968
    virtual UNK_RET MigrateSecureHost(int a1);
    /// 0x08d7062a
    virtual UNK_RET Modify(FSessionSetup const* a1);
    /// 0x08d70e5e
    virtual UNK_RET OpenMessageLog(FStringA const& a1);
    /// 0x08d749b4
    virtual UNK_RET PerformSecureHostMigration(unsigned char const* a1, unsigned int a2) const;
    /// 0x08d7427c
    virtual UNK_RET PlayVoice(int a1, unsigned short a2, unsigned short const* a3, unsigned short const* a4, unsigned char const* const* a5, unsigned short a6, double a7);
    /// 0x08d74544
    virtual UNK_RET ProcessGameOver();
    /// 0x08d744ea
    virtual UNK_RET ProcessLaunch();
    /// 0x08d744c2
    virtual UNK_RET ProcessNetwork();
    /// 0x08d74b5a
    virtual UNK_RET QueueMessages(bool a1);
    /// 0x08d72da0
    virtual UNK_RET ReadyForFileTransfer(FStringA const& a1, unsigned int a2, int a3);
    /// 0x08d73316
    virtual UNK_RET ReadyForStreamTransfer(unsigned int a1, unsigned int a2, int a3);
    /// 0x08d74920
    virtual UNK_RET ReleaseSecureINetAddr(FStringA const& a1) const;
    /// 0x08d7478e
    virtual UNK_RET RemoveSecurePlayer(FNetPlatformDetailsSteam const* a1);
    /// 0x08d74b04
    virtual UNK_RET RemoveTalker(int a1, FNetPlatformDetailsSteam const* a2);
    /// 0x08d72aba
    virtual UNK_RET ResetSyncTime();
    /// 0x08d71822
    virtual UNK_RET SendCopy(FNetMessage& a1, long a2, bool a3) const;
    /// 0x08d71b92
    virtual UNK_RET SendData(long a1, unsigned char const* a2, unsigned int a3, bool a4) const;
    /// 0x08d708a6
    virtual UNK_RET SendIntroduction(long a1);
    /// 0x08d70b82
    virtual UNK_RET SendIntroduction(int a1);
    /// 0x08d70ee0
    virtual UNK_RET SendMessageA(FNetMessage& a1, int a2, bool a3) const;
    /// 0x08d71068
    virtual UNK_RET SendMessageA(FNetMessage& a1, long a2, bool a3) const;
    /// 0x08d74dec
    virtual UNK_RET ServerShutdown();
    /// 0x08d74dc8
    virtual UNK_RET ServerStartup();
    /// 0x08d729fe
    virtual UNK_RET SetDynamicTurns(bool a1);
    /// 0x08d72d04
    virtual UNK_RET SetKeepAliveTime(float a1);
    /// 0x08d74b3e
    virtual UNK_RET SetMutedBy(int a1, bool a2);
    /// 0x08d74366
    virtual UNK_RET SetSyncPaused(bool a1);
    /// 0x08d742f6
    virtual UNK_RET SetSyncing(bool a1);
    /// 0x08d72372
    virtual UNK_RET StartGame();
    /// 0x08d72e4c
    virtual UNK_RET StartOrUpdateHostMigration(int a1, int a2);
    /// 0x08d72d4e
    virtual UNK_RET TransferFile(FStringA const& a1, int a2);
    /// 0x08d72dfa
    virtual UNK_RET TransferStream(FMemoryStream* a1, int a2);
    /// 0x08d71be2
    virtual UNK_RET Update(unsigned int a1);
    /// 0x08d71c2e
    virtual UNK_RET Update(float a1);
    /// 0x08d74594
    virtual UNK_RET ValidateSecurity(unsigned int a1, unsigned char const* a2);
    /// 0x08d74fd2
    virtual UNK_RET vf_103(UNK_PARAMS);
    /// 0x08d74fdc
    virtual UNK_RET vf_107(UNK_PARAMS);
    /// 0x08d74f6a
    virtual UNK_RET vf_73(UNK_PARAMS);
    /// 0x08d74f72
    virtual UNK_RET vf_74(UNK_PARAMS);
    /// 0x08d74f7a
    virtual UNK_RET vf_75(UNK_PARAMS);
    /// 0x08d74f82
    virtual UNK_RET vf_76(UNK_PARAMS);
    /// 0x08d74f8a
    virtual UNK_RET vf_77(UNK_PARAMS);
    /// 0x08d74f92
    virtual UNK_RET vf_78(UNK_PARAMS);
    /// 0x08d74f9a
    virtual UNK_RET vf_79(UNK_PARAMS);
    /// 0x08d74fa2
    virtual UNK_RET vf_80(UNK_PARAMS);
    /// 0x08d74fae
    virtual UNK_RET vf_81(UNK_PARAMS);
    /// 0x08d74fb6
    virtual UNK_RET vf_82(UNK_PARAMS);
    /// 0x08d74fbe
    virtual UNK_RET vf_83(UNK_PARAMS);
    /// 0x08d74fc8
    virtual UNK_RET vf_84(UNK_PARAMS);
    /// 0x08d6f7f0
    UNK_RET Init();
    /// 0x08d6fa04
    UNK_RET Uninit();
    /// 0x08d6fb2a
    UNK_RET Reset();
    /// 0x08d6fd62
    UNK_RET HostSecureSession(FSessionSetup const* a1, int a2);
    /// 0x08d6ff04
    UNK_RET JoinSecureSession(unsigned char const* a1, unsigned int a2, FSessionSetup const* a3, int a4, bool a5);
    /// 0x08d70fb0
    UNK_RET IsValidMessageTarget(int a1) const;
    /// 0x08d71772
    UNK_RET SendAllMessageChunks(unsigned char* a1, unsigned int a2, unsigned int a3, long a4, bool a5, bool a6) const;
    /// 0x08d721aa
    UNK_RET StopHostMigration();
    /// 0x08d721da
    UNK_RET HandleDelayedCloseConnections();
    /// 0x08d723c2
    UNK_RET CheckPreGameStatus();
    /// 0x08d7247e
    UNK_RET LaunchGame();
    /// 0x08d733f8
    UNK_RET CreateVoiceManager();
    /// 0x08d736b2
    UNK_RET GetProfileName() const;
    /// 0x08d7426a
    UNK_RET IsSystemMessage(FNetMessage& a1) const;
    /// 0x08d74704
    UNK_RET CreateSecureSession(int a1);
};
