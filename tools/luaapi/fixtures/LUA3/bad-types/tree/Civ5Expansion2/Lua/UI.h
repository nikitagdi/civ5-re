/// origin guess
#pragma once

struct lua_State;

/// lua class Base : Nope
class Base {
public:
    /// 0x08000010
    /// lua Base:Pick(u: Unti) -> integer
    void Pick(int a);
    /// 0x08000011
    /// lua Base:Call(EventArgs)
    void Call(int a);
};
