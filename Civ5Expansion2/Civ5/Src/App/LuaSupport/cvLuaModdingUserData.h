/// origin guess
#pragma once
#include <lua.h>

class cvLuaEngineInfo;

/// size ?
/// vtable 0x0a2fdde0
class cvLuaModdingUserData {
public:
    /// 0x0895ded8
    cvLuaModdingUserData();
    /// 0x0895df3e
    /// deleting 0x0895df08
    virtual ~cvLuaModdingUserData();
    /// 0x0895df6e
    UNK_RET GetDatabase();
    /// 0x0895df76
    static int OpenUserData(lua_State* a1);
    /// 0x0895e062
    UNK_RET PushMethods(lua_State* a1, int a2);
    /// 0x0895e222
    /// lua ?
    static int lDestroy(lua_State* a1);
    /// 0x0895e254
    /// lua ?
    static int lGetValue(lua_State* a1);
    /// 0x0895e42c
    /// lua ?
    static int lSetValue(lua_State* a1);
    /// 0x0895e63a
    /// lua ?
    static int lQuery(lua_State* a1);
    /// 0x0895e7c4
    /// lua ?
    static int lIterateQuery(lua_State* a1);
};

namespace cvLuaNetworkLibrary {
/// 0x0895e844
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x0895e882
int pRegister(lua_State* a1);
/// 0x0895f8c6
/// lua ?
int lConnect(lua_State* a1);
/// 0x0895f920
/// lua ?
int lSendChangeWar(lua_State* a1);
/// 0x0895f98a
/// lua ?
int lSendPledgeMinorProtection(lua_State* a1);
/// 0x0895f9f4
/// lua ?
int lSendMinorNoUnitSpawning(lua_State* a1);
/// 0x0895fa5e
/// lua ?
int lSendLiberateMinor(lua_State* a1);
/// 0x0895fac0
/// lua ?
int lSendResearch(lua_State* a1);
/// 0x0895fb5e
/// lua ?
int lSendDoTask(lua_State* a1);
/// 0x0895fc78
/// lua ?
int lSendCityBuyPlot(lua_State* a1);
/// 0x0895fcf2
/// lua ?
int lSendSetCityAIFocus(lua_State* a1);
/// 0x0895fd54
/// lua ?
int lSendSetCityAvoidGrowth(lua_State* a1);
/// 0x0895fdbe
/// lua ?
int lSendMinorCivEnterTerritory(lua_State* a1);
/// 0x0895fe06
/// lua ?
int lSendUpdatePolicies(lua_State* a1);
/// 0x0895fe90
/// lua ?
int lSendDiploVote(lua_State* a1);
/// 0x0895fed8
/// lua ?
int lSendBarbarianRansom(lua_State* a1);
/// 0x0895ff3a
/// lua ?
int lSendGiftUnit(lua_State* a1);
/// 0x0895ff9c
/// lua ?
int lSendReturnCivilian(lua_State* a1);
/// 0x0896001e
/// lua ?
int lSendGreatPersonChoice(lua_State* a1);
/// 0x08960080
/// lua ?
int lSendMayaBonusChoice(lua_State* a1);
/// 0x089600e2
/// lua ?
int lSendFaithGreatPersonChoice(lua_State* a1);
/// 0x08960144
/// lua ?
int lSendGoodyChoice(lua_State* a1);
/// 0x089601f6
/// lua ?
int lSendArchaeologyChoice(lua_State* a1);
/// 0x08960270
/// lua ?
int lSendIdeologyChoice(lua_State* a1);
/// 0x089602d2
/// lua ?
int lSendExtendedGame(lua_State* a1);
/// 0x08960300
/// lua ?
int lSendFoundPantheon(lua_State* a1);
/// 0x08960360
/// lua ?
int lSendFoundReligion(lua_State* a1);
/// 0x08960488
/// lua ?
int lSendEnhanceReligion(lua_State* a1);
/// 0x08960578
/// lua ?
int lSendMoveSpy(lua_State* a1);
/// 0x08960630
/// lua ?
int lSendStageCoup(lua_State* a1);
/// 0x08960690
/// lua ?
int lSendFaithPurchase(lua_State* a1);
/// 0x08960708
/// lua ?
int lSendLeagueVoteEnact(lua_State* a1);
/// 0x089607ba
/// lua ?
int lSendLeagueVoteRepeal(lua_State* a1);
/// 0x0896086c
/// lua ?
int lSendLeagueVoteAbstain(lua_State* a1);
/// 0x089608e6
/// lua ?
int lSendLeagueProposeEnact(lua_State* a1);
/// 0x0896097c
/// lua ?
int lSendLeagueProposeRepeal(lua_State* a1);
/// 0x089609f6
/// lua ?
int lSendLeagueEditName(lua_State* a1);
/// 0x08960a78
/// lua ?
int lSendIgnoreWarning(lua_State* a1);
/// 0x08960ac0
/// lua ?
int lSendSetSwappableGreatWork(lua_State* a1);
/// 0x08960b3a
/// lua ?
int lSendSwapGreatWorks(lua_State* a1);
/// 0x08960bd0
/// lua ?
int lSendMoveGreatWorks(lua_State* a1);
/// 0x08960cba
/// lua ?
int lSendChangeIdeology(lua_State* a1);
/// 0x08960cea
/// lua ?
int lSendUpdateCityCitizens(lua_State* a1);
/// 0x08960d32
/// lua ?
int lSendSellBuilding(lua_State* a1);
/// 0x08960d94
/// lua ?
int lSendRenameCity(lua_State* a1);
/// 0x08960e60
/// lua ?
int lSendRenameUnit(lua_State* a1);
/// 0x08960f2c
/// lua ?
int lSendTurnUnready(lua_State* a1);
/// 0x08960f74
/// lua ?
int lHasSentNetTurnComplete(lua_State* a1);
/// 0x08960fbc
/// lua ?
int lHasSentNetTurnAllComplete(lua_State* a1);
/// 0x08961004
/// lua ?
int lGetSynchRandSeed(lua_State* a1);
/// 0x0896108e
/// lua ?
int lGetMapRandSeed(lua_State* a1);
/// 0x08961118
/// lua ?
int lParseURL(lua_State* a1);
/// 0x08961422
/// lua ?
int lURLEncode(lua_State* a1);
/// 0x0896154c
/// lua ?
int lURLDecode(lua_State* a1);
/// 0x08961676
/// lua ?
int lBroadcastGameSettings(lua_State* a1);
/// 0x089616cc
/// lua ?
int lBroadcastPlayerInfo(lua_State* a1);
/// 0x08961824
/// lua ?
int lSendGameOptions(lua_State* a1);
/// 0x08961b5c
/// lua ?
int lSendChat(lua_State* a1);
/// 0x08961cd4
/// lua ?
int lHasReconnectCache(lua_State* a1);
/// 0x08961d02
/// lua ?
int lReconnect(lua_State* a1);
/// 0x08961d2a
/// lua ?
int lIsEveryoneConnected(lua_State* a1);
/// 0x08961d6a
/// lua ?
int lIsPlayerConnected(lua_State* a1);
/// 0x08961dc2
/// lua ?
int lIsPlayerHotJoining(lua_State* a1);
/// 0x08961e16
/// lua ?
int lIsPlayerKicked(lua_State* a1);
/// 0x08961e6a
/// lua ?
int lIsConnectedToSteam(lua_State* a1);
/// 0x08961ec4
/// lua ?
int lCloseConnection(lua_State* a1);
/// 0x08961ec8
/// lua ?
int lForceResync(lua_State* a1);
/// 0x08961ecc
/// lua ?
int lQueueForceResync(lua_State* a1);
/// 0x08961ed0
/// lua ?
int lGetPingTime(lua_State* a1);
/// 0x08961f20
/// lua ?
int lGetDebugLogLevel(lua_State* a1);
/// 0x08961f5c
/// lua ?
int lSetDebugLogLevel(lua_State* a1);
/// 0x08961f9c
/// lua ?
int lGetPlayerTurnSliceInfo(lua_State* a1);
/// 0x08962088
/// lua ?
int lGetLocalTurnSliceInfo(lua_State* a1);
/// 0x08962170
/// lua ?
int lGetTurnSliceMaxMessageCount(lua_State* a1);
/// 0x089621a2
/// lua ?
int lSetTurnSliceMaxMessageCount(lua_State* a1);
/// 0x0896222c
/// lua ?
int lIsDedicatedServer(lua_State* a1);
/// 0x08962266
/// lua ?
int lAutoHost(lua_State* a1);
/// 0x089622a0
/// lua ?
int lAutoReadyHost(lua_State* a1);
/// 0x089622da
/// lua ?
int lSetPlayerDesiredSlot(lua_State* a1);
/// 0x0896231c
/// lua ?
int lGetPlayerDesiredSlot(lua_State* a1);
}

namespace cvLuaOnlyEventLibrary {
/// 0x08962448
int Register(lua_State* a1);
/// 0x08962486
int pRegister(lua_State* a1);
/// 0x08962536
int CreateLuaOnlyEventFactory(lua_State* a1);
/// 0x08962626
/// lua ?
int lEventFactoryIndexer(lua_State* a1);
}

namespace cvLuaOptionsManager {
/// 0x08962794
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x089627d2
int pRegister(lua_State* a1);
/// 0x0896506e
/// lua ?
int lGetHDStrategicView_Cached(lua_State* a1);
/// 0x089650a2
/// lua ?
int lSetHDStrategicView_Cached(lua_State* a1);
/// 0x089650d8
/// lua ?
int lGetGPUTextureDecode_Cached(lua_State* a1);
/// 0x0896510c
/// lua ?
int lSetGPUTextureDecode_Cached(lua_State* a1);
/// 0x08965144
/// lua ?
int lIsGPUTextureDecodeSupported(lua_State* a1);
/// 0x08965178
/// lua ?
int lGetOverlayLevel_Cached(lua_State* a1);
/// 0x089651aa
/// lua ?
int lSetOverlayLevel_Cached(lua_State* a1);
/// 0x089651de
/// lua ?
int lGetShadowLevel_Cached(lua_State* a1);
/// 0x08965210
/// lua ?
int lSetShadowLevel_Cached(lua_State* a1);
/// 0x08965244
/// lua ?
int lGetFOWLevel_Cached(lua_State* a1);
/// 0x08965276
/// lua ?
int lSetFOWLevel_Cached(lua_State* a1);
/// 0x089652aa
/// lua ?
int lGetTerrainDetailLevel_Cached(lua_State* a1);
/// 0x089652dc
/// lua ?
int lSetTerrainDetailLevel_Cached(lua_State* a1);
/// 0x08965310
/// lua ?
int lGetTerrainTessLevel_Cached(lua_State* a1);
/// 0x08965342
/// lua ?
int lSetTerrainTessLevel_Cached(lua_State* a1);
/// 0x08965376
/// lua ?
int lGetTerrainShadowQuality_Cached(lua_State* a1);
/// 0x089653a8
/// lua ?
int lSetTerrainShadowQuality_Cached(lua_State* a1);
/// 0x089653dc
/// lua ?
int lGetWaterQuality_Cached(lua_State* a1);
/// 0x0896540e
/// lua ?
int lSetWaterQuality_Cached(lua_State* a1);
/// 0x08965442
/// lua ?
int lGetTextureQuality_Cached(lua_State* a1);
/// 0x08965474
/// lua ?
int lSetTextureQuality_Cached(lua_State* a1);
/// 0x089654a8
/// lua ?
int lGetNumContinents_Cached(lua_State* a1);
/// 0x089654da
/// lua ?
int lSetNumContinents_Cached(lua_State* a1);
/// 0x0896550e
/// lua ?
int lGetVSync_Cached(lua_State* a1);
/// 0x08965542
/// lua ?
int lSetVSync_Cached(lua_State* a1);
/// 0x08965578
/// lua ?
int lGetMinimizeGrayTiles_Cached(lua_State* a1);
/// 0x089655ac
/// lua ?
int lSetMinimizeGrayTiles_Cached(lua_State* a1);
/// 0x089655e2
/// lua ?
int lGetFadeShadows_Cached(lua_State* a1);
/// 0x08965616
/// lua ?
int lSetFadeShadows_Cached(lua_State* a1);
/// 0x0896564c
/// lua ?
int lGetLeaderQuality_Cached(lua_State* a1);
/// 0x08965680
/// lua ?
int lSetLeaderQuality_Cached(lua_State* a1);
/// 0x089656be
/// lua ?
int lResetDefaultGraphicsOptions(lua_State* a1);
/// 0x089656da
/// lua ?
int lHasUserChangedGraphicsOptions(lua_State* a1);
/// 0x0896570e
/// lua ?
int lHasMaxGraphicsOptions(lua_State* a1);
/// 0x08965742
/// lua ?
int lWarnUserMaxGraphicsOptions(lua_State* a1);
/// 0x08965762
/// lua ?
int lGetGraphicsProfile(lua_State* a1);
/// 0x08965794
/// lua ?
int lGetAASamples_Cached(lua_State* a1);
/// 0x089657c6
/// lua ?
int lSetAASamples_Cached(lua_State* a1);
/// 0x089657fa
/// lua ?
int lIsAALevelSupported(lua_State* a1);
/// 0x0896583e
/// lua ?
int lGetFullscreen(lua_State* a1);
/// 0x08965872
/// lua ?
int lGetFullscreen_Cached(lua_State* a1);
/// 0x089658a6
/// lua ?
int lSetFullscreen_Cached(lua_State* a1);
/// 0x089658dc
/// lua ?
int lGetResolution_Cached(lua_State* a1);
/// 0x0896590e
/// lua ?
int lSetResolution_Cached(lua_State* a1);
/// 0x08965942
/// lua ?
int lGetWindowResolution_Cached(lua_State* a1);
/// 0x08965988
/// lua ?
int lSetWindowResolution_Cached(lua_State* a1);
/// 0x089659d4
/// lua ?
int lHasUserChangedResolution(lua_State* a1);
/// 0x08965a08
/// lua ?
int lGetMaxResolution(lua_State* a1);
/// 0x08965a58
/// lua ?
int lMacIsResolutionHiDPI(lua_State* a1);
/// 0x08965ac0
/// lua ?
int lASLIsSteamBigPicture(lua_State* a1);
/// 0x08965af4
/// lua ?
int lGetPolicyInfo(lua_State* a1);
/// 0x08965b28
/// lua ?
int lGetPolicyInfo_Cached(lua_State* a1);
/// 0x08965b5c
/// lua ?
int lSetPolicyInfo_Cached(lua_State* a1);
/// 0x08965b94
/// lua ?
int lGetSmallUIAssets(lua_State* a1);
/// 0x08965bc8
/// lua ?
int lGetSmallUIAssets_Cached(lua_State* a1);
/// 0x08965bfc
/// lua ?
int lSetSmallUIAssets_Cached(lua_State* a1);
/// 0x08965c34
/// lua ?
int lGetAutoUIAssets(lua_State* a1);
/// 0x08965c68
/// lua ?
int lGetAutoUIAssets_Cached(lua_State* a1);
/// 0x08965c9c
/// lua ?
int lSetAutoUIAssets_Cached(lua_State* a1);
/// 0x08965cd4
/// lua ?
int lGetAutoUnitCycle(lua_State* a1);
/// 0x08965d08
/// lua ?
int lGetAutoUnitCycle_Cached(lua_State* a1);
/// 0x08965d3c
/// lua ?
int lSetAutoUnitCycle_Cached(lua_State* a1);
/// 0x08965d74
/// lua ?
int lGetGridOn(lua_State* a1);
/// 0x08965da8
/// lua ?
int lGetGridOn_Cached(lua_State* a1);
/// 0x08965ddc
/// lua ?
int lSetGridOn_Cached(lua_State* a1);
/// 0x08965e14
/// lua ?
int lGetShowTradeOn(lua_State* a1);
/// 0x08965e48
/// lua ?
int lGetShowTradeOn_Cached(lua_State* a1);
/// 0x08965e7c
/// lua ?
int lSetShowTradeOn_Cached(lua_State* a1);
/// 0x08965eb4
/// lua ?
int lGetScoreList(lua_State* a1);
/// 0x08965ee8
/// lua ?
int lGetScoreList_Cached(lua_State* a1);
/// 0x08965f1c
/// lua ?
int lSetScoreList_Cached(lua_State* a1);
/// 0x08965f54
/// lua ?
int lGetMPScoreList(lua_State* a1);
/// 0x08965f88
/// lua ?
int lGetMPScoreList_Cached(lua_State* a1);
/// 0x08965fbc
/// lua ?
int lSetMPScoreList_Cached(lua_State* a1);
/// 0x08965ff4
/// lua ?
int lGetResourceOn(lua_State* a1);
/// 0x08966028
/// lua ?
int lGetResourceOn_Cached(lua_State* a1);
/// 0x0896605c
/// lua ?
int lSetResourceOn_Cached(lua_State* a1);
/// 0x08966094
/// lua ?
int lGetYieldOn(lua_State* a1);
/// 0x089660c8
/// lua ?
int lGetYieldOn_Cached(lua_State* a1);
/// 0x089660fc
/// lua ?
int lSetYieldOn_Cached(lua_State* a1);
/// 0x08966134
/// lua ?
int lGetHideTouchHelp(lua_State* a1);
/// 0x08966168
/// lua ?
int lGetHideTouchHelp_Cached(lua_State* a1);
/// 0x0896619c
/// lua ?
int lSetHideTouchHelp_Cached(lua_State* a1);
/// 0x089661d4
/// lua ?
int lIsAutoWorkersDontReplace(lua_State* a1);
/// 0x08966208
/// lua ?
int lIsAutoWorkersDontReplace_Cached(lua_State* a1);
/// 0x0896623c
/// lua ?
int lSetAutoWorkersDontReplace_Cached(lua_State* a1);
/// 0x08966274
/// lua ?
int lIsAutoWorkersDontRemoveFeatures(lua_State* a1);
/// 0x089662a8
/// lua ?
int lIsAutoWorkersDontRemoveFeatures_Cached(lua_State* a1);
/// 0x089662dc
/// lua ?
int lSetAutoWorkersDontRemoveFeatures_Cached(lua_State* a1);
/// 0x08966314
/// lua ?
int lIsNoCitizenWarning(lua_State* a1);
/// 0x08966348
/// lua ?
int lIsNoCitizenWarning_Cached(lua_State* a1);
/// 0x0896637c
/// lua ?
int lSetNoCitizenWarning_Cached(lua_State* a1);
/// 0x089663b4
/// lua ?
int lGetTutorialLevel(lua_State* a1);
/// 0x089663e6
/// lua ?
int lGetTutorialLevel_Cached(lua_State* a1);
/// 0x08966418
/// lua ?
int lSetTutorialLevel_Cached(lua_State* a1);
/// 0x0896644c
/// lua ?
int lIsNoRewardPopups(lua_State* a1);
/// 0x08966480
/// lua ?
int lIsNoRewardPopups_Cached(lua_State* a1);
/// 0x089664b4
/// lua ?
int lSetNoRewardPopups_Cached(lua_State* a1);
/// 0x089664ec
/// lua ?
int lIsNoTileRecommendations(lua_State* a1);
/// 0x08966520
/// lua ?
int lIsNoTileRecommendations_Cached(lua_State* a1);
/// 0x08966554
/// lua ?
int lSetNoTileRecommendations_Cached(lua_State* a1);
/// 0x0896658c
/// lua ?
int lIsCivilianYields(lua_State* a1);
/// 0x089665c0
/// lua ?
int lIsCivilianYields_Cached(lua_State* a1);
/// 0x089665f4
/// lua ?
int lSetCivilianYields_Cached(lua_State* a1);
/// 0x0896662c
/// lua ?
int lIsNoBasicHelp(lua_State* a1);
/// 0x08966660
/// lua ?
int lIsNoBasicHelp_Cached(lua_State* a1);
/// 0x08966694
/// lua ?
int lSetNoBasicHelp_Cached(lua_State* a1);
/// 0x089666cc
/// lua ?
int lGetQuickSelectionAdvanceEnabled(lua_State* a1);
/// 0x08966700
/// lua ?
int lGetQuickSelectionAdvanceEnabled_Cached(lua_State* a1);
/// 0x08966734
/// lua ?
int lSetQuickSelectionAdvanceEnabled_Cached(lua_State* a1);
/// 0x0896676c
/// lua ?
int lGetLANNickName(lua_State* a1);
/// 0x089667a8
/// lua ?
int lGetLANNickName_Cached(lua_State* a1);
/// 0x089667e4
/// lua ?
int lSetLANNickName_Cached(lua_State* a1);
/// 0x089668a8
/// lua ?
int lGetTurnNotifySteamInvite(lua_State* a1);
/// 0x089668dc
/// lua ?
int lGetTurnNotifySteamInvite_Cached(lua_State* a1);
/// 0x08966910
/// lua ?
int lSetTurnNotifySteamInvite_Cached(lua_State* a1);
/// 0x08966948
/// lua ?
int lGetTurnNotifyEmail(lua_State* a1);
/// 0x0896697c
/// lua ?
int lGetTurnNotifyEmail_Cached(lua_State* a1);
/// 0x089669b0
/// lua ?
int lSetTurnNotifyEmail_Cached(lua_State* a1);
/// 0x089669e8
/// lua ?
int lGetTurnNotifyEmailAddress(lua_State* a1);
/// 0x08966a24
/// lua ?
int lGetTurnNotifyEmailAddress_Cached(lua_State* a1);
/// 0x08966a60
/// lua ?
int lSetTurnNotifyEmailAddress_Cached(lua_State* a1);
/// 0x08966b24
/// lua ?
int lGetTurnNotifySmtpEmailAddress(lua_State* a1);
/// 0x08966b60
/// lua ?
int lGetTurnNotifySmtpEmailAddress_Cached(lua_State* a1);
/// 0x08966b9c
/// lua ?
int lSetTurnNotifySmtpEmailAddress_Cached(lua_State* a1);
/// 0x08966c60
/// lua ?
int lGetTurnNotifySmtpHost(lua_State* a1);
/// 0x08966c9e
/// lua ?
int lGetTurnNotifySmtpHost_Cached(lua_State* a1);
/// 0x08966cdc
/// lua ?
int lSetTurnNotifySmtpHost_Cached(lua_State* a1);
/// 0x08966da0
/// lua ?
int lGetTurnNotifySmtpPort(lua_State* a1);
/// 0x08966dd6
/// lua ?
int lGetTurnNotifySmtpPort_Cached(lua_State* a1);
/// 0x08966e0c
/// lua ?
int lSetTurnNotifySmtpPort_Cached(lua_State* a1);
/// 0x08966e44
/// lua ?
int lGetTurnNotifySmtpUsername(lua_State* a1);
/// 0x08966e88
/// lua ?
int lGetTurnNotifySmtpUsername_Cached(lua_State* a1);
/// 0x08966ecc
/// lua ?
int lSetTurnNotifySmtpUsername_Cached(lua_State* a1);
/// 0x08966f9c
/// lua ?
int lGetTurnNotifySmtpPassword_Cached(lua_State* a1);
/// 0x0896700c
/// lua ?
int lSetTurnNotifySmtpPassword_Cached(lua_State* a1);
/// 0x089670dc
/// lua ?
int lGetTurnNotifySmtpTLS(lua_State* a1);
/// 0x08967114
/// lua ?
int lGetTurnNotifySmtpTLS_Cached(lua_State* a1);
/// 0x0896714c
/// lua ?
int lSetTurnNotifySmtpTLS_Cached(lua_State* a1);
/// 0x08967186
/// lua ?
int lGetStraightZoom(lua_State* a1);
/// 0x089671ba
/// lua ?
int lGetStraightZoom_Cached(lua_State* a1);
/// 0x089671ee
/// lua ?
int lSetStraightZoom_Cached(lua_State* a1);
/// 0x08967226
/// lua ?
int lGetBindMouseMode(lua_State* a1);
/// 0x0896726e
/// lua ?
int lGetBindMouseMode_Cached(lua_State* a1);
/// 0x089672b6
/// lua ?
int lSetBindMouseMode_Cached(lua_State* a1);
/// 0x08967316
/// lua ?
int lGetTooltip1Seconds(lua_State* a1);
/// 0x0896735e
/// lua ?
int lGetTooltip1Seconds_Cached(lua_State* a1);
/// 0x089673a6
/// lua ?
int lSetTooltip1Seconds_Cached(lua_State* a1);
/// 0x08967408
/// lua ?
int lGetTooltip2Seconds(lua_State* a1);
/// 0x08967450
/// lua ?
int lGetTooltip2Seconds_Cached(lua_State* a1);
/// 0x08967498
/// lua ?
int lSetTooltip2Seconds_Cached(lua_State* a1);
/// 0x089674fa
/// lua ?
int lIsDebugMode(lua_State* a1);
/// 0x0896752e
/// lua ?
int lIsDebugMode_Cached(lua_State* a1);
/// 0x08967562
/// lua ?
int lSetDebugMode_Cached(lua_State* a1);
/// 0x0896759a
/// lua ?
int lGetTurnsBetweenAutosave(lua_State* a1);
/// 0x089675e2
/// lua ?
int lGetTurnsBetweenAutosave_Cached(lua_State* a1);
/// 0x0896762a
/// lua ?
int lSetTurnsBetweenAutosave_Cached(lua_State* a1);
/// 0x0896768c
/// lua ?
int lGetNumAutosavesKept(lua_State* a1);
/// 0x089676d4
/// lua ?
int lGetNumAutosavesKept_Cached(lua_State* a1);
/// 0x0896771c
/// lua ?
int lSetNumAutosavesKept_Cached(lua_State* a1);
/// 0x0896777e
/// lua ?
int lGetSinglePlayerAutoEndTurnEnabled(lua_State* a1);
/// 0x089677b2
/// lua ?
int lGetSinglePlayerAutoEndTurnEnabled_Cached(lua_State* a1);
/// 0x089677e6
/// lua ?
int lSetSinglePlayerAutoEndTurnEnabled_Cached(lua_State* a1);
/// 0x0896781e
/// lua ?
int lGetMultiplayerAutoEndTurnEnabled(lua_State* a1);
/// 0x08967852
/// lua ?
int lGetMultiplayerAutoEndTurnEnabled_Cached(lua_State* a1);
/// 0x08967886
/// lua ?
int lSetMultiplayerAutoEndTurnEnabled_Cached(lua_State* a1);
/// 0x089678be
/// lua ?
int lGetSinglePlayerQuickCombatEnabled(lua_State* a1);
/// 0x089678f2
/// lua ?
int lGetSinglePlayerQuickCombatEnabled_Cached(lua_State* a1);
/// 0x08967926
/// lua ?
int lSetSinglePlayerQuickCombatEnabled_Cached(lua_State* a1);
/// 0x0896795e
/// lua ?
int lGetSinglePlayerQuickMovementEnabled(lua_State* a1);
/// 0x08967992
/// lua ?
int lGetSinglePlayerQuickMovementEnabled_Cached(lua_State* a1);
/// 0x089679c6
/// lua ?
int lSetSinglePlayerQuickMovementEnabled_Cached(lua_State* a1);
/// 0x089679fe
/// lua ?
int lGetMultiplayerQuickCombatEnabled(lua_State* a1);
/// 0x08967a32
/// lua ?
int lGetMultiplayerQuickCombatEnabled_Cached(lua_State* a1);
/// 0x08967a66
/// lua ?
int lSetMultiplayerQuickCombatEnabled_Cached(lua_State* a1);
/// 0x08967a9e
/// lua ?
int lGetMultiplayerQuickMovementEnabled(lua_State* a1);
/// 0x08967ad2
/// lua ?
int lGetMultiplayerQuickMovementEnabled_Cached(lua_State* a1);
/// 0x08967b06
/// lua ?
int lSetMultiplayerQuickMovementEnabled_Cached(lua_State* a1);
/// 0x08967b3e
/// lua ?
int lGetEnableMapInertia(lua_State* a1);
/// 0x08967b72
/// lua ?
int lGetEnableMapInertia_Cached(lua_State* a1);
/// 0x08967ba6
/// lua ?
int lSetEnableMapInertia_Cached(lua_State* a1);
/// 0x08967bde
/// lua ?
int lGetSkipIntroVideo(lua_State* a1);
/// 0x08967c12
/// lua ?
int lGetSkipIntroVideo_Cached(lua_State* a1);
/// 0x08967c46
/// lua ?
int lSetSkipIntroVideo_Cached(lua_State* a1);
/// 0x08967c7e
/// lua ?
int lGetDragSpeed(lua_State* a1);
/// 0x08967cb6
/// lua ?
int lGetDragSpeed_Cached(lua_State* a1);
/// 0x08967cee
/// lua ?
int lSetDragSpeed_Cached(lua_State* a1);
/// 0x08967d22
/// lua ?
int lGetPinchSpeed(lua_State* a1);
/// 0x08967d5a
/// lua ?
int lGetPinchSpeed_Cached(lua_State* a1);
/// 0x08967d92
/// lua ?
int lSetPinchSpeed_Cached(lua_State* a1);
/// 0x08967dc6
/// lua ?
int lResetTutorial(lua_State* a1);
/// 0x08967de2
/// lua ?
int lCommitGameOptions(lua_State* a1);
/// 0x08967e30
/// lua ?
int lSyncGameOptionsCache(lua_State* a1);
/// 0x08967e4c
/// lua ?
int lCommitGraphicsOptions(lua_State* a1);
/// 0x08967e68
/// lua ?
int lSyncGraphicsOptionsCache(lua_State* a1);
/// 0x08967e84
/// lua ?
int lCommitResolutionOptions(lua_State* a1);
/// 0x08967ea0
/// lua ?
int lSyncResolutionOptionsCache(lua_State* a1);
/// 0x08967ebc
/// lua ?
int lResetDefaultGameOptions(lua_State* a1);
}

namespace cvLuaPathLibrary {
/// 0x08967ee0
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x08967f1e
int pRegister(lua_State* a1);
/// 0x08968192
/// lua ?
int lGetDirectoryName(lua_State* a1);
/// 0x08968206
/// lua ?
int lGetExtension(lua_State* a1);
/// 0x0896827a
/// lua ?
int lGetFileName(lua_State* a1);
/// 0x089682ee
/// lua ?
int lGetFileNameWithoutExtension(lua_State* a1);
/// 0x08968362
/// lua ?
int lGetAltDirectorySeparatorChar(lua_State* a1);
/// 0x089683a0
/// lua ?
int lGetDirectorySeparatorChar(lua_State* a1);
/// 0x089683de
/// lua ?
int lGetExtensionSeparatorChar(lua_State* a1);
/// 0x0896841c
/// lua ?
int lGetPathSeparatorChar(lua_State* a1);
/// 0x0896845a
/// lua ?
int lGetVolumeSeparatorChar(lua_State* a1);
/// 0x08968498
/// lua ?
int lNormalizePath(lua_State* a1);
/// 0x08968556
/// lua ?
int lUsesExtension(lua_State* a1);
}

namespace cvLuaPreGame {
/// 0x08968614
int Register(lua_State* a1);
/// 0x08968648
int pRegister(lua_State* a1);
/// 0x08969bc8
/// lua ?
int lGetCivilization(lua_State* a1);
/// 0x08969c68
/// lua ?
int lSetCivilization(lua_State* a1);
/// 0x08969d18
/// lua ?
int lGetMultiplayerAIEnabled(lua_State* a1);
/// 0x08969da6
/// lua ?
int lSetMultiplayerAIEnabled(lua_State* a1);
/// 0x08969e42
/// lua ?
int lGetMapScript(lua_State* a1);
/// 0x08969f12
/// lua ?
int lSetMapScript(lua_State* a1);
/// 0x08969ffe
/// lua ?
int lRandomizeMapSeed(lua_State* a1);
/// 0x0896a080
/// lua ?
int lGetLoadWBScenario(lua_State* a1);
/// 0x0896a10e
/// lua ?
int lSetLoadWBScenario(lua_State* a1);
/// 0x0896a1aa
/// lua ?
int lGetOverrideScenarioHandicap(lua_State* a1);
/// 0x0896a238
/// lua ?
int lSetOverrideScenarioHandicap(lua_State* a1);
/// 0x0896a2d4
/// lua ?
int lIsRandomMapScript(lua_State* a1);
/// 0x0896a362
/// lua ?
int lSetRandomMapScript(lua_State* a1);
/// 0x0896a3fe
/// lua ?
int lGetGameSpeed(lua_State* a1);
/// 0x0896a488
/// lua ?
int lSetGameSpeed(lua_State* a1);
/// 0x0896a51c
/// lua ?
int lGetWorldSize(lua_State* a1);
/// 0x0896a5a6
/// lua ?
int lSetWorldSize(lua_State* a1);
/// 0x0896a642
/// lua ?
int lIsEarthMap(lua_State* a1);
/// 0x0896a670
/// lua ?
int lSetEarthMap(lua_State* a1);
/// 0x0896a674
/// lua ?
int lIsRandomWorldSize(lua_State* a1);
/// 0x0896a702
/// lua ?
int lSetRandomWorldSize(lua_State* a1);
/// 0x0896a79e
/// lua ?
int lGetHandicap(lua_State* a1);
/// 0x0896a840
/// lua ?
int lSetHandicap(lua_State* a1);
/// 0x0896a8f0
/// lua ?
int lGetTeam(lua_State* a1);
/// 0x0896a992
/// lua ?
int lSetTeam(lua_State* a1);
/// 0x0896aa42
/// lua ?
int lGetLeaderName(lua_State* a1);
/// 0x0896ab2a
/// lua ?
int lSetLeaderName(lua_State* a1);
/// 0x0896ac2a
/// lua ?
int lGetLeaderType(lua_State* a1);
/// 0x0896acce
/// lua ?
int lSetLeaderType(lua_State* a1);
/// 0x0896ad78
/// lua ?
int lSetPlayerColor(lua_State* a1);
/// 0x0896ae22
/// lua ?
int lGetLeaderKey(lua_State* a1);
/// 0x0896af0c
/// lua ?
int lSetLeaderKey(lua_State* a1);
/// 0x0896b00c
/// lua ?
int lIsLeaderKeyAvailable(lua_State* a1);
/// 0x0896b0b2
/// lua ?
int lGetLeaderPackageID(lua_State* a1);
/// 0x0896b22a
/// lua ?
int lSetLeaderPackageID(lua_State* a1);
/// 0x0896b34c
/// lua ?
int lGetLeaderPackageTextKey(lua_State* a1);
/// 0x0896b53a
/// lua ?
int lGetCivilizationDescription(lua_State* a1);
/// 0x0896b620
/// lua ?
int lSetCivilizationDescription(lua_State* a1);
/// 0x0896b720
/// lua ?
int lGetCivilizationShortDescription(lua_State* a1);
/// 0x0896b806
/// lua ?
int lSetCivilizationShortDescription(lua_State* a1);
/// 0x0896b906
/// lua ?
int lGetCivilizationAdjective(lua_State* a1);
/// 0x0896b9ec
/// lua ?
int lSetCivilizationAdjective(lua_State* a1);
/// 0x0896baec
/// lua ?
int lGetCivilizationColor(lua_State* a1);
/// 0x0896bb90
/// lua ?
int lGetCivilizationKey(lua_State* a1);
/// 0x0896bc76
/// lua ?
int lSetCivilizationKey(lua_State* a1);
/// 0x0896bd76
/// lua ?
int lIsCivilizationKeyAvailable(lua_State* a1);
/// 0x0896be18
/// lua ?
int lGetCivilizationPackageID(lua_State* a1);
/// 0x0896bf8e
/// lua ?
int lSetCivilizationPackageID(lua_State* a1);
/// 0x0896c0b4
/// lua ?
int lGetCivilizationPackageTextKey(lua_State* a1);
/// 0x0896c2cc
/// lua ?
int lGetPassword(lua_State* a1);
/// 0x0896c3b2
/// lua ?
int lSetPassword(lua_State* a1);
/// 0x0896c746
/// lua ?
int lHasPassword(lua_State* a1);
/// 0x0896c834
/// lua ?
int lTestPassword(lua_State* a1);
/// 0x0896cae0
/// lua ?
int lGetLoadFileName(lua_State* a1);
/// 0x0896ccec
/// lua ?
int lSetLoadFileName(lua_State* a1);
/// 0x0896d2a6
/// lua ?
int lGetNickName(lua_State* a1);
/// 0x0896d38e
/// lua ?
int lSetNickName(lua_State* a1);
/// 0x0896d48e
/// lua ?
int lGetSlotStatus(lua_State* a1);
/// 0x0896d530
/// lua ?
int lSetSlotStatus(lua_State* a1);
/// 0x0896d5e0
/// lua ?
int lGetSlotClaim(lua_State* a1);
/// 0x0896d682
/// lua ?
int lSetSlotClaim(lua_State* a1);
/// 0x0896d732
/// lua ?
int lGetMaxTurns(lua_State* a1);
/// 0x0896d7bc
/// lua ?
int lSetMaxTurns(lua_State* a1);
/// 0x0896d850
/// lua ?
int lGetPitbossTurnTime(lua_State* a1);
/// 0x0896d90a
/// lua ?
int lSetPitbossTurnTime(lua_State* a1);
/// 0x0896d9c6
/// lua ?
int lGetEra(lua_State* a1);
/// 0x0896da54
/// lua ?
int lSetEra(lua_State* a1);
/// 0x0896dae8
/// lua ?
int lGetNumMinorCivs(lua_State* a1);
/// 0x0896db78
/// lua ?
int lSetNumMinorCivs(lua_State* a1);
/// 0x0896dc0c
/// lua ?
int lSetPrivateGame(lua_State* a1);
/// 0x0896dca8
/// lua ?
int lIsPrivateGame(lua_State* a1);
/// 0x0896dd36
/// lua ?
int lGetPersistSettings(lua_State* a1);
/// 0x0896ddc4
/// lua ?
int lSetPersistSettings(lua_State* a1);
/// 0x0896de60
/// lua ?
int lLoadPreGameSettings(lua_State* a1);
/// 0x0896de88
/// lua ?
int lSetReady(lua_State* a1);
/// 0x0896df40
/// lua ?
int lIsReady(lua_State* a1);
/// 0x0896dfe6
/// lua ?
int lCanReadyLocalPlayer(lua_State* a1);
/// 0x0896e070
/// lua ?
int lSetInternetGame(lua_State* a1);
/// 0x0896e10c
/// lua ?
int lIsInternetGame(lua_State* a1);
/// 0x0896e19a
/// lua ?
int lSetMultiplayerGame(lua_State* a1);
/// 0x0896e232
/// lua ?
int lIsMultiplayerGame(lua_State* a1);
/// 0x0896e2c0
/// lua ?
int lIsHotSeatGame(lua_State* a1);
/// 0x0896e34e
/// lua ?
int lSetGameType(lua_State* a1);
/// 0x0896e3e0
/// lua ?
int lGetGameType(lua_State* a1);
/// 0x0896e470
/// lua ?
int lGetGameOption(lua_State* a1);
/// 0x0896e5ea
/// lua ?
int lSetGameOption(lua_State* a1);
/// 0x0896e6c2
/// lua ?
int lResetGameOptions(lua_State* a1);
/// 0x0896e73a
/// lua ?
int lGetMapOption(lua_State* a1);
/// 0x0896e80a
/// lua ?
int lSetMapOption(lua_State* a1);
/// 0x0896e8e2
/// lua ?
int lResetMapOptions(lua_State* a1);
/// 0x0896e95a
/// lua ?
int lResetSlots(lua_State* a1);
/// 0x0896e9d2
/// lua ?
int lReset(lua_State* a1);
/// 0x0896eea0
/// lua ?
int lGetFileHeader(lua_State* a1);
/// 0x0896f520
/// lua ?
int lIsVictory(lua_State* a1);
/// 0x0896f5d0
/// lua ?
int lSetVictory(lua_State* a1);
/// 0x0896f684
/// lua ?
int lSetQuickCombat(lua_State* a1);
/// 0x0896f720
/// lua ?
int lGetQuickCombat(lua_State* a1);
/// 0x0896f7b8
/// lua ?
int lSetQuickMovement(lua_State* a1);
/// 0x0896f854
/// lua ?
int lGetQuickMovement(lua_State* a1);
/// 0x0896f8ec
/// lua ?
int lReadActiveSlotCountFromSaveGame(lua_State* a1);
/// 0x0896fe0c
/// lua ?
int lIsDLCAllowed(lua_State* a1);
/// 0x0896ff3a
/// lua ?
int lSetDLCAllowed(lua_State* a1);
/// 0x08970084
/// lua ?
int lClearDLCAllowed(lua_State* a1);
/// 0x089700f8
/// lua ?
int lGameStarted(lua_State* a1);
}

namespace cvLuaProfilerLibrary {
/// 0x08970190
int Register(lua_State* a1);
/// 0x089701c4
int pRegister(lua_State* a1);
/// 0x08970328
/// lua ?
int lReportSamples(lua_State* a1);
/// 0x089704b8
/// lua ?
int lReportFPS(lua_State* a1);
/// 0x08970598
/// lua ?
int lBeginTiming(lua_State* a1);
/// 0x0897076a
/// lua ?
int lAssert(lua_State* a1);
/// 0x08970816
/// lua ?
int lGetEngineInfoRowString(lua_State* a1);
/// 0x08970aa8
/// lua ?
int lDestroyTimer(lua_State* a1);
/// 0x08970adc
/// lua ?
int lStopTiming(lua_State* a1);
/// 0x08970b32
UNK_RET FindInfo(char const* a1);
/// 0x08970bb4
UNK_RET RegisterInfo(cvLuaEngineInfo* a1);
/// 0x08970c34
UNK_RET Startup();
/// 0x08970c7e
UNK_RET Shutdown();
}
