/// origin guess
#pragma once

#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvStructs.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "FirePlace/FireWorks/FFastList.h"
#include "FirePlace/FireWorks/FFireTypes.h"

#ifndef ICvDLLUtility_h
#define ICvDLLUtility_h

#include "Civ5/Src/App/CvGameCoreDLLUtil/LinkedList.h"
#include <string>
#include "FirePlace/FireWorks/FMemoryStream.h"

#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
//
// abstract interface for utility functions used by DLL
// Copyright 2010 Firaxis Games
//
class ICvEngineScriptSystem1;
class CvDLLInterfaceIFaceBase;

class ICvCombatInfo1;
class ICvCity1;
class ICvDeal1;
class ICvPlot1;
class ICvRandom1;
class ICvUnit1;
class CvUnit;

class CvReplayInfo;


// {B044BCED-E001-42ef-9A42-03A8B09C1362}
/// inline
static const GUID guidICvEngineUtility1 = 
{ 0xb044bced, 0xe001, 0x42ef, { 0x9a, 0x42, 0x3, 0xa8, 0xb0, 0x9c, 0x13, 0x62 } };

// {B4F5594D-BBA4-4a4c-9BBD-CEDF4CD10F2D}
/// inline
static const GUID guidICvEngineUtility2 = 
{ 0xb4f5594d, 0xbba4, 0x4a4c, { 0x9b, 0xbd, 0xce, 0xdf, 0x4c, 0xd1, 0xf, 0x2d } };

// {F95CB893-6D19-4584-A741-6F64B855C7E9}
/// inline
static const GUID guidICvEngineUtility3 = 
{ 0xf95cb893, 0x6d19, 0x4584, { 0xa7, 0x41, 0x6f, 0x64, 0xb8, 0x55, 0xc7, 0xe9 } };

// {DD8A29A2-E752-4bf7-9EE0-42B8155F175F}
/// inline
static const GUID guidICvEngineUtility4 = 
{ 0xdd8a29a2, 0xe752, 0x4bf7, { 0x9e, 0xe0, 0x42, 0xb8, 0x15, 0x5f, 0x17, 0x5f } };


typedef FFastVector<int, true, c_eMPoolTypeGame> CvPlotIndexVector;

/// size 0x4
/// vtable ?
class ICvEngineUtility1 : public ICvUnknown
{
public:
	/// inline
	static GUID DLLCALL GetInterfaceId() { return guidICvEngineUtility1; }

	//This method has been deprecated due to the interface not being versioned.
	//You should use GC.GetEngineUserInterface() instead to obtain a pointer to ICvUserInterface.
	/// pure
	virtual DECLSPEC_DEPRECATED CvDLLInterfaceIFaceBase* getInterfaceIFace() = 0;

	/// pure
	virtual ICvEngineScriptSystem1* GetScriptSystem() = 0;
	
	/// pure
	virtual void reset() = 0;
	/// pure
	virtual void DoMapSetup(int iWorldNumPlots) = 0;

	/// pure
	virtual void DoTurn() = 0;
	/// pure
	virtual bool IsHost() const = 0;
	/// pure
	virtual bool IsPlayerConnected(PlayerTypes playerId) = 0;

	/// pure
	virtual Database::Connection* GetModsDatabase() = 0;
	/// pure
	virtual bool IsModActivated(_In_z_ const char* szModID) = 0;
	/// pure
	virtual bool IsModActivated(_In_z_ const char* szModID, int iVersion) = 0;

	/// pure
	virtual bool IsDLCValid(const GUID& kID) const = 0;

	/// pure
	virtual int getAssignedNetworkID(int iPlayerID) = 0;
	
	/// pure
	virtual bool IsPitbossHost() const = 0;
	/// pure
	virtual CvString GetPitbossSmtpHost() const = 0;
	/// pure
	virtual CvString GetPitbossSmtpLogin() const = 0;
	/// pure
	virtual CvString GetPitbossSmtpPassword() const = 0;
	/// pure
	virtual CvString GetPitbossEmail() const = 0;

	/// pure
	virtual void netMessageDebugLog(const std::string & logMessage) const = 0;
	/// pure
	virtual void sendPlayerInfo(PlayerTypes eActivePlayer) = 0;
	/// pure
	virtual void sendGameInfo(const CvString& szGameName, const CvString& szAdminPassword) = 0;
	/// pure
	virtual void sendPlayerOption(PlayerOptionTypes eOption, bool bValue) = 0;
	/// pure
	virtual void sendExtendedGame() = 0;
	/// pure
	virtual void SendFoundPantheon(PlayerTypes ePlayer, BeliefTypes eBelief) = 0;
	/// pure
	virtual void SendFoundReligion(PlayerTypes ePlayer, ReligionTypes eReligion, const char* szCustomName, BeliefTypes eBelief1, BeliefTypes eBelief2, BeliefTypes eBelief3, BeliefTypes eBelief4, int iCityX, int iCityY) = 0;
	/// pure
	virtual void SendEnhanceReligion(PlayerTypes ePlayer, ReligionTypes eReligion, const char* szCustomName, BeliefTypes eBelief1, BeliefTypes eBelief2, int iCityX, int iCityY) = 0;
	/// pure
	virtual void SendMoveSpy(PlayerTypes ePlayer, int iSpyIndex, int iTargetPlayer, int iTargetCity, bool bAsDiplomat) = 0;
	/// pure
	virtual void SendStageCoup(PlayerTypes eSpyPlayer, int iSpyIndex) = 0;
	/// pure
	virtual void SendFaithPurchase(PlayerTypes ePlayer, FaithPurchaseTypes eFaithPurchaseType, int iFaithPurchaseIndex) = 0;
	/// pure
	virtual void sendAutoMoves() = 0;
	/// pure
	virtual void sendTurnComplete() = 0;
	/// pure
	virtual bool HasSentTurnComplete() = 0;
	/// pure
	virtual bool HasReceivedTurnComplete(PlayerTypes ePlayer) = 0;
	/// pure
	virtual bool HasSentTurnAllComplete() = 0;
	/// pure
	virtual bool HasReceivedTurnAllComplete(PlayerTypes ePlayer) = 0;
	/// pure
	virtual bool HasReceivedTurnAllCompleteFromAllPlayers() = 0;
	/// pure
	virtual bool sendTurnUnready() = 0;
	/// pure
	virtual void sendPushMission(int iUnitID, MissionTypes eMission, int iData1, int iData2, int iFlags, bool bShift) = 0;
	/// pure
	virtual void sendAutoMission(int iUnitID) = 0;
	/// pure
	virtual void sendDoCommand(int iUnitID, CommandTypes eCommand, int iData1, int iData2, bool bAlt) = 0;
	/// pure
	virtual void sendPushOrder(int iCityID, OrderTypes eOrder, int iData, bool bAlt, bool bShift, bool bCtrl) = 0;
	/// pure
	virtual void sendPopOrder(int iCity, int iNum) = 0;
	/// pure
	virtual void sendSwapOrder(int iCity, int iNum0) = 0;
	/// pure
	virtual void sendPurchase(int iCity, UnitTypes eUnitType, BuildingTypes eBuildingType, ProjectTypes eProjectType) = 0;
	/// pure
	virtual void sendDoTask(int iCity, TaskTypes eTask, int iData1, int iData2, bool bOption, bool bAlt, bool bShift, bool bCtrl) = 0;
	/// pure
	virtual void sendResearch(TechTypes eTech, int iDiscover, PlayerTypes ePlayerToStealFrom, bool bShift) = 0;
	/// pure
	virtual void sendChat(const CvString& szChatString, ChatTargetTypes eTargetType, PlayerTypes toWho) = 0;
	/// pure
	virtual void sendPing(int iX, int iY) = 0;
	/// pure
	virtual void sendPause(int iPauseID = -1) = 0;
	/// pure
	virtual void sendChangeWar(TeamTypes iRivalTeam, bool bWar) = 0;
	/// pure
	virtual void sendIgnoreWarning(TeamTypes iRivalTeam) = 0;
	/// pure
	virtual void SendPledgeMinorProtection(PlayerTypes eMinor, bool bValue) = 0; //antonjs: DEPRECATED, use CvLuaGame::lDoMinorPledgeProtection instead
	/// pure
	virtual void SendMinorNoUnitSpawning(PlayerTypes eMinor, bool bValue) = 0;
	/// pure
	virtual void SendLiberateMinor(PlayerTypes eMinor, int iCityID) = 0;
	/// pure
	virtual void sendUpdatePolicies(bool bNOTPolicyBranch, int iID, bool bValue) = 0;
	/// pure
	virtual void SendDiploVote(PlayerTypes eVotePlayer) = 0;
	/// pure
	virtual void sendLaunch(PlayerTypes ePlayer, VictoryTypes eVictory) = 0;
	/// pure
	virtual void sendAdvancedStartAction(AdvancedStartActionTypes eAction, PlayerTypes ePlayer, int iX, int iY, int iData, bool bAdd) = 0;
	/// pure
	virtual void sendMinorCivQuestNoInterest(PlayerTypes eMinor, bool bPeace) = 0;
	/// pure
	virtual void sendMinorCivQuestCompleted(PlayerTypes eMinor, bool bPeace) = 0;
	/// pure
	virtual void sendMinorCivIntrusion(PlayerTypes eMinor, int iOptionChosen) = 0;
	/// pure
	virtual void sendMinorCivEnterTerritory(PlayerTypes eMinor) = 0;
	/// pure
	virtual void sendBarbarianRansom(int iOptionChosen, int iUnitID) = 0;
	/// pure
	virtual void sendGiftUnit(PlayerTypes eMinor, int iUnitID) = 0;
	/// pure
	virtual void sendReturnCivilian(bool bReturn, PlayerTypes ePlayer, int iUnitID) = 0;
	/// pure
	virtual void SendUpdateCityCitizens(int iCityID) = 0;
	/// pure
	virtual void SendSellBuilding(int iCityID, BuildingTypes eBuilding) = 0;
	/// pure
	virtual void SendRenameCity (int iCityID, CvString szNewName) = 0;
	/// pure
	virtual void SendRenameUnit (int iUnitID, CvString szNewName) = 0;
	/// pure
	virtual void sendPlayerHurry(HurryTypes eHurry) = 0;
	/// pure
	virtual void sendSwapUnits(int iUnitID, MissionTypes eMission, int iData1, int iData2, int iFlags, bool bShift) = 0;
	/// pure
	virtual void sendCityBuyPlot (int iCityID, int iX, int iY) = 0;
	/// pure
	virtual void sendMinorPledgeProtection(PlayerTypes eMajor, PlayerTypes eMinor, bool bProtect, bool bPledgeNowBroken) = 0;
	/// pure
	virtual void sendMinorGiftGold(PlayerTypes eMinor, int iGold) = 0;
	/// pure
	virtual void sendMinorGiftTileImprovement(PlayerTypes eMajor, PlayerTypes eMinor, int iPlotX, int iPlotY) = 0;
	/// pure
	virtual void sendMinorBullyGold(PlayerTypes eBully, PlayerTypes eMinor, int iGold) = 0;
	/// pure
	virtual void sendMinorBullyUnit(PlayerTypes eBully, PlayerTypes eMinor, UnitTypes eUnitType) = 0;
	/// pure
	virtual void sendMinorBuyout(PlayerTypes eMajor, PlayerTypes eMinor) = 0;
	/// pure
	virtual void sendGreatPersonChoice(PlayerTypes ePlayer, UnitTypes eUnit) = 0;
	/// pure
	virtual void sendMayaBonusChoice(PlayerTypes ePlayer, UnitTypes eUnit) = 0;
	/// pure
	virtual void sendFaithGreatPersonChoice(PlayerTypes ePlayer, UnitTypes eUnit) = 0;
	/// pure
	virtual void sendSetCityAIFocus (int iCityID, CityAIFocusTypes eFocus) = 0;
	/// pure
	virtual void sendSetCityAvoidGrowth (int iCityID, bool bAvoidGrowth) = 0;
	/// pure
	virtual void sendUnitSyncCheck(PlayerTypes playerId, int unitId, FMemoryStream & syncArchiveData, const std::vector<std::pair<std::string, std::string> > &) const = 0;
	/// pure
	virtual void sendPlotSyncCheck(PlayerTypes playerId, short plotX, short plotY, FMemoryStream & syncArchiveData, const std::vector<std::pair<std::string, std::string> > &) const = 0;
	/// pure
	virtual void sendCitySyncCheck(PlayerTypes playerId, int cityId, FMemoryStream & syncArchiveData, const std::vector<std::pair<std::string, std::string> > &) const = 0;
	/// pure
	virtual void sendRandomNumberGeneratorSyncCheck(PlayerTypes playerId, ICvRandom1* pRandomNumberGenerator) const = 0;
	/// pure
	virtual void sendPlayerSyncCheck(PlayerTypes playerId, FMemoryStream & syncData, const std::vector<std::pair<std::string, std::string> > &) const = 0;
	/// pure
	virtual void sendFromUIDiploEvent(PlayerTypes eOtherPlayer, FromUIDiploEventTypes eEvent, int iArg1, int iArg2) = 0;
	/// pure
	virtual void sendNetDealAccepted(PlayerTypes fromPlayer, PlayerTypes toPlayer, ICvDeal1* pDeal, int iDealValueToMe, int iValueImOffering, int iValueTheyreOffering) const = 0;
	/// pure
	virtual void sendNetDemandAccepted(PlayerTypes fromPlayer, PlayerTypes toPlayer, ICvDeal1* pDeal) const = 0;
	/// pure
	virtual void sendTurnReminder(PlayerTypes toPlayer) = 0;
	/// pure
	virtual void sendGoodyChoice(PlayerTypes ePlayerID, int iPlotX, int iPlotY, GoodyTypes eGoody, int iUnitID) = 0;
	/// pure
	virtual void sendSetSwappableGreatWork(PlayerTypes ePlayer, int iWorkClass, int iWorkIndex) = 0;
	/// pure
	virtual void sendSwapGreatWorks(PlayerTypes ePlayer1, int iWork1, PlayerTypes ePlayer2, int iWork2) = 0;
	/// pure
	virtual void sendMoveGreatWorks(PlayerTypes ePlayer, int iCity1, int iBuildingClass1, int iWorkIndex1, 
																											 int iCity2, int iBuildingClass2, int iWorkIndex2) = 0;
	/// pure
	virtual void netDisconnect() = 0;
	/// pure
	virtual void sendPlayerInitialAIProcessed() const = 0;
	/// pure
	virtual void hotJoinComplete() const = 0;
	/// pure
	virtual bool allAIProcessedThisTurn() const = 0;
	/// pure
	virtual int getMillisecsPerTurn() = 0;
	/// pure
	virtual float getSecsPerTurn() = 0;
	/// pure
	virtual int getTurnsPerSecond() = 0;
	/// pure
	virtual int getTurnsPerMinute() = 0;

	/// pure
	virtual bool CanAdvanceTurn() = 0;

	/// pure
	virtual void openSlot(PlayerTypes eID) = 0;
	/// pure
	virtual void closeSlot(PlayerTypes eID) = 0;

	/// pure
	virtual CvString getMapScriptName() = 0;
	/// pure
	virtual bool getTransferredMap() = 0;
	/// pure
	virtual bool isWBMapScript() = 0;
	/// pure
	virtual bool isWBMapNoPlayers() = 0;
	
	/// pure
	virtual CvString getPlayerName(int iIndex, uint uiForm = 0) = 0;
	/// pure
	virtual CvString getPlayerNameKey(int iIndex) = 0;
	/// pure
	virtual CvString getPlayerDisplayNickName(int iIndex) = 0;
	/// pure
	virtual CvString getCivDescription(int iIndex, uint uiForm = 0) = 0;
	/// pure
	virtual CvString getCivDescriptionKey(int iIndex) = 0;
	/// pure
	virtual CvString getCivShortDesc(int iIndex, uint uiForm = 0) = 0;
	/// pure
	virtual CvString getCivShortDescKey(int iIndex) = 0;
	/// pure
	virtual CvString getCivAdjective(int iIndex, uint uiForm = 0) = 0;
	/// pure
	virtual CvString getCivAdjectiveKey(int iIndex) = 0;
	/// pure
	virtual void stripSpecialCharacters(CvString& szName) = 0;

	/// pure
	virtual void InitGlobals() = 0;
	/// pure
	virtual void UninitGlobals() = 0;

	/// pure
	virtual void SetDone(bool bDone) = 0;
	/// pure
	virtual bool GetDone() = 0;
	/// pure
	virtual bool GetAutorun() = 0;

	/// pure
	virtual int GetAudioTagIndex(const char* szTag, int iScriptType = -1) = 0;

	/// pure
	virtual bool altKey() = 0;
	/// pure
	virtual bool shiftKey() = 0;
	/// pure
	virtual bool ctrlKey() = 0;

	typedef FFastList<char*, c_eMPoolTypeGame, 0> EnumeratedFilesList;
	/// pure
	virtual void EnumerateFiles(EnumeratedFilesList& files, 
								const char* szFolder, const size_t lenFolder, 
								const char* szPattern, const size_t lenPattern, 
								const bool bRecursive = false) = 0;

	/// pure
	virtual void ReleaseEnumeratedFilesList(EnumeratedFilesList &files) = 0;

	/// pure
	virtual void SaveGame(SaveGameTypes eSaveGame) = 0;
	/// pure
	virtual void LoadGame() = 0;
	/// pure
	virtual void AutoSave(bool bInitial = false, bool bPostTurn = false) = 0;
	/// pure
	virtual bool saveReplay() = 0;
	/// pure
	virtual void QuickSave() = 0;
	/// pure
	virtual void QuickLoad() = 0;
	/// pure
	virtual void sendPbemTurn(PlayerTypes ePlayer) = 0;
	/// pure
	virtual void getPassword(PlayerTypes ePlayer) = 0;
	/// pure
	virtual void RestartGame() = 0;

	/// pure
	virtual bool getPlayerOption(PlayerOptionTypes ePlayerOption) = 0;
	
	/// pure
	virtual const char* GetCacheFolderPath() = 0;

	/// pure
	virtual void GameplayEraChanged(PlayerTypes playerID, EraTypes newEra) = 0;

	/// pure
	virtual void GameplayBridgeChanged(bool bBridge, unsigned char newBridgeStyle) = 0;

	/// pure
	virtual void GameplayUnitCreated(_In_ ICvUnit1* pThisUnit) = 0;
	/// pure
	virtual void GameplayUnitMoved(_In_ ICvUnit1* pThisUnit, const CvPlotIndexVector& kPlots) = 0;
	/// pure
	virtual void GameplayUnitTeleported(_In_ ICvUnit1* pThisUnit, _In_ ICvPlot1* pThisPlot) = 0;
	/// pure
	virtual void GameplayUnitDestroyed(_In_ ICvUnit1* pThisUnit) = 0;
	/// pure
	virtual void GameplayUnitDestroyedInCombat(_In_ ICvUnit1* pUnit) = 0;
	/// pure
	virtual void GameplayUnitSetDamage(_In_ ICvUnit1* pThisUnit, int iDamage, int iPreviousDamage) = 0;
	/// pure
	virtual void GameplayUnitFortify(_In_ ICvUnit1* pThisUnit, bool bFortify) = 0;
	/// pure
	virtual void GameplayUnitWork(_In_ ICvUnit1* pThisUnit, int iWorkType) = 0;
	/// pure
	virtual void GameplayUnitParadrop(_In_ ICvUnit1* pThisUnit) = 0;
	/// pure
	virtual void GameplayUnitActivate(_In_ ICvUnit1* pThisUnit) = 0;
	/// pure
	virtual void GameplayUnitEmbark(_In_ ICvUnit1* pThisUnit, bool bEmbark) = 0;
	/// pure
	virtual void GameplayUnitGarrison(_In_ ICvUnit1* pThisUnit, bool bGarrison) = 0;
	/// pure
	virtual void GameplayUnitShouldDimFlag(_In_ ICvUnit1* pThisUnit, bool bDim) = 0;
	/// pure
	virtual void GameplayUnitMarkThreatening(_In_ ICvUnit1* pThisUnit, bool bMark) = 0;
	/// pure
	virtual void GameplayUnitVisibility(_In_ ICvUnit1* pThisUnit, bool bVisible, bool bChangeInvisibility = false, const float fBlendTime = 0.5f) = 0;
	/// pure
	virtual uint GameplayUnitCombat(_In_ ICvCombatInfo1* pCombatInfo) = 0;
	/// pure
	virtual void GameplayCityCreated(_In_ ICvCity1* pThisCity, EraTypes eCurrentEra) = 0;
	/// pure
	virtual void GameplayCityDestroyed(_In_ ICvCity1* pThisCity, PlayerTypes eNewOwner) = 0;
	/// pure
	virtual void GameplayCityCaptured(_In_ ICvCity1* pThisCity, PlayerTypes eNewOwner) = 0;
	/// pure
	virtual void GameplayCitySetDamage(_In_ ICvCity1* pThisCity, int iDamage, int iPreviousDamage) = 0;
	/// pure
	virtual void GameplayCityPopulationChanged(_In_ ICvCity1* pThisCity, int newPopulation) = 0;
	/// pure
	virtual uint GameplayCityCombat(_In_ ICvCombatInfo1* pInfo) = 0;
	/// pure
	virtual void GameplayUnitMissionEnd(_In_ ICvUnit1* pThisUnit) = 0;
	/// pure
	virtual void GameplayUnitRebased(_In_ ICvUnit1* pThisUnit, _In_ ICvPlot1* pOldPlot, _In_ ICvPlot1* pThisPlot) = 0;
	/// pure
	virtual void GameplayUnitResetAnimationState(_In_ ICvUnit1* pThisUnit) = 0;

	/// pure
	virtual void GameplayTechAcquired(TeamTypes eTeam, TechTypes eTech) = 0;

	/// pure
	virtual void GameplayFeatureChanged(_In_ ICvPlot1* pThisPlot, FeatureTypes newFeature) = 0;

	// the last set of functions will shortly be deprecated, this is the replacement
	/// pure
	virtual void GameplayPlotStateChange(_In_ const ICvPlot1* pThisPlot, const ResourceTypes eRevealedResource, const ImprovementTypes eRevealedImprovement, 
        const int iRevealedImprovementState, const RouteTypes eRevealedRoute, const int iRouteState) = 0;
    /// pure
    virtual void GameplayPlotIconStateChange(_In_ const ICvPlot1* pThisPlot, const ResourceTypes eRevealedResource, 
        const ImprovementTypes eRevealedImprovement, const int iRevealedImprovementState) = 0;

	/// pure
	virtual void GameplayNaturalWonderRevealed(_In_ ICvPlot1* pThisPlot) = 0;
	/// pure
	virtual void GameplayWallCreated(_In_ ICvPlot1* pThisPlot) = 0;
	/// pure
	virtual void GameplayDoFX(_In_opt_ ICvPlot1* pThisPlot, int iFXType = -1) = 0;

	/// pure
	virtual void GameplayFOWChanged(int x, int y, int eFOWType, bool bSetAll) = 0;

	/// pure
	virtual void GameplayYieldMightHaveChanged(_In_ ICvPlot1* pThisPlot) = 0;

	/// pure
	virtual void NotifyAILeadersInGame() = 0;
	/// pure
	virtual void NotifySpecificAILeaderInGame(PlayerTypes ePlayer) = 0;
	/// pure
	virtual void GameplayWarStateChanged(TeamTypes eTeam1, TeamTypes eTeam2, bool bWar) = 0;
	/// pure
	virtual void DoClearDiplomacyTradeTable() = 0;
	/// pure
	virtual void GameplayDiplomacyAILeaderMessage(PlayerTypes ePlayer, DiploUIStateTypes eDiploUIState, const char* szLeaderMessage, LeaderheadAnimationTypes eAction, int iData1 = -1) = 0;
	/// pure
	virtual void GameplayMetTeam(TeamTypes eMyTeam, TeamTypes eTheirTeam) = 0;
	/// pure
	virtual void NetMessageDebug(const std::string &) const = 0;

	/// pure
	virtual void endTurnTimerLength(float lengthInSeconds) = 0;
	/// pure
	virtual float endTurnTimerLength() const = 0;

	/// pure
	virtual void GameplayWonderCreated(PlayerTypes ePlayerID, _In_ ICvPlot1* pThisPlot, BuildingTypes eBuildingIndex, int iState) = 0;
	/// pure
	virtual void GameplayWonderEdited(PlayerTypes ePlayerID, _In_ ICvPlot1* pThisPlot, BuildingTypes eBuildingIndex, int iState) = 0;
	/// pure
	virtual void GameplayWonderRemoved(PlayerTypes ePlayerID, _In_ ICvPlot1* pThisPlot, BuildingTypes eBuildingIndex) = 0;

	/// pure
	virtual void GameplaySpaceshipCreated(_In_ ICvPlot1* pThisPlot, int iState) = 0;
	/// pure
	virtual void GameplaySpaceshipEdited(_In_ ICvPlot1* pThisPlot, int iState) = 0;
	/// pure
	virtual void GameplaySpaceshipRemoved(_In_ ICvPlot1* pThisPlot) = 0;

	/// pure
	virtual void GameplayOpenOptionsScreen() = 0;
	/// pure
	virtual void GameplaySearchForPediaEntry(const char * c) = 0;
	/// pure
	virtual void GameplayOpenInfoCorner(int iInfoCornerID) = 0;

	/// pure
	virtual void GameplayWorldAnchor(GenericWorldAnchorTypes eAnchor, bool bAdd, int iX, int iY, int iData1) = 0;

	/// pure
	virtual void GameplayMinimapUnitSelect(int iX, int iY) = 0;
	/// pure
	virtual void GameplayMinimapNotification(int iX, int iY, int iID) = 0;

	/// pure
	virtual void GameplayGoldenAgeStarted() = 0;
	/// pure
	virtual void GameplayGoldenAgeEnded() = 0;

	/// pure
	virtual	void GameplayTurnChanged(int iNewTurn) = 0;
	/// pure
	virtual	void GameplayActivePlayerChanged(PlayerTypes eNewPlayer) = 0;

	/// pure
	virtual void PublishActivePlayer(PlayerTypes ePlayer, PlayerTypes eOldPlayer) = 0;
	/// pure
	virtual void PublishNewGameTurn(int gameTurn) = 0;

	/// pure
	virtual void UnlockAchievement(EAchievement eAchievement) = 0;
	/// pure
	virtual bool IsAchievementUnlocked(EAchievement eAchievement) const = 0;

	/// pure
	virtual bool GetSteamStat(ESteamStat eStat, int32 * pStat) const = 0;
	/// pure
	virtual bool SetSteamStat(ESteamStat eStat, int32 nStat) = 0;

	/// pure
	virtual bool IncrementSteamStat(ESteamStat eStat) = 0;
	/// pure
	virtual bool IncrementSteamStatAndUnlock(ESteamStat eStat, int nThreshold, EAchievement eAchievement) = 0;

	/// pure
	virtual void SetAdvisorBadAttackInterrupt (bool bValue) = 0;
	/// pure
	virtual bool GetAdvisorBadAttackInterrupt (void) = 0;
	/// pure
	virtual void SetAdvisorCityAttackInterrupt (bool bValue) = 0;
	/// pure
	virtual bool GetAdvisorCityAttackInterrupt (void) = 0;

	/// pure
	virtual int  GetTutorialLevel (void) = 0;

	/// pure
	virtual void ReseatPlayer(PlayerTypes p, bool bClosePlayersOldSlot=false) const = 0;
	/// pure
	virtual bool IsNetPlayer(PlayerTypes p) const = 0;
	/// pure
	virtual void SendGameDoTurnProcessed() const = 0;

	/// pure
	virtual bool RecordVictoryInformation(int score) = 0;
	/// pure
	virtual bool RecordLeaderboardScore(int score) = 0;

	/// pure
	virtual bool TunerConnected() = 0;

	// Returns true if the tuner has been connected during this game session.
	// Use the CvGame version of this function to check the save as well.
	/// pure
	virtual bool TunerEverConnected() = 0;

    /// pure
    virtual void PublishEraChanges() = 0;

	/// pure
	virtual void RecordGameCoreMessages(bool bRecord) = 0;
	/// pure
	virtual bool IsProcessingGameCoreMessages() = 0;

	/// pure
	virtual	void GetGameCoreLock() = 0;
	/// pure
	virtual	void ReleaseGameCoreLock() = 0;
	/// pure
	virtual	bool TryGameCoreLock() = 0;
	/// pure
	virtual	bool HasGameCoreLock() = 0;
	/// pure
	virtual	bool IsGameCoreThread() = 0;
	/// pure
	virtual	bool IsGameCoreExecuting() = 0;
};

/// size 0x4
/// vtable ?
class ICvEngineUtility2 : public ICvEngineUtility1
{
public:
	using ICvEngineUtility1::sendPurchase;
	/// inline
	static GUID DLLCALL GetInterfaceId() { return guidICvEngineUtility2; }
	/// pure
	virtual void sendPurchase(int iCity, UnitTypes eUnitType, BuildingTypes eBuildingType, ProjectTypes eProjectType, int ePurchaseYield) = 0;
	/// pure
	virtual bool ReseatConnectedPlayer(PlayerTypes p) const = 0;
};

/// size 0x4
/// vtable ?
class ICvEngineUtility3 : public ICvEngineUtility2
{
public:
	/// inline
	static GUID DLLCALL GetInterfaceId() { return guidICvEngineUtility3; }
	/// pure
	virtual void SendLeagueVoteEnact(LeagueTypes eLeague, int iResolutionID, PlayerTypes eVoter, int iNumVotes, int iChoice) = 0;
	/// pure
	virtual void SendLeagueVoteRepeal(LeagueTypes eLeague, int iResolutionID, PlayerTypes eVoter, int iNumVotes, int iChoice) = 0;
	/// pure
	virtual void SendLeagueVoteAbstain(LeagueTypes eLeague, PlayerTypes eVoter, int iNumVotes) = 0;
	/// pure
	virtual void SendLeagueProposeEnact(LeagueTypes eLeague, ResolutionTypes eResolution, PlayerTypes eProposer, int iChoice) = 0;
	/// pure
	virtual void SendLeagueProposeRepeal(LeagueTypes eLeague, int iResolutionID, PlayerTypes eProposer) = 0;
	/// pure
	virtual void SendLeagueEditName(LeagueTypes eLeague, PlayerTypes ePlayer, const char* szCustomName) = 0;

	/// pure
	virtual void sendChangeIdeology() = 0;

	/// pure
	virtual void TradeVisuals_NewRoute(int iRoute, int iPlayer, TradeConnectionType type, int nPlots, int x[], int y[]) = 0;
	/// pure
	virtual void TradeVisuals_UpdateRouteDirection(int iRoute, bool bForwards) = 0;
	/// pure
	virtual void TradeVisuals_DestroyRoute(int iRoute, int iPlayer) = 0;
	/// pure
	virtual void TradeVisuals_ActivatePopupRoute(int iRoute) = 0;
	/// pure
	virtual void TradeVisuals_DeactivatePopupRoute() = 0;
	
	/// pure
	virtual void FlushTurnReminders() = 0;

	/// pure
	virtual void BeginSendBundle() = 0;
	/// pure
	virtual void EndSendBundle() = 0;

	/// pure
	virtual bool IsPlayerKicked(int iPlayerID) = 0;
	/// pure
	virtual bool IsPlayerHotJoining(int iPlayerID) = 0;

	/// pure
	virtual bool allAICivsProcessedThisTurn() const = 0;
	/// pure
	virtual void SendAICivsProcessed() const = 0;

	/// pure
	virtual bool HasReceivedAICivsProcessed(PlayerTypes ePlayer) const = 0;
	/// pure
	virtual int	 GetNumAICivsProcessed() const = 0;

	/// pure
	virtual void VerifyPlayerSlot(PlayerTypes ePlayer) const = 0;

	/// pure
	virtual bool ShouldValidateGameDatabase() const = 0;
};

/// size 0x4
/// vtable ?
class ICvEngineUtility4 : public ICvEngineUtility3
{
public:
	/// inline
	static GUID DLLCALL GetInterfaceId() { return guidICvEngineUtility4; }

	/// pure
	virtual bool DLLCALL GetEvaluatedMapScriptPath(_In_z_ const char* szPath, _Inout_z_cap_(szEvaluatedPathSize) char* szEvaluatedPath, unsigned int szEvaluatedPathSize) const = 0;
};

#endif	// ICvDLLUtility_h
