/// origin guess
#pragma once
#include "Civ5/Src/App/UI/CheckBoxControl.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FXmlElement;
class LinearEventDispatcher;
class LocalMachineContainer;
class LuaContext;
class NullEventClass;
class RadioSelectionChangedData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2ff290
class RadioButtonControl : public CheckBoxControl {
public:
    /// 0x089c7fbe
    RadioButtonControl();
    /// 0x089c8024
    /// deleting 0x089c7ffc
    virtual ~RadioButtonControl();
    /// 0x089c81b0
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089c810c
    virtual UNK_RET DoClick(unsigned int a1);
    /// 0x089c7db4
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089c80ca
    virtual UNK_RET Initialize();
    /// 0x089c7f3e
    virtual int PushSelf(lua_State* a1);
    /// 0x089c814a
    virtual UNK_RET SetCheck(bool a1);
    /// 0x089c89bc
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089c89c2
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089c89e8
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089c8088
    UNK_RET OnRadioSelectionChanged(EventTemplate<Event_Int2Type<38>, RadioSelectionChangedData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0c080504
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089c839d
/// lua ?
int LuaThunk_RadioButtonControl_SetCheck(lua_State* a1);
/// 0x089c8683
int LuaToString_RadioButtonControl_SetCheck(lua_State* a1);
