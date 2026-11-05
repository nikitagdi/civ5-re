/// origin guess
#pragma once
#include "Civ5/Src/App/ButtonControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <list>
#include <lua.h>
#include <memory>

class ControlBase;
class FGXVector2;
class FGXVector4;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;

/// size ?
/// vtable 0x0a2fe960
class ColorBoxButtonControl : public ButtonControlBase {
public:
    /// 0x089b0ce6
    ColorBoxButtonControl();
    /// 0x089b1952
    /// deleting 0x089b198a
    virtual ~ColorBoxButtonControl();
    /// 0x089b0d64
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089b0eae
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089b102e
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089b0adc
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089b110a
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089b0d36
    virtual UNK_RET Initialize();
    /// 0x089b0c66
    virtual int PushSelf(lua_State* a1);
    /// 0x089b0df6
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089b0f76
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089b10f6
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x089b19cc
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089b19d2
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089b19f8
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x0c07cbc4
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089b1158
/// lua ?
int LuaThunk_ColorBoxButtonControl_SetColor(lua_State* a1);
/// 0x089b1619
int LuaToString_ColorBoxButtonControl_SetColor(lua_State* a1);
