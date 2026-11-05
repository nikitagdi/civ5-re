/// origin guess
#pragma once
#include "Civ5/Src/App/ControlBase.h"
#include "FirePlace/FireWorks/FLua/Include/FLuaTypes.h"
#include <lua.h>

class FGXVector2;
class FGXVector3;
class FGXVector4;
class FXmlElement;
class LuaContext;
class UIRenderBuffer;
enum UIAABB : int;
namespace FireGrafix {
class FGXSurface;
}

/// size ?
/// vtable 0x0a2fee10
class GridControl : public ControlBase {
public:
    /// 0x089bbc1a
    GridControl();
    /// 0x089bbcca
    /// deleting 0x089bbca2
    virtual ~GridControl();
    /// 0x089bc332
    virtual UNK_RET BuildFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089bd4a4
    virtual UNK_RET ClipRender(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089bd588
    virtual UNK_RET ClipRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x089bb5e8
    virtual UNK_RET ExposeSelf(FLua::Table& a1);
    /// 0x089bbd2a
    virtual UNK_RET Initialize();
    /// 0x089bd600
    virtual UNK_RET PerspectiveRender(unsigned int a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x089bd6ec
    virtual UNK_RET PerspectiveRender(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, FGXVector3 const& a4, float const& a5);
    /// 0x089bb772
    virtual int PushSelf(lua_State* a1);
    /// 0x089bcff2
    virtual UNK_RET Render(unsigned int a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089bd286
    virtual UNK_RET Render(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5);
    /// 0x089bcfa8
    virtual UNK_RET SetColor(FGXVector4 a1);
    /// 0x089bccc6
    virtual UNK_RET SetSizeVal(float a1, float a2);
    /// 0x089be248
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x089be24e
    virtual UNK_RET vf_24(UNK_PARAMS);
    /// 0x089be274
    virtual UNK_RET vf_34(UNK_PARAMS);
    /// 0x089bc060
    UNK_RET CalculateAutoSize();
    /// 0x089bc150
    UNK_RET NormalizeTextureCoords();
    /// 0x089bc25c
    UNK_RET CreateSurface(FireGrafix::FGXSurface** a1);
    /// 0x089bc37a
    UNK_RET BuildCoreFromXML(FXmlElement const& a1, LuaContext* a2);
    /// 0x089bcbc8
    UNK_RET SetTextureOffset(FGXVector2 a1);
    /// 0x089bcc0a
    UNK_RET SetTextureOffsetVal(float a1, float a2);
    /// 0x089bcc54
    UNK_RET SetStateIndex(unsigned int a1);
    /// 0x089bcd06
    UNK_RET AdjustComponents();
    /// 0x089bcfca
    UNK_RET DoAutoSize();
    /// 0x089bd0d6
    UNK_RET UpdateShaderDynamics(FGXVector2 const& a1, float const& a2, float const& a3, float const& a4, UIAABB const& a5);
    /// 0x089bd306
    UNK_RET UpdateShaderDynamics(UIRenderBuffer& a1, FGXVector2 const& a2, float const& a3, float const& a4, float const& a5, UIAABB const& a6);
    /// 0x0c07e858
    static UNK_DATA s_Cache[0x1c];
};

/// 0x089bd774
/// lua ?
int LuaThunk_GridControl_SetSizeVal(lua_State* a1);
/// 0x089bd88e
int LuaToString_GridControl_SetSizeVal(lua_State* a1);
/// 0x089bd917
/// lua ?
int LuaThunk_GridControl_DoAutoSize(lua_State* a1);
/// 0x089bd996
int LuaToString_GridControl_DoAutoSize(lua_State* a1);
/// 0x089bda09
/// lua ?
int LuaThunk_GridControl_SetTextureOffset(lua_State* a1);
/// 0x089bdbcf
int LuaToString_GridControl_SetTextureOffset(lua_State* a1);
/// 0x089bdc50
/// lua ?
int LuaThunk_GridControl_SetTextureOffsetVal(lua_State* a1);
/// 0x089bdc93
int LuaToString_GridControl_SetTextureOffsetVal(lua_State* a1);

namespace GridControlRenderResources {
/// 0x089bb7f2
UNK_RET Startup();
/// 0x089bbac9
UNK_RET Shutdown();
/// 0x089bbb50
UNK_RET UpdateShaderGlobals(FGXVector3 const& a1, FGXVector3 const& a2);
/// 0x0c07e87c
extern UNK_DATA g_hIB[0x4];
/// 0x0c07e878
extern UNK_DATA g_hVB[0x4];
/// 0x0c07e874
extern UNK_DATA g_pShader[0x4];
}
