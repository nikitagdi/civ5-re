/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FGXVector2;
class FGXVector4;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;

/// size ?
/// vtable 0x0a2ff050
class MeterControl : public ControlBase {
public:
    /// 0x089c1a0e
    MeterControl();
    /// 0x089c369e
    /// deleting 0x089c3702
    virtual ~MeterControl();
    /// 0x089c1d6c
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089c24e0
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089c2836
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089c1804
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089c1b44
    virtual UNK_RET Initialize();
    /// 0x089c198e
    virtual int PushSelf(lua_State* a1);
    /// 0x089c234e
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089c26a4
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089c29fa
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x089c376e
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089c3774
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089c379a
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089c1ff0
    UNK_RET SetPercent(float a1);
    /// 0x089c2026
    UNK_RET UpdateMeters();
    /// 0x089c22c8
    UNK_RET SetShadowPercent(float a1);
    /// 0x089c22fe
    UNK_RET SetPercents(float a1, float a2);
    /// 0x089c2a4e
    UNK_RET SetShadowColor(FGXVector4 a1);
    /// 0x0c07f6ac
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089c2aa2
/// lua ?
int LuaThunk_MeterControl_SetPercent(lua_State* a1);
/// 0x089c2b92
int LuaToString_MeterControl_SetPercent(lua_State* a1);
/// 0x089c2c13
/// lua ?
int LuaThunk_MeterControl_SetShadowPercent(lua_State* a1);
/// 0x089c2c56
int LuaToString_MeterControl_SetShadowPercent(lua_State* a1);
/// 0x089c2cd7
/// lua ?
int LuaThunk_MeterControl_SetPercents(lua_State* a1);
/// 0x089c2dc3
int LuaToString_MeterControl_SetPercents(lua_State* a1);
/// 0x089c2e4c
/// lua ?
int LuaThunk_MeterControl_SetShadowColor(lua_State* a1);
/// 0x089c30f2
int LuaToString_MeterControl_SetShadowColor(lua_State* a1);
