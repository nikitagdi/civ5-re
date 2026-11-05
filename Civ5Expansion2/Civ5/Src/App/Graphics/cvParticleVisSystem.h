/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvVisEffectSystem.h"
#include "FirePlace/FireWorks/FJob.h"
#include <unordered_map>

class FrameGlobalsData;
class HexFOWStateChangedData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class WorldGlobalsData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvParticleVisDataDef;
class cvParticleVisSystem;
class cvParticleVis_ChangeEffectKey;
class cvParticleVis_DeSpawnEffect;
class cvParticleVis_OneTimeEffect;
class cvParticleVis_SpawnEffect;
class cvParticleVis_TriggerEffect;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1, template <class> class T2> class cvGeneralRefKey;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <class T0> class cvParticleVisSystemAccessor;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class CCommandStream;
}

/// size ?
class cvParticleVisSystem {
public:
    class cvParticleVisExternalJob;
    class cvParticleVis;
    /// size ?
    /// vtable 0x0a2fdb78
    class cvParticleVisExternalJob : public FJob {
    public:
        /// 0x088d61b4
        virtual void Execute(unsigned int a1);
    };
    /// 0x088d61b6
    cvParticleVisSystem();
    /// 0x088d629e
    ~cvParticleVisSystem();
    /// 0x088d62f0
    UNK_RET Startup(unsigned int a1);
    /// 0x088d63d2
    UNK_RET SubscribeToEvents();
    /// 0x088d64a8
    UNK_RET OnVisEffectHandleDestroyed(void* a1, unsigned int a2);
    /// 0x088d659c
    UNK_RET Shutdown();
    /// 0x088d6620
    UNK_RET UnSubscribeToEvents();
    /// 0x088d66f6
    UNK_RET DestroyVises();
    /// 0x088d673e
    UNK_RET StartupGame();
    /// 0x088d676a
    UNK_RET ShutdownGame();
    /// 0x088d67a8
    UNK_RET BeginUpdate();
    /// 0x088d67e4
    UNK_RET Update(float a1);
    /// 0x088d681c
    UNK_RET EndUpdate();
    /// 0x088d6848
    UNK_RET RenderInternal();
    /// 0x088d6874
    UNK_RET OnRenderView(unsigned int a1);
    /// 0x088d68b0
    UNK_RET OnRenderShadow(unsigned int a1);
    /// 0x088d68e4
    UNK_RET OnRenderView(unsigned int a1, unsigned int a2, FireGrafix::CCommandStream* a3, unsigned int a4, unsigned short a5);
    /// 0x088d6942
    UNK_RET OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088d6976
    UNK_RET OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088d69b4
    UNK_RET OnHexFOWStateChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088d6a26
    UNK_RET OnParticleEffectReloadRequested(EventTemplate<Event_Int2Type<44>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088d6a52
    UNK_RET OnParticleEffectStatsRequested(EventTemplate<Event_Int2Type<45>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088d6b02
    UNK_RET OnParticleEffectInfoRequested(EventTemplate<Event_Int2Type<47>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088d6c4c
    UNK_RET SetActiveParticleVisSystem(unsigned int a1);
    /// 0x088d6c80
    UNK_RET DestroyAllActiveParticleVises();
    /// 0x088d6cac
    UNK_RET UpdateFromKey(cvGeneralRefKey<cvParticleVisSystem, cvParticleVisDataDef, cvParticleVisSystemAccessor> a1, cvParticleVis_OneTimeEffect const& a2);
    /// 0x088d6d28
    UNK_RET UpdateFromKey(cvGeneralRefKey<cvParticleVisSystem, cvParticleVisDataDef, cvParticleVisSystemAccessor> a1, cvParticleVis_ChangeEffectKey const& a2);
    /// 0x088d6e7c
    UNK_RET UpdateFromKey(cvGeneralRefKey<cvParticleVisSystem, cvParticleVisDataDef, cvParticleVisSystemAccessor> a1, cvParticleVis_TriggerEffect const& a2);
    /// 0x088d6ee4
    UNK_RET CreateAndUpdateKey(cvGeneralRefKey<cvParticleVisSystem, cvParticleVisDataDef, cvParticleVisSystemAccessor> const& a1, cvVisEffectSystem::VisEffect_Init const& a2);
    /// 0x088d6ff2
    UNK_RET UpdateFromKey(cvGeneralRefKey<cvParticleVisSystem, cvParticleVisDataDef, cvParticleVisSystemAccessor> a1, cvParticleVis_SpawnEffect const& a2);
    /// 0x088d705a
    UNK_RET UpdateFromKey(cvGeneralRefKey<cvParticleVisSystem, cvParticleVisDataDef, cvParticleVisSystemAccessor> a1, cvParticleVis_DeSpawnEffect const& a2);
    /// 0x088d70ac
    UNK_RET ReleaseKey(std::__hash_map_iterator<std::__hash_iterator<std::__hash_node<std::__hash_value_type<cvGeneralRefKey<cvParticleVisSystem, cvParticleVisDataDef, cvParticleVisSystemAccessor>, cvParticleVisSystem::cvParticleVis>, void*>*> > const& a1);
    /// 0x088d7138
    UNK_RET GetVisEffectSystem();
};
