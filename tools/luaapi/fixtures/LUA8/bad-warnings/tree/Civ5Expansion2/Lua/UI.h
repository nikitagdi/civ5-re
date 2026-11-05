/// origin guess
#pragma once

struct lua_State;

template <int N> struct Event_Int2Type {};
struct NullEventClass {};
struct LinearEventDispatcher {};
struct NonSerialContainer {};
struct SerialContainer {};
template <class... A> struct EventTemplate {};

struct UnitData { int id; };
struct Other { int id; };

/// lua event Events.UnitCreated
typedef EventTemplate<Event_Int2Type<7>, UnitData, LinearEventDispatcher, SerialContainer> EvX;


/// 0x08000030
/// lua push (id: integer)
void ToLua(lua_State* L, Other& d);
/// 0x08000031
/// lua pull (id: string)
void FromLua(lua_State* L, Other& d);
