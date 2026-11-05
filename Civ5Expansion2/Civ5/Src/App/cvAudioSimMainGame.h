/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include <list>
#include <lua.h>
#include <memory>

class AILeaderMessageData;
class AdvisorDisplayShowData;
class CameraViewData;
class FGXVector3;
class FXmlDocument;
class FrameGlobalsData;
class GameMessagePopupData;
class GameMessagePopupProcessedData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NonSerialContainer;
class NotificationAddedData;
class NullEventClass;
class SerialContainer;
class UnitSelectionChangedData;
class cvAudioSystem_SpeakerVolumes;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <class T0> class FRect;
template <int T0> class Event_Int2Type;

/// size ?
class cvAudioSimMainGame {
public:
    class SoundscapeData;
    /// 0x089f6a38
    cvAudioSimMainGame();
    /// 0x089f6e0a
    UNK_RET Startup();
    /// 0x089f71ca
    UNK_RET BuildEventSoundTable();
    /// 0x089f747c
    UNK_RET BuildAdvisorSoundTable();
    /// 0x089f772e
    UNK_RET BuildNotificationSoundTable();
    /// 0x089f7b38
    UNK_RET BuildButtonPopupSoundTable();
    /// 0x089f8d52
    UNK_RET BuildMinorCivSoundTable(std::list<FXmlDocument, std::allocator<FXmlDocument> > const& a1);
    /// 0x089f9030
    UNK_RET SubscribeToEvents();
    /// 0x089f93c8
    UNK_RET Shutdown();
    /// 0x089f9516
    UNK_RET DestroyEventSoundTable();
    /// 0x089f9526
    UNK_RET DestroyAdvisorSoundTable();
    /// 0x089f9536
    UNK_RET DestroyNotificationSoundTable();
    /// 0x089f958c
    UNK_RET DestroyButtonPopupSoundTable();
    /// 0x089f95e2
    UNK_RET DestroyMinorCivSoundTable();
    /// 0x089f95f2
    UNK_RET UnSubscribeFromEvents();
    /// 0x089f9812
    UNK_RET StopAllSoundscapes();
    /// 0x089f98be
    UNK_RET StopAdvisorSounds();
    /// 0x089f98fc
    UNK_RET OnLoadingScreenClose(EventTemplate<Event_Int2Type<170>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f990c
    UNK_RET OnCameraViewChanged(EventTemplate<Event_Int2Type<4>, CameraViewData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f9992
    UNK_RET OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f99c6
    UNK_RET OnLeaderViewMessage(GameCoreEventTemplate<Event_Int2Type<118>, AILeaderMessageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f99f4
    UNK_RET OnLeaveLeaderView(EventTemplate<Event_Int2Type<39>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f9a54
    UNK_RET OnNotificationAdded(GameCoreEventTemplate<Event_Int2Type<121>, NotificationAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f9ac8
    UNK_RET OnAdvisorDisplayShow(GameCoreEventTemplate<Event_Int2Type<134>, AdvisorDisplayShowData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f9b78
    UNK_RET OnAdvisorDisplayHide(GameCoreEventTemplate<Event_Int2Type<135>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f9bb6
    UNK_RET OnUnitSelectionChanged(GameCoreEventTemplate<Event_Int2Type<92>, UnitSelectionChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f9e0e
    UNK_RET OnActivePlayerTurnStart(GameCoreEventTemplate<Event_Int2Type<124>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089f9e10
    UNK_RET OnButtonPopupDisplayShow(GameCoreEventTemplate<Event_Int2Type<153>, GameMessagePopupData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089fa478
    UNK_RET OnButtonPopupDisplayHide(GameCoreEventTemplate<Event_Int2Type<154>, GameMessagePopupProcessedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089fa48c
    UNK_RET OnEnterCityScreen(EventTemplate<Event_Int2Type<123>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089fa4b2
    UNK_RET OnExitCityScreen(EventTemplate<Event_Int2Type<124>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089fa506
    UNK_RET OnCityViewSelectionChanged(EventTemplate<Event_Int2Type<9>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089fa526
    UNK_RET StartCityScreenSound();
    /// 0x089fa8f2
    UNK_RET StopCityScreenSound();
    /// 0x089fa942
    UNK_RET OnLeaderScreenShow();
    /// 0x089fab7e
    UNK_RET PushNotificationSound(unsigned int a1);
    /// 0x089fac1e
    UNK_RET OnNotificationUpdate();
    /// 0x089fad4c
    UNK_RET TryPopNotificationSound(unsigned int& a1);
    /// 0x089fadac
    UNK_RET Play2DSound(unsigned int a1);
    /// 0x089fae3c
    UNK_RET Stop2DSound(long a1);
    /// 0x089fae9e
    UNK_RET Play2DSound(char const* a1);
    /// 0x089faf36
    UNK_RET StopAllPopupSounds();
    /// 0x089fafa8
    UNK_RET PlayMinorCivSound(PlayerTypes a1);
    /// 0x089fb188
    UNK_RET GetSoundScape(int a1, int* a2, cvAudioSystem_SpeakerVolumes* a3, bool* a4);
    /// 0x089fb20a
    UNK_RET GetSoundScapeQuadrant(int a1, int* a2, cvAudioSystem_SpeakerVolumes* a3, bool* a4);
    /// 0x089fc1ba
    UNK_RET UpdateListenerPosition();
    /// 0x089fc4d4
    UNK_RET CalculateGridRect();
    /// 0x089fc5b2
    UNK_RET UpdateSoundscape(cvAudioSimMainGame::SoundscapeData* a1, FGXVector3 const& a2);
    /// 0x089fc77c
    UNK_RET PlaySoundscapeAt(int a1, int a2, int a3, bool a4);
    /// 0x089fca6e
    UNK_RET UpdateSoundscapes();
    /// 0x089fcac0
    UNK_RET UpdateSoundscapesUniform();
    /// 0x089fcee2
    UNK_RET UpdateSoundscapesWeightedQuadrant();
    /// 0x089fd398
    UNK_RET GetSoundIDFromPlot(ICvPlot1* a1, unsigned int a2, TeamTypes a3, bool a4);
    /// 0x089fd6ec
    UNK_RET FindDominantSound(FRect<int> const& a1, unsigned int a2);
    /// 0x089fdb5a
    UNK_RET PlaySoundscapeAt(int a1, FRect<int> const& a2, bool* a3);
    /// 0x089fde2a
    UNK_RET UpdateCamera(bool a1);
    /// 0x089fde6c
    UNK_RET Update(float a1);
    /// 0x089fdfd0
    UNK_RET PlayEventSound(char const* a1);
    /// 0x089fe002
    UNK_RET HasNotificationSoundInQueue() const;
};

/// 0x089fabe5
UNK_RET OutputSoundIDInfo(long a1);
/// 0x089fb2a6
UNK_RET GetSoundScapeInfo(int a1);
/// 0x089fb4a8
/// lua ?
int LuaThunk_GetSoundScapeInfo(lua_State* a1);
/// 0x089fb54b
int LuaToString_GetSoundScapeInfo(lua_State* a1);
/// 0x089fb5d2
UNK_RET GetSoundScapeQuadrantInfo(int a1);
/// 0x089fb659
/// lua ?
int LuaThunk_GetSoundScapeQuadrantInfo(lua_State* a1);
/// 0x089fb686
int LuaToString_GetSoundScapeQuadrantInfo(lua_State* a1);
/// 0x089fb70d
UNK_RET GetSoundInstanceInfo(int a1, int a2);
/// 0x089fb8fa
/// lua ?
int LuaThunk_GetSoundInstanceInfo(lua_State* a1);
/// 0x089fb98b
int LuaToString_GetSoundInstanceInfo(lua_State* a1);
/// 0x089fba1a
UNK_RET GetSoundListenerInfo();
/// 0x089fbe72
/// lua ?
int LuaThunk_GetSoundListenerInfo(lua_State* a1);
/// 0x089fbed7
int LuaToString_GetSoundListenerInfo(lua_State* a1);
/// 0x089fbf4a
UNK_RET Get3DSoundInstanceChannelInfo(int a1);
/// 0x089fc106
/// lua ?
int LuaThunk_Get3DSoundInstanceChannelInfo(lua_State* a1);
/// 0x089fc133
int LuaToString_Get3DSoundInstanceChannelInfo(lua_State* a1);
