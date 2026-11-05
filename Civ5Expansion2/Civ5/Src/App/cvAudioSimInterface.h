/// origin guess
#pragma once

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class cvAudioSimInterface {
public:
    /// 0x089f62fc
    cvAudioSimInterface();
    /// 0x089f62fe
    UNK_RET Startup();
    /// 0x089f633a
    UNK_RET SubscribeToEvents();
    /// 0x089f6376
    UNK_RET Shutdown();
    /// 0x089f63b2
    UNK_RET UnSubscribeFromEvents();
    /// 0x089f63ee
    UNK_RET OnButtonClicked(EventTemplate<Event_Int2Type<23>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};
