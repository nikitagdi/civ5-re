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
/// vtable 0x0a2ff3c0
class ScrollPanelControl : public ControlBase {
public:
    /// 0x089c9b9e
    ScrollPanelControl();
    /// 0x089c9cd2
    /// deleting 0x089c9caa
    virtual ~ScrollPanelControl();
    /// 0x089ca36c
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089cb16e
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089cb4c4
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089c9994
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089cb900
    virtual UNK_RET ExposeTree(FLua::Table& a1, bool a2);
    /// 0x089cb9a2
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089c9d90
    virtual UNK_RET Initialize();
    /// 0x089ca224
    virtual UNK_RET ProcessAnchoring();
    /// 0x089ca72e
    virtual UNK_RET ProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x089c9b1e
    virtual int PushSelf(lua_State* a1);
    /// 0x089cab5a
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089cae64
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089cb81a
    virtual UNK_RET SetSizeVal(float a1, float a2);
    /// 0x089ccc20
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089ccc26
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089ccca0
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089c9e5c
    UNK_RET UpPressed(unsigned int a1, unsigned int a2);
    /// 0x089c9e94
    UNK_RET DownPressed(unsigned int a1, unsigned int a2);
    /// 0x089c9ecc
    UNK_RET BarMoved(float a1, unsigned int a2, unsigned int a3);
    /// 0x089c9ee0
    UNK_RET CalculateInternalSize();
    /// 0x089ca27c
    UNK_RET SetScrollValue(float a1);
    /// 0x089ca348
    UNK_RET HideScrollBar(bool a1);
    /// 0x0c080c2c
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089cba4b
/// lua ?
int LuaThunk_ScrollPanelControl_CalculateInternalSize(lua_State* a1);
/// 0x089cbac0
int LuaToString_ScrollPanelControl_CalculateInternalSize(lua_State* a1);
/// 0x089cbb33
/// lua ?
int LuaThunk_ScrollPanelControl_HideScrollBar(lua_State* a1);
/// 0x089cbbcd
int LuaToString_ScrollPanelControl_HideScrollBar(lua_State* a1);
/// 0x089cbc4e
/// lua ?
int LuaThunk_ScrollPanelControl_SetScrollValue(lua_State* a1);
/// 0x089cbcdb
int LuaToString_ScrollPanelControl_SetScrollValue(lua_State* a1);
/// 0x089cbd5c
/// lua ?
int LuaThunk_ScrollPanelControl_GetScrollValue(lua_State* a1);
/// 0x089cbe35
int LuaToString_ScrollPanelControl_GetScrollValue(lua_State* a1);
/// 0x089cbea8
/// lua ?
int LuaThunk_ScrollPanelControl_GetScrollBar(lua_State* a1);
/// 0x089cbf87
int LuaToString_ScrollPanelControl_GetScrollBar(lua_State* a1);
/// 0x089cbffa
/// lua ?
int LuaThunk_ScrollPanelControl_GetUpButton(lua_State* a1);
/// 0x089cc132
int LuaToString_ScrollPanelControl_GetUpButton(lua_State* a1);
/// 0x089cc1a5
/// lua ?
int LuaThunk_ScrollPanelControl_GetDownButton(lua_State* a1);
/// 0x089cc1e6
int LuaToString_ScrollPanelControl_GetDownButton(lua_State* a1);
/// 0x089cc259
/// lua ?
int LuaThunk_ScrollPanelControl_GetRatio(lua_State* a1);
/// 0x089cc29a
int LuaToString_ScrollPanelControl_GetRatio(lua_State* a1);
/// 0x089cc30d
/// lua ?
int LuaThunk_ScrollPanelControl_RegisterUpEndCallback(lua_State* a1);
/// 0x089cc488
int LuaToString_ScrollPanelControl_RegisterUpEndCallback(lua_State* a1);
/// 0x089cc509
/// lua ?
int LuaThunk_ScrollPanelControl_RegisterDownEndCallback(lua_State* a1);
/// 0x089cc54c
int LuaToString_ScrollPanelControl_RegisterDownEndCallback(lua_State* a1);
