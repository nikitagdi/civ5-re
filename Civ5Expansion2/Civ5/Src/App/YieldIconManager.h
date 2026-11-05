/// origin guess
#pragma once

class CameraRephasedData;
class FGXInt2;
class HexCultureChangedData;
class HexFOWStateChangedData;
class HexID;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class RequestYieldDisplayData;
class SerialContainer;
class VisibilityUpdatedData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class YieldIconManager {
public:
    enum DisplayMode : int;
    /// 0x086daa30
    YieldIconManager();
    /// 0x086daac4
    UNK_RET Reset();
    /// 0x086dab70
    UNK_RET RegisterForEvents();
    /// 0x086dac46
    UNK_RET OnRequestYieldDisplay(EventTemplate<Event_Int2Type<87>, RequestYieldDisplayData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086db53c
    UNK_RET OnVisibilityUpdated(EventTemplate<Event_Int2Type<35>, VisibilityUpdatedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086db6ec
    UNK_RET OnSerialEventHexCultureChanged(GameCoreEventTemplate<Event_Int2Type<29>, HexCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086db870
    UNK_RET OnHexYieldMightHaveChanged(GameCoreEventTemplate<Event_Int2Type<116>, HexID, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086db8f0
    UNK_RET OnHexFOWStateChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086db96e
    UNK_RET OnCameraRephased(EventTemplate<Event_Int2Type<59>, CameraRephasedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086db978
    UNK_RET UnregisterForEvents();
    /// 0x086dba4e
    UNK_RET InitializeCamera(EventTemplate<Event_Int2Type<35>, VisibilityUpdatedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1, unsigned int a2);
    /// 0x086dbb68
    UNK_RET AdjustCameraDeltas(FGXInt2& a1, FGXInt2& a2, FGXInt2& a3, FGXInt2& a4);
    /// 0x086dc216
    UNK_RET BroadcastCurrent(bool a1);
    /// 0x086dc2f4
    UNK_RET SwitchToDisplayMode(YieldIconManager::DisplayMode a1);
};
