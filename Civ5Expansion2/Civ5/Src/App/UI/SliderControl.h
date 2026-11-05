/// origin guess
#pragma once
#include "Civ5/Src/App/ButtonControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <list>
#include <lua.h>
#include <memory>

class ButtonUpData;
class ControlBase;
class FGXVector2;
class FGXVector4;
class FXmlElement;
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
/// vtable 0x0a2ff4e0
class SliderControl : public ButtonControlBase {
public:
    /// 0x089cd9b6
    SliderControl();
    /// 0x089cdae4
    /// deleting 0x089cdabc
    virtual ~SliderControl();
    /// 0x089cde20
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089ce998
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089cebe2
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089cd7ac
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089ced54
    virtual UNK_RET GetCompoundChildren(std::list<ControlBase const*, std::allocator<ControlBase const*> >& a1) const;
    /// 0x089cdbdc
    virtual UNK_RET Initialize();
    /// 0x089ce276
    virtual UNK_RET ProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x089cd936
    virtual int PushSelf(lua_State* a1);
    /// 0x089ce880
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089ceaca
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089cf8be
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x089ce164
    virtual UNK_RET SetSizeVal(float a1, float a2);
    /// 0x089ced14
    virtual UNK_RET SetVisState(unsigned int a1);
    /// 0x089ce778
    virtual UNK_RET Update(float a1);
    /// 0x089cf8b8
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089cf8f8
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089cf91e
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089cdb9c
    UNK_RET ForcedButtonUp(EventTemplate<Event_Int2Type<65>, ButtonUpData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x089cdca4
    UNK_RET SetValue(float a1);
    /// 0x089cdd4e
    UNK_RET SetAndCall(float a1);
    /// 0x089ce220
    UNK_RET SetLength(float a1);
    /// 0x089ce7d4
    UNK_RET SetShuttleRatio(float a1);
    /// 0x0c081354
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089cedcb
/// lua ?
int LuaThunk_SliderControl_RegisterSliderCallback(lua_State* a1);
/// 0x089ceeed
int LuaToString_SliderControl_RegisterSliderCallback(lua_State* a1);
/// 0x089cef6e
/// lua ?
int LuaThunk_SliderControl_GetValue(lua_State* a1);
/// 0x089ceff6
int LuaToString_SliderControl_GetValue(lua_State* a1);
/// 0x089cf069
/// lua ?
int LuaThunk_SliderControl_SetValue(lua_State* a1);
/// 0x089cf159
int LuaToString_SliderControl_SetValue(lua_State* a1);
/// 0x089cf1da
/// lua ?
int LuaThunk_SliderControl_SetAndCall(lua_State* a1);
/// 0x089cf21d
int LuaToString_SliderControl_SetAndCall(lua_State* a1);
