/// origin guess
#pragma once

struct lua_State;

/// lua shape Vector2 {x: number, y: number}
struct FGXVector2 { float x; float y; };
struct Mystery {};

/// lua class ControlBase
class ControlBase {
public:

    /// 0x08000010
    /// lua ControlBase:SetHide(hide: boolean)
    void SetHide(bool b);
    /// 0x08000011
    /// lua ControlBase:SetHide(hide: boolean)
    void SetHide2(bool b);
};

/// 0x08000020
/// lua ?
int LuaThunk_ControlBase_SetHide(lua_State* L);
/// 0x08000021
int LuaToString_ControlBase_SetHide(lua_State* L);
