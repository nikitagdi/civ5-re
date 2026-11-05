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
    void SetHide(Mystery m);
};

/// 0x08000020
int LuaThunk_ControlBase_SetHide(lua_State* L);
/// 0x08000021
int LuaToString_ControlBase_SetHide(lua_State* L);
