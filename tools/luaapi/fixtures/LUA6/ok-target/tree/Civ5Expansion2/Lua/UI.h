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
    /// lua ControlBase:SetSize(size: Vector2, n?: integer) -> number
    float SetSize(const FGXVector2& v, double n);
    /// 0x08000012
    /// lua ControlBase:GetParent() -> ControlBase?
    ControlBase* GetParent();
    /// 0x08000013
    /// lua ControlBase:Raw(...: any)
    int Raw(lua_State* L);
};

/// 0x08000020
int LuaThunk_ControlBase_SetHide(lua_State* L);
/// 0x08000021
int LuaToString_ControlBase_SetHide(lua_State* L);

/// 0x08000022
int LuaThunk_ControlBase_SetSize(lua_State* L);
/// 0x08000023
int LuaThunk_ControlBase_GetParent(lua_State* L);
/// 0x08000024
int LuaThunk_ControlBase_Raw(lua_State* L);
