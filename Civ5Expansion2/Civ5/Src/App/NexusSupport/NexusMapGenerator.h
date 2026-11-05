/// origin guess
#pragma once
#include "Civ5/Src/App/NexusSupport/Mode.h"
#include <lua.h>

class LinearEventDispatcher;
class LocalMachineContainer;
class LuaOutputData;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a300680
class NexusMapGenerator : public Nexus::Mode {
public:
    /// 0x08b58b88
    NexusMapGenerator();
    /// 0x08b58bea
    /// deleting 0x08b58bb4
    virtual ~NexusMapGenerator();
    /// 0x08b59dba
    virtual UNK_RET Shutdown();
    /// 0x08b58c1a
    virtual UNK_RET Startup();
    /// 0x08b59dbc
    virtual UNK_RET Update();
    /// 0x08b58e4c
    UNK_RET SetMapOption(char const* a1, int a2);
    /// 0x08b58ed4
    UNK_RET GenerateMap(char const* a1, int a2, int a3);
    /// 0x08b594b6
    UNK_RET ScatterResources(char const* a1);
    /// 0x08b59938
    UNK_RET ScatterGoodies(char const* a1);
    /// 0x08b59dbe
    UNK_RET OnLuaOutputMessage(EventTemplate<Event_Int2Type<67>, LuaOutputData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};

/// 0x08b59df1
int WorldBuilderMapLoaderScatterResources(lua_State* a1);
/// 0x08b59ead
int WorldBuilderMapLoaderScatterGoodies(lua_State* a1);
