/// origin guess
#pragma once

#include <list>

#include <Windows.h>
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvStructs.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include <string>
#include "FirePlace/FireWorks/FFireTypes.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"


// {02891D70-8B76-412a-A8A5-5363DBF0B22B}
/// inline
static const GUID guidICvUserInterface1 = 
{ 0x2891d70, 0x8b76, 0x412a, { 0xa8, 0xa5, 0x53, 0x63, 0xdb, 0xf0, 0xb2, 0x2b } };

// {9EB813D2-A2EE-4fbf-9433-4A8A811922EC}
/// inline
static const GUID guidICvUserInterface2 = 
{ 0x9eb813d2, 0xa2ee, 0x4fbf, { 0x94, 0x33, 0x4a, 0x8a, 0x81, 0x19, 0x22, 0xec } };


//
// abstract class containing InterfaceBuddy functions that the DLL needs
//
#include "Civ5/Src/App/CvGameCoreDLLUtil/LinkedList.h"

class ICvEnumerator;
class ICvCity1;
class ICvDeal1;

typedef std::list<CvPopupInfo*> CvPopupQueue;

//This interface has been deprecated and should no longer be used. 
//Please use the versioned interfaces instead.
//PROGRAMMERS: This interface is locked, do not modify this interface in ANY way.
/// size 0x4
/// vtable ?
class CvDLLInterfaceIFaceBase
{
public:
	enum TURN_STATUS_TYPE {
		TURN_START = 0,
		TURN_END = 1,
		TURN_STEP = 2
	};

	/// pure
	virtual void lookAtSelectionPlot(bool bRelease = false) = 0;

	//virtual bool canHandleAction(int iAction, ICvPlot1* pPlot = NULL, bool bTestVisible = false) = 0;
	/// pure
	virtual bool CanDoInterfaceMode(InterfaceModeTypes eInterfaceMode, bool bTestVisible = false) = 0;

	/// pure
	_Ret_maybenull_ virtual ICvPlot1* getSelectionPlot() = 0;
	/// pure
	virtual ICvUnit1* GetHeadSelectedUnit() = 0;
	/// pure
	virtual bool DoAutoUnitCycle() = 0;
	
	/// pure
	virtual ICvDeal1* GetScratchDeal() = 0;
	/// pure
	virtual void SetScratchDeal(ICvDeal1* pDeal) = 0;

	/// pure
	virtual bool isPopupUp() = 0;
	/// pure
	virtual bool isPopupQueued() = 0;
	/// pure
	virtual bool isDiploOrPopupWaiting() = 0;
	/// pure
	virtual bool isDiploActive() = 0;

	/// pure
	virtual bool IsUnitSelected(PlayerTypes playerID, int unitID) = 0;

	/// pure
	virtual void selectUnit(ICvUnit1* pUnit, bool bClear, bool bToggle = false, bool bSound = false) = 0;
	/// pure
	virtual void selectGroup(ICvUnit1* pUnit, bool bShift, bool bCtrl, bool bAlt) = 0;
	/// pure
	virtual void selectAll(ICvPlot1* pPlot) = 0;

	/// pure
	virtual bool RemoveFromSelectionList(ICvUnit1* pUnit) = 0;
	/// pure
	virtual void makeSelectionListDirty() = 0;
	/// pure
	virtual bool CanSelectionListFound() = 0;

	/// pure
	virtual ICvUnit1* getLastSelectedUnit() = 0;
	/// pure
	virtual void setLastSelectedUnit(ICvUnit1* pUnit) = 0;
	/// pure
	virtual void changePlotListColumn(int iChange) = 0;
	/// pure
	virtual int getOriginalPlotIndex() = 0;

	/// pure
	virtual void ClearSelectionList() = 0;
	/// pure
	virtual void InsertIntoSelectionList(ICvUnit1* pUnit, bool bClear, bool bToggle, bool bGroup = false, bool bSound = false, bool bMinimalChange = false) = 0;
	/// pure
	virtual int GetLengthSelectionList() = 0;
	/// pure
	virtual void VisuallyDeselectUnit(ICvUnit1* pUnit) = 0;
	/// pure
	virtual ICvEnumerator* GetSelectedUnits() = 0;

	/// pure
	virtual void selectCity(ICvCity1* pNewValue, bool bTestProduction = false) = 0;
	/// pure
	virtual void selectLookAtCity(bool bAdd = false) = 0;
	/// pure
	virtual void addSelectedCity(ICvCity1* pNewValue, bool bToggle = false) = 0;
	/// pure
	virtual void clearSelectedCities() = 0;
	/// pure
	virtual bool isCitySelected(ICvCity1 *pCity) = 0;
	/// pure
	virtual ICvCity1* getHeadSelectedCity() = 0;
	/// pure
	virtual bool isCitySelection() = 0;
	/// pure
	virtual IDInfo* nextSelectedCitiesNode(IDInfo* pNode) = 0;
	/// pure
	virtual const IDInfo* nextSelectedCitiesNode(const IDInfo* pNode) const = 0;
	/// pure
	virtual IDInfo* headSelectedCitiesNode() = 0;
	/// pure
	virtual void updateCityScreen(ICvCity1* pOldCity = NULL) = 0;

	/// pure
	virtual uint AddMessage(uint uiParentEvent, PlayerTypes ePlayer, bool bForce, int iLength, const char* strString, LPCTSTR pszSound = NULL,
		InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
		int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false) = 0;

	/// pure
	virtual uint AddUnitMessage(uint uiParentEvent, const IDInfo& unitID, PlayerTypes ePlayer, bool bForce, int iLength, const char* szString, LPCTSTR pszSound = NULL,
		InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
		int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false) = 0;

	/// pure
	virtual uint AddPlotMessage(uint uiParentEvent, int iPlotID, PlayerTypes ePlayer, bool bForce, int iLength, const char* szString, LPCTSTR pszSound = NULL,
		InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
		int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false) = 0;

	/// pure
	virtual uint AddCityMessage(uint uiParentEvent, const IDInfo& cityID, PlayerTypes ePlayer, bool bForce, int iLength, const char* szString, LPCTSTR pszSound = NULL,
		InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
		int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false) = 0;

	/// pure
	virtual void clearEventMessages() = 0;
	/// pure
	virtual void AddPopup(const CvPopupInfo& kPopup) = 0;
	/// pure
	virtual void AddPopupText( int x, int y, const char *szText, float fDelay = 0 ) = 0;
	/// pure
	virtual void PublishActivePlayerTurnStart() = 0;	// Only the 'active' (local human) player
    /// pure
    virtual void PublishActivePlayerTurnEnd() = 0;		// Only the 'active' (local human) player
	/// pure
	virtual void PublishRemotePlayerTurnEnd() = 0;
	/// pure
	virtual void PublishPlayerTurnStatus(TURN_STATUS_TYPE eStatus, PlayerTypes ePlayer, const char* pszTag = NULL) = 0;
	/// pure
	virtual void AddNotification( int iID, NotificationTypes type, const char *cTooltip, const char* cSummary, int iGameData, int iExtraGameData, PlayerTypes ePlayer, int iX = -1, int iY = -1) = 0;
	/// pure
	virtual void RemoveNotification( int iID, PlayerTypes ePlayer ) = 0;
	/// pure
	virtual void ActivateNotification (int iID, NotificationTypes type, const char* cString, int iX, int iY, int iGameIndex, int iExtraGameDataValue, PlayerTypes ePlayer) = 0;

	/// pure
	virtual int getCycleSelectionCounter() = 0;
	/// pure
	virtual void setCycleSelectionCounter(int iNewValue) = 0;
	/// pure
	virtual void changeCycleSelectionCounter(int iChange) = 0;

	/// pure
	virtual bool IsSelectedUnitRevealingNewPlots() const = 0;
	/// pure
	virtual void SetSelectedUnitRevealingNewPlots(bool bValue) = 0;

	/// pure
	virtual int getEndTurnCounter() = 0;
	/// pure
	virtual void setEndTurnCounter(int iNewValue) = 0;
	/// pure
	virtual void changeEndTurnCounter(int iChange) = 0;

	/// pure
	virtual bool isCombatFocus() = 0;
	/// pure
	virtual void setCombatFocus(bool bNewValue) = 0;

	/// pure
	virtual bool isDirty(InterfaceDirtyBits eDirtyItem) = 0;
	/// pure
	virtual void setDirty(InterfaceDirtyBits eDirtyItem, bool bNewValue) = 0;
	/// pure
	virtual void makeInterfaceDirty() = 0;
	/// pure
	virtual bool updateCursorType() = 0;

	/// pure
	virtual void lookAt(const ICvPlot1* pPlot, CameraLookAtTypes type) = 0;
	/// pure
	virtual void centerCamera(ICvUnit1*) = 0;
	/// pure
	virtual void releaseLockedCamera() = 0;
	/// pure
	virtual bool isFocusedWidget() = 0;
	/// pure
	virtual bool isFocused() = 0;
	/// pure
	virtual void refreshYieldVisibleMode() = 0;
	/// pure
	virtual void toggleYieldVisibleMode() = 0;
	/// pure
	virtual void toggleResourceVisibleMode() = 0;
	/// pure
	virtual void toggleGridVisibleMode() = 0;

	/// pure
	virtual int getOriginalPlotCount() = 0;
	/// pure
	virtual bool isCityScreenUp() = 0;
	/// pure
	virtual void setCityScreenUp(bool bUp) = 0;

	/// pure
	virtual void setNoSelectionListCycle(bool bValue) = 0;

	/// pure
	virtual bool canEndTurn() const = 0;
	/// pure
	virtual void UpdateEndTurn() = 0;
	/// pure
	virtual void SetEndTurnBlockingChanged(EndTurnBlockingTypes ePrevBlockingType, EndTurnBlockingTypes eNewBlockingType) = 0;
	/// pure
	virtual void setInterfaceMode(InterfaceModeTypes eNewValue) = 0;
	/// pure
	virtual InterfaceModeTypes getInterfaceMode() = 0;
	/// pure
	virtual int GetInterfaceModeValue() = 0;
	/// pure
	virtual void setFlashing(PlayerTypes eWho, bool bFlashing = true) = 0;
	/// pure
	virtual bool isFlashing(PlayerTypes eWho) = 0;
	/// pure
	virtual void setDiplomacyLocked(bool bLocked) = 0;
	/// pure
	virtual bool isDiplomacyLocked() = 0;

	/// pure
	virtual void setCanEndTurn(bool bNewValue) = 0;
	/// pure
	virtual void updateEndTurnTimer(float percentComplete) const = 0;
	/// pure
	virtual bool waitingForRemotePlayers() const = 0;

	/// pure
	virtual bool isHasMovedUnit() = 0;
	/// pure
	virtual void setHasMovedUnit(bool bNewValue) = 0;

	/// pure
	virtual bool isForcePopup() = 0;
	/// pure
	virtual void setForcePopup(bool bNewValue) = 0;

	/// pure
	virtual void lookAtCityOffset(int iCity) = 0;

	/// pure
	virtual int getPlotListColumn() = 0;
	/// pure
	virtual void verifyPlotListColumn() = 0;
	/// pure
	virtual int getPlotListOffset() = 0;

	/// pure
	virtual void exitingToMainMenu(const char* szLoadFile=NULL) = 0;
	
	/// pure
	virtual bool isInAdvancedStart() const = 0;
	/// pure
	virtual void setInAdvancedStart(bool bAdvancedStart) = 0;

	/// pure
	virtual void setBusy(bool bBusy) = 0;

	/// pure
	virtual void UpdateCountryBorder(ICvPlot1* pThisPlot) = 0;
	/// pure
	virtual void UpdateAllCountryBorders() = 0;

	/// pure
	virtual void DebugCityRoutePlots (ICvPlot1* pPlot, bool bOn) = 0;

	/// pure
	virtual void DirectionAction(DirectionTypes eDirection) = 0;

	/// pure
	virtual void SetSpecificCityInfoDirty(ICvCity1* pCity, CityUpdateTypes eUpdateType) = 0;

	/// pure
	virtual void AddHexToUIRange(ICvPlot1* pThisPlot) = 0;

	/// pure
	virtual void OpenEndGameMenu() = 0;

	/// pure
	virtual void doTurn() = 0;

	/// pure
	virtual bool IsOptionNoRewardPopups() = 0;

	/// pure
	virtual bool IsAIRequestingConcessions() const = 0;
	/// pure
	virtual void SetAIRequestingConcessions(bool bValue) = 0;
	/// pure
	virtual bool IsHumanMakingDemand() const = 0;
	/// pure
	virtual void SetHumanMakingDemand(bool bValue) = 0;

	/// pure
	virtual int GetStartDiploRepeatCount() const = 0;
	/// pure
	virtual void SetStartDiploRepeatCount(int iValue) = 0;
	/// pure
	virtual void ChangeStartDiploRepeatCount(int iChange) = 0;
	/// pure
	virtual int GetOfferTradeRepeatCount() const = 0;
	/// pure
	virtual void SetOfferTradeRepeatCount(int iValue) = 0;
	/// pure
	virtual void ChangeOfferTradeRepeatCount(int iChange) = 0;

	/// pure
	virtual void OpenTechTree() = 0;
	/// pure
	virtual void OpenPlayerDealScreen( PlayerTypes ePlayer ) = 0;

	/// pure
	virtual bool IsPolicyNotificationSeen() const = 0;
	/// pure
	virtual void SetPolicyNotificationSeen(bool bValue) = 0;

	/// pure
	virtual bool IsForceDiscussionModeQuitOnBack() const = 0;
	/// pure
	virtual void SetForceDiscussionModeQuitOnBack(bool bValue) = 0;

	/// pure
	virtual void SetTempString(CvString strString) = 0;
	/// pure
	virtual void PublishEndTurnDirty() const = 0;

	/// pure
	virtual void AddDeferredWonderCommand(WonderCommandType eCommandType, ICvCity1* pThisCity, BuildingTypes eBuildingIndex, int iState) = 0;

	/// pure
	virtual bool IsLoadedGame() const = 0;

	/// pure
	virtual void ToggleStrategicView() = 0;
	/// pure
	virtual bool IsPopupQueueEmpty() = 0;
	/// pure
	virtual bool IsModalStackEmpty() = 0;

	/// pure
	virtual bool IsDontShowPopups() const = 0;
	/// pure
	virtual void SetDontShowPopups(bool bValue) = 0;

	/// pure
	virtual bool IsDealInTransit() = 0;
	/// pure
	virtual void SetDealInTransit(bool bValue) = 0;

	/// pure
	virtual bool IsSPAutoEndTurnEnabled() = 0;
	/// pure
	virtual void SetSPAutoEndTurnEnabled(bool bEnable) = 0;

	/// pure
	virtual bool IsMPAutoEndTurnEnabled() = 0;
	/// pure
	virtual void SetMPAutoEndTurnEnabled(bool bEnable) = 0;

	/// pure
	virtual bool IsSPQuickCombatEnabled() = 0;
	/// pure
	virtual void SetSPQuickCombatEnabled(bool bEnable) = 0;

	/// pure
	virtual bool IsMPQuickCombatEnabled() = 0;
	/// pure
	virtual void SetMPQuickCombatEnabled(bool bEnable) = 0;
};


//THIS INTERFACE IS LOCKED.  DO NOT MODIFY IN ANY WAY.
//This interface was used for base game/expansion 1 support.
/// size 0x4
/// vtable ?
class ICvUserInterface1 : public ICvUnknown
{
public:
	/// inline
	static GUID DLLCALL GetInterfaceId() { return guidICvUserInterface1; }

	/// pure
	virtual void lookAtSelectionPlot(bool bRelease = false) = 0;

	/// pure
	virtual bool CanDoInterfaceMode(InterfaceModeTypes eInterfaceMode, bool bTestVisible = false) = 0;

	/// pure
	_Ret_maybenull_ virtual ICvPlot1* getSelectionPlot() = 0;
	/// pure
	virtual ICvUnit1* GetHeadSelectedUnit() = 0;
	/// pure
	virtual bool DoAutoUnitCycle() = 0;

	/// pure
	virtual ICvDeal1* GetScratchDeal() = 0;
	/// pure
	virtual void SetScratchDeal(ICvDeal1* pDeal) = 0;

	/// pure
	virtual bool isPopupUp() = 0;
	/// pure
	virtual bool isPopupQueued() = 0;
	/// pure
	virtual bool isDiploOrPopupWaiting() = 0;
	/// pure
	virtual bool isDiploActive() = 0;

	/// pure
	virtual bool IsUnitSelected(PlayerTypes playerID, int unitID) = 0;

	/// pure
	virtual void selectUnit(ICvUnit1* pUnit, bool bClear, bool bToggle = false, bool bSound = false) = 0;
	/// pure
	virtual void selectGroup(ICvUnit1* pUnit, bool bShift, bool bCtrl, bool bAlt) = 0;
	/// pure
	virtual void selectAll(ICvPlot1* pPlot) = 0;

	/// pure
	virtual bool RemoveFromSelectionList(ICvUnit1* pUnit) = 0;
	/// pure
	virtual void makeSelectionListDirty() = 0;
	/// pure
	virtual bool CanSelectionListFound() = 0;

	/// pure
	virtual ICvUnit1* getLastSelectedUnit() = 0;
	/// pure
	virtual void setLastSelectedUnit(ICvUnit1* pUnit) = 0;
	/// pure
	virtual void changePlotListColumn(int iChange) = 0;
	/// pure
	virtual int getOriginalPlotIndex() = 0;

	/// pure
	virtual void ClearSelectionList() = 0;
	/// pure
	virtual void InsertIntoSelectionList(ICvUnit1* pUnit, bool bClear, bool bToggle, bool bGroup = false, bool bSound = false, bool bMinimalChange = false) = 0;
	/// pure
	virtual int GetLengthSelectionList() = 0;
	/// pure
	virtual void VisuallyDeselectUnit(ICvUnit1* pUnit) = 0;
	/// pure
	virtual ICvEnumerator* GetSelectedUnits() = 0;

	/// pure
	virtual void selectCity(ICvCity1* pNewValue, bool bTestProduction = false) = 0;
	/// pure
	virtual void selectLookAtCity(bool bAdd = false) = 0;
	/// pure
	virtual void addSelectedCity(ICvCity1* pNewValue, bool bToggle = false) = 0;
	/// pure
	virtual void clearSelectedCities() = 0;
	/// pure
	virtual bool isCitySelected(ICvCity1 *pCity) = 0;
	/// pure
	virtual ICvCity1* getHeadSelectedCity() = 0;
	/// pure
	virtual bool isCitySelection() = 0;
	/// pure
	virtual IDInfo* nextSelectedCitiesNode(IDInfo* pNode) = 0;
	/// pure
	virtual const IDInfo* nextSelectedCitiesNode(const IDInfo* pNode) const = 0;
	/// pure
	virtual IDInfo* headSelectedCitiesNode() = 0;
	/// pure
	virtual void updateCityScreen(ICvCity1* pOldCity = NULL) = 0;

	/// pure
	virtual uint AddMessage(uint uiParentEvent, PlayerTypes ePlayer, bool bForce, int iLength, const char* strString, LPCTSTR pszSound = NULL,
		InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
		int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false) = 0;

	/// pure
	virtual uint AddUnitMessage(uint uiParentEvent, const IDInfo& unitID, PlayerTypes ePlayer, bool bForce, int iLength, const char* szString, LPCTSTR pszSound = NULL,
		InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
		int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false) = 0;

	/// pure
	virtual uint AddPlotMessage(uint uiParentEvent, int iPlotID, PlayerTypes ePlayer, bool bForce, int iLength, const char* szString, LPCTSTR pszSound = NULL,
		InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
		int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false) = 0;

	/// pure
	virtual uint AddCityMessage(uint uiParentEvent, const IDInfo& cityID, PlayerTypes ePlayer, bool bForce, int iLength, const char* szString, LPCTSTR pszSound = NULL,
		InterfaceMessageTypes eType = MESSAGE_TYPE_INFO, LPCSTR pszIcon = NULL, ColorTypes eFlashColor = NO_COLOR,
		int iFlashX = -1, int iFlashY = -1, bool bShowOffScreenArrows = false, bool bShowOnScreenArrows = false) = 0;

	/// pure
	virtual void clearEventMessages() = 0;
	/// pure
	virtual void AddPopup(const CvPopupInfo& kPopup) = 0;
	/// pure
	virtual void AddPopupText( int x, int y, const char *szText, float fDelay = 0 ) = 0;
	/// pure
	virtual void PublishActivePlayerTurnStart() = 0;	// Only the 'active' (local human) player
	/// pure
	virtual void PublishActivePlayerTurnEnd() = 0;		// Only the 'active' (local human) player
	/// pure
	virtual void PublishRemotePlayerTurnEnd() = 0;
	/// pure
	virtual void PublishPlayerTurnStatus(CvDLLInterfaceIFaceBase::TURN_STATUS_TYPE eStatus, PlayerTypes ePlayer, const char* pszTag = NULL) = 0;
	/// pure
	virtual void AddNotification( int iID, NotificationTypes type, const char *cTooltip, const char* cSummary, int iGameData, int iExtraGameData, PlayerTypes ePlayer, int iX = -1, int iY = -1) = 0;
	/// pure
	virtual void RemoveNotification( int iID, PlayerTypes ePlayer ) = 0;
	/// pure
	virtual void ActivateNotification (int iID, NotificationTypes type, const char* cString, int iX, int iY, int iGameIndex, int iExtraGameDataValue, PlayerTypes ePlayer) = 0;

	/// pure
	virtual int getCycleSelectionCounter() = 0;
	/// pure
	virtual void setCycleSelectionCounter(int iNewValue) = 0;
	/// pure
	virtual void changeCycleSelectionCounter(int iChange) = 0;

	/// pure
	virtual bool IsSelectedUnitRevealingNewPlots() const = 0;
	/// pure
	virtual void SetSelectedUnitRevealingNewPlots(bool bValue) = 0;

	/// pure
	virtual int getEndTurnCounter() = 0;
	/// pure
	virtual void setEndTurnCounter(int iNewValue) = 0;
	/// pure
	virtual void changeEndTurnCounter(int iChange) = 0;

	/// pure
	virtual bool isCombatFocus() = 0;
	/// pure
	virtual void setCombatFocus(bool bNewValue) = 0;

	/// pure
	virtual bool isDirty(InterfaceDirtyBits eDirtyItem) = 0;
	/// pure
	virtual void setDirty(InterfaceDirtyBits eDirtyItem, bool bNewValue) = 0;
	/// pure
	virtual void makeInterfaceDirty() = 0;
	/// pure
	virtual bool updateCursorType() = 0;

	/// pure
	virtual void lookAt(const ICvPlot1* pPlot, CameraLookAtTypes type) = 0;
	/// pure
	virtual void centerCamera(ICvUnit1*) = 0;
	/// pure
	virtual void releaseLockedCamera() = 0;
	/// pure
	virtual bool isFocusedWidget() = 0;
	/// pure
	virtual bool isFocused() = 0;
	/// pure
	virtual void refreshYieldVisibleMode() = 0;
	/// pure
	virtual void toggleYieldVisibleMode() = 0;
	/// pure
	virtual void toggleResourceVisibleMode() = 0;
	/// pure
	virtual void toggleGridVisibleMode() = 0;

	/// pure
	virtual int getOriginalPlotCount() = 0;
	/// pure
	virtual bool isCityScreenUp() = 0;
	/// pure
	virtual void setCityScreenUp(bool bUp) = 0;

	/// pure
	virtual void setNoSelectionListCycle(bool bValue) = 0;

	/// pure
	virtual bool canEndTurn() const = 0;
	/// pure
	virtual void UpdateEndTurn() = 0;
	/// pure
	virtual void SetEndTurnBlockingChanged(EndTurnBlockingTypes ePrevBlockingType, EndTurnBlockingTypes eNewBlockingType) = 0;
	/// pure
	virtual void setInterfaceMode(InterfaceModeTypes eNewValue) = 0;
	/// pure
	virtual InterfaceModeTypes getInterfaceMode() = 0;
	/// pure
	virtual int GetInterfaceModeValue() = 0;
	/// pure
	virtual void setFlashing(PlayerTypes eWho, bool bFlashing = true) = 0;
	/// pure
	virtual bool isFlashing(PlayerTypes eWho) = 0;
	/// pure
	virtual void setDiplomacyLocked(bool bLocked) = 0;
	/// pure
	virtual bool isDiplomacyLocked() = 0;

	/// pure
	virtual void setCanEndTurn(bool bNewValue) = 0;
	/// pure
	virtual void updateEndTurnTimer(float percentComplete) const = 0;
	/// pure
	virtual bool waitingForRemotePlayers() const = 0;

	/// pure
	virtual bool isHasMovedUnit() = 0;
	/// pure
	virtual void setHasMovedUnit(bool bNewValue) = 0;

	/// pure
	virtual bool isForcePopup() = 0;
	/// pure
	virtual void setForcePopup(bool bNewValue) = 0;

	/// pure
	virtual void lookAtCityOffset(int iCity) = 0;

	/// pure
	virtual int getPlotListColumn() = 0;
	/// pure
	virtual void verifyPlotListColumn() = 0;
	/// pure
	virtual int getPlotListOffset() = 0;

	/// pure
	virtual void exitingToMainMenu(const char* szLoadFile=NULL) = 0;

	/// pure
	virtual bool isInAdvancedStart() const = 0;
	/// pure
	virtual void setInAdvancedStart(bool bAdvancedStart) = 0;

	/// pure
	virtual void setBusy(bool bBusy) = 0;

	/// pure
	virtual void UpdateCountryBorder(ICvPlot1* pThisPlot) = 0;
	/// pure
	virtual void UpdateAllCountryBorders() = 0;

	/// pure
	virtual void DebugCityRoutePlots (ICvPlot1* pPlot, bool bOn) = 0;

	/// pure
	virtual void DirectionAction(DirectionTypes eDirection) = 0;

	/// pure
	virtual void SetSpecificCityInfoDirty(ICvCity1* pCity, CityUpdateTypes eUpdateType) = 0;

	/// pure
	virtual void AddHexToUIRange(ICvPlot1* pThisPlot) = 0;

	/// pure
	virtual void OpenEndGameMenu() = 0;

	/// pure
	virtual void doTurn() = 0;

	/// pure
	virtual bool IsOptionNoRewardPopups() = 0;

	/// pure
	virtual bool IsAIRequestingConcessions() const = 0;
	/// pure
	virtual void SetAIRequestingConcessions(bool bValue) = 0;
	/// pure
	virtual bool IsHumanMakingDemand() const = 0;
	/// pure
	virtual void SetHumanMakingDemand(bool bValue) = 0;

	/// pure
	virtual int GetStartDiploRepeatCount() const = 0;
	/// pure
	virtual void SetStartDiploRepeatCount(int iValue) = 0;
	/// pure
	virtual void ChangeStartDiploRepeatCount(int iChange) = 0;
	/// pure
	virtual int GetOfferTradeRepeatCount() const = 0;
	/// pure
	virtual void SetOfferTradeRepeatCount(int iValue) = 0;
	/// pure
	virtual void ChangeOfferTradeRepeatCount(int iChange) = 0;

	/// pure
	virtual void OpenTechTree() = 0;
	/// pure
	virtual void OpenPlayerDealScreen( PlayerTypes ePlayer ) = 0;

	/// pure
	virtual bool IsPolicyNotificationSeen() const = 0;
	/// pure
	virtual void SetPolicyNotificationSeen(bool bValue) = 0;

	/// pure
	virtual bool IsForceDiscussionModeQuitOnBack() const = 0;
	/// pure
	virtual void SetForceDiscussionModeQuitOnBack(bool bValue) = 0;

	/// pure
	virtual void SetTempString(CvString strString) = 0;
	/// pure
	virtual void PublishEndTurnDirty() const = 0;

	/// pure
	virtual void AddDeferredWonderCommand(WonderCommandType eCommandType, ICvCity1* pThisCity, BuildingTypes eBuildingIndex, int iState) = 0;

	/// pure
	virtual bool IsLoadedGame() const = 0;

	/// pure
	virtual void ToggleStrategicView() = 0;
	/// pure
	virtual bool IsPopupQueueEmpty() = 0;
	/// pure
	virtual bool IsModalStackEmpty() = 0;

	/// pure
	virtual bool IsDontShowPopups() const = 0;
	/// pure
	virtual void SetDontShowPopups(bool bValue) = 0;

	/// pure
	virtual bool IsDealInTransit() = 0;
	/// pure
	virtual void SetDealInTransit(bool bValue) = 0;

	/// pure
	virtual bool IsSPAutoEndTurnEnabled() = 0;
	/// pure
	virtual void SetSPAutoEndTurnEnabled(bool bEnable) = 0;

	/// pure
	virtual bool IsMPAutoEndTurnEnabled() = 0;
	/// pure
	virtual void SetMPAutoEndTurnEnabled(bool bEnable) = 0;

	/// pure
	virtual bool IsSPQuickCombatEnabled() = 0;
	/// pure
	virtual void SetSPQuickCombatEnabled(bool bEnable) = 0;

	/// pure
	virtual bool IsMPQuickCombatEnabled() = 0;
	/// pure
	virtual void SetMPQuickCombatEnabled(bool bEnable) = 0;
};

//This interface contains methods necessary to support the second expansion and updates to the XP1 and base DLL
/// size 0x4
/// vtable ?
class ICvUserInterface2 : public ICvUserInterface1
{
public:
	/// inline
	static GUID DLLCALL GetInterfaceId() { return guidICvUserInterface2; }

	using ICvUserInterface1::AddPopup;
	/// pure
	virtual void PublishRemotePlayerTurnStart() = 0;	// Only remote human players
	/// pure
	virtual void AddPopup(int iPopupType, int iIntArraySize, const int *pIntArray, int iBoolArraySize, const bool *pBoolArray) = 0;
};
