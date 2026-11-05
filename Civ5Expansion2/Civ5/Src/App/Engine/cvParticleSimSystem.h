/// origin guess
#pragma once
#include <d3d9.h>

class GameplayFXData;
class HEXVec_i;
class HexID;
class LinearEventDispatcher;
class LocalMachineContainer;
class MapGlobalsData;
class NullEventClass;
class ParticleEffectData;
class ParticleEffectDeSpawnedData;
class ParticleEffectHandleChangedData;
class SerialContainer;
class UnitCreatedData;
class UnitHandleData;
class cvEventContext;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvParticleVisSystem;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <class T0> class cvParticleVisSystemAccessor;
template <int T0> class Event_Int2Type;

/// size ?
class cvParticleSimSystem {
public:
    /// 0x087ea690
    cvParticleSimSystem();
    /// 0x087ea75e
    ~cvParticleSimSystem();
    /// 0x087ea7a2
    UNK_RET Startup(cvParticleVisSystemAccessor<cvParticleVisSystem> const& a1);
    /// 0x087ea826
    UNK_RET SubscribeToEvents();
    /// 0x087ea96a
    UNK_RET LoadEventEffectsFromXml();
    /// 0x087eae10
    UNK_RET StartupGame();
    /// 0x087eae12
    UNK_RET Shutdown();
    /// 0x087eae5a
    UNK_RET UnSubscribeToEvents();
    /// 0x087eaf8a
    UNK_RET ShutdownGame();
    /// 0x087eafb0
    UNK_RET Update(float a1);
    /// 0x087eafd6
    UNK_RET OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087eb00a
    UNK_RET OnUnitCreated(GameCoreEventTemplate<Event_Int2Type<69>, UnitCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, cvEventContext>& a1);
    /// 0x087eb196
    UNK_RET OnUnitDestroyedInCombat(GameCoreEventTemplate<Event_Int2Type<71>, UnitHandleData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087eb302
    UNK_RET OnGameplayFX(GameCoreEventTemplate<Event_Int2Type<143>, GameplayFXData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087eb49c
    UNK_RET OnParticleEffectTriggered(EventTemplate<Event_Int2Type<40>, ParticleEffectData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087eb594
    UNK_RET OnParticleEffectSpawned(EventTemplate<Event_Int2Type<41>, ParticleEffectData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087eb68c
    UNK_RET OnParticleEffectDeSpawned(EventTemplate<Event_Int2Type<42>, ParticleEffectDeSpawnedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087eb744
    UNK_RET OnParticleEffectHandleChanged(EventTemplate<Event_Int2Type<43>, ParticleEffectHandleChangedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087eb804
    UNK_RET OnParticleEffectOneShot(EventTemplate<Event_Int2Type<49>, ParticleEffectData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087eb8c2
    UNK_RET GetWorldTransform(HEXVec_i const& a1, D3DXMATRIX& a2);
    /// 0x087eb992
    UNK_RET OnNaturalWonderRevealed(GameCoreEventTemplate<Event_Int2Type<140>, HexID, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087ebb2c
    UNK_RET CreateHandle(char const* a1);
    /// 0x087ebbb4
    UNK_RET HandleToName(unsigned int a1);
    /// 0x087ebbe2
    UNK_RET HandleNameContainsMacros(unsigned int a1);
    /// 0x087ebc10
    UNK_RET VisEffectTypeHandleValid(unsigned int a1);
};
