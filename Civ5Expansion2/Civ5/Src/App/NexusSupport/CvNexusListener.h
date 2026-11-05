/// origin guess
#pragma once
#include "Civ5/Src/App/NexusSupport/Listener.h"
#include "FirePlace/FireWorks/FGlobalFacility.h"
#include "FirePlace/FireWorks/FMemHooks.h"

class AppUpdateData;
class CvNexusListener;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, eMPoolType T1, int T2> class FNewInstanceBuilder;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fdea0
class CvNexusListener : public Nexus::Listener, public FGlobalFacility<CvNexusListener, FNewInstanceBuilder<CvNexusListener, (eMPoolType)26, 0> > {
public:
    /// 0x08992a1c
    CvNexusListener();
    /// 0x08992ad2
    /// deleting 0x08992aaa
    virtual ~CvNexusListener();
    /// 0x08992c54
    virtual UNK_RET RegisterModes();
    /// 0x08992b74
    UNK_RET Enable(char const* a1, unsigned short a2);
    /// 0x08992c48
    UNK_RET OnIdle(float a1);
    /// 0x089933a6
    UNK_RET OnUpdate(EventTemplate<Event_Int2Type<2>, AppUpdateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};
