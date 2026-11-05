/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FGXVector2;
class FGXVector4;
class IStrategicViewFont;
class StrategicViewTextColorSet;
namespace FireGrafix {
class CCommandStream;
class SVFontShader;
class SurfaceSet;
}

/// size ?
class StrategicViewText {
public:
    /// 0x08c93bd8
    StrategicViewText();
    /// 0x08c93d26
    ~StrategicViewText();
    /// 0x08c93d92
    UNK_RET Release();
    /// 0x08c93e80
    UNK_RET Initialize(FireGrafix::SVFontShader* a1);
    /// 0x08c93ea0
    UNK_RET RebuildText();
    /// 0x08c93ed4
    UNK_RET SetColor(FGXVector4 a1, int a2);
    /// 0x08c93f26
    UNK_RET SetColorSet(StrategicViewTextColorSet& a1);
    /// 0x08c93f54
    UNK_RET SetStyle(FStringA const& a1);
    /// 0x08c93f96
    UNK_RET SetWrapWidth(int a1);
    /// 0x08c93fba
    UNK_RET SetFont(IStrategicViewFont* a1);
    /// 0x08c93fde
    UNK_RET SetText(char const* a1);
    /// 0x08c94092
    UNK_RET ParseTextIntoBlocks();
    /// 0x08c948be
    UNK_RET CalculateSize();
    /// 0x08c94ade
    UNK_RET GetText() const;
    /// 0x08c94aec
    UNK_RET AddBlock(unsigned int a1, unsigned int a2, unsigned long* a3, bool a4);
    /// 0x08c94e2e
    UNK_RET AddIconBlock(unsigned int a1);
    /// 0x08c94e82
    UNK_RET BuildBuffersFromBlocks();
    /// 0x08c9541c
    UNK_RET AllocateAndFillBuffers();
    /// 0x08c95964
    UNK_RET Render(FGXVector2 const& a1, FireGrafix::CCommandStream* a2);
    /// 0x0d266d68
    static UNK_DATA m_TempIconVB[0xc];
    /// 0x0d266c90
    static UNK_DATA m_TempVBLayer[0xd8];
    /// 0x0d266c84
    static UNK_DATA m_abBreakRenderChars[0xc];
    /// 0x0d266c78
    static UNK_DATA m_acBreakChars[0xc];
    /// 0x0d266c74
    static UNK_DATA m_fAccumulatedLineLength[0x4];
    /// 0x0d266c70
    static UNK_DATA sm_hIB[0x4];
};

namespace FireGrafix {
/// 0x0d266e26
extern UNK_DATA g_MapEditorUI_MapEditorUIShaderPS_PS[0x2];
/// 0x0d266e24
extern UNK_DATA g_MapEditorUI_MapEditorUIShaderVS_VS[0x2];
/// 0x0d266e2c
extern UNK_DATA g_MapEditorUI__DS[0x2];
/// 0x0d266e28
extern UNK_DATA g_MapEditorUI__GS[0x2];
/// 0x0d266e2a
extern UNK_DATA g_MapEditorUI__HS[0x2];
/// 0x0d266e50
extern UNK_DATA g_MapShader_AirEscortOverlayShaderPS_PS[0x2];
/// 0x0d266e52
extern UNK_DATA g_MapShader_AirSeaEscortOverlayShaderPS_PS[0x2];
/// 0x0d266e58
extern UNK_DATA g_MapShader_BorderGlowShaderPS_PS[0x2];
/// 0x0d266e36
extern UNK_DATA g_MapShader_BorderShaderVS_VS[0x2];
/// 0x0d266e72
extern UNK_DATA g_MapShader_CityTextBackgroundPS_PS[0x2];
/// 0x0d266e6a
extern UNK_DATA g_MapShader_ContinentBorderShaderPS_PS[0x2];
/// 0x0d266e46
extern UNK_DATA g_MapShader_ContinentTypePS_PS[0x2];
/// 0x0d266e32
extern UNK_DATA g_MapShader_ContinentTypeShaderVS_VS[0x2];
/// 0x0d266e6c
extern UNK_DATA g_MapShader_CultureBorderShaderPS_PS[0x2];
/// 0x0d266e54
extern UNK_DATA g_MapShader_CultureTypePS_PS[0x2];
/// 0x0d266e4e
extern UNK_DATA g_MapShader_EscortOverlayShaderPS_PS[0x2];
/// 0x0d266e66
extern UNK_DATA g_MapShader_FOWShaderPS_PS[0x2];
/// 0x0d266e6e
extern UNK_DATA g_MapShader_HealthBarPS_PS[0x2];
/// 0x0d266e68
extern UNK_DATA g_MapShader_HighlightShaderPS_PS[0x2];
/// 0x0d266e70
extern UNK_DATA g_MapShader_HorizHealthBarPS_PS[0x2];
/// 0x0d266e42
extern UNK_DATA g_MapShader_MapAlphaShaderPS_PS[0x2];
/// 0x0d266e40
extern UNK_DATA g_MapShader_MapShaderPS_PS[0x2];
/// 0x0d266e2e
extern UNK_DATA g_MapShader_MapShaderVS_VS[0x2];
/// 0x0d266e62
extern UNK_DATA g_MapShader_RedShiftPS_PS[0x2];
/// 0x0d266e38
extern UNK_DATA g_MapShader_RiverShaderVS_VS[0x2];
/// 0x0d266e74
extern UNK_DATA g_MapShader_RouteShaderPS_PS[0x2];
/// 0x0d266e3a
extern UNK_DATA g_MapShader_RouteShaderVS_VS[0x2];
/// 0x0d266e48
extern UNK_DATA g_MapShader_TerrainOverlayShaderPS_PS[0x2];
/// 0x0d266e34
extern UNK_DATA g_MapShader_TerrainOverlayShaderVS_VS[0x2];
/// 0x0d266e44
extern UNK_DATA g_MapShader_TerrainShaderPS_PS[0x2];
/// 0x0d266e30
extern UNK_DATA g_MapShader_TerrainShaderVS_VS[0x2];
/// 0x0d266e56
extern UNK_DATA g_MapShader_TexturedGlowEffectShaderPS_PS[0x2];
/// 0x0d266e78
extern UNK_DATA g_MapShader_TradeRoutePS_PS[0x2];
/// 0x0d266e3e
extern UNK_DATA g_MapShader_TradeRouteVS_VS[0x2];
/// 0x0d266e64
extern UNK_DATA g_MapShader_UnderContructionPS_PS[0x2];
/// 0x0d266e60
extern UNK_DATA g_MapShader_UnitBackgroundGlowShaderPS_PS[0x2];
/// 0x0d266e5e
extern UNK_DATA g_MapShader_UnitBackgroundNoHBShaderPS_PS[0x2];
/// 0x0d266e5c
extern UNK_DATA g_MapShader_UnitBackgroundShaderPS_PS[0x2];
/// 0x0d266e4a
extern UNK_DATA g_MapShader_UnitCombatTypeOverlayShaderPS_PS[0x2];
/// 0x0d266e4c
extern UNK_DATA g_MapShader_UnitDualCombatTypeOverlayShaderPS_PS[0x2];
/// 0x0d266e5a
extern UNK_DATA g_MapShader_UnitShaderPS_PS[0x2];
/// 0x0d266e76
extern UNK_DATA g_MapShader_VisibilityBoxPS_PS[0x2];
/// 0x0d266e3c
extern UNK_DATA g_MapShader_VisibilityBoxVS_VS[0x2];
/// 0x0d266e7e
extern UNK_DATA g_MapShader__DS[0x2];
/// 0x0d266e7a
extern UNK_DATA g_MapShader__GS[0x2];
/// 0x0d266e7c
extern UNK_DATA g_MapShader__HS[0x2];
/// 0x0d266e88
extern UNK_DATA g_SVFontShader_FontPSClip_PS[0x2];
/// 0x0d266e84
extern UNK_DATA g_SVFontShader_FontPS_PS[0x2];
/// 0x0d266e82
extern UNK_DATA g_SVFontShader_FontPerspectiveVS_VS[0x2];
/// 0x0d266e80
extern UNK_DATA g_SVFontShader_FontVS_VS[0x2];
/// 0x0d266e8a
extern UNK_DATA g_SVFontShader_IconFontPSClip_PS[0x2];
/// 0x0d266e86
extern UNK_DATA g_SVFontShader_IconFontPS_PS[0x2];
/// 0x0d266e90
extern UNK_DATA g_SVFontShader__DS[0x2];
/// 0x0d266e8c
extern UNK_DATA g_SVFontShader__GS[0x2];
/// 0x0d266e8e
extern UNK_DATA g_SVFontShader__HS[0x2];
/// 0x0d266e18
extern UNK_DATA g_pMapEditorUIShaderGlobalConstants[0x4];
/// 0x0d266e1c
extern UNK_DATA g_pMapShaderGlobalConstants[0x4];
/// 0x0d266e20
extern UNK_DATA g_pSVFontShaderGlobal[0x4];
}

namespace FireGrafix { namespace MapEditorUIShaderDataDynamic {
/// 0x08c96376
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08c963a8
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace MapEditorUIShaderGlobalConstants {
/// 0x08c96344
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace MapShaderDataDynamic {
/// 0x08c9640c
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08c9643e
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace MapShaderGlobalConstants {
/// 0x08c963da
UNK_RET LockGlobal(unsigned int a1);
}}

namespace FireGrafix { namespace SVFontShaderDynamic {
/// 0x08c964a2
UNK_RET LockDynamic(FireGrafix::CCommandStream* a1);
/// 0x08c964d4
UNK_RET LockDynamic(FireGrafix::SurfaceSet* a1);
}}

namespace FireGrafix { namespace SVFontShaderGlobal {
/// 0x08c96470
UNK_RET LockGlobal(unsigned int a1);
}}
