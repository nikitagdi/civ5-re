/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include "FirePlace/FireWorks/FString.h"
#include <lua.h>

class ColorSet;
class ControlBase;
class FGXInt2;
class FGXVector2;
class FXmlElement;
class HEXVec;
class LuaContext;

/// size ?
/// vtable 0x0a2f98b0
class LuaContext : public ControlBase {
public:
    /// 0x086bf31e
    LuaContext();
    /// 0x086bf7a2
    /// deleting 0x086bf77a
    virtual ~LuaContext();
    /// 0x086c57a8
    virtual UNK_RET AddChild(ControlBase* a1);
    /// 0x086c57d8
    virtual UNK_RET AddChildAtIndex(ControlBase* a1, int a2);
    /// 0x086c1514
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x086bf114
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x086bfcd6
    virtual UNK_RET Initialize();
    /// 0x086c1dbc
    virtual UNK_RET ProcessInput(unsigned int a1, unsigned int a2, long a3, FGXVector2 const& a4);
    /// 0x086bf29e
    virtual int PushSelf(lua_State* a1);
    /// 0x086c1d24
    virtual UNK_RET SetHide(bool a1);
    /// 0x086c1c18
    virtual UNK_RET Update(float a1);
    /// 0x086c5810
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x086c5816
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x086c583c
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x086c0e7e
    virtual UNK_RET Initialize(FStringW const& a1);
    /// 0x086bfb48
    UNK_RET Release();
    /// 0x086c04ee
    UNK_RET LoadXML();
    /// 0x086c0d8e
    /// lua ?
    static int lLookUpControl(lua_State* a1);
    /// 0x086c0dfe
    UNK_RET RegisterNamedControl(char const* a1, ControlBase* a2);
    /// 0x086c0ec6
    UNK_RET LoadLayoutDefaults(FXmlElement a1);
    /// 0x086c1430
    UNK_RET ExtractColorSet(char const* a1, ColorSet& a2);
    /// 0x086c17b2
    ControlBase* LookUpControl(char const* a1);
    /// 0x086c1b10
    UNK_RET CheckReload();
    /// 0x086c1b8a
    UNK_RET Reload();
    /// 0x086c1bfc
    UNK_RET IsHotLoad();
    /// 0x086c1cfc
    UNK_RET DoDeferredLoad();
    /// 0x086c1ea6
    UNK_RET LoadNewContext(FStringW& a1);
    /// 0x086c1f10
    UNK_RET ScriptLoadNewContext(FStringA& a1);
    /// 0x086c1fea
    UNK_RET BuildInstance(char const* a1, FLua::Table a2);
    /// 0x086c20d0
    UNK_RET BuildInstanceFromElement(FXmlElement const& a1, FLua::Table a2, ControlBase* a3);
    /// 0x086c2148
    UNK_RET BuildInstanceForControl(char const* a1, FLua::Table a2, ControlBase* a3);
    /// 0x086c2246
    UNK_RET BuildInstanceForControlAtIndex(char const* a1, FLua::Table a2, ControlBase* a3, int a4);
    /// 0x086c234c
    UNK_RET UnloadChildContexts();
    /// 0x086c23b0
    UNK_RET UnloadChildContext(LuaContext* a1);
    /// 0x0a3cb880
    static UNK_DATA s_Cache[0x1c];
};

/// 0x086c23f0
int ToGridFromHex(lua_State* a1);
/// 0x086c24cb
/// lua ?
int LuaThunk_ToGridFromHex(lua_State* a1);
/// 0x086c24df
int LuaToString_ToGridFromHex(lua_State* a1);
/// 0x086c253e
UNK_RET ToHexFromGrid(FGXInt2 a1);
/// 0x086c25fb
/// lua ?
int LuaThunk_ToHexFromGrid(lua_State* a1);
/// 0x086c2860
int LuaToString_ToHexFromGrid(lua_State* a1);
/// 0x086c28db
int GridToWorld(lua_State* a1);
/// 0x086c2a9a
/// lua ?
int LuaThunk_GridToWorld(lua_State* a1);
/// 0x086c2aae
int LuaToString_GridToWorld(lua_State* a1);
/// 0x086c2b0d
UNK_RET HexToWorld(HEXVec a1);
/// 0x086c2b6a
/// lua ?
int LuaThunk_HexToWorld(lua_State* a1);
/// 0x086c2c3c
int LuaToString_HexToWorld(lua_State* a1);
/// 0x086c2cbd
UNK_RET HexVertexToWorld(HEXVec a1, int a2);
/// 0x086c2d65
/// lua ?
int LuaThunk_HexVertexToWorld(lua_State* a1);
/// 0x086c2e31
int LuaToString_HexVertexToWorld(lua_State* a1);
/// 0x086c2ec0
UNK_RET ReloadTextures(int a1, int a2);
/// 0x086c2ed4
/// lua ?
int LuaThunk_ReloadTextures(lua_State* a1);
/// 0x086c2f49
int LuaToString_ReloadTextures(lua_State* a1);
/// 0x086c2fd2
UNK_RET SystemBuildYield();
/// 0x086c2ff8
/// lua ?
int LuaThunk_SystemBuildYield(lua_State* a1);
/// 0x086c308a
int LuaToString_SystemBuildYield(lua_State* a1);
/// 0x086c30fd
UNK_RET IsOnLiveActive();
/// 0x086c3100
/// lua ?
int LuaThunk_IsOnLiveActive(lua_State* a1);
/// 0x086c3164
int LuaToString_IsOnLiveActive(lua_State* a1);
/// 0x086c31d7
UNK_RET IsSteamActive();
/// 0x086c31dd
/// lua ?
int LuaThunk_IsSteamActive(lua_State* a1);
/// 0x086c3241
int LuaToString_IsSteamActive(lua_State* a1);
/// 0x086c32b4
/// lua ?
int LuaThunk_LuaContext_SetShutdown(lua_State* a1);
/// 0x086c342f
int LuaToString_LuaContext_SetShutdown(lua_State* a1);
/// 0x086c34b0
/// lua ?
int LuaThunk_LuaContext_SetUpdate(lua_State* a1);
/// 0x086c34f3
int LuaToString_LuaContext_SetUpdate(lua_State* a1);
/// 0x086c3574
/// lua ?
int LuaThunk_LuaContext_ClearUpdate(lua_State* a1);
/// 0x086c363c
int LuaToString_LuaContext_ClearUpdate(lua_State* a1);
/// 0x086c36af
/// lua ?
int LuaThunk_LuaContext_SetPostInit(lua_State* a1);
/// 0x086c36f2
int LuaToString_LuaContext_SetPostInit(lua_State* a1);
/// 0x086c3773
/// lua ?
int LuaThunk_LuaContext_SetInputHandler(lua_State* a1);
/// 0x086c37b6
int LuaToString_LuaContext_SetInputHandler(lua_State* a1);
/// 0x086c3837
/// lua ?
int LuaThunk_LuaContext_SetShowHideHandler(lua_State* a1);
/// 0x086c387a
int LuaToString_LuaContext_SetShowHideHandler(lua_State* a1);
/// 0x086c38fb
/// lua ?
int LuaThunk_LuaContext_LoadNewContext(lua_State* a1);
/// 0x086c3ab5
int LuaToString_LuaContext_LoadNewContext(lua_State* a1);
/// 0x086c3b3c
/// lua ?
int LuaThunk_LuaContext_BuildInstance(lua_State* a1);
/// 0x086c3c91
int LuaToString_LuaContext_BuildInstance(lua_State* a1);
/// 0x086c3d26
/// lua ?
int LuaThunk_LuaContext_BuildInstanceForControl(lua_State* a1);
/// 0x086c3e94
int LuaToString_LuaContext_BuildInstanceForControl(lua_State* a1);
/// 0x086c3f31
/// lua ?
int LuaThunk_LuaContext_BuildInstanceForControlAtIndex(lua_State* a1);
/// 0x086c40bd
int LuaToString_LuaContext_BuildInstanceForControlAtIndex(lua_State* a1);
/// 0x086c4168
/// lua ?
int LuaThunk_LuaContext_IsHotLoad(lua_State* a1);
/// 0x086c41f5
int LuaToString_LuaContext_IsHotLoad(lua_State* a1);
/// 0x086c4268
/// lua ?
int LuaThunk_LuaContext_CallShowHideHandler(lua_State* a1);
/// 0x086c4354
int LuaToString_LuaContext_CallShowHideHandler(lua_State* a1);
/// 0x086c43d5
/// lua ?
int LuaThunk_LuaContext_CallParentShowHideHandler(lua_State* a1);
/// 0x086c4418
int LuaToString_LuaContext_CallParentShowHideHandler(lua_State* a1);
/// 0x086c4499
/// lua ?
int LuaThunk_LuaContext_DoDeferredLoad(lua_State* a1);
/// 0x086c44dc
int LuaToString_LuaContext_DoDeferredLoad(lua_State* a1);
/// 0x086c454f
/// lua ?
int LuaThunk_LuaContext_Reload(lua_State* a1);
/// 0x086c4592
int LuaToString_LuaContext_Reload(lua_State* a1);

namespace FLua { namespace Details {
/// 0x086c69b5 PushFunction<int (*)(lua_State*)>
template <class T0>
void PushFunction(lua_State* a1, T0 a2, char const* a3);
}}
