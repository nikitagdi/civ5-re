/// origin guess
#pragma once

struct lua_State;

template <int N> struct Event_Int2Type {};
struct NullEventClass {};
struct LinearEventDispatcher {};
struct NonSerialContainer {};
struct SerialContainer {};
template <class... A> struct EventTemplate {};

/// lua shape Vector2 {x: number}
struct V1 { float x; };
/// lua shape Vector2 {y: number}
struct V2 { float y; };

/// lua event Events.A
typedef EventTemplate<Event_Int2Type<1>, NullEventClass, LinearEventDispatcher, NonSerialContainer> EvA;
/// lua event Events.A
typedef EventTemplate<Event_Int2Type<2>, NullEventClass, LinearEventDispatcher, NonSerialContainer> EvB;
/// lua event Events.C
typedef EventTemplate<Event_Int2Type<1>, NullEventClass, LinearEventDispatcher, NonSerialContainer> EvC;
