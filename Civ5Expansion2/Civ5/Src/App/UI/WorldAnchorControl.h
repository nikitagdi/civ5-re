/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FGXVector2;
class FGXVector3;
class FXmlElement;
class FrameGlobalsData;
class LinearEventDispatcher;
class LocalMachineContainer;
class LuaContext;
class NullEventClass;
class UIRenderBuffer;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
enum UIAABB : int;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2ffa00
class WorldAnchorControl : public ControlBase {
public:
    /// 0x089d9c7a
    WorldAnchorControl();
    /// 0x089d9d46
    /// deleting 0x089d9d1e
    virtual ~WorldAnchorControl();
    /// 0x089da6c6
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089db7c0
    virtual UNK_RET CalculateVisibilityBox();
    /// 0x089daa46
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089dac44
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089d9a70
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089da41e
    virtual UNK_RET Initialize();
    /// 0x089da8a6
    virtual UNK_RET ProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x089d9bfa
    virtual int PushSelf(lua_State* a1);
    /// 0x089da90a
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089dab36
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089db752
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089db758
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089db77e
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089d9daa
    UNK_RET OnCameraViewChanged(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089d9dbe
    UNK_RET StartUp();
    /// 0x089d9e7c
    UNK_RET RecalcPosition();
    /// 0x089da370
    UNK_RET LocalSetOffsetVal(float a1, float a2);
    /// 0x089da516
    UNK_RET SetWorldPosition(FGXVector3 a1);
    /// 0x089da54a
    UNK_RET SetWorldPositionVal(float a1, float a2, float a3);
    /// 0x089da58c
    UNK_RET SetHexPosition(int a1, int a2);
    /// 0x0c083240
    static UNK_DATA m_Viewport[0x18];
    /// 0x0c083200
    static UNK_DATA m_mWorld[0x40];
    /// 0x0c0831e4
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089dad19
/// lua ?
int LuaThunk_WorldAnchorControl_SetWorldPosition(lua_State* a1);
/// 0x089daf1c
int LuaToString_WorldAnchorControl_SetWorldPosition(lua_State* a1);
/// 0x089daf9d
/// lua ?
int LuaThunk_WorldAnchorControl_SetWorldPositionVal(lua_State* a1);
/// 0x089db068
int LuaToString_WorldAnchorControl_SetWorldPositionVal(lua_State* a1);
/// 0x089db0f9
/// lua ?
int LuaThunk_WorldAnchorControl_SetHexPosition(lua_State* a1);
/// 0x089db19e
int LuaToString_WorldAnchorControl_SetHexPosition(lua_State* a1);
