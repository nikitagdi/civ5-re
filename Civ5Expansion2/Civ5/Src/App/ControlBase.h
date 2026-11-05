/// origin guess
#pragma once
#include "Civ5/Src/App/cvTunerListener.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include "FirePlace/FireWorks/FString.h"
#include <list>
#include <lua.h>
#include <memory>

class ControlBase;
class FGXVector2;
class FGXVector3;
class FGXVector4;
class FXmlElement;
class LinearEventDispatcher;
class LocalMachineContainer;
class LuaContext;
class NullEventClass;
class SystemUpdateUIData;
class UIRenderBuffer;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
enum UIAABB : int;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size 0x8c
/// vtable 0x0a2f9030
class ControlBase : public cvTunerListener::ITreeNode {
public:
    class FlagNode;
    /// size ?
    /// vtable 0x0a2f9110
    class FlagNode : public cvTunerListener::ITreeNode {
    public:
        /// absent
        virtual ~FlagNode();
        /// 0x08692614
        virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1);
        /// 0x08696c28
        virtual UNK_RET GetNodes(FFastVector<cvTunerListener::ITreeNode*, false, 0u, 0u, BaseVector<cvTunerListener::ITreeNode*, false>::FDefaultFastVectorAllocator>& a1);
        /// 0x08696bfc
        virtual UNK_RET GetText(cvTunerListener::ITreeNode::TextWriter& a1);
    };
    /// 0x0868e75a
    ControlBase();
    /// 0x0868e89e
    /// deleting 0x0868e876
    virtual ~ControlBase();
    /// 0x0869231c
    virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x08692a74
    virtual UNK_RET GetNodes(FFastVector<cvTunerListener::ITreeNode*, false, 0u, 0u, BaseVector<cvTunerListener::ITreeNode*, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x08691f06
    virtual UNK_RET GetText(cvTunerListener::ITreeNode::TextWriter& a1);
    /// 0x0868ea88
    virtual UNK_RET Initialize();
    /// 0x0868ee14
    virtual UNK_RET ProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x0868f304
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x0868f606
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0868f7e2
    virtual UNK_RET PerspectiveRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x0868f1de
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x0868f3c4
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0868f8a2
    virtual UNK_RET PerspectiveRender(unsigned int a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x0869181c
    virtual UNK_RET TestHit(FGXVector2 const& a1, FGXVector2 const& a2);
    /// 0x08691888
    virtual UNK_RET ClipTestHit(FGXVector2 const& a1, FGXVector2 const& a2, FGXVector4 const& a3);
    /// 0x08691538
    virtual UNK_RET AddChild(ControlBase* a1);
    /// 0x08691590
    virtual UNK_RET AddChildAtIndex(ControlBase* a1, int a2);
    /// 0x0868f962
    virtual UNK_RET Update(float a1);
    /// 0x0868fa12
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x08696b0c
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x0868ec88
    virtual UNK_RET ProcessAnchoring();
    /// 0x08691916
    virtual UNK_RET SetHide(bool a1);
    /// 0x0868f9c0
    virtual UNK_RET BranchResetAnimation();
    /// 0x08696b10
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x08696b12
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x08691eb4
    virtual UNK_RET GetChildMemSize();
    /// 0x08692bae
    virtual UNK_RET ExposeTree(FLua::Table& a1, bool a2);
    /// 0x08692e58
    virtual UNK_RET HasFocus() const;
    /// 0x08692e5c
    virtual UNK_RET LostFocus();
    /// 0x08692e5e
    virtual UNK_RET TestLostFocus();
    /// 0x08692e60
    virtual UNK_RET TakeFocus();
    /// 0x08692e62
    virtual UNK_RET GetFocusStop() const;
    /// 0x0868e510
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x0868e6da
    virtual int PushSelf(lua_State* a1);
    /// 0x08696b38
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x0868f01a
    virtual UNK_RET SetDisabled(bool a1);
    /// 0x0868f15e
    virtual UNK_RET GrantedMouseOver();
    /// 0x0868f19e
    virtual UNK_RET LostMouseOver();
    /// 0x08691acc
    virtual UNK_RET CalculateVisibilityBox();
    /// 0x08691958
    virtual UNK_RET SetSizeVal(float a1, float a2);
    /// 0x08696b7a
    virtual UNK_RET vf_40(UNK_PARAMS);
    /// 0x08696b8a
    virtual UNK_RET vf_41(UNK_PARAMS);
    /// 0x08691aae
    virtual UNK_RET GetSize();
    /// 0x08696b92
    virtual UNK_RET vf_43(UNK_PARAMS);
    /// 0x08696b9a
    virtual UNK_RET vf_44(UNK_PARAMS);
    /// 0x08696ba2
    virtual UNK_RET vf_45(UNK_PARAMS);
    /// 0x08696baa
    virtual UNK_RET vf_46(UNK_PARAMS);
    /// 0x08696bb2
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x0868e6b8
    UNK_RET GetID() const;
    /// 0x0868ea10
    UNK_RET OnSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0868eb76
    UNK_RET SetID(char const* a1);
    /// 0x0868ebf0
    UNK_RET SetSize(FGXVector2 a1);
    /// 0x0868ec2e
    UNK_RET ReprocessAnchoring();
    /// 0x0868ee74
    UNK_RET BaseProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x0868ef86
    UNK_RET DoConsumeMouse(unsigned int a1, FGXVector2 const& a2);
    /// 0x0868f08a
    UNK_RET ShowMouseOverChildren(bool a1);
    /// 0x086902c0
    UNK_RET ExtractCoords(char const* a1, FGXVector2& a2);
    /// 0x086905e4
    UNK_RET LoadAnchorFlags(char const* a1);
    /// 0x0869091c
    UNK_RET LoadAnchorSideFlags(char const* a1);
    /// 0x08690bfa
    UNK_RET SetToolTipString(char const* a1);
    /// 0x08690caa
    UNK_RET AddMouseOverChild(ControlBase* a1, bool a2);
    /// 0x08690d32
    UNK_RET EnableToolTip(bool a1);
    /// 0x08690d56
    UNK_RET GetToolTipString();
    /// 0x08690d68
    /// lua ?
    static int lSetToolTipString(lua_State* a1);
    /// 0x08690f58
    UNK_RET SetToolTipType(char const* a1);
    /// 0x08690fda
    /// lua ?
    static int lSetToolTipType(lua_State* a1);
    /// 0x08691064
    UNK_RET ExtractStrings(char const* a1, FStringA& a2, FStringA& a3);
    /// 0x086911d4
    UNK_RET BuildFromStyle(FXmlElement const& a1, LuaContext* a2);
    /// 0x0869162e
    UNK_RET ReleaseChild(ControlBase* a1);
    /// 0x086916d0
    UNK_RET ReleaseAllChildren();
    /// 0x08691746
    UNK_RET ChangeParent(ControlBase* a1);
    /// 0x0869178a
    UNK_RET ExcludeChild(ControlBase* a1);
    /// 0x08691934
    UNK_RET SetNoClip(bool a1);
    /// 0x086919c2
    static int GetOffsetVal(lua_State* a1);
    /// 0x08691a38
    static int GetSizeVal(lua_State* a1);
    /// 0x08691bec
    UNK_RET SetOffset(FGXVector2 a1);
    /// 0x08691cac
    UNK_RET SetOffsetVal(float a1, float a2);
    /// 0x08691d64
    UNK_RET SetAnchor(char const* a1);
    /// 0x08691d9a
    UNK_RET SortChildren(FLua::Function a1);
    /// 0x08692aec
    UNK_RET SetUpToolTip();
    /// 0x08692c7c
    static int LocalizeAndSetToolTip(lua_State* a1);
    /// 0x08692e68
    UNK_RET FindCurrentFocus();
    /// 0x08692ef4
    UNK_RET FindNextFocus(ControlBase*& a1, int a2, bool a3);
    /// 0x08693010
    UNK_RET FindNextFocus(int a1, bool a2);
    /// 0x0869309e
    UNK_RET SetModal(bool a1);
    /// 0x0a3c9730
    static UNK_DATA NO_COLOR[0x10];
    /// 0x0a3c9728
    static UNK_DATA NO_OFFSET[0x8];
    /// 0x0a3c96f0
    static UNK_DATA s_Cache[0x1c];

    char unk_4[0x88];                                  ///< +0x4
};

/// 0x086930c2
/// lua ?
int LuaThunk_ControlBase_SetOffset(lua_State* a1);
/// 0x086932ab
int LuaToString_ControlBase_SetOffset(lua_State* a1);
/// 0x0869332c
/// lua ?
int LuaThunk_ControlBase_SetOffsetX(lua_State* a1);
/// 0x08693421
int LuaToString_ControlBase_SetOffsetX(lua_State* a1);
/// 0x086934a2
/// lua ?
int LuaThunk_ControlBase_SetOffsetY(lua_State* a1);
/// 0x086934e5
int LuaToString_ControlBase_SetOffsetY(lua_State* a1);
/// 0x08693566
/// lua ?
int LuaThunk_ControlBase_SetOffsetVal(lua_State* a1);
/// 0x08693687
int LuaToString_ControlBase_SetOffsetVal(lua_State* a1);
/// 0x08693710
/// lua ?
int LuaThunk_ControlBase_GetOffsetX(lua_State* a1);
/// 0x086937ec
int LuaToString_ControlBase_GetOffsetX(lua_State* a1);
/// 0x0869385f
/// lua ?
int LuaThunk_ControlBase_GetOffsetY(lua_State* a1);
/// 0x0869389e
int LuaToString_ControlBase_GetOffsetY(lua_State* a1);
/// 0x08693911
/// lua ?
int LuaThunk_ControlBase_GetOffsetVal(lua_State* a1);
/// 0x08693925
int LuaToString_ControlBase_GetOffsetVal(lua_State* a1);
/// 0x08693984
/// lua ?
int LuaThunk_ControlBase_SetAnchor(lua_State* a1);
/// 0x08693a28
int LuaToString_ControlBase_SetAnchor(lua_State* a1);
/// 0x08693aaf
/// lua ?
int LuaThunk_ControlBase_SetHide(lua_State* a1);
/// 0x08693b9e
int LuaToString_ControlBase_SetHide(lua_State* a1);
/// 0x08693c1f
/// lua ?
int LuaThunk_ControlBase_SetNoClip(lua_State* a1);
/// 0x08693c62
int LuaToString_ControlBase_SetNoClip(lua_State* a1);
/// 0x08693ce3
/// lua ?
int LuaThunk_ControlBase_IsHidden(lua_State* a1);
/// 0x08693dc4
int LuaToString_ControlBase_IsHidden(lua_State* a1);
/// 0x08693e37
/// lua ?
int LuaThunk_ControlBase_GetNumChildren(lua_State* a1);
/// 0x08693ed8
int LuaToString_ControlBase_GetNumChildren(lua_State* a1);
/// 0x08693f4b
/// lua ?
int LuaThunk_ControlBase_ReprocessAnchoring(lua_State* a1);
/// 0x08694018
int LuaToString_ControlBase_ReprocessAnchoring(lua_State* a1);
/// 0x0869408b
/// lua ?
int LuaThunk_ControlBase_GetSize(lua_State* a1);
/// 0x08694218
int LuaToString_ControlBase_GetSize(lua_State* a1);
/// 0x0869428b
/// lua ?
int LuaThunk_ControlBase_GetSizeX(lua_State* a1);
/// 0x086942ca
int LuaToString_ControlBase_GetSizeX(lua_State* a1);
/// 0x0869433d
/// lua ?
int LuaThunk_ControlBase_GetSizeY(lua_State* a1);
/// 0x0869437c
int LuaToString_ControlBase_GetSizeY(lua_State* a1);
/// 0x086943ef
/// lua ?
int LuaThunk_ControlBase_GetSizeVal(lua_State* a1);
/// 0x08694403
int LuaToString_ControlBase_GetSizeVal(lua_State* a1);
/// 0x08694462
/// lua ?
int LuaThunk_ControlBase_SetSize(lua_State* a1);
/// 0x086944a5
int LuaToString_ControlBase_SetSize(lua_State* a1);
/// 0x08694526
/// lua ?
int LuaThunk_ControlBase_SetSizeX(lua_State* a1);
/// 0x08694569
int LuaToString_ControlBase_SetSizeX(lua_State* a1);
/// 0x086945ea
/// lua ?
int LuaThunk_ControlBase_SetSizeY(lua_State* a1);
/// 0x0869462d
int LuaToString_ControlBase_SetSizeY(lua_State* a1);
/// 0x086946ae
/// lua ?
int LuaThunk_ControlBase_SetSizeVal(lua_State* a1);
/// 0x086946ef
int LuaToString_ControlBase_SetSizeVal(lua_State* a1);
/// 0x08694778
/// lua ?
int LuaThunk_ControlBase_SetColor(lua_State* a1);
/// 0x086949f6
int LuaToString_ControlBase_SetColor(lua_State* a1);
/// 0x08694a77
/// lua ?
int LuaThunk_ControlBase_SetAlpha(lua_State* a1);
/// 0x08694ab8
int LuaToString_ControlBase_SetAlpha(lua_State* a1);
/// 0x08694b39
/// lua ?
int LuaThunk_ControlBase_GetAlpha(lua_State* a1);
/// 0x08694b78
int LuaToString_ControlBase_GetAlpha(lua_State* a1);
/// 0x08694beb
/// lua ?
int LuaThunk_ControlBase_SortChildren(lua_State* a1);
/// 0x08694d69
int LuaToString_ControlBase_SortChildren(lua_State* a1);
/// 0x08694dea
/// lua ?
int LuaThunk_ControlBase_BranchResetAnimation(lua_State* a1);
/// 0x08694e2b
int LuaToString_ControlBase_BranchResetAnimation(lua_State* a1);
/// 0x08694e9e
/// lua ?
int LuaThunk_ControlBase_GetID(lua_State* a1);
/// 0x08694f2e
int LuaToString_ControlBase_GetID(lua_State* a1);
/// 0x08694fa1
/// lua ?
int LuaThunk_ControlBase_SetDisabled(lua_State* a1);
/// 0x08694fe2
int LuaToString_ControlBase_SetDisabled(lua_State* a1);
/// 0x08695063
/// lua ?
int LuaThunk_ControlBase_IsDisabled(lua_State* a1);
/// 0x0869513f
int LuaToString_ControlBase_IsDisabled(lua_State* a1);
/// 0x086951b2
/// lua ?
int LuaThunk_ControlBase_SetModal(lua_State* a1);
/// 0x086951f5
int LuaToString_ControlBase_SetModal(lua_State* a1);
/// 0x08695276
/// lua ?
int LuaThunk_ControlBase_IsModal(lua_State* a1);
/// 0x086952b7
int LuaToString_ControlBase_IsModal(lua_State* a1);
/// 0x0869532a
/// lua ?
int LuaThunk_ControlBase_CalculateVisibilityBox(lua_State* a1);
/// 0x0869536b
int LuaToString_ControlBase_CalculateVisibilityBox(lua_State* a1);
/// 0x086953de
/// lua ?
int LuaThunk_ControlBase_SetToolTipCallback(lua_State* a1);
/// 0x08695421
int LuaToString_ControlBase_SetToolTipCallback(lua_State* a1);
/// 0x086954a2
/// lua ?
int LuaThunk_ControlBase_IsToolTipEnabled(lua_State* a1);
/// 0x086954e3
int LuaToString_ControlBase_IsToolTipEnabled(lua_State* a1);
/// 0x08695556
/// lua ?
int LuaThunk_ControlBase_EnableToolTip(lua_State* a1);
/// 0x08695599
int LuaToString_ControlBase_EnableToolTip(lua_State* a1);
/// 0x0869561a
/// lua ?
int LuaThunk_ControlBase_SetToolTipType(lua_State* a1);
/// 0x0869562e
int LuaToString_ControlBase_SetToolTipType(lua_State* a1);
/// 0x0869568d
/// lua ?
int LuaThunk_ControlBase_SetToolTipString(lua_State* a1);
/// 0x086956a1
int LuaToString_ControlBase_SetToolTipString(lua_State* a1);
/// 0x08695700
/// lua ?
int LuaThunk_ControlBase_LocalizeAndSetToolTip(lua_State* a1);
/// 0x08695714
int LuaToString_ControlBase_LocalizeAndSetToolTip(lua_State* a1);
/// 0x08695773
/// lua ?
int LuaThunk_ControlBase_GetToolTipString(lua_State* a1);
/// 0x08695802
int LuaToString_ControlBase_GetToolTipString(lua_State* a1);
/// 0x08695875
/// lua ?
int LuaThunk_ControlBase_ChangeParent(lua_State* a1);
/// 0x0869595a
int LuaToString_ControlBase_ChangeParent(lua_State* a1);
/// 0x086959e1
/// lua ?
int LuaThunk_ControlBase_HasMouseOver(lua_State* a1);
/// 0x08695a22
int LuaToString_ControlBase_HasMouseOver(lua_State* a1);
/// 0x08695a95
/// lua ?
int LuaThunk_ControlBase_ReleaseChild(lua_State* a1);
/// 0x08695ad8
int LuaToString_ControlBase_ReleaseChild(lua_State* a1);
/// 0x08695b5f
/// lua ?
int LuaThunk_ControlBase_SetConsumeMouseOver(lua_State* a1);
/// 0x08695ba2
int LuaToString_ControlBase_SetConsumeMouseOver(lua_State* a1);
/// 0x08695c23
/// lua ?
int LuaThunk_ControlBase_SetConsumeMouseButton(lua_State* a1);
/// 0x08695c66
int LuaToString_ControlBase_SetConsumeMouseButton(lua_State* a1);
/// 0x08695ce7
/// lua ?
int LuaThunk_ControlBase_SetConsumeMouseWheel(lua_State* a1);
/// 0x08695d2a
int LuaToString_ControlBase_SetConsumeMouseWheel(lua_State* a1);
/// 0x08695dab
/// lua ?
int LuaThunk_ControlBase_GetConsumeMouseOver(lua_State* a1);
/// 0x08695dec
int LuaToString_ControlBase_GetConsumeMouseOver(lua_State* a1);
/// 0x08695e5f
/// lua ?
int LuaThunk_ControlBase_GetConsumeMouseButton(lua_State* a1);
/// 0x08695ea0
int LuaToString_ControlBase_GetConsumeMouseButton(lua_State* a1);
/// 0x08695f13
/// lua ?
int LuaThunk_ControlBase_GetConsumeMouseWheel(lua_State* a1);
/// 0x08695f54
int LuaToString_ControlBase_GetConsumeMouseWheel(lua_State* a1);
/// 0x08695fc7
/// lua ?
int LuaThunk_ControlBase_DestroyAllChildren(lua_State* a1);
/// 0x0869600a
int LuaToString_ControlBase_DestroyAllChildren(lua_State* a1);
