/// origin guess
#pragma once
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class ControlBase;
class FXmlElement;
class GridControl;
class LinearEventDispatcher;
class LocalMachineContainer;
class LuaContext;
class NullEventClass;
class SystemUpdateUIData;
class TextControl;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class ToolTipManager {
public:
    /// 0x086c7078
    ToolTipManager();
    /// 0x086c72e2
    ~ToolTipManager();
    /// 0x086c715c
    UNK_RET HandleSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x086c736a
    UNK_RET StartUp();
    /// 0x086c742c
    UNK_RET ShutDown();
    /// 0x086c74da
    UNK_RET RegisterBasicControls(ControlBase* a1, ControlBase* a2, TextControl* a3, GridControl* a4);
    /// 0x086c7546
    UNK_RET ProcessMouseMove();
    /// 0x086c763a
    UNK_RET CheckEdgesInternal();
    /// 0x086c77c0
    UNK_RET RequestToolTip(ControlBase* a1);
    /// 0x086c77ee
    UNK_RET RefreshToolTip(ControlBase* a1);
    /// 0x086c781c
    UNK_RET SetToolTipType(unsigned int a1);
    /// 0x086c788a
    UNK_RET SetTypeInternal(unsigned int a1);
    /// 0x086c7908
    UNK_RET SetToolTipTypeByName(char const* a1);
    /// 0x086c7962
    UNK_RET GetTypeControlTable(char const* a1, FLua::Table a2);
    /// 0x086c7a08
    UNK_RET SetToolTipString(char const* a1);
    /// 0x086c7a62
    UNK_RET RegisterToolTipType(FXmlElement const& a1, LuaContext* a2);
    /// 0x086c7d06
    UNK_RET ControlClosing(ControlBase* a1);
};

/// 0x086c7d6a
/// lua ?
int LuaThunk_ToolTipManager_SetToolTipString(lua_State* a1);
/// 0x086c7e56
int LuaToString_ToolTipManager_SetToolTipString(lua_State* a1);
/// 0x086c7edd
/// lua ?
int LuaThunk_ToolTipManager_RegisterBasicControls(lua_State* a1);
/// 0x086c83ed
int LuaToString_ToolTipManager_RegisterBasicControls(lua_State* a1);
/// 0x086c8498
/// lua ?
int LuaThunk_ToolTipManager_SetToolTipType(lua_State* a1);
/// 0x086c84db
int LuaToString_ToolTipManager_SetToolTipType(lua_State* a1);
/// 0x086c8562
/// lua ?
int LuaThunk_ToolTipManager_GetTypeControlTable(lua_State* a1);
/// 0x086c8641
int LuaToString_ToolTipManager_GetTypeControlTable(lua_State* a1);
