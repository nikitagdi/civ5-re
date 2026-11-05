/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include <d3d9.h>

class FGXVector2;
class FGXVector3;
class FGXVector4;
class UITextureInfo;
namespace FireGrafix {
class ArrowShaderConstants;
class BuildingGlobal;
class CCommandStream;
class FGXConstantSet;
class FogGlobalData_Terrain;
class NormalGlobs;
class OverlayGlobals;
class ShadowGlobals;
class TerrainGlobalData;
class TerrainGlobalTextureData;
class TradeArrowGlobals;
class UIShaderGlobalData;
class UnitShaderGlobalData;
class WorldCurrentsData;
}

/// size ?
class UIRenderBuffer {
public:
    class DrawCall;
    class FontDraw;
    class GridDraw;
    class LineDraw;
    class RenderState;
    class StreamID;
    class UIQuadDraw;
    class UIRenderBuffer_Rect;
    /// 0x0876c218
    UIRenderBuffer();
    /// 0x0876c32c
    ~UIRenderBuffer();
    /// 0x0876c38e
    UNK_RET Init(unsigned int a1, unsigned int a2);
    /// 0x0876c43e
    UNK_RET SetShaderGlobals(D3DXMATRIX const& a1, D3DXMATRIX const& a2, D3DXMATRIX const& a3, float a4, bool a5);
    /// 0x0876c7dc
    UNK_RET Begin(UIRenderBuffer::StreamID a1);
    /// 0x0876c838
    UNK_RET End();
    /// 0x0876c94c
    UNK_RET MakeUIQuadSurface(UIRenderBuffer::UIQuadDraw const& a1);
    /// 0x0876caae
    UNK_RET MakeGridSurface(UIRenderBuffer::GridDraw const& a1);
    /// 0x0876cb6e
    UNK_RET MakeLineSurface(UIRenderBuffer::LineDraw const& a1);
    /// 0x0876cbea
    UNK_RET MakeFontSurface(UIRenderBuffer::FontDraw const& a1);
    /// 0x0876cbf2
    UNK_RET SortDrawCalls(UIRenderBuffer::DrawCall* a1, unsigned int a2);
    /// 0x0876d354
    UNK_RET Submit(unsigned int a1, FireGrafix::CCommandStream** a2);
    /// 0x0876d5a6
    UNK_RET DrawUILine(unsigned int a1, FGXVector2 const& a2, FGXVector2 const& a3, float a4, unsigned char a5);
    /// 0x0876d750
    UNK_RET GetOrthoRect(FGXVector4 const& a1);
    /// 0x0876d834
    UNK_RET DrawUILine_WithRect(UIRenderBuffer::UIRenderBuffer_Rect const& a1, unsigned int a2, unsigned char a3);
    /// 0x0876d8ba
    UNK_RET DrawUIQuad(UITextureInfo const* a1, UITextureInfo const* a2, unsigned int a3, unsigned int a4, unsigned char a5);
    /// 0x0876d996
    UNK_RET DrawUIQuad_WithRect(UIRenderBuffer::UIRenderBuffer_Rect const& a1, UITextureInfo const* a2, UITextureInfo const* a3, unsigned int a4, unsigned int a5, unsigned char a6);
    /// 0x0876da36
    UNK_RET DrawGrid(UITextureInfo const* a1, unsigned char a2);
    /// 0x0876daf6
    UNK_RET GetGridSizePos();
    /// 0x0876db90
    UNK_RET DrawGrid_WithRect(UIRenderBuffer::UIRenderBuffer_Rect const& a1, UITextureInfo const* a2, unsigned char a3);
    /// 0x0876dc16
    UNK_RET DrawFonts(unsigned int a1, unsigned char a2, FGXVector4 const& a3);
    /// 0x0876dcc8
    UNK_RET DrawFonts_WithRect(UIRenderBuffer::UIRenderBuffer_Rect const& a1, unsigned int a2, unsigned char a3);
    /// 0x0876dd4e
    UNK_RET DrawUIQuad_Perspective(UITextureInfo const* a1, UITextureInfo const* a2, unsigned int a3, unsigned int a4, unsigned char a5);
    /// 0x0876de7c
    UNK_RET GetPerspectiveRect(FGXVector4 const& a1, FGXVector3 const& a2, FGXVector3 const& a3, D3DXMATRIX const& a4, FGXVector3 const& a5, float a6);
    /// 0x0876e060
    UNK_RET DrawGrid_Perspective(UITextureInfo const* a1, unsigned char a2);
    /// 0x0876e194
    UNK_RET DrawFonts_Perspective(unsigned int a1, unsigned char a2, FGXVector4 const& a3);
    /// 0x0876e2a8
    UNK_RET PushDynamics(unsigned int a1);
    /// 0x0876e2e2
    UNK_RET DumpStatistics(float a1);
    /// 0x0876ee30 ConstructSurfaces<UIRenderBuffer::FontDraw, unsigned int (*)(UIRenderBuffer::FontDraw const&)>
    /// 0x0876ebd8 ConstructSurfaces<UIRenderBuffer::GridDraw, unsigned int (*)(UIRenderBuffer::GridDraw const&)>
    /// 0x0876ed04 ConstructSurfaces<UIRenderBuffer::LineDraw, unsigned int (*)(UIRenderBuffer::LineDraw const&)>
    /// 0x0876ea38 ConstructSurfaces<UIRenderBuffer::UIQuadDraw, unsigned int (*)(UIRenderBuffer::UIQuadDraw const&)>
    template <class T0, class T1>
    void ConstructSurfaces(T0* a1, unsigned int a2, T1 a3);
    /// 0x0a3e5b50
    static UNK_DATA SCREEN[0x10];
};

/// 0x0876e2cf
UNK_RET operator<(UIRenderBuffer::RenderState const& a1, UIRenderBuffer::RenderState const& a2);

namespace FireGrafix {
/// 0x0a3e60e0
extern ArrowShaderConstants* g_pArrowShaderConstants;
/// 0x0a3e6064
extern UNK_DATA g_pBGColor[0x4];
/// 0x0a3e6088
extern BuildingGlobal* g_pBuildingGlobal;
/// 0x0a3e604c
extern UNK_DATA g_pCameraParameters[0x4];
/// 0x0a3e60a4
extern UNK_DATA g_pErrorGlobalConstants[0x4];
/// 0x0a3e6080
extern UNK_DATA g_pFXShaderGlobalData[0x4];
/// 0x0a3e607c
extern UNK_DATA g_pFXShaderMaxData[0x4];
/// 0x0a3e6094
extern FogGlobalData_Terrain* g_pFogGlobalData_Terrain;
/// 0x0a3e60a8
extern UNK_DATA g_pFontShaderGlobal[0x4];
/// 0x0a3e60ac
extern UNK_DATA g_pForestGlobalConstants[0x4];
/// 0x0a3e6084
extern UNK_DATA g_pForkDefaultShaderGlobal[0x4];
/// 0x0a3e60b8
extern UNK_DATA g_pGridShaderGlobal[0x4];
/// 0x0a3e60f0
extern UNK_DATA g_pIceGlobalConstants[0x4];
/// 0x0a3e60d0
extern UNK_DATA g_pLandmarkShaderGlobalData[0x4];
/// 0x0a3e6068
extern UNK_DATA g_pLeaderGlowScale[0x4];
/// 0x0a3e6050
extern UNK_DATA g_pLeaderLights[0x4];
/// 0x0a3e6070
extern UNK_DATA g_pLeader_Glass_HackHilight_PREVIEW[0x4];
/// 0x0a3e6060
extern UNK_DATA g_pLeader_VFX_CameraParameters[0x4];
/// 0x0a3e6078
extern UNK_DATA g_pLeader_VFX_Globals[0x4];
/// 0x0a3e6058
extern UNK_DATA g_pLeader_VFX_Time[0x4];
/// 0x0a3e60d4
extern UNK_DATA g_pMenuShaderGlobal[0x4];
/// 0x0a3e608c
extern UNK_DATA g_pMiniMapCameraGlobal[0x4];
/// 0x0a3e60a0
extern UNK_DATA g_pMiniMapGlobal[0x4];
/// 0x0a3e60e4
extern NormalGlobs* g_pNormalGlobs;
/// 0x0a3e60bc
extern OverlayGlobals* g_pOverlayGlobals;
/// 0x0a3e6054
extern UNK_DATA g_pPostSettings[0x4];
/// 0x0a3e60c0
extern UNK_DATA g_pReplayMapShaderGlobalConstants[0x4];
/// 0x0a3e60c4
extern UNK_DATA g_pRiverShaderGlobals[0x4];
/// 0x0a3e60e8
extern ShadowGlobals* g_pShadowGlobals;
/// 0x0a3e605c
extern UNK_DATA g_pShadowMapParameters[0x4];
/// 0x0a3e60c8
extern UNK_DATA g_pSimpleGlobalConstants[0x4];
/// 0x0a3e60cc
extern UNK_DATA g_pSimpleSkinnedGlobalConstants[0x4];
/// 0x0a3e60dc
extern UNK_DATA g_pSkinBlurGlobal[0x4];
/// 0x0a3e606c
extern UNK_DATA g_pSkinBlurWeights[0x4];
/// 0x0a3e6074
extern TerrainGlobalData* g_pTerrainGlobalData;
/// 0x0a3e6090
extern TerrainGlobalTextureData* g_pTerrainGlobalTextureData;
/// 0x0a3e60ec
extern TradeArrowGlobals* g_pTradeArrowGlobals;
/// 0x0a3e60d8
extern UNK_DATA g_pUILineShaderGlobal[0x4];
/// 0x0a3e6098
extern UIShaderGlobalData* g_pUIShaderGlobalData;
/// 0x0a3e609c
extern UnitShaderGlobalData* g_pUnitShaderGlobalData;
/// 0x0a3e60b0
extern FGXConstantSet* g_pWaterGlobalData;
/// 0x0a3e60b4
extern FGXConstantSet* g_pWaveGlobalConsts;
/// 0x0a3e60f4
extern WorldCurrentsData* g_pWorldCurrentsData;
}

namespace LeaderboardUserData {
/// 0x08774f18
UNK_RET Write(FDataStream& a1);
/// 0x08775008
UNK_RET Read(FDataStream& a1);
/// 0x087750f0
UNK_RET DebugLog();
}
