/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "Civ5/Src/App/cvTunerListener.h"
#include "FirePlace/FireWorks/FFastVector.h"
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
/// vtable 0x0a2ff170
class PullDownControl : public ControlBase {
public:
    /// 0x089c4392
    PullDownControl();
    /// 0x089c4506
    /// deleting 0x089c44de
    virtual ~PullDownControl();
    /// 0x089c4b2c
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089c56e4
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089c5aaa
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089c4188
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089c5eb0
    virtual UNK_RET ExposeTree(FLua::Table& a1, bool a2);
    /// 0x089c5fa8
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089c5d1e
    virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x089c5c74
    virtual UNK_RET GetNodes(FFastVector<cvTunerListener::ITreeNode*, false, 0u, 0u, BaseVector<cvTunerListener::ITreeNode*, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x089c4600
    virtual UNK_RET Initialize();
    /// 0x089c48b4
    virtual UNK_RET ProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x089c4312
    virtual int PushSelf(lua_State* a1);
    /// 0x089c54e8
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089c58ae
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089c5f58
    virtual UNK_RET SetDisabled(bool a1);
    /// 0x089c7100
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089c7106
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089c712c
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089c4762
    UNK_RET MainButtonPressed(unsigned int a1, unsigned int a2);
    /// 0x089c4776
    UNK_RET ToggleOpen();
    /// 0x089c47de
    UNK_RET SubButtonPressed(unsigned int a1, unsigned int a2);
    /// 0x089c5102
    UNK_RET ClearEntries();
    /// 0x089c5128
    UNK_RET BuildEntry(char const* a1, FLua::Table a2);
    /// 0x089c53ac
    UNK_RET CalculateInternals();
    /// 0x089c5e0e
    static int LuaGetButton(lua_State* a1);
    /// 0x0c07fdd4
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089c6028
/// lua ?
int LuaThunk_PullDownControl_ClearEntries(lua_State* a1);
/// 0x089c60f5
int LuaToString_PullDownControl_ClearEntries(lua_State* a1);
/// 0x089c6168
/// lua ?
int LuaThunk_PullDownControl_BuildEntry(lua_State* a1);
/// 0x089c62c2
int LuaToString_PullDownControl_BuildEntry(lua_State* a1);
/// 0x089c6357
/// lua ?
int LuaThunk_PullDownControl_CalculateInternals(lua_State* a1);
/// 0x089c639a
int LuaToString_PullDownControl_CalculateInternals(lua_State* a1);
/// 0x089c640d
/// lua ?
int LuaThunk_PullDownControl_RegisterSelectionCallback(lua_State* a1);
/// 0x089c6534
int LuaToString_PullDownControl_RegisterSelectionCallback(lua_State* a1);
/// 0x089c65b5
/// lua ?
int LuaThunk_PullDownControl_SetDisabled(lua_State* a1);
/// 0x089c6652
int LuaToString_PullDownControl_SetDisabled(lua_State* a1);
/// 0x089c66d3
/// lua ?
int LuaThunk_PullDownControl_GetGrid(lua_State* a1);
/// 0x089c67bf
int LuaToString_PullDownControl_GetGrid(lua_State* a1);
/// 0x089c6832
/// lua ?
int LuaThunk_PullDownControl_GetScrollPanel(lua_State* a1);
/// 0x089c691e
int LuaToString_PullDownControl_GetScrollPanel(lua_State* a1);
/// 0x089c6991
/// lua ?
int LuaThunk_PullDownControl_GetStack(lua_State* a1);
/// 0x089c6a7d
int LuaToString_PullDownControl_GetStack(lua_State* a1);
/// 0x089c6af0
/// lua ?
int LuaThunk_PullDownControl_GetButton(lua_State* a1);
/// 0x089c6b04
int LuaToString_PullDownControl_GetButton(lua_State* a1);
