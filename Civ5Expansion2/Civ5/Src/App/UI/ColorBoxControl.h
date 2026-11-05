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
namespace FireGrafix {
class FGXSurface;
}

/// size ?
/// vtable 0x0a2fea90
class ColorBoxControl : public ControlBase {
public:
    /// 0x089b25f2
    ColorBoxControl();
    /// 0x089b266e
    /// deleting 0x089b2646
    virtual ~ColorBoxControl();
    /// 0x089b2718
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089b2a58
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089b2cc4
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089b23e8
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089b2704
    virtual UNK_RET Initialize();
    /// 0x089b2572
    virtual int PushSelf(lua_State* a1);
    /// 0x089b2912
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089b2bb6
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089b38ca
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x089b38c4
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089b38ec
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089b3912
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089b2682
    UNK_RET CreateSurface(FireGrafix::FGXSurface** a1);
    /// 0x089b28fa
    UNK_RET SetColorChannel(int a1, float a2);
    /// 0x0c07d2e8
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089b2de7
/// lua ?
int LuaThunk_ColorBoxControl_SetColor(lua_State* a1);
/// 0x089b3060
int LuaToString_ColorBoxControl_SetColor(lua_State* a1);
/// 0x089b30e1
/// lua ?
int LuaThunk_ColorBoxControl_SetColorVal(lua_State* a1);
/// 0x089b31d4
int LuaToString_ColorBoxControl_SetColorVal(lua_State* a1);
/// 0x089b326d
/// lua ?
int LuaThunk_ColorBoxControl_SetColorChannel(lua_State* a1);
/// 0x089b3309
int LuaToString_ColorBoxControl_SetColorChannel(lua_State* a1);
