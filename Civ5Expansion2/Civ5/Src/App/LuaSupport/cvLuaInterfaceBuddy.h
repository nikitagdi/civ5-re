/// origin guess
#pragma once
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

/// size ?
class cvLuaInterfaceBuddy {
public:
    /// 0x08943c48
    UNK_RET Register(lua_State* a1, lua_State* a2);
    /// 0x08943c86
    UNK_RET GetInstanceName();
    /// 0x08943c9a
    static int pRegisterLibrary(lua_State* a1);
    /// 0x08943cda
    static int RegisterMembers(lua_State* a1);
    /// 0x08946702
    UNK_RET GetInstance(lua_State* a1, int a2);
    /// 0x0894671e
    /// lua ?
    static int lLookAtSelectionPlot(lua_State* a1);
    /// 0x08946774
    /// lua ?
    static int lCanDoInterfaceMode(lua_State* a1);
    /// 0x089467d8
    /// lua ?
    static int lGetSelectionPlot(lua_State* a1);
    /// 0x089468e6
    /// lua ?
    static int lGetHeadSelectedUnit(lua_State* a1);
    /// 0x08946a20
    /// lua ?
    static int lGetSelectedUnitID(lua_State* a1);
    /// 0x08946aba
    /// lua ?
    static int lSelectUnit(lua_State* a1);
    /// 0x08946c5c
    /// lua ?
    static int lSelectGroup(lua_State* a1);
    /// 0x08946dee
    /// lua ?
    static int lSelectAll(lua_State* a1);
    /// 0x08946f0c
    /// lua ?
    static int lRemoveFromSelectionList(lua_State* a1);
    /// 0x08947044
    /// lua ?
    static int lMakeSelectionListDirty(lua_State* a1);
    /// 0x08947072
    /// lua ?
    static int lCanSelectionListFound(lua_State* a1);
    /// 0x089470b4
    /// lua ?
    static int lCanSelectionListWork(lua_State* a1);
    /// 0x089470f8
    /// lua ?
    static int lIsPopupUp(lua_State* a1);
    /// 0x0894713a
    /// lua ?
    static int lIsPopupQueued(lua_State* a1);
    /// 0x0894717c
    /// lua ?
    static int lIsDiploOrPopupWaiting(lua_State* a1);
    /// 0x089471be
    /// lua ?
    static int lIsPopupTypeOpen(lua_State* a1);
    /// 0x08947220
    /// lua ?
    static int lGetPopupType(lua_State* a1);
    /// 0x089472a6
    /// lua ?
    static int lGetPopupTypeName(lua_State* a1);
    /// 0x08947348
    /// lua ?
    static int lGetPopupTypeByIndex(lua_State* a1);
    /// 0x0894739e
    /// lua ?
    static int lGetPopupTypeCount(lua_State* a1);
    /// 0x089473e0
    /// lua ?
    static int lRegisterPopupType(lua_State* a1);
    /// 0x08947540
    /// lua ?
    static int lIsCityScreenViewingMode(lua_State* a1);
    /// 0x08947584
    /// lua ?
    static int lSetCityScreenViewingMode(lua_State* a1);
    /// 0x089475c6
    /// lua ?
    static int lGetMultiplayerLobbyMode(lua_State* a1);
    /// 0x0894760a
    /// lua ?
    static int lSetMultiplayerLobbyMode(lua_State* a1);
    /// 0x0894764c
    /// lua ?
    static int lGetLastSelectedUnit(lua_State* a1);
    /// 0x0894775c
    /// lua ?
    static int lSetLastSelectedUnit(lua_State* a1);
    /// 0x0894787a
    /// lua ?
    static int lChangePlotListColumn(lua_State* a1);
    /// 0x089478ba
    /// lua ?
    static int lGetOriginalPlot(lua_State* a1);
    /// 0x08947a26
    /// lua ?
    static int lClearSelectionList(lua_State* a1);
    /// 0x08947a54
    /// lua ?
    static int lInsertIntoSelectionList(lua_State* a1);
    /// 0x08947c50
    /// lua ?
    static int lSelectCity(lua_State* a1);
    /// 0x08947da2
    /// lua ?
    static int lSelectLookAtCity(lua_State* a1);
    /// 0x08947dfa
    /// lua ?
    static int lAddSelectedCity(lua_State* a1);
    /// 0x08947f4c
    /// lua ?
    static int lClearSelectedCities(lua_State* a1);
    /// 0x08947f7a
    /// lua ?
    static int lIsCitySelected(lua_State* a1);
    /// 0x089480ba
    /// lua ?
    static int lGetHeadSelectedCity(lua_State* a1);
    /// 0x089481ce
    /// lua ?
    static int lIsCitySelection(lua_State* a1);
    /// 0x08948210
    /// lua ?
    static int lUpdateCityScreen(lua_State* a1);
    /// 0x08948332
    /// lua ?
    static int lSetPlaceUnit(lua_State* a1);
    /// 0x0894844e
    /// lua ?
    static int lGetPlaceUnit(lua_State* a1);
    /// 0x0894855c
    /// lua ?
    static int lClearPlaceUnit(lua_State* a1);
    /// 0x0894858a
    /// lua ?
    static int lCanPlaceUnitAt(lua_State* a1);
    /// 0x08948732
    /// lua ?
    static int lHighlightCanPlacePlots(lua_State* a1);
    /// 0x089488b8
    /// lua ?
    static int lClearEventMessages(lua_State* a1);
    /// 0x089488bc
    /// lua ?
    static int lGetCycleSelectionCounter(lua_State* a1);
    /// 0x089488fe
    /// lua ?
    static int lSetCycleSelectionCounter(lua_State* a1);
    /// 0x0894893e
    /// lua ?
    static int lChangeCycleSelectionCounter(lua_State* a1);
    /// 0x0894897e
    /// lua ?
    static int lGetEndTurnCounter(lua_State* a1);
    /// 0x089489c0
    /// lua ?
    static int lSetEndTurnCounter(lua_State* a1);
    /// 0x08948a00
    /// lua ?
    static int lChangeEndTurnCounter(lua_State* a1);
    /// 0x08948a40
    /// lua ?
    static int lCheckForCommandLineInvitation(lua_State* a1);
    /// 0x08948a84
    /// lua ?
    static int lIsCombatFocus(lua_State* a1);
    /// 0x08948ac6
    /// lua ?
    static int lSetCombatFocus(lua_State* a1);
    /// 0x08948b06
    /// lua ?
    static int lIsDirty(lua_State* a1);
    /// 0x08948b48
    /// lua ?
    static int lSetDirty(lua_State* a1);
    /// 0x08948b88
    /// lua ?
    static int lMakeInterfaceDirty(lua_State* a1);
    /// 0x08948bb8
    /// lua ?
    static int lUpdateCursorType(lua_State* a1);
    /// 0x08948bfa
    /// lua ?
    static int lLookAt(lua_State* a1);
    /// 0x08948d3e
    /// lua ?
    static int lCenterCamera(lua_State* a1);
    /// 0x08948e60
    /// lua ?
    static int lReleaseLockedCamera(lua_State* a1);
    /// 0x08948e90
    /// lua ?
    static int lIsCameraMoving(lua_State* a1);
    /// 0x08948ed4
    /// lua ?
    static int lIsFocused(lua_State* a1);
    /// 0x08948f16
    /// lua ?
    static int lRefreshYieldVisibleMode(lua_State* a1);
    /// 0x08948f46
    /// lua ?
    static int lToggleYieldVisibleMode(lua_State* a1);
    /// 0x08948f76
    /// lua ?
    static int lSetYieldVisibleMode(lua_State* a1);
    /// 0x08948fb8
    /// lua ?
    static int lToggleResourceVisibleMode(lua_State* a1);
    /// 0x08948fe8
    /// lua ?
    static int lSetResourceVisibleMode(lua_State* a1);
    /// 0x0894902a
    /// lua ?
    static int lToggleGridVisibleMode(lua_State* a1);
    /// 0x0894905a
    /// lua ?
    static int lSetGridVisibleMode(lua_State* a1);
    /// 0x0894909c
    /// lua ?
    static int lGetOriginalPlotCount(lua_State* a1);
    /// 0x089490de
    /// lua ?
    static int lIsCityScreenUp(lua_State* a1);
    /// 0x08949120
    /// lua ?
    static int lSetCityScreenUp(lua_State* a1);
    /// 0x08949160
    /// lua ?
    static int lCanEndTurn(lua_State* a1);
    /// 0x089491a2
    /// lua ?
    static int lWaitingForRemotePlayers(lua_State* a1);
    /// 0x089491e4
    /// lua ?
    static int lSetInterfaceMode(lua_State* a1);
    /// 0x08949224
    /// lua ?
    static int lGetInterfaceMode(lua_State* a1);
    /// 0x08949266
    /// lua ?
    static int lSetInterfaceModeValue(lua_State* a1);
    /// 0x089492ae
    /// lua ?
    static int lGetInterfaceModeValue(lua_State* a1);
    /// 0x089492f0
    /// lua ?
    static int lGetInterfaceModeDebugItemID1(lua_State* a1);
    /// 0x08949334
    /// lua ?
    static int lSetInterfaceModeDebugItemID1(lua_State* a1);
    /// 0x08949376
    /// lua ?
    static int lGetInterfaceModeDebugItemID2(lua_State* a1);
    /// 0x089493ba
    /// lua ?
    static int lSetInterfaceModeDebugItemID2(lua_State* a1);
    /// 0x089493fc
    /// lua ?
    static int lSetFlashing(lua_State* a1);
    /// 0x0894943c
    /// lua ?
    static int lIsFlashing(lua_State* a1);
    /// 0x0894947e
    /// lua ?
    static int lSetDiplomacyLocked(lua_State* a1);
    /// 0x089494be
    /// lua ?
    static int lIsDiplomacyLocked(lua_State* a1);
    /// 0x08949500
    /// lua ?
    static int lSetCanEndTurn(lua_State* a1);
    /// 0x08949540
    /// lua ?
    static int lIsHasMovedUnit(lua_State* a1);
    /// 0x08949582
    /// lua ?
    static int lSetHasMovedUnit(lua_State* a1);
    /// 0x089495c2
    /// lua ?
    static int lIsForcePopup(lua_State* a1);
    /// 0x08949604
    /// lua ?
    static int lSetForcePopup(lua_State* a1);
    /// 0x08949644
    /// lua ?
    static int lLookAtCityOffset(lua_State* a1);
    /// 0x08949684
    /// lua ?
    static int lGetPlotListColumn(lua_State* a1);
    /// 0x089496c6
    /// lua ?
    static int lVerifyPlotListColumn(lua_State* a1);
    /// 0x089496f6
    /// lua ?
    static int lGetPlotListOffset(lua_State* a1);
    /// 0x08949738
    /// lua ?
    static int lExitingToMainMenu(lua_State* a1);
    /// 0x08949792
    /// lua ?
    static int lIsInAdvancedStart(lua_State* a1);
    /// 0x089497d4
    /// lua ?
    static int lSetInAdvancedStart(lua_State* a1);
    /// 0x08949814
    /// lua ?
    static int lSetBusy(lua_State* a1);
    /// 0x08949854
    /// lua ?
    static int lIsNoSelectionListCycle(lua_State* a1);
    /// 0x08949898
    /// lua ?
    static int lSetNoSelectionListCycle(lua_State* a1);
    /// 0x089498d8
    /// lua ?
    static int lLocationSelect(lua_State* a1);
    /// 0x08949a86
    /// lua ?
    static int lDoSelectCityAtPlot(lua_State* a1);
    /// 0x08949bc2
    /// lua ?
    static int lAltKeyDown(lua_State* a1);
    /// 0x08949c0a
    /// lua ?
    static int lShiftKeyDown(lua_State* a1);
    /// 0x08949c52
    /// lua ?
    static int lCtrlKeyDown(lua_State* a1);
    /// 0x08949c9a
    /// lua ?
    static int lGetMouseOverHex(lua_State* a1);
    /// 0x08949cee
    /// lua ?
    static int lGetScratchDeal(lua_State* a1);
    /// 0x08949dfe
    /// lua ?
    static int lActivateNotification(lua_State* a1);
    /// 0x08949e44
    /// lua ?
    static int lRemoveNotification(lua_State* a1);
    /// 0x08949e8a
    /// lua ?
    static int lRebroadcastNotifications(lua_State* a1);
    /// 0x08949eb8
    /// lua ?
    static int lGetVersionInfo(lua_State* a1);
    /// 0x08949f20
    /// lua ?
    static int lExitGame(lua_State* a1);
    /// 0x08949f58
    /// lua ?
    static int lSaveFileList(lua_State* a1);
    /// 0x0894a22c
    /// lua ?
    static int lLoadSaveFile(lua_State* a1);
    /// 0x0894a2f4
    /// lua ?
    static int lQuickSave(lua_State* a1);
    /// 0x0894a320
    /// lua ?
    static int lSaveGame(lua_State* a1);
    /// 0x0894a5de
    /// lua ?
    static int lDeleteSavedGame(lua_State* a1);
    /// 0x0894a76a
    /// lua ?
    static int lGetSavedGameModificationTime(lua_State* a1);
    /// 0x0894a964
    /// lua ?
    static int lGetSavedGameModificationTimeRaw(lua_State* a1);
    /// 0x0894aaec
    /// lua ?
    static int lCopyLastAutoSave(lua_State* a1);
    /// 0x0894b10e
    /// lua ?
    static int lSaveMap(lua_State* a1);
    /// 0x0894b488
    /// lua ?
    static int lGetMapPreview(lua_State* a1);
    /// 0x0894b5ba
    /// lua ?
    static int lGetMapPlayers(lua_State* a1);
    /// 0x0894b676
    /// lua ?
    static int lIsMapScenario(lua_State* a1);
    /// 0x0894b816
    /// lua ?
    static int lResetScenarioPlayerSlots(lua_State* a1);
    /// 0x0894b8c8
    /// lua ?
    static int lMoveScenarioPlayerToSlot(lua_State* a1);
    /// 0x0894b9b8
    /// lua ?
    static int lSetNextGameState(lua_State* a1);
    /// 0x0894b9fa
    /// lua ?
    static int lGetCurrentGameState(lua_State* a1);
    /// 0x0894ba3e
    /// lua ?
    static int lincTurnTimerSemaphore(lua_State* a1);
    /// 0x0894ba6c
    /// lua ?
    static int ldecTurnTimerSemaphore(lua_State* a1);
    /// 0x0894ba9a
    /// lua ?
    static int linterruptTurnTimer(lua_State* a1);
    /// 0x0894bac8
    /// lua ?
    static int lOnHumanOpenedTradeScreen(lua_State* a1);
    /// 0x0894bb0a
    /// lua ?
    static int lOnHumanDemand(lua_State* a1);
    /// 0x0894bb4c
    /// lua ?
    static int lDoProposeDeal(lua_State* a1);
    /// 0x0894bb7a
    /// lua ?
    static int lDoDemand(lua_State* a1);
    /// 0x0894bba8
    /// lua ?
    static int lDoEqualizeDealWithHuman(lua_State* a1);
    /// 0x0894bbd6
    /// lua ?
    static int lDoWhatWillAIGive(lua_State* a1);
    /// 0x0894bc04
    /// lua ?
    static int lDoWhatDoesAIWant(lua_State* a1);
    /// 0x0894bc32
    /// lua ?
    static int lIsAIRequestingConcessions(lua_State* a1);
    /// 0x0894bc74
    /// lua ?
    static int lIsHumanMakingDemand(lua_State* a1);
    /// 0x0894bcb6
    /// lua ?
    static int lGetRepeatActionPlayer(lua_State* a1);
    /// 0x0894bcfa
    /// lua ?
    static int lSetRepeatActionPlayer(lua_State* a1);
    /// 0x0894bd3c
    /// lua ?
    static int lGetStartDiploRepeatCount(lua_State* a1);
    /// 0x0894bd7e
    /// lua ?
    static int lChangeStartDiploRepeatCount(lua_State* a1);
    /// 0x0894bdbe
    /// lua ?
    static int lGetOfferTradeRepeatCount(lua_State* a1);
    /// 0x0894be00
    /// lua ?
    static int lSetOfferTradeRepeatCount(lua_State* a1);
    /// 0x0894be40
    /// lua ?
    static int lChangeOfferTradeRepeatCount(lua_State* a1);
    /// 0x0894be80
    /// lua ?
    static int lOpenTechTree(lua_State* a1);
    /// 0x0894beb0
    /// lua ?
    static int lBindMouse(lua_State* a1);
    /// 0x0894bef2
    /// lua ?
    static int lDebugKeyHandler(lua_State* a1);
    /// 0x0894bf34
    /// lua ?
    static int lBulkHideUI(lua_State* a1);
    /// 0x0894bf76
    /// lua ?
    static int lHasMadeProposal(lua_State* a1);
    /// 0x0894c066
    /// lua ?
    static int lProposedDealExists(lua_State* a1);
    /// 0x0894c170
    /// lua ?
    static int lLoadProposedDeal(lua_State* a1);
    /// 0x0894c1d2
    /// lua ?
    static int lLoadCurrentDeal(lua_State* a1);
    /// 0x0894c214
    /// lua ?
    static int lLoadHistoricDeal(lua_State* a1);
    /// 0x0894c256
    /// lua ?
    static int lGetNumCurrentDeals(lua_State* a1);
    /// 0x0894c29a
    /// lua ?
    static int lGetNumHistoricDeals(lua_State* a1);
    /// 0x0894c2de
    /// lua ?
    static int lDoFinalizePlayerDeal(lua_State* a1);
    /// 0x0894c366
    /// lua ?
    static int lRequestMinimapBroadcast(lua_State* a1);
    /// 0x0894c394
    /// lua ?
    static int lGetTempString(lua_State* a1);
    /// 0x0894c422
    /// lua ?
    static int lIsForceDiscussionModeQuitOnBack(lua_State* a1);
    /// 0x0894c464
    /// lua ?
    static int lSetForceDiscussionModeQuitOnBack(lua_State* a1);
    /// 0x0894c4a4
    /// lua ?
    static int lIsLoadedGame(lua_State* a1);
    /// 0x0894c4ec
    /// lua ?
    static int lGetCredits(lua_State* a1);
    /// 0x0894c786
    /// lua ?
    static int lGetHallofFameData(lua_State* a1);
    /// 0x0894d50e
    /// lua ?
    static int lRequestLeaderboardScores(lua_State* a1);
    /// 0x0894d53c
    /// lua ?
    static int lGetLeaderboardScores(lua_State* a1);
    /// 0x0894d572
    /// lua ?
    static int lGetLeaderboardTitle(lua_State* a1);
    /// 0x0894d5a8
    /// lua ?
    static int lGetLeaderboardUserDataVersion(lua_State* a1);
    /// 0x0894d5d6
    /// lua ?
    static int lGetAvailableLeaderboards(lua_State* a1);
    /// 0x0894d60c
    /// lua ?
    static int lSetLeaderboard(lua_State* a1);
    /// 0x0894d67a
    /// lua ?
    static int lSetLeaderboardCategory(lua_State* a1);
    /// 0x0894d6be
    /// lua ?
    static int lScrollLeaderboardUp(lua_State* a1);
    /// 0x0894d6e8
    /// lua ?
    static int lScrollLeaderboardDown(lua_State* a1);
    /// 0x0894d712
    /// lua ?
    static int lGetDllGUID(lua_State* a1);
    /// 0x0894d89e
    /// lua ?
    static int lSendPathfinderUpdate(lua_State* a1);
    /// 0x0894d8cc
    /// lua ?
    static int lDebugFlag(lua_State* a1);
    /// 0x0894d90a
    /// lua ?
    static int lHasShownLegal(lua_State* a1);
    /// 0x0894d948
    /// lua ?
    static int lRequestLeaveLeader(lua_State* a1);
    /// 0x0894d976
    /// lua ?
    static int lGetLeaderHeadRootUp(lua_State* a1);
    /// 0x0894d9ba
    /// lua ?
    static int lSetLeaderHeadRootUp(lua_State* a1);
    /// 0x0894d9fc
    /// lua ?
    static int lAddPopup(lua_State* a1);
    /// 0x0894dce8
    /// lua ?
    static int lIsDX9(lua_State* a1);
    /// 0x0894dd16
    /// lua ?
    static int lAreMediumLeadersAllowed(lua_State* a1);
    /// 0x0894dd4a
    /// lua ?
    static int lUnlockAchievement(lua_State* a1);
    /// 0x0894e460
    /// lua ?
    static int lTestLeaderMessage(lua_State* a1);
    /// 0x0894e596
    /// lua ?
    static int lGetReplayFiles(lua_State* a1);
    /// 0x0894e996
    /// lua ?
    static int lDeleteReplayFile(lua_State* a1);
    /// 0x0894eae4
    /// lua ?
    static int lGetReplayFileHeader(lua_State* a1);
    /// 0x0894ef70
    /// lua ?
    static int lGetReplayModificationTime(lua_State* a1);
    /// 0x0894f126
    /// lua ?
    static int lGetReplayModificationTimeRaw(lua_State* a1);
    /// 0x0894f26e
    /// lua ?
    static int lGetReplayInfo(lua_State* a1);
    /// 0x0894f49a
    /// lua ?
    static int lCompareFileTime(lua_State* a1);
    /// 0x0894f52a
    /// lua ?
    static int lSetAdvisorMessageHasBeenSeen(lua_State* a1);
    /// 0x0894f59a
    /// lua ?
    static int lHasAdvisorMessageBeenSeen(lua_State* a1);
    /// 0x0894f5fc
    /// lua ?
    static int lPauseSounds(lua_State* a1);
    /// 0x0894f682
    /// lua ?
    static int lIsSoundPaused(lua_State* a1);
    /// 0x0894f6c2
    /// lua ?
    static int lAddPopupText(lua_State* a1);
    /// 0x0894f776
    /// lua ?
    static int lSetDontShowPopups(lua_State* a1);
    /// 0x0894f7c8
    /// lua ?
    static int lGetDontShowPopups(lua_State* a1);
    /// 0x0894f810
    /// lua ?
    static int lPostKeyMessage(lua_State* a1);
    /// 0x0894f862
    /// lua ?
    static int lIsTouchScreenEnabled(lua_State* a1);
    /// 0x0894f8a8
    /// lua ?
    static int lGetUnitFlagIcon(lua_State* a1);
    /// 0x0894fc8c
    /// lua ?
    static int lGetUnitPortraitIcon(lua_State* a1);
    /// 0x08950070
    UNK_RET GetFileListFromDir(int& a1, FLua::Table& a2, int a3, int a4, bool a5);
};

/// 0x0895de62
UNK_RET isnalum(char a1);

namespace cvLuaLocaleLibrary {
/// 0x0895131c
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x0895135a
int pRegister(lua_State* a1);
/// 0x08951a12
/// lua ?
int lConvertTextKey(lua_State* a1);
/// 0x08951b92
/// lua ?
int lHasTextKey(lua_State* a1);
/// 0x08951c12
/// lua ?
int lGetHotKeyDescription(lua_State* a1);
/// 0x08951d6c
/// lua ?
int lLookup(lua_State* a1);
/// 0x08951eec
/// lua ?
int lLookupLanguage(lua_State* a1);
/// 0x0895210a
/// lua ?
int lStartsWith(lua_State* a1);
/// 0x089521a8
/// lua ?
int lEndsWith(lua_State* a1);
/// 0x0895235a
/// lua ?
int lIsNilOrEmpty(lua_State* a1);
/// 0x089523be
/// lua ?
int lIsNilOrWhitespace(lua_State* a1);
/// 0x089524e6
/// lua ?
int lLength(lua_State* a1);
/// 0x08952558
/// lua ?
int lToUpper(lua_State* a1);
/// 0x08952602
/// lua ?
int lToLower(lua_State* a1);
/// 0x089526ac
/// lua ?
int lToCurrency(lua_State* a1);
/// 0x0895275e
/// lua ?
int lToNumber(lua_State* a1);
/// 0x08952846
/// lua ?
int lToPercent(lua_State* a1);
/// 0x089528f8
/// lua ?
int lToRomanNumeral(lua_State* a1);
/// 0x089529aa
/// lua ?
int lToSpellout(lua_State* a1);
/// 0x08952a5c
/// lua ?
int lToOrdinal(lua_State* a1);
/// 0x08952b0e
/// lua ?
int lTruncateString(lua_State* a1);
/// 0x08952c56
/// lua ?
int lSubstring(lua_State* a1);
/// 0x08952d40
/// lua ?
int lCompare(lua_State* a1);
/// 0x08952e10
/// lua ?
int lSetCurrentLanguage(lua_State* a1);
/// 0x0895309e
/// lua ?
int lGetCurrentLanguage(lua_State* a1);
/// 0x08953158
/// lua ?
int lGetSupportedLanguages(lua_State* a1);
/// 0x08953254
/// lua ?
int lGetCurrentSpokenLanguage(lua_State* a1);
/// 0x08953546
/// lua ?
int lSetCurrentSpokenLanguage(lua_State* a1);
/// 0x08953872
/// lua ?
int lGetSupportedSpokenLanguages(lua_State* a1);
/// 0x08953cc0
/// lua ?
int lIsASCII(lua_State* a1);
/// 0x08953d32
/// lua ?
int lIsUTF8(lua_State* a1);
/// 0x08953dbc
/// lua ?
int lLocalSystemCPToUTF8(lua_State* a1);
/// 0x08953f8e
/// lua ?
int lUTF8ToLocalSystemCP(lua_State* a1);
/// 0x08954160
/// lua ?
int lConvertPosixTimeToString(lua_State* a1);
}

namespace cvLuaMatchmakingLibrary {
/// 0x08954530
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x0895456e
int pRegister(lua_State* a1);
/// 0x08954b8a
/// lua ?
int lInitInternetLobby(lua_State* a1);
/// 0x08954bd8
/// lua ?
int lInitLanLobby(lua_State* a1);
/// 0x08954c26
/// lua ?
int lRefreshLANGameList(lua_State* a1);
/// 0x08954c74
/// lua ?
int lRefreshInternetGameList(lua_State* a1);
/// 0x08954d82
/// lua ?
int lGetMultiplayerGameList(lua_State* a1);
/// 0x08954e0c
/// lua ?
int lGetMultiplayerServerEntry(lua_State* a1);
/// 0x08954e9c
/// lua ?
int lGetMultiplayerGameCount(lua_State* a1);
/// 0x08954ee4
/// lua ?
int lSetMultiplayerGameListType(lua_State* a1);
/// 0x08954f50
/// lua ?
int lGetMultiplayerGameListType(lua_State* a1);
/// 0x08954fb4
/// lua ?
int lIsRefreshingGameList(lua_State* a1);
/// 0x08955002
/// lua ?
int lStopRefreshingGameList(lua_State* a1);
/// 0x08955034
/// lua ?
int lHostLANGame(lua_State* a1);
/// 0x089551c0
/// lua ?
int lHostInternetGame(lua_State* a1);
/// 0x089553ca
/// lua ?
int lHostHotSeatGame(lua_State* a1);
/// 0x0895548e
/// lua ?
int lHostServerGame(lua_State* a1);
/// 0x089556ae
/// lua ?
int lJoinIPAddress(lua_State* a1);
/// 0x0895572e
/// lua ?
int lJoinMultiplayerGame(lua_State* a1);
/// 0x089557c0
/// lua ?
int lLeaveMultiplayerGame(lua_State* a1);
/// 0x0895585e
/// lua ?
int lLaunchMultiplayerGame(lua_State* a1);
/// 0x08955886
/// lua ?
int lIsLaunchingGame(lua_State* a1);
/// 0x089558ce
/// lua ?
int lGetPlayerList(lua_State* a1);
/// 0x08955dae
/// lua ?
int lGetPlayerCount(lua_State* a1);
/// 0x08955df8
/// lua ?
int lGetCurrentGameName(lua_State* a1);
/// 0x08955f94
/// lua ?
int lGetFriendList(lua_State* a1);
/// 0x08956130
/// lua ?
int lIsHost(lua_State* a1);
/// 0x08956178
/// lua ?
int lGetHostID(lua_State* a1);
/// 0x089561be
/// lua ?
int lGetLocalID(lua_State* a1);
/// 0x08956204
/// lua ?
int lSetMultiplayerResumeGame(lua_State* a1);
/// 0x089565ea
/// lua ?
int lKickPlayer(lua_State* a1);
}

namespace cvLuaModdingLibrary {
/// 0x08957234
UNK_RET Register(lua_State* a1, lua_State* a2);
/// 0x08957272
int pRegister(lua_State* a1);
/// 0x08957f76
/// lua ?
int lDiscoverMods(lua_State* a1);
/// 0x08957fa2
/// lua ?
int lUpdateModdingSystem(lua_State* a1);
/// 0x08957fce
/// lua ?
int lGetModBrowserInstalledListings(lua_State* a1);
/// 0x08958436
/// lua ?
int lGetModBrowserDownloadingListings(lua_State* a1);
/// 0x0895868a
/// lua ?
int lGetModsBrowserInstalledListingsState(lua_State* a1);
/// 0x089586cc
/// lua ?
int lGetDownloadProgress(lua_State* a1);
/// 0x0895875e
/// lua ?
int lGetInstalledModDetails(lua_State* a1);
/// 0x089588d2
/// lua ?
int lHasPendingInstalls(lua_State* a1);
/// 0x08958916
/// lua ?
int lInstallMods(lua_State* a1);
/// 0x08958942
/// lua ?
int lGetInstallProgress(lua_State* a1);
/// 0x08958a4a
/// lua ?
int lGetActivatedMods(lua_State* a1);
/// 0x08958bd0
/// lua ?
int lGetInstalledMods(lua_State* a1);
/// 0x08958f24
/// lua ?
int lGetLatestInstalledModVersion(lua_State* a1);
/// 0x08958f84
/// lua ?
int lIsModInstalled(lua_State* a1);
/// 0x08959038
/// lua ?
int lGetInstalledFiraxisScenarios(lua_State* a1);
/// 0x08959096
/// lua ?
int lGetActivatedModVersion(lua_State* a1);
/// 0x08959190
/// lua ?
int lGetActivatedModEntryPoints(lua_State* a1);
/// 0x08959228
/// lua ?
int lGetEnabledModsByActivationOrder(lua_State* a1);
/// 0x0895980c
/// lua ?
int lGetEvaluatedFilePath(lua_State* a1);
/// 0x08959a94
/// lua ?
int lGetModsRequiredToDisableMod(lua_State* a1);
/// 0x0895a1f8
/// lua ?
int lGetModsRequiredToEnableMod(lua_State* a1);
/// 0x0895a1fc
/// lua ?
int lGetModEntryPoints(lua_State* a1);
/// 0x0895a2fe
/// lua ?
int lGetModProperty(lua_State* a1);
/// 0x0895a462
/// lua ?
int lGetModProperties(lua_State* a1);
/// 0x0895a5d2
/// lua ?
int lGetMapFiles(lua_State* a1);
/// 0x0895a838
/// lua ?
int lActivateSpecificMod(lua_State* a1);
/// 0x0895a922
/// lua ?
int lActivateEnabledMods(lua_State* a1);
/// 0x0895a9c0
/// lua ?
int lDeactivateMods(lua_State* a1);
/// 0x0895aa04
/// lua ?
int lDeactivateDLC(lua_State* a1);
/// 0x0895aa9c
/// lua ?
int lActivateDLC(lua_State* a1);
/// 0x0895ab42
/// lua ?
int lActivateAllowedDLC(lua_State* a1);
/// 0x0895ad22
/// lua ?
int lActivateModsAndDLCForReplay(lua_State* a1);
/// 0x0895aedc
/// lua ?
int lPerformActions(lua_State* a1);
/// 0x0895af44
/// lua ?
int lPostProcessModActions(lua_State* a1);
/// 0x0895af70
/// lua ?
int lCanEnableMod(lua_State* a1);
/// 0x0895b6f2
/// lua ?
int lEnableMod(lua_State* a1);
/// 0x0895b76e
/// lua ?
int lDisableMod(lua_State* a1);
/// 0x0895b7ea
/// lua ?
int lUpdateMod(lua_State* a1);
/// 0x0895b852
/// lua ?
int lCanUnsubscribeMod(lua_State* a1);
/// 0x0895b8ca
/// lua ?
int lCanDeleteMod(lua_State* a1);
/// 0x0895b942
/// lua ?
int lUnsubscribeMod(lua_State* a1);
/// 0x0895b9a8
/// lua ?
int lDeleteMod(lua_State* a1);
/// 0x0895ba32
/// lua ?
int lDeleteUserData(lua_State* a1);
/// 0x0895baac
/// lua ?
int lHasUserData(lua_State* a1);
/// 0x0895bb82
/// lua ?
int lAllActivatedModsContainPropertyValue(lua_State* a1);
/// 0x0895bc4e
/// lua ?
int lAnyActivatedModsContainPropertyValue(lua_State* a1);
/// 0x0895bcd2
/// lua ?
int lAllEnabledModsContainPropertyValue(lua_State* a1);
/// 0x0895bd9e
/// lua ?
int lAnyEnabledModsContainPropertyValue(lua_State* a1);
/// 0x0895be22
/// lua ?
int lGetCloudSaveRequirements(lua_State* a1);
/// 0x0895c23e
/// lua ?
int lGetReplayRequirements(lua_State* a1);
/// 0x0895c746
/// lua ?
int lGetSavedGameRequirements(lua_State* a1);
/// 0x0895cc4e
/// lua ?
int lCanLoadCloudSave(lua_State* a1);
/// 0x0895ce72
/// lua ?
int lCanLoadReplay(lua_State* a1);
/// 0x0895d02e
/// lua ?
int lCanLoadSavedGame(lua_State* a1);
/// 0x0895d1ec
/// lua ?
int lGetDlcNameDescriptionKeys(lua_State* a1);
/// 0x0895d3dc
/// lua ?
int lGetModAssociations(lua_State* a1);
/// 0x0895d6cc
/// lua ?
int lGetDlcAssociations(lua_State* a1);
/// 0x0895d95c
/// lua ?
int lGetGameVersionAssociations(lua_State* a1);
/// 0x0895dba2
/// lua ?
int lGetSystemProperty(lua_State* a1);
/// 0x0895dc8a
/// lua ?
int lSetSystemProperty(lua_State* a1);
/// 0x0895dd82
/// lua ?
int lPublishedFileIdsMatch(lua_State* a1);
/// 0x0895dde4
/// lua ?
int lIterateQuery(lua_State* a1);
}
