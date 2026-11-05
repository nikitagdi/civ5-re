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
class cvAudioSimMainMenu {
public:
    /// 0x0878cf88
    cvAudioSimMainMenu();
    /// 0x0878cf9a
    UNK_RET Startup();
    /// 0x0878cff8
    UNK_RET SubscribeToEvents();
    /// 0x0878d056
    UNK_RET Shutdown();
    /// 0x0878d0b4
    UNK_RET UnSubscribeFromEvents();
    /// 0x0878d112
    UNK_RET OnBeginMainMenu(EventTemplate<Event_Int2Type<160>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0878d126
    UNK_RET OnAfterModsActivate(EventTemplate<Event_Int2Type<91>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0878d128
    UNK_RET UpdateMusic();
    /// 0x0878d1ca
    UNK_RET Update(float a1);
    /// 0x0878d1cc
    UNK_RET StopMusic();
};
