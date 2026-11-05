/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FMemoryStream.h"
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
/// vtable 0x0a304ce0
class FNetSessionIFace {
public:
    /// 0x08d65344
    FNetSessionIFace(int a1, FNetSessionCallbacksIFace* a2);
    /// 0x08d653b8
    /// deleting 0x08d6537e
    virtual ~FNetSessionIFace() = 0;
    /// 0x08d655d8
    virtual UNK_RET ActivateSync(bool a1);
    /// 0x08d655da
    virtual UNK_RET ActivatePlayer(int a1, bool a2);
    /// 0x08d655dc
    virtual UNK_RET SetSyncing(bool a1);
    /// pure
    virtual UNK_RET GetSyncing() const = 0;
    /// 0x08d655de
    virtual UNK_RET SetSyncPaused(bool a1);
    /// 0x08d655e0
    virtual UNK_RET GetSyncPaused() const;
    /// 0x08d655e4
    virtual UNK_RET Host(TransportType a1, int a2, FSessionSetup const* a3, int a4, bool a5);
    /// 0x08d655ea
    virtual UNK_RET Join(int a1, FStringA const& a2, bool a3);
    /// pure
    virtual UNK_RET Join(int a1, CSteamID a2, bool a3) = 0;
    /// 0x08d655f0
    virtual UNK_RET Join(int a1, long a2, FSessionSetup const* a3, int a4, bool a5);
    /// 0x08d655f6
    virtual UNK_RET Modify(FSessionSetup const* a1);
    /// 0x08d655fa
    virtual UNK_RET CheckConnectionAttempt(FNetPlatformDetailsSteam const* a1, FDataStream* a2, FDataStream* a3);
    /// 0x08d655fe
    virtual UNK_RET SendIntroduction(long a1);
    /// 0x08d65600
    virtual UNK_RET SendIntroduction(int a1);
    /// 0x08d65602
    virtual UNK_RET OpenMessageLog(FStringA const& a1);
    /// pure
    virtual UNK_RET SendMessageA(FNetMessage& a1, int a2, bool a3) const = 0;
    /// pure
    virtual UNK_RET SendMessageA(FNetMessage& a1, long a2, bool a3) const = 0;
    /// pure
    virtual UNK_RET SendCopy(FNetMessage& a1, long a2, bool a3) const = 0;
    /// pure
    virtual UNK_RET BroadcastMessage(FNetMessage& a1, bool a2) const = 0;
    /// pure
    virtual UNK_RET SendData(long a1, unsigned char const* a2, unsigned int a3, bool a4) const = 0;
    /// 0x08d65606
    virtual UNK_RET Update(unsigned int a1);
    /// 0x08d65608
    virtual UNK_RET Update(float a1);
    /// 0x08d6560a
    virtual UNK_RET StartGame();
    /// 0x08d6560e
    virtual UNK_RET EndGame();
    /// 0x08d65610
    virtual UNK_RET AddDelayedCloseConnection(int a1);
    /// 0x08d65612
    virtual UNK_RET CloseConnection(int a1);
    /// 0x08d65614
    virtual UNK_RET Close();
    /// 0x08d65616
    virtual UNK_RET Abandon(KickReason a1);
    /// 0x08d65618
    virtual UNK_RET ConnectionClosed(FNetPlayer* a1);
    /// 0x08d6561a
    virtual UNK_RET SetDynamicTurns(bool a1);
    /// 0x08d6561c
    virtual UNK_RET GetCommTurnTime() const;
    /// 0x08d65620
    virtual UNK_RET GetLastProcessedTurn() const;
    /// 0x08d65626
    virtual UNK_RET ResetSyncTime();
    /// 0x08d6562a
    virtual UNK_RET GetNumPlayers() const;
    /// 0x08d6562e
    virtual UNK_RET GetLocalPlayerID() const;
    /// 0x08d65634
    virtual UNK_RET GetHostPlayerID() const;
    /// 0x08d6563a
    virtual UNK_RET GetPlayer(int a1) const;
    /// 0x08d6563e
    virtual UNK_RET CompressPlayers();
    /// 0x08d65640
    virtual UNK_RET SetKeepAliveTime(float a1);
    /// 0x08d65642
    virtual UNK_RET GetPingTime(int a1);
    /// 0x08d65648
    virtual UNK_RET CheckPingTimes();
    /// 0x08d6544c
    virtual UNK_RET UsingSecureSession() const;
    /// 0x08d65478
    virtual UNK_RET AreInvitationsAllowed() const;
    /// 0x08d654b8
    virtual UNK_RET AreStatsEnabled() const;
    /// 0x08d654fa
    virtual UNK_RET IsValidationRequired() const;
    /// 0x08d6553c
    virtual UNK_RET GetSessionHandle() const;
    /// 0x08d6564c
    virtual UNK_RET IsActive() const;
    /// 0x08d65650
    virtual UNK_RET IsOnline() const;
    /// 0x08d65654
    virtual UNK_RET IsHost() const;
    /// 0x08d65658
    virtual UNK_RET IsServer() const;
    /// 0x08d6565c
    virtual UNK_RET GetTransportType() const;
    /// 0x08d65660
    virtual UNK_RET ServerStartup();
    /// 0x08d65662
    virtual UNK_RET ServerShutdown();
    /// 0x08d65664
    virtual UNK_RET TransferFile(FStringA const& a1, int a2);
    /// 0x08d65666
    virtual UNK_RET ReadyForFileTransfer(FStringA const& a1, unsigned int a2, int a3);
    /// 0x08d65668
    virtual UNK_RET TransferStream(FMemoryStream* a1, int a2);
    /// 0x08d6566a
    virtual UNK_RET ReadyForStreamTransfer(unsigned int a1, unsigned int a2, int a3);
    /// 0x08d6566c
    virtual UNK_RET StartOrUpdateHostMigration(int a1, int a2);
    /// 0x08d6566e
    virtual UNK_RET Connect(FStringA const& a1, bool a2, long& a3, char const* a4, unsigned int a5);
    /// 0x08d65672
    virtual UNK_RET Connect(CSteamID a1, bool a2, unsigned long long a3, long& a4, char const* a5, unsigned int a6);
    /// 0x08d65676
    virtual UNK_RET HandleConnectionFailure();
    /// 0x08d65678
    virtual UNK_RET JoinRefusedResponse(long a1);
    /// 0x08d6567a
    virtual UNK_RET ProcessNetwork();
    /// 0x08d6567c
    virtual UNK_RET MessageReceived(FNetMessage& a1, bool a2);
    /// 0x08d6567e
    virtual UNK_RET HandleMessage(FNetMessage& a1);
    /// 0x08d65680
    virtual UNK_RET PlayVoice(int a1, unsigned short a2, unsigned short const* a3, unsigned short const* a4, unsigned char const* const* a5, unsigned short a6, double a7);
    /// 0x08d65682
    virtual UNK_RET GetLocalAddress() const;
    /// 0x08d656cc
    virtual UNK_RET GetLocalPort() const;
    /// 0x08d656d0
    virtual UNK_RET GetHostLocation() const;
    /// 0x08d6571a
    virtual UNK_RET GetLocalSteamID() const;
    /// 0x08d6572e
    virtual UNK_RET GetSteamID(long a1) const;
    /// 0x08d65742
    virtual UNK_RET vf_73(UNK_PARAMS);
    /// 0x08d65746
    virtual UNK_RET vf_74(UNK_PARAMS);
    /// 0x08d6574a
    virtual UNK_RET vf_75(UNK_PARAMS);
    /// 0x08d6574e
    virtual UNK_RET vf_76(UNK_PARAMS);
    /// 0x08d65752
    virtual UNK_RET vf_77(UNK_PARAMS);
    /// 0x08d65756
    virtual UNK_RET vf_78(UNK_PARAMS);
    /// 0x08d6575a
    virtual UNK_RET vf_79(UNK_PARAMS);
    /// 0x08d6575e
    virtual UNK_RET vf_80(UNK_PARAMS);
    /// 0x08d65760
    virtual UNK_RET vf_81(UNK_PARAMS);
    /// 0x08d65764
    virtual UNK_RET vf_82(UNK_PARAMS);
    /// 0x08d65768
    virtual UNK_RET vf_83(UNK_PARAMS);
    /// 0x08d6576c
    virtual UNK_RET vf_84(UNK_PARAMS);
    /// 0x08d65770
    virtual UNK_RET AddSecurePlayer(FNetPlatformDetailsSteam const* a1);
    /// 0x08d65774
    virtual UNK_RET RemoveSecurePlayer(FNetPlatformDetailsSteam const* a1);
    /// 0x08d65776
    virtual UNK_RET GetSecureINetAddr(void const* a1) const;
    /// 0x08d657c0
    virtual UNK_RET ReleaseSecureINetAddr(FStringA const& a1) const;
    /// 0x08d657c2
    virtual UNK_RET MigrateSecureHost(int a1);
    /// 0x08d657c6
    virtual UNK_RET PerformSecureHostMigration(unsigned char const* a1, unsigned int a2) const;
    /// 0x08d657ca
    virtual UNK_RET AddTalker(int a1, FNetPlatformDetailsSteam const* a2);
    /// 0x08d657cc
    virtual UNK_RET RemoveTalker(int a1, FNetPlatformDetailsSteam const* a2);
    /// 0x08d657ce
    virtual UNK_RET SetMutedBy(int a1, bool a2);
    /// 0x08d657d0
    virtual UNK_RET ProcessLaunch();
    /// 0x08d657d4
    virtual UNK_RET ValidateSecurity(unsigned int a1, unsigned char const* a2);
    /// 0x08d657d8
    virtual UNK_RET HandleValidationResponse(int a1, unsigned int a2, unsigned char const* a3);
    /// 0x08d657dc
    virtual UNK_RET CheckValidations();
    /// 0x08d657e0
    virtual UNK_RET ProcessGameOver();
    /// 0x08d657e4
    virtual UNK_RET GetSecureHostData() const;
    /// 0x08d657e8
    virtual UNK_RET GetSecureHostDataLen() const;
    /// 0x08d657ec
    virtual UNK_RET GetLocalPlatformDetails();
    /// pure
    virtual UNK_RET QueueMessages(bool a1) = 0;
    /// pure
    virtual UNK_RET vf_103(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET FlushQueue(int a1) = 0;
    /// pure
    virtual UNK_RET HasMessagesQueued() const = 0;
    /// pure
    virtual UNK_RET CreateOnlineComponents(bool a1) = 0;
    /// 0x08d657f0
    virtual UNK_RET vf_107(UNK_PARAMS);
    /// 0x08d653ec
    UNK_RET GetLobby() const;
    /// 0x08d6541c
    UNK_RET GetProfile() const;
};

namespace FNetSessionFactory {
/// 0x08d6557c
UNK_RET CreateNetSession(int a1, FNetSessionCallbacksIFace* a2);
}
