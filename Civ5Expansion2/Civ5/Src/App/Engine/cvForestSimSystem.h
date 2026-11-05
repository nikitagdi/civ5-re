/// origin guess
#pragma once

class ForestChangedData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class TerrainStencilUpdatedData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class cvForestSimSystem {
public:
    /// 0x087d252c
    cvForestSimSystem();
    /// 0x087d253e
    UNK_RET Startup();
    /// 0x087d2552
    UNK_RET SubscribeToEvents();
    /// 0x087d262a
    UNK_RET Shutdown();
    /// 0x087d263e
    UNK_RET UnsubscribeToEvents();
    /// 0x087d2716
    UNK_RET StartupGame();
    /// 0x087d2744
    UNK_RET ShutdownGame();
    /// 0x087d2772
    UNK_RET OnForestCreated(GameCoreEventTemplate<Event_Int2Type<63>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d27cc
    UNK_RET OnForestRemoved(GameCoreEventTemplate<Event_Int2Type<64>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d281e
    UNK_RET OnJungleCreated(GameCoreEventTemplate<Event_Int2Type<65>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d2878
    UNK_RET OnJungleRemoved(GameCoreEventTemplate<Event_Int2Type<66>, ForestChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087d28ca
    UNK_RET OnStencilUpdate(EventTemplate<Event_Int2Type<32>, TerrainStencilUpdatedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};
