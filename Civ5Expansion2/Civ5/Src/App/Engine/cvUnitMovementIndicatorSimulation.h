/// origin guess
#pragma once

class DisplayMovementIndicatorData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class PathFinderData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class cvUnitMovementIndicatorSimulation {
public:
    /// 0x087e987c
    cvUnitMovementIndicatorSimulation();
    /// 0x087e988e
    ~cvUnitMovementIndicatorSimulation();
    /// 0x087e9890
    UNK_RET Startup();
    /// 0x087e98ee
    UNK_RET SubscribeToEvents();
    /// 0x087e994c
    UNK_RET Shutdown();
    /// 0x087e99aa
    UNK_RET UnsubscribeToEvents();
    /// 0x087e9a08
    UNK_RET StartupGame();
    /// 0x087e9a0a
    UNK_RET ShutdownGame();
    /// 0x087e9a0c
    UNK_RET OnUpdatePath(EventTemplate<Event_Int2Type<81>, PathFinderData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087e9ad4
    UNK_RET OnPathDisplay(EventTemplate<Event_Int2Type<164>, DisplayMovementIndicatorData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};
