/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <list>
#include <lua.h>
#include <memory>

class ControlBase;
class FGXVector2;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;

/// size ?
/// vtable 0x0a2fe4f0
class BarControl : public ControlBase {
public:
    /// 0x089a6362
    BarControl();
    /// 0x089a78b4
    /// deleting 0x089a78fc
    virtual ~BarControl();
    /// 0x089a64ee
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089a6a78
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089a6ca6
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089a6158
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089a6dd0
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089a6478
    virtual UNK_RET Initialize();
    /// 0x089a62e2
    virtual int PushSelf(lua_State* a1);
    /// 0x089a6974
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089a6ba2
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089a794c
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089a7952
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089a7978
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089a6774
    UNK_RET SetPercent(float a1);
    /// 0x0c07af48
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089a6e47
/// lua ?
int LuaThunk_BarControl_SetFGColor(lua_State* a1);
/// 0x089a710d
int LuaToString_BarControl_SetFGColor(lua_State* a1);
/// 0x089a718e
/// lua ?
int LuaThunk_BarControl_SetBGColor(lua_State* a1);
/// 0x089a71d1
int LuaToString_BarControl_SetBGColor(lua_State* a1);
/// 0x089a7252
/// lua ?
int LuaThunk_BarControl_SetPercent(lua_State* a1);
/// 0x089a72df
int LuaToString_BarControl_SetPercent(lua_State* a1);
