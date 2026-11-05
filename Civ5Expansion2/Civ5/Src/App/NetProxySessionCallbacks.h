/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FMemoryStream.h"
#include "FirePlace/FireWorks/FNetSessionCallbacksIFace.h"
#include "FirePlace/FireWorks/FString.h"

class FNetMessage;
class KickReason;
class NetProxy;

/// size ?
/// vtable 0x0a2fc260
class NetProxySessionCallbacks : public FNetSessionCallbacksIFace {
public:
    /// 0x0873b9c0
    NetProxySessionCallbacks(NetProxy* a1);
    /// 0x0873cf68
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x0873ccd2
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x0873b9ea
    virtual UNK_RET ConnectionAttempt(FStringA const& a1, FDataStream* a2, FDataStream* a3);
    /// 0x0873c1de
    virtual UNK_RET ConnectionRejected(FDataStream* a1);
    /// 0x0873c5e0
    virtual UNK_RET ConnectionComplete();
    /// 0x0873c5e2
    virtual UNK_RET ConnectionComplete(int a1, FDataStream* a2);
    /// 0x0873be58
    virtual UNK_RET PlayerRequestingConnection(int a1, FStringA const& a2);
    /// 0x0873be90
    virtual UNK_RET GetNewPlayerID(FStringA const& a1, bool a2);
    /// 0x0873bec8
    virtual UNK_RET PlayerValidHostCandidate(int a1);
    /// 0x0873be2c
    virtual UNK_RET PlayerConnected(int a1);
    /// 0x0873bef8
    virtual UNK_RET PlayerRegistered(int a1);
    /// 0x0873c176
    virtual UNK_RET PlayersSwapped(int a1, int a2);
    /// 0x0873c1aa
    virtual UNK_RET PlayerIDChanged(int a1, int a2);
    /// 0x0873c816
    virtual UNK_RET PlayerLeft(int a1);
    /// 0x0873c864
    virtual UNK_RET PlayerNonResponsive(int a1);
    /// 0x0873c866
    virtual UNK_RET PlayerInvalid(int a1);
    /// 0x0873c868
    virtual UNK_RET GameLaunched();
    /// 0x0873c8be
    virtual UNK_RET GameOver();
    /// 0x0873c892
    virtual UNK_RET GameAbandoned(KickReason a1);
    /// 0x0873c8ea
    virtual UNK_RET GameLastPlayer();
    /// 0x0873c90e
    virtual UNK_RET HostMigrated(int a1, int a2);
    /// 0x0873c942
    virtual UNK_RET MessageReceived(FNetMessage& a1);
    /// 0x0873c96e
    virtual UNK_RET DataSent(int a1, int a2, bool a3);
    /// 0x0873c9f2
    virtual UNK_RET IncomingDataNotification(int a1, int a2, int a3, FStringA const& a4);
    /// 0x0873c9b0
    virtual UNK_RET DataReceived(unsigned int a1, int a2, bool a3);
    /// 0x0873ca30
    virtual UNK_RET TurnComplete(unsigned int a1, float a2, float a3);
    /// 0x0873ca5c
    virtual UNK_RET PlayerVersionMismatch(int a1, char const* a2, bool a3);
    /// 0x0873caa4
    virtual UNK_RET GetConnectionAttemptExtraData(FMemoryStream& a1, bool a2, bool a3);
    /// 0x0873cb3a
    virtual UNK_RET SetPlayerKicked(int a1, bool a2);
    /// 0x0873cb72
    virtual UNK_RET GetLANNickName();
    /// 0x0873cce6
    virtual UNK_RET vf_30(UNK_PARAMS);
};

/// 0x0873cf6c
UNK_RET Fow();
/// 0x0873cf72
UNK_RET GameplayRoadEventSubscribe();
/// 0x0873cf78
UNK_RET GameEventProcessing();
/// 0x0873cf7e
UNK_RET Fow2();
/// 0x0873cf84
bool DONT_FLIP_CAMERA();
/// 0x0873cf8a
unsigned int BlendCellSize();
/// 0x0873cf90
UNK_RET FarmEventSubscribe();
/// 0x0873cfa3
UNK_RET Projectile();
/// 0x09158204
extern float g_09158204;
/// 0x09158208
extern float g_09158208;
/// 0x0873cfb6
float ViewMax();
/// 0x0873cfc9
float ViewFOV();
/// 0x0873cfdc
UNK_RET DoneThreshhold();
/// 0x0873cfef
UNK_RET BenchMarkMode();
/// 0x0873cff5
UNK_RET BackBufferFormat();
/// 0x0873cffb
UNK_RET StrategicViewNotHacked();
/// 0x0873d001
UNK_RET CoastTempHeapTrample();
/// 0x0873d007
UNK_RET PrimaryEventChannel();
/// 0x0873d00d
UNK_RET GTH();
/// 0x0873d013
UNK_RET GetNextGameStateValue();
/// 0x0873d019
UNK_RET GeteSerialEventUnitCreated();
/// 0x0873d01f
UNK_RET GeteSerialEventUnitDestroyed();
/// 0x0873d025
UNK_RET GeteSerialEventUnitMove();
/// 0x0873d02b
UNK_RET GeteSerialEventCityCreated();
/// 0x0873d031
UNK_RET GeteSerialEventJungleCreated();
/// 0x0873d037
UNK_RET GeteSerialEventForestCreated();
/// 0x0873d03d
UNK_RET GeteSerialEventImprovementCreated();
/// 0x0873d043
UNK_RET GeteSerialEventCameraLeft();
/// 0x0873d049
UNK_RET GeteSerialEventCameraRight();
/// 0x0873d04f
UNK_RET GeteDragCamera();
/// 0x0873d055
UNK_RET GeteSerialEventCameraIn();
/// 0x0873d05b
UNK_RET GeteSerialEventCameraOut();
/// 0x0873d061
UNK_RET GeteFrameGlobalsUpdated();
