/// origin guess
#pragma once
#include "Civ5/Src/App/ButtonControlBase.h"
#include "Civ5/Src/App/cvTunerListener.h"
#include "FirePlace/FireWorks/FFastVector.h"
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
/// vtable 0x0a2fe610
class ButtonControl : public ButtonControlBase {
public:
    /// 0x089a8572
    ButtonControl();
    /// 0x089a861c
    /// deleting 0x089a85f4
    virtual ~ButtonControl();
    /// 0x089a8892
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089a8dca
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089a904a
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089a8368
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089a9864
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089a929c
    virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x089a93d2
    virtual UNK_RET GetNodes(FFastVector<cvTunerListener::ITreeNode*, false, 0u, 0u, BaseVector<cvTunerListener::ITreeNode*, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x089a8690
    virtual UNK_RET Initialize();
    /// 0x089a84f2
    virtual int PushSelf(lua_State* a1);
    /// 0x089a8c98
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089a8f18
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089aa7f4
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x089aa890
    virtual UNK_RET SetSizeVal(float a1, float a2);
    /// 0x089a920c
    virtual UNK_RET SetVisState(unsigned int a1);
    /// 0x089aa7ee
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089aa816
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089aa84e
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089a8770
    UNK_RET SetTextureOffset(FGXVector2 a1);
    /// 0x089a87be
    UNK_RET SetTextureOffsetVal(float a1, float a2);
    /// 0x089a8818
    UNK_RET SetText(char const* a1);
    /// 0x089a9198
    UNK_RET SetTexture(char const* a1);
    /// 0x089a9462
    static int LocalizeAndSetText(lua_State* a1);
    /// 0x0c07b66c
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089a98b7
/// lua ?
int LuaThunk_ButtonControl_SetTexture(lua_State* a1);
/// 0x089a9960
int LuaToString_ButtonControl_SetTexture(lua_State* a1);
/// 0x089a99e7
/// lua ?
int LuaThunk_ButtonControl_SetTextureOffset(lua_State* a1);
/// 0x089a9bcb
int LuaToString_ButtonControl_SetTextureOffset(lua_State* a1);
/// 0x089a9c4c
/// lua ?
int LuaThunk_ButtonControl_SetTextureOffsetVal(lua_State* a1);
/// 0x089a9d68
int LuaToString_ButtonControl_SetTextureOffsetVal(lua_State* a1);
/// 0x089a9df1
/// lua ?
int LuaThunk_ButtonControl_SetText(lua_State* a1);
/// 0x089a9e86
int LuaToString_ButtonControl_SetText(lua_State* a1);
/// 0x089a9f0d
/// lua ?
int LuaThunk_ButtonControl_LocalizeAndSetText(lua_State* a1);
/// 0x089a9f21
int LuaToString_ButtonControl_LocalizeAndSetText(lua_State* a1);
/// 0x089a9f80
/// lua ?
int LuaThunk_ButtonControl_GetText(lua_State* a1);
/// 0x089aa012
int LuaToString_ButtonControl_GetText(lua_State* a1);
/// 0x089aa085
/// lua ?
int LuaThunk_ButtonControl_GetTextControl(lua_State* a1);
/// 0x089aa188
int LuaToString_ButtonControl_GetTextControl(lua_State* a1);
/// 0x089aa1fb
/// lua ?
int LuaThunk_ButtonControl_SetTextureHandle(lua_State* a1);
/// 0x089aa2e8
int LuaToString_ButtonControl_SetTextureHandle(lua_State* a1);
/// 0x089aa369
/// lua ?
int LuaThunk_ButtonControl_SetSize(lua_State* a1);
/// 0x089aa3ac
int LuaToString_ButtonControl_SetSize(lua_State* a1);
/// 0x089aa42d
/// lua ?
int LuaThunk_ButtonControl_SetSizeVal(lua_State* a1);
/// 0x089aa46e
int LuaToString_ButtonControl_SetSizeVal(lua_State* a1);
