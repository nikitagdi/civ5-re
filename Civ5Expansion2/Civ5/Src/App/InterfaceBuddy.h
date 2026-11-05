/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvStructs.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/ICvDLLUserInterface.h"
#include "Civ5/Src/App/cvEventSubscriber.h"
#include <Windows.h>

class ChangeGameStateData;
class CvInterfacePopupInfo;
class GameMessageDawnOfManData;
class GameMessagePopupProcessedData;
class HexID;
class LinearEventDispatcher;
class LoadGameData;
class LoadMapData;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class ShowPlayerChangeUIData;
class UnitBooleanData;
class UnitHandleData;
class UnitPositionChangedData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2f9340
class InterfaceBuddy : public cvEventSubscriber, public CvDLLInterfaceIFaceBase, public ICvUserInterface2 {
public:
    enum MultiplayerLobbyMode : int;
    char unk_c[0x19e];    ///< +0xc
    bool m_bCanEndTurn;    ///< +0x1aa
    bool m_bHasMovedUnit;    ///< +0x1ab
    /// 0x086a3262
    InterfaceBuddy();
    /// 0x086b0192
    /// thunk 0x086b02a4
    virtual void ActivateNotification(int a1, NotificationTypes a2, char const* a3, int a4, int a5, int a6, int a7, PlayerTypes a8);
    /// 0x086ab360
    /// thunk 0x086ab50e
    virtual uint AddCityMessage(unsigned int a1, IDInfo const& a2, PlayerTypes a3, bool a4, int a5, char const* a6, char const* a7, InterfaceMessageTypes a8, char const* a9, ColorTypes a10, int a11, int a12, bool a13, bool a14);
    /// 0x086b2ce8
    /// thunk 0x086b2df0
    virtual void AddDeferredWonderCommand(WonderCommandType a1, ICvCity1* a2, BuildingTypes a3, int a4);
    /// 0x086b07ba
    /// thunk 0x086b0da0
    virtual void AddHexToUIRange(ICvPlot1* a1);
    /// 0x086aaf6a
    /// thunk 0x086ab11c
    virtual uint AddMessage(unsigned int a1, PlayerTypes a2, bool a3, int a4, char const* a5, char const* a6, InterfaceMessageTypes a7, char const* a8, ColorTypes a9, int a10, int a11, bool a12, bool a13);
    /// 0x086afaf2
    /// thunk 0x086afe3c
    virtual void AddNotification(int a1, NotificationTypes a2, char const* a3, char const* a4, int a5, int a6, PlayerTypes a7, int a8, int a9);
    /// 0x086ab562
    /// thunk 0x086ab708
    virtual uint AddPlotMessage(unsigned int a1, int a2, PlayerTypes a3, bool a4, int a5, char const* a6, char const* a7, InterfaceMessageTypes a8, char const* a9, ColorTypes a10, int a11, int a12, bool a13, bool a14);
    /// 0x086ab8c8
    /// thunk 0x086ab970
    virtual void AddPopup(CvPopupInfo const& a1);
    /// 0x086b04f6
    /// thunk 0x086b077c
    virtual void AddPopupText(int a1, int a2, char const* a3, float a4);
    /// 0x086ab166
    /// thunk 0x086ab30c
    virtual uint AddUnitMessage(unsigned int a1, IDInfo const& a2, PlayerTypes a3, bool a4, int a5, char const* a6, char const* a7, InterfaceMessageTypes a8, char const* a9, ColorTypes a10, int a11, int a12, bool a13, bool a14);
    /// 0x086a866e
    /// thunk 0x086a87b8
    virtual bool CanDoInterfaceMode(InterfaceModeTypes a1, bool a2);
    /// 0x086a9258
    /// thunk 0x086a9308
    virtual bool CanSelectionListFound();
    /// 0x086b1da4
    /// thunk 0x086b1db4
    virtual void ChangeOfferTradeRepeatCount(int a1);
    /// 0x086b1d4c
    /// thunk 0x086b1d5c
    virtual void ChangeStartDiploRepeatCount(int a1);
    /// 0x086a983a
    /// thunk 0x086a9902
    virtual void ClearSelectionList();
    /// 0x086aae72
    /// thunk 0x086aaf3e
    virtual void DebugCityRoutePlots(ICvPlot1* a1, bool a2);
    /// 0x086aebc2
    /// thunk 0x086aede4
    virtual void DirectionAction(DirectionTypes a1);
    /// 0x086a88d2
    /// thunk 0x086a88ea
    virtual bool DoAutoUnitCycle();
    /// 0x086a8902
    /// thunk 0x086a8962
    virtual ICvUnit1 * GetHeadSelectedUnit();
    /// 0x086adbf8
    /// thunk 0x086adc04
    virtual int GetInterfaceModeValue();
    /// 0x086a9de2
    /// thunk 0x086a9df6
    virtual int GetLengthSelectionList();
    /// 0x086b1d6c
    /// thunk 0x086b1d78
    virtual int GetOfferTradeRepeatCount() const;
    /// 0x086b0dcc
    /// thunk 0x086b0e8e
    virtual ICvDeal1 * GetScratchDeal();
    /// 0x086a89fa
    /// thunk 0x086a8a4a
    virtual ICvEnumerator * GetSelectedUnits();
    /// 0x086b1d14
    /// thunk 0x086b1d20
    virtual int GetStartDiploRepeatCount() const;
    /// 0x086a9926
    /// thunk 0x086a9b7e
    virtual void InsertIntoSelectionList(ICvUnit1* a1, bool a2, bool a3, bool a4, bool a5, bool a6);
    /// 0x086b1c0a
    /// thunk 0x086b1c16
    virtual bool IsAIRequestingConcessions() const;
    /// 0x086b1c7a
    /// thunk 0x086b1c86
    virtual bool IsDealInTransit();
    /// 0x086af71c
    /// thunk 0x086af7ae
    virtual bool IsDontShowPopups() const;
    /// 0x086af6e4
    /// thunk 0x086af6f0
    virtual bool IsForceDiscussionModeQuitOnBack() const;
    /// 0x086b1c42
    /// thunk 0x086b1c4e
    virtual bool IsHumanMakingDemand() const;
    /// 0x086b2e38
    /// thunk 0x086b2e44
    virtual bool IsLoadedGame() const;
    /// 0x086b2fe0
    /// thunk 0x086b2ff8
    virtual bool IsMPAutoEndTurnEnabled();
    /// 0x086b30a8
    /// thunk 0x086b30c0
    virtual bool IsMPQuickCombatEnabled();
    /// 0x086b31ca
    /// thunk 0x086b31ec
    virtual bool IsModalStackEmpty();
    /// 0x086a853c
    /// thunk 0x086a8554
    virtual bool IsOptionNoRewardPopups();
    /// 0x086af6ac
    /// thunk 0x086af6b8
    virtual bool IsPolicyNotificationSeen() const;
    /// 0x086b3186
    /// thunk 0x086b31a8
    virtual bool IsPopupQueueEmpty();
    /// 0x086b2f7c
    /// thunk 0x086b2f94
    virtual bool IsSPAutoEndTurnEnabled();
    /// 0x086b3044
    /// thunk 0x086b305c
    virtual bool IsSPQuickCombatEnabled();
    /// 0x086ac82e
    /// thunk 0x086ac83a
    virtual bool IsSelectedUnitRevealingNewPlots() const;
    /// 0x086a8986
    /// thunk 0x086a89c0
    virtual bool IsUnitSelected(PlayerTypes a1, int a2);
    /// 0x086b25de
    /// thunk 0x086b2850
    virtual void OpenEndGameMenu();
    /// 0x086b1e7a
    /// thunk 0x086b1ec4
    virtual void OpenPlayerDealScreen(PlayerTypes a1);
    /// 0x086b24d0
    /// thunk 0x086b253c
    virtual void OpenTechTree();
    /// 0x086af91e
    /// thunk 0x086af95e
    virtual void PublishActivePlayerTurnEnd();
    /// 0x086af81e
    /// thunk 0x086af85e
    virtual void PublishActivePlayerTurnStart();
    /// 0x086b2e50
    /// thunk 0x086b2e90
    virtual void PublishEndTurnDirty() const;
    /// 0x086afa1e
    /// thunk 0x086afac0
    virtual void PublishPlayerTurnStatus(CvDLLInterfaceIFaceBase::TURN_STATUS_TYPE a1, PlayerTypes a2, char const* a3);
    /// 0x086af99e
    /// thunk 0x086af9de
    virtual void PublishRemotePlayerTurnEnd();
    /// 0x086a8de0
    /// thunk 0x086a8ea6
    virtual bool RemoveFromSelectionList(ICvUnit1* a1);
    /// 0x086afeac
    /// thunk 0x086aff58
    virtual void RemoveNotification(int a1, PlayerTypes a2);
    /// 0x086b1c22
    /// thunk 0x086b1c32
    virtual void SetAIRequestingConcessions(bool a1);
    /// 0x086b1c92
    /// thunk 0x086b1ca2
    virtual void SetDealInTransit(bool a1);
    /// 0x086af7d6
    /// thunk 0x086af7e6
    virtual void SetDontShowPopups(bool a1);
    /// 0x086add7c
    /// thunk 0x086addd2
    virtual void SetEndTurnBlockingChanged(EndTurnBlockingTypes a1, EndTurnBlockingTypes a2);
    /// 0x086af6fc
    /// thunk 0x086af70c
    virtual void SetForceDiscussionModeQuitOnBack(bool a1);
    /// 0x086b1c5a
    /// thunk 0x086b1c6a
    virtual void SetHumanMakingDemand(bool a1);
    /// 0x086b3010
    /// thunk 0x086b302a
    virtual void SetMPAutoEndTurnEnabled(bool a1);
    /// 0x086b30d8
    /// thunk 0x086b30f2
    virtual void SetMPQuickCombatEnabled(bool a1);
    /// 0x086b1d84
    /// thunk 0x086b1d94
    virtual void SetOfferTradeRepeatCount(int a1);
    /// 0x086af6c4
    /// thunk 0x086af6d4
    virtual void SetPolicyNotificationSeen(bool a1);
    /// 0x086b2fac
    /// thunk 0x086b2fc6
    virtual void SetSPAutoEndTurnEnabled(bool a1);
    /// 0x086b3074
    /// thunk 0x086b308e
    virtual void SetSPQuickCombatEnabled(bool a1);
    /// 0x086b0eb2
    /// thunk 0x086b0fb6
    virtual void SetScratchDeal(ICvDeal1* a1);
    /// 0x086ac846
    /// thunk 0x086ac866
    virtual void SetSelectedUnitRevealingNewPlots(bool a1);
    /// 0x086aee10
    /// thunk 0x086aef04
    virtual void SetSpecificCityInfoDirty(ICvCity1* a1, CityUpdateTypes a2);
    /// 0x086b1d2c
    /// thunk 0x086b1d3c
    virtual void SetStartDiploRepeatCount(int a1);
    /// 0x086b2ba6
    /// thunk 0x086b2bd4
    virtual void SetTempString(CvString a1);
    /// 0x086b2ed0
    /// thunk 0x086b2f3a
    virtual void ToggleStrategicView();
    /// 0x086aeafa
    /// thunk 0x086aeba8
    virtual void UpdateAllCountryBorders();
    /// 0x086ae988
    /// thunk 0x086aead8
    virtual void UpdateCountryBorder(ICvPlot1* a1);
    /// 0x086adcfc
    /// thunk 0x086add3c
    virtual void UpdateEndTurn();
    /// 0x086a9be0
    /// thunk 0x086a9d8c
    virtual void VisuallyDeselectUnit(ICvUnit1* a1);
    /// 0x086aa1c6
    /// thunk 0x086aa314
    virtual void addSelectedCity(ICvCity1* a1, bool a2);
    /// 0x086ada06
    /// thunk 0x086ada12
    virtual bool canEndTurn() const;
    /// 0x086ad474
    /// thunk 0x086ad4f2
    virtual void centerCamera(ICvUnit1* a1);
    /// 0x086ac7b2
    /// thunk 0x086ac7ee
    virtual void changeCycleSelectionCounter(int a1);
    /// 0x086ac8be
    /// thunk 0x086ac8fa
    virtual void changeEndTurnCounter(int a1);
    /// 0x086a9836
    /// thunk 0x086a9838
    virtual void changePlotListColumn(int a1);
    /// 0x086ab75c
    /// thunk 0x086ab75e
    virtual void clearEventMessages();
    /// 0x086aa34c
    /// thunk 0x086aa3d0
    virtual void clearSelectedCities();
    /// 0x086a8238
    /// thunk 0x086a8518
    virtual void doTurn();
    /// 0x086adeee
    /// thunk 0x086adef0
    virtual void exitingToMainMenu(char const* a1);
    /// 0x086ac62a
    /// thunk 0x086ac636
    virtual int getCycleSelectionCounter();
    /// 0x086ac886
    /// thunk 0x086ac892
    virtual int getEndTurnCounter();
    /// 0x086aa494
    /// thunk 0x086aa53e
    virtual ICvCity1 * getHeadSelectedCity();
    /// 0x086adbe0
    /// thunk 0x086adbec
    virtual InterfaceModeTypes getInterfaceMode();
    /// 0x086a972e
    /// thunk 0x086a9754
    virtual ICvUnit1 * getLastSelectedUnit();
    /// 0x086b4942
    virtual int getOriginalPlotCount();
    /// 0x086b4936
    virtual int getOriginalPlotIndex();
    /// 0x086adeda
    /// thunk 0x086adede
    virtual int getPlotListColumn();
    /// 0x086adee6
    /// thunk 0x086adeea
    virtual int getPlotListOffset();
    /// 0x086a87f2
    /// thunk 0x086a88ae
    virtual ICvPlot1 * getSelectionPlot();
    /// 0x086aa656
    /// thunk 0x086aa66c
    virtual IDInfo * headSelectedCitiesNode();
    /// 0x086b494e
    virtual bool isCityScreenUp();
    /// 0x086aa3f4
    /// thunk 0x086aa464
    virtual bool isCitySelected(ICvCity1* a1);
    /// 0x086aa562
    /// thunk 0x086aa58e
    virtual bool isCitySelection();
    /// 0x086ac93a
    /// thunk 0x086ac946
    virtual bool isCombatFocus();
    /// 0x086a965a
    /// thunk 0x086a9706
    virtual bool isDiploActive();
    /// 0x086a9576
    /// thunk 0x086a9632
    virtual bool isDiploOrPopupWaiting();
    /// 0x086adc68
    /// thunk 0x086adc6c
    virtual bool isDiplomacyLocked();
    /// 0x086ac9ee
    /// thunk 0x086aca0e
    virtual bool isDirty(InterfaceDirtyBits a1);
    /// 0x086adc5c
    /// thunk 0x086adc60
    virtual bool isFlashing(PlayerTypes a1);
    /// 0x086ad5bc
    /// thunk 0x086ad68e
    virtual bool isFocused();
    /// 0x086ad5a4
    /// thunk 0x086ad5b0
    virtual bool isFocusedWidget();
    /// 0x086adeca
    /// thunk 0x086adece
    virtual bool isForcePopup();
    /// 0x086ade92
    /// thunk 0x086ade9e
    virtual bool isHasMovedUnit();
    /// 0x086adf02
    /// thunk 0x086adf06
    virtual bool isInAdvancedStart() const;
    /// 0x086a9404
    /// thunk 0x086a9408
    virtual bool isPopupQueued();
    /// 0x086a93e0
    /// thunk 0x086a93f2
    virtual bool isPopupUp();
    /// 0x086accfc
    /// thunk 0x086ad440
    virtual void lookAt(ICvPlot1 const* a1, CameraLookAtTypes a2);
    /// 0x086aded6
    /// thunk 0x086aded8
    virtual void lookAtCityOffset(int a1);
    /// 0x086a856c
    /// thunk 0x086a863e
    virtual void lookAtSelectionPlot(bool a1);
    /// 0x086acb00
    /// thunk 0x086acccc
    virtual void makeInterfaceDirty();
    /// 0x086a8ed6
    /// thunk 0x086a9234
    virtual void makeSelectionListDirty();
    /// 0x086aa5be
    /// thunk 0x086aa5e4
    virtual const IDInfo * nextSelectedCitiesNode(IDInfo const* a1) const;
    /// 0x086aa60a
    /// thunk 0x086aa630
    virtual IDInfo * nextSelectedCitiesNode(IDInfo* a1);
    /// 0x086ad6b6
    /// thunk 0x086ad6f6
    virtual void refreshYieldVisibleMode();
    /// 0x086ad51e
    /// thunk 0x086ad520
    virtual void releaseLockedCamera();
    /// 0x086a8c76
    /// thunk 0x086a8d0c
    virtual void selectAll(ICvPlot1* a1);
    /// 0x086a9e0a
    /// thunk 0x086a9fe8
    virtual void selectCity(ICvCity1* a1, bool a2);
    /// 0x086a8b72
    /// thunk 0x086a8c24
    virtual void selectGroup(ICvUnit1* a1, bool a2, bool a3, bool a4);
    /// 0x086aa020
    /// thunk 0x086aa196
    virtual void selectLookAtCity(bool a1);
    /// 0x086a8a6e
    /// thunk 0x086a8b20
    virtual void selectUnit(ICvUnit1* a1, bool a2, bool a3, bool a4);
    /// 0x086adf0e
    /// thunk 0x086adf10
    virtual void setBusy(bool a1);
    /// 0x086adc70
    /// thunk 0x086adccc
    virtual void setCanEndTurn(bool a1);
    /// 0x086af03a
    /// thunk 0x086af12c
    virtual void setCityScreenUp(bool a1);
    /// 0x086ac952
    /// thunk 0x086ac9be
    virtual void setCombatFocus(bool a1);
    /// 0x086ac642
    /// thunk 0x086ac786
    virtual void setCycleSelectionCounter(int a1);
    /// 0x086adc64
    /// thunk 0x086adc66
    virtual void setDiplomacyLocked(bool a1);
    /// 0x086aca2e
    /// thunk 0x086acac8
    virtual void setDirty(InterfaceDirtyBits a1, bool a2);
    /// 0x086ac89e
    /// thunk 0x086ac8ae
    virtual void setEndTurnCounter(int a1);
    /// 0x086adc58
    /// thunk 0x086adc5a
    virtual void setFlashing(PlayerTypes a1, bool a2);
    /// 0x086aded2
    /// thunk 0x086aded4
    virtual void setForcePopup(bool a1);
    /// 0x086adeaa
    /// thunk 0x086adeba
    virtual void setHasMovedUnit(bool a1);
    /// 0x086adf0a
    /// thunk 0x086adf0c
    virtual void setInAdvancedStart(bool a1);
    /// 0x086ada6e
    /// thunk 0x086adbb4
    virtual void setInterfaceMode(InterfaceModeTypes a1);
    /// 0x086a977a
    /// thunk 0x086a97d8
    virtual void setLastSelectedUnit(ICvUnit1* a1);
    /// 0x086b495a
    virtual void setNoSelectionListCycle(bool a1);
    /// 0x086ad930
    /// thunk 0x086ad9bc
    virtual void toggleGridVisibleMode();
    /// 0x086ad822
    /// thunk 0x086ad8ca
    virtual void toggleResourceVisibleMode();
    /// 0x086ad736
    /// thunk 0x086ad7ca
    virtual void toggleYieldVisibleMode();
    /// 0x086aa682
    /// thunk 0x086aaa78
    virtual void updateCityScreen(ICvCity1* a1);
    /// 0x086accf0
    /// thunk 0x086accf6
    virtual bool updateCursorType();
    /// 0x086ade28
    /// thunk 0x086ade6c
    virtual void updateEndTurnTimer(float a1) const;
    /// 0x086adee2
    /// thunk 0x086adee4
    virtual void verifyPlotListColumn();
    /// 0x086ada1e
    /// thunk 0x086ada46
    virtual bool waitingForRemotePlayers() const;
    /// 0x086a518c
    /// deleting 0x086a5150
    virtual ~InterfaceBuddy();
    /// 0x086a58b8
    /// thunk 0x086a5944
    virtual void * QueryInterface(_GUID a1);
    /// 0x086ab99c
    /// thunk 0x086aba72
    virtual void AddPopup(int a1, int a2, int const* a3, int a4, bool const* a5);
    /// 0x086af89e
    /// thunk 0x086af8de
    virtual void PublishRemotePlayerTurnStart();
    /// 0x086a599e
    /// thunk 0x086a59b0
    virtual void Destroy();
    /// 0x086a3b50
    UNK_RET OnPlotSelected(EventTemplate<Event_Int2Type<110>, HexID, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a3cb6
    UNK_RET OnUnitFlagSelected(EventTemplate<Event_Int2Type<158>, UnitHandleData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a3e6c
    UNK_RET OnExitToMainMenu(EventTemplate<Event_Int2Type<161>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a3fdc
    UNK_RET OnPlayerChoseToLoadGame(EventTemplate<Event_Int2Type<162>, LoadGameData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a40e8
    UNK_RET OnPlayerChoseToLoadMap(EventTemplate<Event_Int2Type<163>, LoadMapData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a43e4
    UNK_RET OnCameraStartMove(EventTemplate<Event_Int2Type<57>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a43f0
    UNK_RET OnCameraStopMove(EventTemplate<Event_Int2Type<58>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a4428
    UNK_RET OnUnitHexChanged(GameCoreEventTemplate<Event_Int2Type<132>, UnitHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a45ac
    UNK_RET OnUnitHexChangedAttack(GameCoreEventTemplate<Event_Int2Type<133>, UnitHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a470c
    UNK_RET OnUnitPositionChanged(EventTemplate<Event_Int2Type<1>, UnitPositionChangedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a4784
    UNK_RET OnUnitMoveQueueChanged(GameCoreEventTemplate<Event_Int2Type<148>, UnitBooleanData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a47ba
    UNK_RET OnProcessedPopup(GameCoreEventTemplate<Event_Int2Type<154>, GameMessagePopupProcessedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a47e6
    UNK_RET OnDawnOfManShow(EventTemplate<Event_Int2Type<156>, GameMessageDawnOfManData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a4b2e
    UNK_RET OnDawnOfManHide(EventTemplate<Event_Int2Type<157>, GameMessageDawnOfManData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a4b42
    UNK_RET OnChangeGameState(GameCoreEventTemplate<Event_Int2Type<151>, ChangeGameStateData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a4b76
    UNK_RET OnShowPlayerChangeUI(GameCoreEventTemplate<Event_Int2Type<156>, ShowPlayerChangeUIData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a4b90
    UNK_RET OnRestartGame(GameCoreEventTemplate<Event_Int2Type<157>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086a5120
    UNK_RET init();
    /// 0x086a5178
    void operator delete(void* a1);
    /// 0x086a5782
    UNK_RET uninit();
    /// 0x086a5938
    UNK_RET IncrementReference();
    /// 0x086a5980
    UNK_RET DecrementReference();
    /// 0x086a5996
    UNK_RET GetReferenceCount();
    /// 0x086a59c2
    void* operator new(unsigned int a1);
    /// 0x086a59d6
    UNK_RET reset(bool a1);
    /// 0x086a59fe
    UNK_RET resetState();
    /// 0x086a5bec
    UNK_RET RegisterDefaultPopupTypes();
    /// 0x086a628e
    UNK_RET setup();
    /// 0x086a6612
    UNK_RET update();
    /// 0x086a6644
    UNK_RET updateIfNotPaused();
    /// 0x086a7082
    UNK_RET updateCommon();
    /// 0x086a70e0
    UNK_RET GetDawnOfManSound();
    /// 0x086a7126
    UNK_RET UnDuckMainMenuMusic();
    /// 0x086a719e
    UNK_RET ProcessDelayedPopups();
    /// 0x086a7246
    UNK_RET updateLogitechLCD(ICvPlayer1* a1);
    /// 0x086a800c
    UNK_RET processDeferredWonderCommands();
    /// 0x086a8d38
    UNK_RET AddToSelectionList(ICvUnit1* a1);
    /// 0x086a9330
    UNK_RET CanSelectionListWork();
    /// 0x086a940c
    UNK_RET IsPopupTypeOpen(unsigned int a1);
    /// 0x086a9448
    UNK_RET GetPopupTypeName(unsigned int a1);
    /// 0x086a94a0
    UNK_RET GetPopupType(char const* a1);
    /// 0x086a951e
    UNK_RET GetPopupTypeByIndex(int a1);
    /// 0x086a956a
    UNK_RET GetPopupTypeCount();
    /// 0x086a9dae
    UNK_RET makeSelectedCitiesDirty();
    /// 0x086aa872
    UNK_RET updateCityFog();
    /// 0x086aaa9c
    UNK_RET SetPlaceUnit(ICvUnit1* a1);
    /// 0x086aaae0
    UNK_RET GetPlaceUnit();
    /// 0x086aab06
    UNK_RET ClearPlaceUnit();
    /// 0x086aab20
    UNK_RET CanPlaceUnitAt(ICvUnit1* a1, ICvPlot1* a2);
    /// 0x086aabb4
    UNK_RET HighlightCanPlacePlots(ICvUnit1* a1, ICvPlot1* a2);
    /// 0x086ab760
    UNK_RET ActuallyAddPopup(CvInterfacePopupInfo const& a1);
    /// 0x086abac4
    UNK_RET HasPendingAIMessage();
    /// 0x086abad6
    UNK_RET RequestLeaveLeader();
    /// 0x086abd2c
    UNK_RET SetNextGameState(int a1, PlayerTypes a2);
    /// 0x086ac208
    UNK_RET GameplayDiplomacyAILeaderMessage(PlayerTypes a1, DiploUIStateTypes a2, char const* a3, LeaderheadAnimationTypes a4, int a5);
    /// 0x086ac4f4
    UNK_RET ProcessedPopup(unsigned int a1);
    /// 0x086ad522
    UNK_RET IsCameraMoving();
    /// 0x086ad52e
    UNK_RET isUnitFocus();
    /// 0x086ad78e
    UNK_RET SetYieldVisibleMode(bool a1);
    /// 0x086ad888
    UNK_RET SetResourceVisibleMode(bool a1);
    /// 0x086ad97a
    UNK_RET SetGridVisibleMode(bool a1);
    /// 0x086adc10
    UNK_RET SetInterfaceModeValue(int a1);
    /// 0x086adc20
    UNK_RET GetInterfaceModeDebugItemID1();
    /// 0x086adc2c
    UNK_RET SetInterfaceModeDebugItemID1(int a1);
    /// 0x086adc3c
    UNK_RET GetInterfaceModeDebugItemID2();
    /// 0x086adc48
    UNK_RET SetInterfaceModeDebugItemID2(int a1);
    /// 0x086adef2
    UNK_RET SetIsLoadedGame(bool a1);
    /// 0x086adf12
    UNK_RET locationSelect(ICvPlot1* a1, bool a2, bool a3, bool a4);
    /// 0x086ae588
    UNK_RET DoSelectCityAtPlot(ICvPlot1* a1);
    /// 0x086ae978
    UNK_RET SetCityScreenViewingMode(bool a1);
    /// 0x086aef2e
    UNK_RET incTurnTimerSemaphore();
    /// 0x086aefac
    UNK_RET decTurnTimerSemaphore();
    /// 0x086af15c
    UNK_RET interruptTurnTimer() const;
    /// 0x086af1d0
    UNK_RET setMouseOverHex(HexID const& a1);
    /// 0x086af1e2
    UNK_RET SendPathfinderUpdate();
    /// 0x086af7f6
    UNK_RET IsCityScreenViewingMode() const;
    /// 0x086af802
    UNK_RET GetMultiplayerLobbyMode() const;
    /// 0x086af80e
    UNK_RET SetMultiplayerLobbyMode(InterfaceBuddy::MultiplayerLobbyMode a1);
    /// 0x086afb62
    UNK_RET AddVisualNotification(int a1, NotificationTypes a2, char const* a3, char const* a4, int a5, int a6, PlayerTypes a7, int a8, int a9);
    /// 0x086aff02
    UNK_RET RemoveVisualNotification(int a1, PlayerTypes a2);
    /// 0x086affae
    UNK_RET RemoveGameplayNotification(int a1);
    /// 0x086b01f2
    UNK_RET ActivateVisualNotification(int a1, NotificationTypes a2, char const* a3, int a4, int a5, int a6, int a7, PlayerTypes a8);
    /// 0x086b0304
    UNK_RET ActivateGameplayNotification(int a1);
    /// 0x086b0424
    UNK_RET RebroadcastNotifications();
    /// 0x086b09d2
    UNK_RET MoveWillResultInAttack(ICvPlot1* a1);
    /// 0x086b0fe2
    UNK_RET OnHumanOpenedTradeScreen(PlayerTypes a1);
    /// 0x086b1124
    UNK_RET OnHumanDemand(PlayerTypes a1);
    /// 0x086b1266
    UNK_RET DoProposeDeal();
    /// 0x086b1522
    UNK_RET DoDemand();
    /// 0x086b16d2
    UNK_RET DoAttemptDealWithHuman(bool a1, bool a2);
    /// 0x086b1b74
    UNK_RET DoEqualizeDealWithHuman();
    /// 0x086b1ba6
    UNK_RET DoWhatWillAIGive();
    /// 0x086b1bd8
    UNK_RET DoWhatDoesAIWant();
    /// 0x086b1cb2
    UNK_RET GetRepeatActionPlayer() const;
    /// 0x086b1cbe
    UNK_RET SetRepeatActionPlayer(PlayerTypes a1);
    /// 0x086b1dc4
    UNK_RET DoFinalizePlayerDeal(PlayerTypes a1, PlayerTypes a2, bool a3);
    /// 0x086b1f0e
    UNK_RET LoadProposedDeal(PlayerTypes a1, PlayerTypes a2);
    /// 0x086b2078
    UNK_RET GetNumCurrentDeals(PlayerTypes a1);
    /// 0x086b213a
    UNK_RET GetNumHistoricDeals(PlayerTypes a1);
    /// 0x086b21fc
    UNK_RET LoadCurrentDeal(PlayerTypes a1, unsigned int a2);
    /// 0x086b2366
    UNK_RET LoadHistoricDeal(PlayerTypes a1, unsigned int a2);
    /// 0x086b25ac
    UNK_RET BindMouse(bool a1);
    /// 0x086b25dc
    UNK_RET IsBindInWindowed();
    /// 0x086b286a
    UNK_RET IsMainMenuMusicCurrentlyPlaying();
    /// 0x086b28b8
    UNK_RET ShouldDawnOfManSpeechPlay();
    /// 0x086b296c
    UNK_RET StopDawnOfManSound();
    /// 0x086b29f2
    UNK_RET PlayDawnOfManSound(char const* a1);
    /// 0x086b2ada
    UNK_RET DuckMainMenuMusic();
    /// 0x086b2b4c
    UNK_RET GetTempString() const;
    /// 0x086b2c02
    UNK_RET DebugKeyHandler(int a1, int a2, int a3);
    /// 0x086b2c56
    UNK_RET BulkHideUI(bool a1);
    /// 0x086b2cc4
    UNK_RET RequestMinimapBroadcast();
    /// 0x086b310c
    UNK_RET CheckForCommandLineInvitation();
    /// 0x086b320e
    UNK_RET GameCoreUpdateComplete(bool a1);
    /// 0x086b32a6
    UNK_RET GetPassword(PlayerTypes a1);
    /// 0x086b3308
    UNK_RET PostKeyMessage(unsigned int a1);
    /// 0x086b334c
    UNK_RET IsTouchScreenEnabled() const;
    /// 0x086b3350
    UNK_RET RegisterPopupType(char const* a1, bool a2);
};
