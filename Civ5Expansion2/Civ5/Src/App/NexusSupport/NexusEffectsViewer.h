/// origin guess
#pragma once
#include "Civ5/Src/App/NexusSupport/Mode.h"
#include <Windows.h>

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class ParticleEffectInfoResponseData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a3005f0
class NexusEffectsViewer : public Nexus::Mode {
public:
    /// 0x08b55cd4
    NexusEffectsViewer();
    /// 0x08b55e2e
    /// deleting 0x08b55e06
    virtual ~NexusEffectsViewer();
    /// 0x08b56dba
    virtual UNK_RET Shutdown();
    /// 0x08b55e90
    virtual UNK_RET Startup();
    /// 0x08b56ef6
    virtual UNK_RET Update();
    /// 0x08b56662
    UNK_RET OnEffectInfoResponse(EventTemplate<Event_Int2Type<48>, ParticleEffectInfoResponseData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b56740
    UNK_RET SetTimeScale(float a1);
    /// 0x08b56754
    UNK_RET SetWindow(HWND__* a1);
    /// 0x08b56826
    UNK_RET WindowResizing();
    /// 0x08b5683a
    UNK_RET WindowResized();
    /// 0x08b5687c
    UNK_RET MouseMove(int a1, int a2);
    /// 0x08b56990
    UNK_RET MiddleMouseButton(bool a1);
    /// 0x08b569a0
    UNK_RET MouseWheel(int a1);
    /// 0x08b569ce
    UNK_RET ResetCamera();
    /// 0x08b569f4
    UNK_RET SendEffectsList();
    /// 0x08b56a88
    UNK_RET SetEffect(unsigned int a1);
    /// 0x08b56bec
    UNK_RET SetOneShotEffects(bool a1);
    /// 0x08b56caa
    UNK_RET ReloadEffects();
    /// 0x08b56d3e
    UNK_RET SendBackgroundColor();
    /// 0x08b56d8a
    UNK_RET SetBackgroundColor(float a1, float a2, float a3);
    /// 0x08b57184
    UNK_RET UpdateGlobals();
    /// 0x08b5792c
    UNK_RET UpdateCameraAspectRatio();
};
