/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FGXVector2;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;

/// size ?
/// vtable 0x0a2ff860
class TextureBarControl : public ControlBase {
public:
    /// 0x089d59f2
    TextureBarControl();
    /// 0x089d75c8
    /// deleting 0x089d7610
    virtual ~TextureBarControl();
    /// 0x089d5b8a
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089d64c4
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089d6794
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089d57e8
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089d5af4
    virtual UNK_RET Initialize();
    /// 0x089d5972
    virtual int PushSelf(lua_State* a1);
    /// 0x089d636e
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089d663e
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089d7660
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089d7666
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089d768c
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089d5f0e
    UNK_RET SetPercent(float a1);
    /// 0x089d6084
    UNK_RET SetShadowPercent(float a1);
    /// 0x089d625e
    UNK_RET SetPercents(float a1, float a2);
    /// 0x089d62a2
    UNK_RET SetTexture(char const* a1);
    /// 0x0c0828fc
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089d690e
/// lua ?
int LuaThunk_TextureBarControl_SetShadowColor(lua_State* a1);
/// 0x089d6b74
int LuaToString_TextureBarControl_SetShadowColor(lua_State* a1);
/// 0x089d6bf5
/// lua ?
int LuaThunk_TextureBarControl_SetPercent(lua_State* a1);
/// 0x089d6ce5
int LuaToString_TextureBarControl_SetPercent(lua_State* a1);
/// 0x089d6d66
/// lua ?
int LuaThunk_TextureBarControl_SetShadowPercent(lua_State* a1);
/// 0x089d6da9
int LuaToString_TextureBarControl_SetShadowPercent(lua_State* a1);
/// 0x089d6e2a
/// lua ?
int LuaThunk_TextureBarControl_SetPercents(lua_State* a1);
/// 0x089d6ee3
int LuaToString_TextureBarControl_SetPercents(lua_State* a1);
/// 0x089d6f6c
/// lua ?
int LuaThunk_TextureBarControl_SetTexture(lua_State* a1);
/// 0x089d7015
int LuaToString_TextureBarControl_SetTexture(lua_State* a1);
