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
/// vtable 0x0a2fe830
class CheckBoxControl : public ButtonControlBase {
public:
    /// 0x089ae1ea
    CheckBoxControl();
    /// 0x089ae32e
    /// deleting 0x089ae306
    virtual ~CheckBoxControl();
    /// 0x089ae662
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089aee5e
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089af0fe
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089aebfc
    virtual UNK_RET ClipTestHit(FGXVector2 const& a1, FGXVector2 const& a2, FGXVector4 const& a3);
    /// 0x089ae508
    virtual UNK_RET DoClick(unsigned int a1);
    /// 0x089adfe0
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089af25e
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089ae428
    virtual UNK_RET Initialize();
    /// 0x089ae16a
    virtual int PushSelf(lua_State* a1);
    /// 0x089aed1e
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089aefbe
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089aecd2
    virtual UNK_RET SetVisState(unsigned int a1);
    /// 0x089aeb5a
    virtual UNK_RET TestHit(FGXVector2 const& a1, FGXVector2 const& a2);
    /// 0x089b00c2
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089b00c8
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089b00ee
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089aeab2
    virtual UNK_RET ProcessMouse(unsigned int a1, unsigned int a2, long a3, bool a4, FGXVector2& a5);
    /// 0x089aecae
    virtual UNK_RET SetCheck(bool a1);
    /// 0x089ae590
    UNK_RET SetAndCall(bool a1);
    /// 0x0c07c498
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089af2d5
/// lua ?
int LuaThunk_CheckBoxControl_SetCheck(lua_State* a1);
/// 0x089af3bf
int LuaToString_CheckBoxControl_SetCheck(lua_State* a1);
/// 0x089af440
/// lua ?
int LuaThunk_CheckBoxControl_SetAndCall(lua_State* a1);
/// 0x089af483
int LuaToString_CheckBoxControl_SetAndCall(lua_State* a1);
/// 0x089af504
/// lua ?
int LuaThunk_CheckBoxControl_IsChecked(lua_State* a1);
/// 0x089af58b
int LuaToString_CheckBoxControl_IsChecked(lua_State* a1);
/// 0x089af5fe
/// lua ?
int LuaThunk_CheckBoxControl_RegisterCheckHandler(lua_State* a1);
/// 0x089af720
int LuaToString_CheckBoxControl_RegisterCheckHandler(lua_State* a1);
/// 0x089af7a1
/// lua ?
int LuaThunk_CheckBoxControl_GetButton(lua_State* a1);
/// 0x089af882
int LuaToString_CheckBoxControl_GetButton(lua_State* a1);
/// 0x089af8f5
/// lua ?
int LuaThunk_CheckBoxControl_GetTextButton(lua_State* a1);
/// 0x089af9d6
int LuaToString_CheckBoxControl_GetTextButton(lua_State* a1);
