/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class ControlBase;
class FGXVector2;
class FGXVector3;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;

/// size ?
/// vtable 0x0a2ff610
class StackControl : public ControlBase {
public:
    class StackType;
    /// 0x089d059a
    StackControl();
    /// absent
    virtual ~StackControl();
    /// 0x089d0b08
    virtual UNK_RET AddChild(ControlBase* a1);
    /// 0x089d0b48
    virtual UNK_RET AddChildAtIndex(ControlBase* a1, int a2);
    /// 0x089d061e
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089d10b4
    virtual UNK_RET CalculateVisibilityBox();
    /// 0x089d1a06
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089d1d62
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089d0390
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089d0b88
    virtual UNK_RET Initialize();
    /// 0x089d20be
    virtual UNK_RET PerspectiveRender(unsigned int a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x089d2292
    virtual UNK_RET PerspectiveRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x089d0fb2
    virtual UNK_RET ProcessAnchoring();
    /// 0x089d14ae
    virtual UNK_RET ProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x089d051a
    virtual int PushSelf(lua_State* a1);
    /// 0x089d165e
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089d1832
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089d2d46
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089d2d4c
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089d2d72
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089d060c
    UNK_RET SetWrapWidth(int a1);
    /// 0x089d0caa
    UNK_RET CalculateSize();
    /// 0x089d10dc
    UNK_RET CalculateNextStackOffset(ControlBase* a1, FGXVector2& a2);
    /// 0x089d2466
    UNK_RET AddInitialPadding(FGXVector2& a1);
    /// 0x089d24a4
    UNK_RET AddPadding(StackControl::StackType a1, FGXVector2& a2);
    /// 0x089d2514
    UNK_RET AddWrapPadding(FGXVector2& a1);
    /// 0x089d2590
    UNK_RET AddGrowthPadding(FGXVector2& a1);
    /// 0x0c081a78
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089d2601
/// lua ?
int LuaThunk_StackControl_SetWrapWidth(lua_State* a1);
/// 0x089d2689
int LuaToString_StackControl_SetWrapWidth(lua_State* a1);
/// 0x089d270a
/// lua ?
int LuaThunk_StackControl_CalculateSize(lua_State* a1);
/// 0x089d277f
int LuaToString_StackControl_CalculateSize(lua_State* a1);
