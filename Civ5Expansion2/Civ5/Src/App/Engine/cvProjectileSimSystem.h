/// origin guess
#pragma once

class ChangeProjectileTargetData;
class ChangeProjectileTransformData;
class CreateProjectileData;
class EditProjectileData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvProjectileSim;
class cvProjectileSim_Init;
class cvProjectileSim_Transform;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class cvProjectileSimSystem {
public:
    /// 0x087efd1c
    cvProjectileSimSystem();
    /// 0x087efd8a
    ~cvProjectileSimSystem();
    /// 0x087efdd4
    UNK_RET Startup();
    /// 0x087efe0e
    UNK_RET SubscribeToEvents();
    /// 0x087efed0
    UNK_RET StartupGame();
    /// 0x087efed2
    UNK_RET Shutdown();
    /// 0x087eff06
    UNK_RET UnSubscribeToEvents();
    /// 0x087effa0
    UNK_RET DestroyAllSims();
    /// 0x087f0008
    UNK_RET ShutdownGame();
    /// 0x087f001c
    UNK_RET Update(float a1);
    /// 0x087f0086
    UNK_RET UpdateSimTransform(unsigned int const& a1, cvProjectileSim_Transform const& a2);
    /// 0x087f00ce
    UNK_RET FinishSim(unsigned int const& a1);
    /// 0x087f010e
    UNK_RET GetVisEffectInfoHandle(unsigned int const& a1);
    /// 0x087f0138
    UNK_RET OnCreateProjectile(EventTemplate<Event_Int2Type<51>, CreateProjectileData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087f02ea
    UNK_RET InitializeSim(cvProjectileSim& a1, cvProjectileSim_Init& a2);
    /// 0x087f039e
    UNK_RET OnDestroyProjectile(EventTemplate<Event_Int2Type<52>, EditProjectileData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087f043a
    UNK_RET OnChangeProjectileTarget(EventTemplate<Event_Int2Type<55>, ChangeProjectileTargetData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087f04c0
    UNK_RET OnChangeProjectileTransform(EventTemplate<Event_Int2Type<56>, ChangeProjectileTransformData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};
