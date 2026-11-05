/// origin guess
#pragma once

struct lua_State;

/// lua table UI
class cvLuaUI {
public:
    /// 0x08000010
    /// lua UI.GetX() -> integer
    /// lua UI.Sub.GetY() -> integer
    static int lGetX(lua_State* L);
    /// 0x08000011
    /// lua math.clamp(v: number, lo: number, hi: number) -> number
    static int pClamp(lua_State* L);
    /// 0x08000012
    /// lua print(...: any)
    static int pPrint(lua_State* L);
};

/// lua class ControlBase
class ControlBase {
public:
    /// 0x08000020
    /// lua ControlBase:SetHide(hide: boolean)
    /// lua AlphaAnim:SetHide(hide: boolean)
    void SetHide(bool b);
};

/// lua class AlphaAnim : ControlBase xml AlphaAnim
class AlphaAnimCtl : public ControlBase {};
