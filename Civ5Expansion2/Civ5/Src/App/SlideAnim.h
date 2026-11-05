/// origin guess
#pragma once
#include "Civ5/Src/App/UI/AnimControl.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FXmlElement;
class LuaContext;

/// size ?
/// vtable 0x0a2fe130
class SlideAnim : public AnimControl {
public:
    /// 0x0899e902
    SlideAnim();
    /// 0x089a2be8
    virtual ~SlideAnim();
    /// 0x0899e9ce
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x0899cbf6
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x0899e96a
    virtual UNK_RET Initialize();
    /// 0x0899cd80
    virtual int PushSelf(lua_State* a1);
    /// 0x0899eb90
    virtual UNK_RET UpdateAnim(float a1);
    /// 0x089a2c2a
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089a2c30
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089a2c56
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089a2c98
    virtual UNK_RET vf_49(UNK_PARAMS);
    /// 0x0899ec08
    UNK_RET SetBeginVal(float a1, float a2);
    /// 0x0899ec2a
    UNK_RET SetEndVal(float a1, float a2);
    /// 0x0c079258
    static UNK_DATA s_Cache[0x1c];
};
