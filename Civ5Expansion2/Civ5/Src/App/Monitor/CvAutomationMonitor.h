/// origin binary
#pragma once
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include "FirePlace/FireWorks/FString.h"

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SequencerContainer;
class SerialContainer;
class TerrainCacheData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSequencerAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
namespace cvAutomation {
class EventType;
class StateType;
}

/// size ?
class cvAutomationMonitor {
public:
    class EventScriptEntry;
    /// size ?
    class EventScriptEntry {
    public:
        /// 0x0899190e
        EventScriptEntry();
        /// 0x08991984
        EventScriptEntry(char const* a1);
        /// 0x08991c18
        EventScriptEntry(FLua::Function a1);
        /// 0x08991c96
        EventScriptEntry(cvAutomationMonitor::EventScriptEntry const& a1);
    };
    /// 0x08990d88
    cvAutomationMonitor();
    /// 0x08990f96
    ~cvAutomationMonitor();
    /// 0x08990e28
    UNK_RET SetFileNameTag(char const* a1);
    /// 0x08990efc
    UNK_RET SubscribeToMessages();
    /// 0x08991000
    UNK_RET UnsubscribeFromMessages();
    /// 0x0899109a
    UNK_RET Startup();
    /// 0x089911d2
    UNK_RET Shutdown();
    /// 0x089911d4
    UNK_RET SetEventScript(cvAutomation::EventType a1, char const* a2);
    /// 0x0899133e
    UNK_RET SetEventFunction(cvAutomation::EventType a1, FLua::Function a2);
    /// 0x089914d2
    UNK_RET OnEvent(cvAutomation::EventType a1);
    /// 0x08991574
    UNK_RET PreloadEventScript(cvAutomation::EventType a1);
    /// 0x08991822
    UNK_RET OnStartGame(GameCoreEventTemplate<Event_Int2Type<3>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08991858
    UNK_RET OnGameInitComplete(EventTemplate<Event_Int2Type<3>, NullEventClass, LinearEventDispatcher, SequencerContainer, NullEventClass, BaseEvent<cvEventSystemSequencerAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0899188e
    UNK_RET OnTerrainDone(EventTemplate<Event_Int2Type<2>, NullEventClass, LinearEventDispatcher, SequencerContainer, NullEventClass, BaseEvent<cvEventSystemSequencerAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08991890
    UNK_RET OnTerrainCaching(EventTemplate<Event_Int2Type<28>, TerrainCacheData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089918d8
    UNK_RET SetState(cvAutomation::StateType a1);
    /// 0x08991a2e
    UNK_RET GetStateTypeName(cvAutomation::StateType a1);
    /// 0x08991a52
    UNK_RET ReplaceFileNameTag(FStringA& a1, unsigned int* a2);
    /// 0x0c0785b8
    static UNK_DATA m_pInst[0x4];
    /// 0x0c0785bc
    static UNK_DATA ms_szAutomationFileNameTag[0x4];
    /// 0x0c0785c0
    static UNK_DATA ms_uiUniqueID[0x4];
};
