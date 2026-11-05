/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class AILeaderMessageData;
class CvLeaderHeadErrorHook;
class CvLeaderHeadScene;
class FrameGlobalsData;
class LeaderArtDefineData;
class LeaderConfigureShadowsData;
class LeaderPlayAnimationData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NonSerialContainer;
class NullEventClass;
class SerialContainer;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class CvLeaderHeadSystem {
public:
    /// 0x088954a4
    CvLeaderHeadSystem();
    /// 0x08895714
    UNK_RET Startup(bool a1, bool a2, bool a3);
    /// 0x0889597e
    UNK_RET OnToggleManualCamera(EventTemplate<Event_Int2Type<129>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088959ca
    UNK_RET OnPlayLeaderAnimation(EventTemplate<Event_Int2Type<130>, LeaderPlayAnimationData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08895a0c
    UNK_RET OnGameplayLeaderMessage(GameCoreEventTemplate<Event_Int2Type<118>, AILeaderMessageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08895b8e
    UNK_RET OnLeaderInGame(GameCoreEventTemplate<Event_Int2Type<117>, LeaderArtDefineData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08895dec
    UNK_RET OnConfigureShadows(EventTemplate<Event_Int2Type<131>, LeaderConfigureShadowsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08895e5c
    UNK_RET OnGraphicsOptionsChanged(EventTemplate<Event_Int2Type<3>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08895ea4
    UNK_RET CanUseSM41();
    /// 0x08895ea8
    UNK_RET CanUseSM50();
    /// 0x08895eac
    UNK_RET ConfigureViewport(float a1, float a2);
    /// 0x08895fe8
    UNK_RET Shutdown();
    /// 0x08896126
    UNK_RET DestroyGameplayLeaderScenes();
    /// 0x088961da
    UNK_RET ClearCachedAssets();
    /// 0x08896200
    UNK_RET DisableManualCamera();
    /// 0x08896236
    UNK_RET StartupGame();
    /// 0x08896238
    UNK_RET ShutdownGame();
    /// 0x08896294
    UNK_RET CanUseLeaderAA();
    /// 0x08896298
    UNK_RET UseSM41(bool a1);
    /// 0x0889639a
    UNK_RET UnloadScene(CvLeaderHeadScene* a1);
    /// 0x088963c8
    UNK_RET PlayScene(CvLeaderHeadScene* a1);
    /// 0x08896484
    UNK_RET Update(float a1, bool a2);
    /// 0x08896576
    UNK_RET Render(unsigned int a1, bool a2);
    /// 0x088965d6
    UNK_RET RenderCompressionBenchmark(unsigned int a1);
    /// 0x08896610
    UNK_RET FillFrameGlobals(FrameGlobalsData& a1) const;
    /// 0x0889670e
    UNK_RET EnableManualCamera();
    /// 0x0889678e
    UNK_RET ResetCamera();
    /// 0x088967b8
    UNK_RET ReloadAssets();
    /// 0x0889682a
    UNK_RET AddErrorHook(CvLeaderHeadErrorHook* a1);
    /// 0x0889689e
    UNK_RET RemoveErrorHook(CvLeaderHeadErrorHook* a1);
    /// 0x0889692c
    UNK_RET DispatchErrorMessage(FStringA const& a1);
    /// 0x088969c8
    UNK_RET OnResolutionChanged();
    /// 0x08896a1c
    UNK_RET PutSceneInCache(CvLeaderHeadScene* a1);
    /// 0x08896b00
    UNK_RET MakeRoomInCache();
};
