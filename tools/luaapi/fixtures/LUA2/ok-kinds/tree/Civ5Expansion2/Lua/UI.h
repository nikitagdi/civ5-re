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
    static int lGetX(lua_State* L);
    // Registered but never exposed.
    /// 0x08000011
    /// lua none
    static int lNone(lua_State* L);
};

/// lua class Base
class Base {};

/// lua enum Keys
enum Keys { VK_A = 1 };

/// lua event Events.SerialEventX
typedef EventTemplate<Event_Int2Type<7>, NullEventClass, LinearEventDispatcher, NonSerialContainer> EvX;

/// 0x08000020
/// lua ?
int LuaThunk_Base_Run(lua_State* L);

/// 0x08000021
int LuaToString_Base_Run(lua_State* L);

/// 0x08000030
/// lua push (a: integer, b: integer)
void ToLua(lua_State* L, Base& b);

/// 0x08000031
/// lua pull (a: integer, b: integer)
void FromLua(lua_State* L, Base& b);

/// 0x08000032
/// lua shape Pair {a: integer, b: integer}
void Pairing(int a);
