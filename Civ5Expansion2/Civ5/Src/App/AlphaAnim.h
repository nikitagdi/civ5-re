/// origin guess
#pragma once
#include "Civ5/Src/App/UI/AnimControl.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FGXVector2;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;

/// size ?
/// vtable 0x0a2fe200
class AlphaAnim : public AnimControl {
public:
    /// 0x0899ec4c
    AlphaAnim();
    /// 0x0899ecbe
    /// deleting 0x0899ec96
    virtual ~AlphaAnim();
    /// 0x0899edbe
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x0899f12a
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0899f2c6
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0899c9ec
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x0899ed46
    virtual UNK_RET Initialize();
    /// 0x0899cb76
    virtual int PushSelf(lua_State* a1);
    /// 0x0899f064
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x0899f200
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x0899f39c
    virtual UNK_RET SetSizeVal(float a1, float a2);
    /// 0x0899efcc
    virtual UNK_RET UpdateAnim(float a1);
    /// 0x089a2cf4
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089a2cfa
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089a2d36
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089a2d78
    virtual UNK_RET vf_49(UNK_PARAMS);
    /// 0x0899f00c
    UNK_RET SetTexture(char const* a1);
    /// 0x0c07923c
    static UNK_DATA s_Cache[0x1c];
};
