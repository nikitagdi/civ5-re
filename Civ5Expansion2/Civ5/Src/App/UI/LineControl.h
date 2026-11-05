/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FGXVector2;
class FGXVector4;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;
namespace FireGrafix {
class FGXSurface;
}

/// size ?
/// vtable 0x0a2fc450
class LineControl : public ControlBase {
public:
    /// 0x08752c6a
    LineControl();
    /// 0x08752d32
    /// deleting 0x08752d0a
    virtual ~LineControl();
    /// 0x087531d6
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x087535ec
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x08753714
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x08752a60
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x08752df2
    virtual UNK_RET Initialize();
    /// 0x08752bea
    virtual int PushSelf(lua_State* a1);
    /// 0x08753440
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x08753536
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x08754318
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x08754312
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x0875433a
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x08754360
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x08752e54
    UNK_RET InitVB();
    /// 0x08753148
    UNK_RET CreateSurface(FireGrafix::FGXSurface** a1);
    /// 0x0a3e2920
    static UNK_DATA s_Cache[0x1c];
};

/// 0x087537fa
/// lua ?
int LuaThunk_LineControl_SetStartVal(lua_State* a1);
/// 0x08753916
int LuaToString_LineControl_SetStartVal(lua_State* a1);
/// 0x0875399f
/// lua ?
int LuaThunk_LineControl_SetEndVal(lua_State* a1);
/// 0x087539e2
int LuaToString_LineControl_SetEndVal(lua_State* a1);
/// 0x08753a6b
/// lua ?
int LuaThunk_LineControl_SetColorVal(lua_State* a1);
/// 0x08753b5e
int LuaToString_LineControl_SetColorVal(lua_State* a1);
/// 0x08753bf7
/// lua ?
int LuaThunk_LineControl_SetWidth(lua_State* a1);
/// 0x08753c86
int LuaToString_LineControl_SetWidth(lua_State* a1);
