/// origin guess
#pragma once

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SystemUpdateUIData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class FGXIMEFontManager {
public:
    /// 0x0873d32a
    FGXIMEFontManager();
    /// 0x0873d460
    ~FGXIMEFontManager();
    /// 0x0873d3c4
    UNK_RET OnDeviceReset(EventTemplate<Event_Int2Type<106>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0873d422
    UNK_RET OnSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0873d4f0
    UNK_RET ReleaseAllCachedFonts();
    /// 0x0873d57a
    UNK_RET GetFont(int a1);
    /// 0x0873d79c
    UNK_RET GetDefaultIMEFont();
};
