/// origin guess
#pragma once

struct lua_State;

/// lua table UI
class cvLuaUI {
public:
    /// 0x08000010
    /// lua UI.GetX(a: integer, b?: string) -> Unit?
    /// lua UI.GetXs(...: any) -> table<string, integer[]>, count: integer
    static int lGetX(lua_State* L);
};

/// lua enum Keys
enum Keys { VK_A = 1 };

/// lua shape Vector2 {x: number, y: number}
struct Vector2 { float x; float y; };
