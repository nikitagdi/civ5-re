/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "Civ5/Src/App/cvTunerListener.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class ButtonUpData;
class DragCameraData;
class FGXVector2;
class FXmlElement;
class LinearEventDispatcher;
class LocalMachineContainer;
class LuaContext;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

// A bound native method: m_pObject is the `this` and m_pfnMethod/m_iAdjust the Itanium member-function pointer.
// CallDelegates invokes it as (*method)(object + m_iAdjust, void1).
/// size 0xc
struct ButtonNativeDelegate {
    void* m_pObject;   ///< +0x0
    void* m_pfnMethod; ///< +0x4
    int m_iAdjust;     ///< +0x8
};

/// size ?
/// vtable 0x0a2fe740
class ButtonControlBase : public ControlBase {
public:
    class UIAudioEvent;
    /// 0x089aba84
    ButtonControlBase();
    /// 0x089abb64
    /// deleting 0x089abb3c
    virtual ~ButtonControlBase();
    /// 0x089abcee
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089ac8f4
    virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x089ac69a
    virtual UNK_RET GrantedMouseOver();
    /// 0x089abcb6
    virtual UNK_RET Initialize();
    /// 0x089ac784
    virtual UNK_RET LostMouseOver();
    /// 0x089abf18
    virtual UNK_RET ProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x089ac83e
    virtual UNK_RET SetDisabled(bool a1);
    /// 0x089ac3f6
    virtual UNK_RET Update(float a1);
    /// 0x089abfea
    virtual UNK_RET ProcessMouse(unsigned int a1, unsigned int a2, long a3, bool a4, FGXVector2 const& a5);
    /// 0x089ac482
    virtual UNK_RET DoClick(unsigned int a1);
    /// 0x089ac472
    virtual UNK_RET SetVisState(unsigned int a1);
    /// 0x089abc4a
    UNK_RET ForcedButtonUp(EventTemplate<Event_Int2Type<65>, ButtonUpData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089ac452
    UNK_RET OnDragCamera(EventTemplate<Event_Int2Type<100>, DragCameraData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089ac504
    UNK_RET CallDelegates(unsigned int a1);
    /// 0x089ac658
    UNK_RET PlayAudioEvent(ButtonControlBase::UIAudioEvent a1);
    /// 0x089ac7de
    UNK_RET RegisterCallback(unsigned int a1, FLua::Function a2);
    /// 0x089ac812
    UNK_RET ClearCallback(unsigned int a1);

    char unk_8c[0x38];                                 ///< +0x8c
    // Indexed by the callback kind of CallDelegates (RegisterCallback keeps the Lua side in +0x8c).
    ButtonNativeDelegate m_aNativeDelegates[7];        ///< +0xc4
};

/// 0x089ac9f0
/// lua ?
int LuaThunk_ButtonControlBase_RegisterCallback(lua_State* a1);
/// 0x089acb91
int LuaToString_ButtonControlBase_RegisterCallback(lua_State* a1);
/// 0x089acc20
/// lua ?
int LuaThunk_ButtonControlBase_ClearCallback(lua_State* a1);
/// 0x089acd06
int LuaToString_ButtonControlBase_ClearCallback(lua_State* a1);
/// 0x089acd87
/// lua ?
int LuaThunk_ButtonControlBase_GetVoid1(lua_State* a1);
/// 0x089ace60
int LuaToString_ButtonControlBase_GetVoid1(lua_State* a1);
/// 0x089aced3
/// lua ?
int LuaThunk_ButtonControlBase_GetVoid2(lua_State* a1);
/// 0x089acf14
int LuaToString_ButtonControlBase_GetVoid2(lua_State* a1);
/// 0x089acf87
/// lua ?
int LuaThunk_ButtonControlBase_SetVoid1(lua_State* a1);
/// 0x089ad06b
int LuaToString_ButtonControlBase_SetVoid1(lua_State* a1);
/// 0x089ad0ec
/// lua ?
int LuaThunk_ButtonControlBase_SetVoid2(lua_State* a1);
/// 0x089ad12f
int LuaToString_ButtonControlBase_SetVoid2(lua_State* a1);
/// 0x089ad1b0
/// lua ?
int LuaThunk_ButtonControlBase_SetVoids(lua_State* a1);
/// 0x089ad251
int LuaToString_ButtonControlBase_SetVoids(lua_State* a1);
/// 0x089ad2da
/// lua ?
int LuaThunk_ButtonControlBase_IsTrackingLeftMouseButton(lua_State* a1);
/// 0x089ad3b6
int LuaToString_ButtonControlBase_IsTrackingLeftMouseButton(lua_State* a1);
/// 0x089ad429
/// lua ?
int LuaThunk_ButtonControlBase_IsTrackingRightMouseButton(lua_State* a1);
/// 0x089ad46a
int LuaToString_ButtonControlBase_IsTrackingRightMouseButton(lua_State* a1);
/// 0x089ad4dd
/// lua ?
int LuaThunk_ButtonControlBase_IsTrackingMiddleMouseButton(lua_State* a1);
/// 0x089ad51e
int LuaToString_ButtonControlBase_IsTrackingMiddleMouseButton(lua_State* a1);
