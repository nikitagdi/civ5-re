/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/ICvDLLUtility.h"
#include "FirePlace/FireWorks/FFastList.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FMemoryStream.h"
#include <Windows.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class FNetMessage;

/// size ?
/// vtable 0x0a2fc8b0
class CvDLLUtility : public ICvEngineUtility4 {
public:
    /// 0x08794a00
    CvDLLUtility();
    /// 0x0879a068
    virtual void AutoSave(bool a1, bool a2);
    /// 0x087ab4e4
    virtual void BeginSendBundle();
    /// 0x08796518
    virtual bool CanAdvanceTurn();
    /// 0x08794f5c
    virtual void Destroy();
    /// 0x087a8a44
    virtual void DoClearDiplomacyTradeTable();
    /// 0x0879502c
    virtual void DoMapSetup(int a1);
    /// 0x087952e2
    virtual void DoTurn();
    /// 0x087ab534
    virtual void EndSendBundle();
    /// 0x08799e98
    virtual void EnumerateFiles(FFastList<char*, 21u, 0u>& a1, char const* a2, unsigned int a3, char const* a4, unsigned int a5, bool a6);
    /// 0x087ab45e
    virtual void FlushTurnReminders();
    /// 0x087aad70
    virtual void GameplayActivePlayerChanged(PlayerTypes a1);
    /// 0x087a8388
    virtual void GameplayBridgeChanged(bool a1, unsigned char a2);
    /// 0x0879e7b0
    virtual void GameplayCityCaptured(ICvCity1* a1, PlayerTypes a2);
    /// 0x0879ef1a
    virtual uint GameplayCityCombat(ICvCombatInfo1* a1);
    /// 0x0879e0be
    virtual void GameplayCityCreated(ICvCity1* a1, EraTypes a2);
    /// 0x0879e9f8
    virtual void GameplayCityDestroyed(ICvCity1* a1, PlayerTypes a2);
    /// 0x087a7c9a
    virtual void GameplayCityPopulationChanged(ICvCity1* a1, int a2);
    /// 0x0879ec40
    virtual void GameplayCitySetDamage(ICvCity1* a1, int a2, int a3);
    /// 0x087a8aea
    virtual void GameplayDiplomacyAILeaderMessage(PlayerTypes a1, DiploUIStateTypes a2, char const* a3, LeaderheadAnimationTypes a4, int a5);
    /// 0x087a7ac2
    virtual void GameplayDoFX(ICvPlot1* a1, int a2);
    /// 0x087a7f1a
    virtual void GameplayEraChanged(PlayerTypes a1, EraTypes a2);
    /// 0x087a5a32
    virtual void GameplayFOWChanged(int a1, int a2, int a3, bool a4);
    /// 0x087a4e0c
    virtual void GameplayFeatureChanged(ICvPlot1* a1, FeatureTypes a2);
    /// 0x087aa4b6
    virtual void GameplayGoldenAgeEnded();
    /// 0x087aa476
    virtual void GameplayGoldenAgeStarted();
    /// 0x087a8b44
    virtual void GameplayMetTeam(TeamTypes a1, TeamTypes a2);
    /// 0x087aa4f6
    virtual void GameplayMinimapNotification(int a1, int a2, int a3);
    /// 0x087aa3ba
    virtual void GameplayMinimapUnitSelect(int a1, int a2);
    /// 0x087a774a
    virtual void GameplayNaturalWonderRevealed(ICvPlot1* a1);
    /// 0x087aa1f4
    virtual void GameplayOpenInfoCorner(int a1);
    /// 0x087aa0d2
    virtual void GameplayOpenOptionsScreen();
    /// 0x087a724c
    virtual void GameplayPlotIconStateChange(ICvPlot1 const* a1, ResourceTypes a2, ImprovementTypes a3, int a4);
    /// 0x087a5c70
    virtual void GameplayPlotStateChange(ICvPlot1 const* a1, ResourceTypes a2, ImprovementTypes a3, int a4, RouteTypes a5, int a6);
    /// 0x087aa102
    virtual void GameplaySearchForPediaEntry(char const* a1);
    /// 0x087a991c
    virtual void GameplaySpaceshipCreated(ICvPlot1* a1, int a2);
    /// 0x087a9bc0
    virtual void GameplaySpaceshipEdited(ICvPlot1* a1, int a2);
    /// 0x087a9e76
    virtual void GameplaySpaceshipRemoved(ICvPlot1* a1);
    /// 0x087a59c8
    virtual void GameplayTechAcquired(TeamTypes a1, TechTypes a2);
    /// 0x087aad4e
    virtual void GameplayTurnChanged(int a1);
    /// 0x087a152c
    virtual void GameplayUnitActivate(ICvUnit1* a1);
    /// 0x0879ffd4
    virtual uint GameplayUnitCombat(ICvCombatInfo1* a1);
    /// 0x0879c6d4
    virtual void GameplayUnitCreated(ICvUnit1* a1);
    /// 0x087a0c0c
    virtual void GameplayUnitDestroyed(ICvUnit1* a1);
    /// 0x087a0e74
    virtual void GameplayUnitDestroyedInCombat(ICvUnit1* a1);
    /// 0x087a3924
    virtual void GameplayUnitEmbark(ICvUnit1* a1, bool a2);
    /// 0x087a1334
    virtual void GameplayUnitFortify(ICvUnit1* a1, bool a2);
    /// 0x087a46fa
    virtual void GameplayUnitGarrison(ICvUnit1* a1, bool a2);
    /// 0x087a4a6e
    virtual void GameplayUnitMarkThreatening(ICvUnit1* a1, bool a2);
    /// 0x087a29ce
    virtual void GameplayUnitMissionEnd(ICvUnit1* a1);
    /// 0x0879cf42
    virtual void GameplayUnitMoved(ICvUnit1* a1, FFastVector<int, true, 21u, 0u, BaseVector<int, true>::FDefaultFastVectorAllocator> const& a2);
    /// 0x087a2112
    virtual void GameplayUnitParadrop(ICvUnit1* a1);
    /// 0x0879d6a4
    virtual void GameplayUnitRebased(ICvUnit1* a1, ICvPlot1* a2, ICvPlot1* a3);
    /// 0x087a172c
    virtual void GameplayUnitResetAnimationState(ICvUnit1* a1);
    /// 0x087a1152
    virtual void GameplayUnitSetDamage(ICvUnit1* a1, int a2, int a3);
    /// 0x087a48b4
    virtual void GameplayUnitShouldDimFlag(ICvUnit1* a1, bool a2);
    /// 0x0879dca0
    virtual void GameplayUnitTeleported(ICvUnit1* a1, ICvPlot1* a2);
    /// 0x087a4b9e
    virtual void GameplayUnitVisibility(ICvUnit1* a1, bool a2, bool a3, float a4);
    /// 0x087a1920
    virtual void GameplayUnitWork(ICvUnit1* a1, int a2);
    /// 0x087a7976
    virtual void GameplayWallCreated(ICvPlot1* a1);
    /// 0x087a896e
    virtual void GameplayWarStateChanged(TeamTypes a1, TeamTypes a2, bool a3);
    /// 0x087a8d48
    virtual void GameplayWonderCreated(PlayerTypes a1, ICvPlot1* a2, BuildingTypes a3, int a4);
    /// 0x087a91b2
    virtual void GameplayWonderEdited(PlayerTypes a1, ICvPlot1* a2, BuildingTypes a3, int a4);
    /// 0x087a95fc
    virtual void GameplayWonderRemoved(PlayerTypes a1, ICvPlot1* a2, BuildingTypes a3);
    /// 0x087aa256
    virtual void GameplayWorldAnchor(GenericWorldAnchorTypes a1, bool a2, int a3, int a4, int a5);
    /// 0x087a8458
    virtual void GameplayYieldMightHaveChanged(ICvPlot1* a1);
    /// 0x087aa802
    virtual bool GetAdvisorBadAttackInterrupt();
    /// 0x087aa848
    virtual bool GetAdvisorCityAttackInterrupt();
    /// 0x08799684
    virtual int GetAudioTagIndex(char const* a1, int a2);
    /// 0x08799604
    virtual bool GetAutorun();
    /// 0x0879ad00
    virtual const char * GetCacheFolderPath();
    /// 0x087995e6
    virtual bool GetDone();
    /// 0x087ab5b8
    virtual bool GetEvaluatedMapScriptPath(char const* a1, char* a2, unsigned int a3) const;
    /// 0x087ab14c
    virtual void GetGameCoreLock();
    /// 0x08795380
    virtual Database::Connection * GetModsDatabase();
    /// 0x08796260
    virtual int GetNumAICivsProcessed() const;
    /// 0x08795684
    virtual CvString GetPitbossEmail() const;
    /// 0x087955e2
    virtual CvString GetPitbossSmtpHost() const;
    /// 0x08795618
    virtual CvString GetPitbossSmtpLogin() const;
    /// 0x0879564e
    virtual CvString GetPitbossSmtpPassword() const;
    /// 0x08795300
    virtual ICvEngineScriptSystem1 * GetScriptSystem();
    /// 0x087aa6e2
    virtual bool GetSteamStat(ESteamStat a1, int* a2) const;
    /// 0x087aa860
    virtual int GetTutorialLevel();
    /// 0x087ab188
    virtual bool HasGameCoreLock();
    /// 0x0879623c
    virtual bool HasReceivedAICivsProcessed(PlayerTypes a1) const;
    /// 0x08796204
    virtual bool HasReceivedTurnAllComplete(PlayerTypes a1);
    /// 0x08796228
    virtual bool HasReceivedTurnAllCompleteFromAllPlayers();
    /// 0x087961e0
    virtual bool HasReceivedTurnComplete(PlayerTypes a1);
    /// 0x08796152
    virtual bool HasSentTurnAllComplete();
    /// 0x087960c4
    virtual bool HasSentTurnComplete();
    /// 0x087aa75a
    virtual bool IncrementSteamStat(ESteamStat a1);
    /// 0x087aa78e
    virtual bool IncrementSteamStatAndUnlock(ESteamStat a1, int a2, EAchievement a3);
    /// 0x087995ae
    virtual void InitGlobals();
    /// 0x087aa6ae
    virtual bool IsAchievementUnlocked(EAchievement a1) const;
    /// 0x0879552a
    virtual bool IsDLCValid(_GUID const& a1) const;
    /// 0x087ab104
    virtual bool IsGameCoreExecuting();
    /// 0x087ab19c
    virtual bool IsGameCoreThread();
    /// 0x08795784
    virtual bool IsHost() const;
    /// 0x0879539a
    virtual bool IsModActivated(char const* a1);
    /// 0x08795454
    virtual bool IsModActivated(char const* a1, int a2);
    /// 0x087aab1a
    virtual bool IsNetPlayer(PlayerTypes a1) const;
    // Constant false in this build.
    /// 0x087955de
    virtual bool IsPitbossHost() const;
    /// 0x087957ac
    virtual bool IsPlayerConnected(PlayerTypes a1);
    /// 0x087ab4b4
    virtual bool IsPlayerHotJoining(int a1);
    /// 0x087ab484
    virtual bool IsPlayerKicked(int a1);
    /// 0x087aadbc
    virtual bool IsProcessingGameCoreMessages();
    /// 0x08799ff0
    virtual void LoadGame();
    /// 0x087a8c12
    virtual void NetMessageDebug(std::string const& a1) const;
    /// 0x087a8618
    virtual void NotifyAILeadersInGame();
    /// 0x087a86b4
    virtual void NotifySpecificAILeaderInGame(PlayerTypes a1);
    /// 0x087aa5b2
    virtual void PublishActivePlayer(PlayerTypes a1, PlayerTypes a2);
    /// 0x087aacca
    virtual void PublishEraChanges();
    /// 0x087aa61c
    virtual void PublishNewGameTurn(int a1);
    /// 0x08794e76
    virtual void * QueryInterface(_GUID a1);
    /// 0x0879a186
    virtual void QuickLoad();
    /// 0x0879a150
    virtual void QuickSave();
    /// 0x087aad92
    virtual void RecordGameCoreMessages(bool a1);
    /// 0x087aac40
    virtual bool RecordLeaderboardScore(int a1);
    /// 0x087aac1c
    virtual bool RecordVictoryInformation(int a1);
    /// 0x08799ef8
    virtual void ReleaseEnumeratedFilesList(FFastList<char*, 21u, 0u>& a1);
    /// 0x087ab160
    virtual void ReleaseGameCoreLock();
    /// 0x087aa884
    virtual bool ReseatConnectedPlayer(PlayerTypes a1) const;
    /// 0x087aa876
    virtual void ReseatPlayer(PlayerTypes a1, bool a2) const;
    /// 0x0879ab2c
    virtual void RestartGame();
    /// 0x08799f78
    virtual void SaveGame(SaveGameTypes a1);
    /// 0x087aab5e
    virtual void SendAICivsProcessed() const;
    /// 0x08797860
    virtual void SendDiploVote(PlayerTypes a1);
    /// 0x08795bfe
    virtual void SendEnhanceReligion(PlayerTypes a1, ReligionTypes a2, char const* a3, BeliefTypes a4, BeliefTypes a5, int a6, int a7);
    /// 0x08795d3c
    virtual void SendFaithPurchase(PlayerTypes a1, FaithPurchaseTypes a2, int a3);
    /// 0x08795b12
    virtual void SendFoundPantheon(PlayerTypes a1, BeliefTypes a2);
    /// 0x08795b5a
    virtual void SendFoundReligion(PlayerTypes a1, ReligionTypes a2, char const* a3, BeliefTypes a4, BeliefTypes a5, BeliefTypes a6, BeliefTypes a7, int a8, int a9);
    /// 0x087aab50
    virtual void SendGameDoTurnProcessed() const;
    /// 0x08795f56
    virtual void SendLeagueEditName(LeagueTypes a1, PlayerTypes a2, char const* a3);
    /// 0x08795eac
    virtual void SendLeagueProposeEnact(LeagueTypes a1, ResolutionTypes a2, PlayerTypes a3, int a4);
    /// 0x08795f06
    virtual void SendLeagueProposeRepeal(LeagueTypes a1, int a2, PlayerTypes a3);
    /// 0x08795e5c
    virtual void SendLeagueVoteAbstain(LeagueTypes a1, PlayerTypes a2, int a3);
    /// 0x08795d8c
    virtual void SendLeagueVoteEnact(LeagueTypes a1, int a2, PlayerTypes a3, int a4, int a5);
    /// 0x08795df4
    virtual void SendLeagueVoteRepeal(LeagueTypes a1, int a2, PlayerTypes a3, int a4, int a5);
    /// 0x087976f0
    virtual void SendLiberateMinor(PlayerTypes a1, int a2);
    /// 0x0879764a
    virtual void SendMinorNoUnitSpawning(PlayerTypes a1, bool a2);
    /// 0x08795c8c
    virtual void SendMoveSpy(PlayerTypes a1, int a2, int a3, int a4, bool a5);
    /// 0x0879759c
    virtual void SendPledgeMinorProtection(PlayerTypes a1, bool a2);
    /// 0x08797d94
    virtual void SendRenameCity(int a1, CvString a2);
    /// 0x08797eee
    virtual void SendRenameUnit(int a1, CvString a2);
    /// 0x08797cf0
    virtual void SendSellBuilding(int a1, BuildingTypes a2);
    /// 0x08795cf4
    virtual void SendStageCoup(PlayerTypes a1, int a2);
    /// 0x08797c54
    virtual void SendUpdateCityCitizens(int a1);
    /// 0x087aa7d4
    virtual void SetAdvisorBadAttackInterrupt(bool a1);
    /// 0x087aa81a
    virtual void SetAdvisorCityAttackInterrupt(bool a1);
    /// 0x087995b2
    virtual void SetDone(bool a1);
    /// 0x087aa71e
    virtual bool SetSteamStat(ESteamStat a1, int a2);
    /// 0x087ab590
    virtual bool ShouldValidateGameDatabase() const;
    /// 0x087ab3ea
    virtual void TradeVisuals_ActivatePopupRoute(int a1);
    /// 0x087ab424
    virtual void TradeVisuals_DeactivatePopupRoute();
    /// 0x087ab3a8
    virtual void TradeVisuals_DestroyRoute(int a1, int a2);
    /// 0x087ab2d4
    virtual void TradeVisuals_NewRoute(int a1, int a2, TradeConnectionType a3, int a4, int* a5, int* a6);
    /// 0x087ab36a
    virtual void TradeVisuals_UpdateRouteDirection(int a1, bool a2);
    /// 0x087ab174
    virtual bool TryGameCoreLock();
    /// 0x087aac76
    virtual bool TunerConnected();
    /// 0x087aaca0
    virtual bool TunerEverConnected();
    /// 0x087995b0
    virtual void UninitGlobals();
    /// 0x087aa67e
    virtual void UnlockAchievement(EAchievement a1);
    /// 0x08796274
    virtual void VerifyPlayerSlot(PlayerTypes a1) const;
    /// 0x08798bb0
    virtual bool allAICivsProcessedThisTurn() const;
    /// 0x08798ba2
    virtual bool allAIProcessedThisTurn() const;
    /// 0x0879983c
    virtual bool altKey();
    /// 0x08799420
    virtual void closeSlot(PlayerTypes a1);
    /// 0x08799882
    virtual bool ctrlKey();
    /// 0x087a8c34
    virtual void endTurnTimerLength(float lengthInSeconds);
    /// 0x087a8cb6
    virtual float endTurnTimerLength() const;
    /// 0x0879555c
    virtual int getAssignedNetworkID(int a1);
    /// 0x087991c2
    virtual CvString getCivAdjective(int a1, unsigned int a2);
    /// 0x0879924c
    virtual CvString getCivAdjectiveKey(int a1);
    /// 0x08798f9a
    virtual CvString getCivDescription(int a1, unsigned int a2);
    /// 0x08799024
    virtual CvString getCivDescriptionKey(int a1);
    /// 0x087990ae
    virtual CvString getCivShortDesc(int a1, unsigned int a2);
    /// 0x08799138
    virtual CvString getCivShortDescKey(int a1);
    /// 0x087952e4
    virtual CvDLLInterfaceIFaceBase * getInterfaceIFace();
    /// 0x08798bea
    virtual CvString getMapScriptName();
    /// 0x08798bc4
    virtual int getMillisecsPerTurn();
    /// 0x0879aa62
    virtual void getPassword(PlayerTypes a1);
    /// 0x08798f0e
    virtual CvString getPlayerDisplayNickName(int a1);
    /// 0x08798df6
    virtual CvString getPlayerName(int a1, unsigned int a2);
    /// 0x08798e82
    virtual CvString getPlayerNameKey(int a1);
    /// 0x0879ab6c
    virtual bool getPlayerOption(PlayerOptionTypes a1);
    /// 0x08798bca
    virtual float getSecsPerTurn();
    /// 0x08798c6c
    virtual bool getTransferredMap();
    /// 0x08798be4
    virtual int getTurnsPerMinute();
    /// 0x08798bde
    virtual int getTurnsPerSecond();
    /// 0x08798ba0
    virtual void hotJoinComplete() const;
    /// 0x08798d74
    virtual bool isWBMapNoPlayers();
    /// 0x08798cf2
    virtual bool isWBMapScript();
    /// 0x08798b58
    virtual void netDisconnect();
    /// 0x087957e2
    virtual void netMessageDebugLog(std::string const& a1) const;
    /// 0x087992d6
    virtual void openSlot(PlayerTypes a1);
    /// 0x08794ff8
    virtual void reset();
    /// 0x0879a1bc
    virtual bool saveReplay();
    /// 0x087979bc
    virtual void sendAdvancedStartAction(AdvancedStartActionTypes a1, PlayerTypes a2, int a3, int a4, int a5, bool a6);
    /// 0x08796b1e
    virtual void sendAutoMission(int a1);
    /// 0x08795fb4
    virtual void sendAutoMoves();
    /// 0x08797a60
    virtual void sendBarbarianRansom(int a1, int a2);
    /// 0x08798ab0
    virtual void sendChangeIdeology();
    /// 0x0879745a
    virtual void sendChangeWar(TeamTypes a1, bool a2);
    /// 0x0879718e
    virtual void sendChat(CvString const& a1, ChatTargetTypes a2, PlayerTypes a3);
    /// 0x08798124
    virtual void sendCityBuyPlot(int a1, int a2, int a3);
    /// 0x08796944
    virtual void sendCitySyncCheck(PlayerTypes a1, int a2, FMemoryStream& a3, std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > const& a4) const;
    /// 0x08796bba
    virtual void sendDoCommand(int a1, CommandTypes a2, int a3, int a4, bool a5);
    /// 0x08796fd0
    virtual void sendDoTask(int a1, TaskTypes a2, int a3, int a4, bool a5, bool a6, bool a7, bool a8);
    /// 0x08795a72
    virtual void sendExtendedGame();
    /// 0x08798478
    virtual void sendFaithGreatPersonChoice(PlayerTypes a1, UnitTypes a2);
    /// 0x087986ea
    virtual void sendFromUIDiploEvent(PlayerTypes a1, FromUIDiploEventTypes a2, int a3, int a4);
    /// 0x08795810
    virtual void sendGameInfo(CvString const& a1, CvString const& a2);
    /// 0x08797b04
    virtual void sendGiftUnit(PlayerTypes a1, int a2);
    /// 0x087984b8
    virtual void sendGoodyChoice(PlayerTypes a1, int a2, int a3, GoodyTypes a4, int a5);
    /// 0x087983f8
    virtual void sendGreatPersonChoice(PlayerTypes a1, UnitTypes a2);
    /// 0x08797500
    virtual void sendIgnoreWarning(TeamTypes a1);
    /// 0x087978fc
    virtual void sendLaunch(PlayerTypes a1, VictoryTypes a2);
    /// 0x08798438
    virtual void sendMayaBonusChoice(PlayerTypes a1, UnitTypes a2);
    /// 0x08798328
    virtual void sendMinorBullyGold(PlayerTypes a1, PlayerTypes a2, int a3);
    /// 0x08798370
    virtual void sendMinorBullyUnit(PlayerTypes a1, PlayerTypes a2, UnitTypes a3);
    /// 0x087983b8
    virtual void sendMinorBuyout(PlayerTypes a1, PlayerTypes a2);
    /// 0x087979c4
    virtual void sendMinorCivEnterTerritory(PlayerTypes a1);
    /// 0x087979c2
    virtual void sendMinorCivIntrusion(PlayerTypes a1, int a2);
    /// 0x087979c0
    virtual void sendMinorCivQuestCompleted(PlayerTypes a1, bool a2);
    /// 0x087979be
    virtual void sendMinorCivQuestNoInterest(PlayerTypes a1, bool a2);
    /// 0x0879822c
    virtual void sendMinorGiftGold(PlayerTypes a1, int a2);
    /// 0x087982d0
    virtual void sendMinorGiftTileImprovement(PlayerTypes a1, PlayerTypes a2, int a3, int a4);
    /// 0x087981d0
    virtual void sendMinorPledgeProtection(PlayerTypes a1, PlayerTypes a2, bool a3, bool a4);
    /// 0x08798a28
    virtual void sendMoveGreatWorks(PlayerTypes a1, int a2, int a3, int a4, int a5, int a6, int a7);
    /// 0x087987a6
    virtual void sendNetDealAccepted(PlayerTypes a1, PlayerTypes a2, ICvDeal1* a3, int a4, int a5, int a6) const;
    /// 0x087988f0
    virtual void sendNetDemandAccepted(PlayerTypes a1, PlayerTypes a2, ICvDeal1* a3) const;
    /// 0x087973ba
    virtual void sendPause(int a1);
    /// 0x0879aa60
    virtual void sendPbemTurn(PlayerTypes a1);
    /// 0x0879730a
    virtual void sendPing(int a1, int a2);
    /// 0x08798048
    virtual void sendPlayerHurry(HurryTypes a1);
    /// 0x087956ba
    virtual void sendPlayerInfo(PlayerTypes a1);
    /// 0x08798832
    virtual void sendPlayerInitialAIProcessed() const;
    /// 0x08795812
    virtual void sendPlayerOption(PlayerOptionTypes a1, bool a2);
    /// 0x08796a08
    virtual void sendPlayerSyncCheck(PlayerTypes a1, FMemoryStream& a2, std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > const& a3) const;
    /// 0x08796874
    virtual void sendPlotSyncCheck(PlayerTypes a1, short a2, short a3, FMemoryStream& a4, std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > const& a5) const;
    /// 0x08796e88
    virtual void sendPopOrder(int a1, int a2);
    /// 0x08796d6a
    virtual void sendPurchase(int a1, UnitTypes a2, BuildingTypes a3, ProjectTypes a4);
    /// 0x08796dbc
    virtual void sendPurchase(int a1, UnitTypes a2, BuildingTypes a3, ProjectTypes a4, int a5);
    /// 0x087965ba
    virtual void sendPushMission(int a1, MissionTypes a2, int a3, int a4, int a5, bool a6);
    /// 0x08796c8a
    virtual void sendPushOrder(int a1, OrderTypes a2, int a3, bool a4, bool a5, bool a6);
    /// 0x08796aba
    virtual void sendRandomNumberGeneratorSyncCheck(PlayerTypes a1, ICvRandom1* a2) const;
    /// 0x087970ca
    virtual void sendResearch(TechTypes a1, int a2, PlayerTypes a3, bool a4);
    /// 0x08797ba8
    virtual void sendReturnCivilian(bool a1, PlayerTypes a2, int a3);
    /// 0x087985a0
    virtual void sendSetCityAIFocus(int a1, CityAIFocusTypes a2);
    /// 0x08798644
    virtual void sendSetCityAvoidGrowth(int a1, bool a2);
    /// 0x0879897e
    virtual void sendSetSwappableGreatWork(PlayerTypes a1, int a2, int a3);
    /// 0x087989ce
    virtual void sendSwapGreatWorks(PlayerTypes a1, int a2, PlayerTypes a3, int a4);
    /// 0x08796f2c
    virtual void sendSwapOrder(int a1, int a2);
    /// 0x0879804a
    virtual void sendSwapUnits(int a1, MissionTypes a2, int a3, int a4, int a5, bool a6);
    /// 0x08796048
    virtual void sendTurnComplete();
    /// 0x08798950
    virtual void sendTurnReminder(PlayerTypes a1);
    /// 0x0879652c
    virtual bool sendTurnUnready();
    /// 0x087967b0
    virtual void sendUnitSyncCheck(PlayerTypes a1, int a2, FMemoryStream& a3, std::vector<std::pair<std::string, std::string>, std::allocator<std::pair<std::string, std::string> > > const& a4) const;
    /// 0x08797794
    virtual void sendUpdatePolicies(bool a1, int a2, bool a3);
    /// 0x0879985e
    virtual bool shiftKey();
    /// 0x08799504
    virtual void stripSpecialCharacters(CvString& a1);
    /// 0x08794cb4
    ~CvDLLUtility();
    /// 0x08798518
    virtual UNK_RET sendArchaeologyChoice(PlayerTypes a1, int a2, ArchaeologyChoiceType a3);
    /// 0x08798560
    virtual UNK_RET sendIdeologyChoice(PlayerTypes a1, PolicyBranchTypes a2);
    /// 0x08798cee
    virtual UNK_RET isDescFileName(char const* a1);
    /// 0x087ab208
    virtual UNK_RET NetMessageStaticsReset();
    /// 0x08794dc4
    UNK_RET DestroyGameCoreThread();
    /// 0x08794f32
    UNK_RET IncrementReference();
    /// 0x08794f3e
    UNK_RET DecrementReference();
    /// 0x08794f54
    UNK_RET GetReferenceCount();
    /// 0x08794f6e
    void operator delete(void* a1);
    /// 0x08794f82
    void* operator new(unsigned int a1);
    /// 0x08794f96
    UNK_RET uninit();
    /// 0x0879571e
    UNK_RET SendNetMessage(FNetMessage& a1);
    /// 0x087998a2
    UNK_RET EnumerateFilesRecursive(FFastList<char*, 21u, 0u>& a1, char const* a2, unsigned int a3, char const* a4, unsigned int a5, unsigned int a6);
    /// 0x0879a0de
    UNK_RET WaitForGameCoreUpdate();
    /// 0x0879ad26
    UNK_RET GetUnitTypeMapEntryBasedOnUnitCultureAndEra(PlayerTypes a1, UnitTypes a2);
    /// 0x0879bf0c
    UNK_RET GetUnitTypeMapEntryBasedOnUnitCultureAndEra(ICvUnit1* a1);
    /// 0x0879bf48
    UNK_RET GetBuildingTypeMapEntryBasedOnCultureAndEra(PlayerTypes a1, ICvPlot1* a2, BuildingTypes a3);
    /// 0x0879fe6a
    UNK_RET PrepareForCombat(ICvUnit1* a1, bool a2);
    /// 0x087aaeae
    UNK_RET ExecuteGameCoreUpdate(bool a1);
    /// 0x087ab0c8
    UNK_RET SetGameCoreThreadState(bool a1);
    /// 0x087ab1f8
    UNK_RET SetGameCoreThreadTheadUsesJobManager(bool a1);
};
