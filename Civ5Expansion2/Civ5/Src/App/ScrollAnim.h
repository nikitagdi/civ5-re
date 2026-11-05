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
/// vtable 0x0a2fe2e0
class ScrollAnim : public AnimControl {
public:
    /// 0x089a28fc
    /// deleting 0x089a294e
    virtual ~ScrollAnim();
    /// 0x0899dbb8
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x0899df4e
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0899e0ce
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0899d00a
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x0899db60
    virtual UNK_RET Initialize();
    /// 0x0899d194
    virtual int PushSelf(lua_State* a1);
    /// 0x0899de96
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x0899e016
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x0899dd4c
    virtual UNK_RET UpdateAnim(float a1);
    /// 0x089a29a8
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089a29ae
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089a29d4
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089a2a16
    virtual UNK_RET vf_49(UNK_PARAMS);
    /// 0x0899dd76
    UNK_RET SetTexture(char const* a1);
    /// 0x0899ddce
    UNK_RET SetMask(char const* a1);
    /// 0x0899de26
    UNK_RET SetTextureAndMask(char const* a1, char const* a2);
    /// 0x0c079290
    static UNK_DATA s_Cache[0x1c];
};
