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
/// vtable 0x0a2fece0
class GridButtonControl : public ButtonControlBase {
public:
    /// 0x089b8fd6
    GridButtonControl();
    /// 0x089b9066
    /// deleting 0x089b903e
    virtual ~GridButtonControl();
    /// 0x089b91de
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089b95de
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089b9844
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089b8dcc
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089b9fa2
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089b99c4
    virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x089b99d8
    virtual UNK_RET GetNodes(FFastVector<cvTunerListener::ITreeNode*, false, 0u, 0u, BaseVector<cvTunerListener::ITreeNode*, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x089b90da
    virtual UNK_RET Initialize();
    /// 0x089b8f56
    virtual int PushSelf(lua_State* a1);
    /// 0x089b94b8
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089b971e
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089bab50
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x089babf2
    virtual UNK_RET SetSizeVal(float a1, float a2);
    /// 0x089b9984
    virtual UNK_RET SetVisState(unsigned int a1);
    /// 0x089bab4a
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089bab8a
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089babb0
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089b9168
    UNK_RET SetText(char const* a1);
    /// 0x089b9a82
    UNK_RET DoAutoSize();
    /// 0x089b9ba0
    static int LocalizeAndSetText(lua_State* a1);
    /// 0x0c07e130
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089ba01d
/// lua ?
int LuaThunk_GridButtonControl_SetTextureOffset(lua_State* a1);
/// 0x089ba1d1
int LuaToString_GridButtonControl_SetTextureOffset(lua_State* a1);
/// 0x089ba252
/// lua ?
int LuaThunk_GridButtonControl_SetTextureOffsetVal(lua_State* a1);
/// 0x089ba316
int LuaToString_GridButtonControl_SetTextureOffsetVal(lua_State* a1);
/// 0x089ba39f
/// lua ?
int LuaThunk_GridButtonControl_SetText(lua_State* a1);
/// 0x089ba434
int LuaToString_GridButtonControl_SetText(lua_State* a1);
/// 0x089ba4bb
/// lua ?
int LuaThunk_GridButtonControl_LocalizeAndSetText(lua_State* a1);
/// 0x089ba4cf
int LuaToString_GridButtonControl_LocalizeAndSetText(lua_State* a1);
/// 0x089ba52e
/// lua ?
int LuaThunk_GridButtonControl_GetText(lua_State* a1);
/// 0x089ba5c0
int LuaToString_GridButtonControl_GetText(lua_State* a1);
/// 0x089ba633
/// lua ?
int LuaThunk_GridButtonControl_GetTextControl(lua_State* a1);
/// 0x089ba736
int LuaToString_GridButtonControl_GetTextControl(lua_State* a1);
/// 0x089ba7a9
/// lua ?
int LuaThunk_GridButtonControl_DoAutoSize(lua_State* a1);
/// 0x089ba81e
int LuaToString_GridButtonControl_DoAutoSize(lua_State* a1);
