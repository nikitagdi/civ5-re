/// origin guess
#pragma once

struct lua_State;

/// lua class Base
class Base {
public:
    /// 0x08000010
    /// lua Base:Pick(u: Unit, v: Vector2, k: Keys) -> Base?
    Base* Pick(int a);
};

/// lua class Event
class EvBase {
public:
    /// 0x08000011
    /// lua Event:Add(handler: fun(EventArgs))
    /// lua Event:__call(EventArgs)
    void Add(int a);
};

/// lua enum Keys
enum Keys { VK_A = 1 };

/// lua shape Vector2 {x: number, y: number}
struct Vector2 { float x; float y; };
