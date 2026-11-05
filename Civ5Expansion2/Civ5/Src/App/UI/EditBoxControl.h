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
/// vtable 0x0a2febb0
class EditBoxControl : public ControlBase {
public:
    /// 0x089b450a
    EditBoxControl();
    /// 0x089b465e
    /// deleting 0x089b4636
    virtual ~EditBoxControl();
    /// 0x089b5c12
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089b62bc
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089b671e
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089b4300
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089b6e50
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089b6e44
    virtual UNK_RET GetFocusStop() const;
    /// 0x089b6de4
    virtual UNK_RET HasFocus() const;
    /// 0x089b472e
    virtual UNK_RET Initialize();
    /// 0x089b4c56
    virtual UNK_RET LostFocus();
    /// 0x089b4870
    virtual UNK_RET ProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x089b448a
    virtual int PushSelf(lua_State* a1);
    /// 0x089b60ba
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089b651c
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089b4a4a
    virtual UNK_RET TakeFocus();
    /// 0x089b490a
    virtual UNK_RET TestLostFocus();
    /// 0x089b5bac
    virtual UNK_RET Update(float a1);
    /// 0x089b83b2
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089b83b8
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089b83de
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089b551e
    virtual UNK_RET ProcessChar(unsigned int a1, long a2);
    /// 0x089b50f6
    virtual UNK_RET ProcessControlChar(unsigned int a1, long a2);
    /// 0x089b4cc0
    virtual UNK_RET ImeBeginComposition();
    /// 0x089b4d9e
    virtual UNK_RET ImeComposition(wchar_t const* a1);
    /// 0x089b4f0e
    virtual UNK_RET ImeSetCursorOffset(int a1);
    /// 0x089b507e
    virtual UNK_RET ImeEndComposition();
    /// 0x089b49e4
    UNK_RET InvokeCallback(bool a1);
    /// 0x089b4baa
    UNK_RET CheckStringOffset();
    /// 0x089b4ce6
    UNK_RET SaveState();
    /// 0x089b4e56
    UNK_RET RestoreState();
    /// 0x089b501e
    UNK_RET GetCharacterIndexAtOffset(unsigned int a1);
    /// 0x089b5022
    UNK_RET GetCharacterOffsetAtIndex(unsigned int a1);
    /// 0x089b547e
    UNK_RET ClearString();
    /// 0x089b5b30
    UNK_RET GetCharacterCount();
    /// 0x089b697e
    UNK_RET SetText(char const* a1);
    /// 0x089b6b16
    static int LocalizeAndSetText(lua_State* a1);
    /// 0x089b6df6
    UNK_RET SetTextObscured(bool a1);
    /// 0x0c07da0c
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089b6eed
/// lua ?
int LuaThunk_EditBoxControl_SetText(lua_State* a1);
/// 0x089b6f82
int LuaToString_EditBoxControl_SetText(lua_State* a1);
/// 0x089b7009
/// lua ?
int LuaThunk_EditBoxControl_LocalizeAndSetText(lua_State* a1);
/// 0x089b701d
int LuaToString_EditBoxControl_LocalizeAndSetText(lua_State* a1);
/// 0x089b707c
/// lua ?
int LuaThunk_EditBoxControl_RegisterCallback(lua_State* a1);
/// 0x089b719e
int LuaToString_EditBoxControl_RegisterCallback(lua_State* a1);
/// 0x089b721f
/// lua ?
int LuaThunk_EditBoxControl_TakeFocus(lua_State* a1);
/// 0x089b72e5
int LuaToString_EditBoxControl_TakeFocus(lua_State* a1);
/// 0x089b7358
/// lua ?
int LuaThunk_EditBoxControl_HasFocus(lua_State* a1);
/// 0x089b75db
int LuaToString_EditBoxControl_HasFocus(lua_State* a1);
/// 0x089b764e
/// lua ?
int LuaThunk_EditBoxControl_ClearString(lua_State* a1);
/// 0x089b7691
int LuaToString_EditBoxControl_ClearString(lua_State* a1);
/// 0x089b7704
/// lua ?
int LuaThunk_EditBoxControl_GetText(lua_State* a1);
/// 0x089b7852
int LuaToString_EditBoxControl_GetText(lua_State* a1);
/// 0x089b78c5
/// lua ?
int LuaThunk_EditBoxControl_GetVoid1(lua_State* a1);
/// 0x089b799e
int LuaToString_EditBoxControl_GetVoid1(lua_State* a1);
/// 0x089b7a11
/// lua ?
int LuaThunk_EditBoxControl_GetVoid2(lua_State* a1);
/// 0x089b7a52
int LuaToString_EditBoxControl_GetVoid2(lua_State* a1);
/// 0x089b7ac5
/// lua ?
int LuaThunk_EditBoxControl_SetVoid1(lua_State* a1);
/// 0x089b7ba9
int LuaToString_EditBoxControl_SetVoid1(lua_State* a1);
/// 0x089b7c2a
/// lua ?
int LuaThunk_EditBoxControl_SetVoid2(lua_State* a1);
/// 0x089b7c6d
int LuaToString_EditBoxControl_SetVoid2(lua_State* a1);
/// 0x089b7cee
/// lua ?
int LuaThunk_EditBoxControl_SetVoids(lua_State* a1);
/// 0x089b7d8f
int LuaToString_EditBoxControl_SetVoids(lua_State* a1);
