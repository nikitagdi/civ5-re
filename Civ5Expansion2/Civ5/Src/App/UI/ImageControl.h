/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "Civ5/Src/App/cvTunerListener.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FGXVector2;
class FGXVector3;
class FGXVector4;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;

/// size ?
/// vtable 0x0a2fef30
class ImageControl : public ControlBase {
public:
    /// 0x089bee6e
    ImageControl();
    /// 0x089c0d4c
    /// deleting 0x089c0d84
    virtual ~ImageControl();
    /// 0x089bf0d4
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089bf676
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089bf8ae
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089bec64
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089bfa2e
    virtual UNK_RET GetDataMembers(FFastVector<cvTunerListener::TreeNodeDataMember, false, 0u, 0u, BaseVector<cvTunerListener::TreeNodeDataMember, false>::FDefaultFastVectorAllocator>& a1);
    /// 0x089beec8
    virtual UNK_RET Initialize();
    /// 0x089bf73e
    virtual UNK_RET PerspectiveRender(unsigned int a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x089bf976
    virtual UNK_RET PerspectiveRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x089bedee
    virtual int PushSelf(lua_State* a1);
    /// 0x089bf5be
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089bf7f6
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089c0dcc
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x089bf44a
    virtual UNK_RET SetSizeVal(float a1, float a2);
    /// 0x089c0dc6
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089c0dee
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089c0e14
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089bef50
    UNK_RET Resize(float a1, float a2);
    /// 0x089bf47e
    UNK_RET SetTextureSizeVal(float a1, float a2);
    /// 0x089bf4a0
    UNK_RET SetTexture(char const* a1);
    /// 0x089bf4f8
    UNK_RET SetTextureAndResize(char const* a1);
    /// 0x0c07ef84
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089bfbb1
/// lua ?
int LuaThunk_ImageControl_SetSizeVal(lua_State* a1);
/// 0x089bfccb
int LuaToString_ImageControl_SetSizeVal(lua_State* a1);
/// 0x089bfd54
/// lua ?
int LuaThunk_ImageControl_SetTextureSizeVal(lua_State* a1);
/// 0x089bfd97
int LuaToString_ImageControl_SetTextureSizeVal(lua_State* a1);
/// 0x089bfe20
/// lua ?
int LuaThunk_ImageControl_SetTextureOffset(lua_State* a1);
/// 0x089bffcd
int LuaToString_ImageControl_SetTextureOffset(lua_State* a1);
/// 0x089c004e
/// lua ?
int LuaThunk_ImageControl_SetTextureOffsetVal(lua_State* a1);
/// 0x089c0091
int LuaToString_ImageControl_SetTextureOffsetVal(lua_State* a1);
/// 0x089c011a
/// lua ?
int LuaThunk_ImageControl_SetTexture(lua_State* a1);
/// 0x089c021b
int LuaToString_ImageControl_SetTexture(lua_State* a1);
/// 0x089c02a2
/// lua ?
int LuaThunk_ImageControl_SetTextureAndResize(lua_State* a1);
/// 0x089c02e3
int LuaToString_ImageControl_SetTextureAndResize(lua_State* a1);
/// 0x089c036a
/// lua ?
int LuaThunk_ImageControl_SetTextureHandle(lua_State* a1);
/// 0x089c0453
int LuaToString_ImageControl_SetTextureHandle(lua_State* a1);
/// 0x089c04d4
/// lua ?
int LuaThunk_ImageControl_UnloadTexture(lua_State* a1);
/// 0x089c059c
int LuaToString_ImageControl_UnloadTexture(lua_State* a1);
/// 0x089c060f
/// lua ?
int LuaThunk_ImageControl_NormalizeTexture(lua_State* a1);
/// 0x089c0652
int LuaToString_ImageControl_NormalizeTexture(lua_State* a1);
/// 0x089c06c5
/// lua ?
int LuaThunk_ImageControl_Resize(lua_State* a1);
/// 0x089c0708
int LuaToString_ImageControl_Resize(lua_State* a1);
