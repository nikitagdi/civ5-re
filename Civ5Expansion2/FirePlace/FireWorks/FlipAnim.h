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
/// vtable 0x0a2fe060
class FlipAnim : public AnimControl {
public:
    /// 0x0899e196
    FlipAnim();
    /// 0x089a2a72
    /// deleting 0x089a2ac4
    virtual ~FlipAnim();
    /// 0x0899e2d4
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x0899e6ba
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0899e83a
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0899ce00
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x0899e250
    virtual UNK_RET Initialize();
    /// 0x0899cf8a
    virtual int PushSelf(lua_State* a1);
    /// 0x0899e602
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x0899e782
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x0899e5bc
    virtual UNK_RET UpdateAnim(float a1);
    /// 0x089a2b1e
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089a2b24
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089a2b4a
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089a2b8c
    virtual UNK_RET vf_49(UNK_PARAMS);
    /// 0x0899e546
    UNK_RET SetFrame(int a1);
    /// 0x0c079274
    static UNK_DATA s_Cache[0x1c];
};
