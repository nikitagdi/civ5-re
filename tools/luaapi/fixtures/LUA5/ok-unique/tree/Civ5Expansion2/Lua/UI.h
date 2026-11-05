/// origin guess
#pragma once

struct lua_State;

template <int N> struct Event_Int2Type {};
struct NullEventClass {};
struct LinearEventDispatcher {};
struct NonSerialContainer {};
struct SerialContainer {};
template <class... A> struct EventTemplate {};

/// lua table UI
class cvLuaUI {
public:
    /// 0x08000010
    /// lua UI.GetX() -> integer
    /// lua UI.GetX(a: integer) -> integer
    static int lGetX(lua_State* L);
    /// 0x08000011
    /// lua UI.GetY() -> integer
    static int lGetY(lua_State* L);
};

/// lua event Events.A
typedef EventTemplate<Event_Int2Type<1>, NullEventClass, LinearEventDispatcher, NonSerialContainer> EvA;
/// lua event Events.B
typedef EventTemplate<Event_Int2Type<2>, NullEventClass, LinearEventDispatcher, NonSerialContainer> EvB;
/// lua event Events.C
typedef EventTemplate<Event_Int2Type<1>, NullEventClass, LinearEventDispatcher, SerialContainer> EvC;
