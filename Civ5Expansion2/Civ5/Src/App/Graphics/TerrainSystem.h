/// origin guess
#pragma once
#include <d3d9.h>
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/TerrainDecalLayer.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FTimer.h"
#include "FirePlace/FireWorks/FUnrolledList_Core.h"

class CoastCP;
class FGXVector2;
class FGXVector4;
class FJobSet;
class HexPatternCell;
class LinearEventDispatcher;
class LocalMachineContainer;
class LuaContext;
class NullEventClass;
class PlotPolygon;
class Road;
class SquareGridPolicy;
class Terrain;
class TerrainCacheData;
class TerrainData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
class cvTerrainSampler;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, unsigned int T2, unsigned int T3> class DynamicHexGrid;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class FUnrolledList_Tail_Pointer;
template <class T0, unsigned int T1, unsigned int T2> class NodeArray;
template <int T0> class Event_Int2Type;
namespace B_SPLINE {
enum BSPLINE_BEGIN_TYPES : int;
enum BSPLINE_END_TYPES : int;
}
namespace FireGrafix {
class CCommandStream;
class FGXShaderPackageInstanceView;
class ShadowShader;
class WaterShader;
class WorldCurrents;
}
namespace GameSpline {
class ControlPoint;
}

// Filled by TerrainSystem (init timings) and Terrain::CalcStats (cache figures).
/// size ?
class TerrainStats {
public:
    float m_fInitTime;                 ///< +0x0
    float m_fGPUInitTime;              ///< +0x4
    unsigned int m_uiNumCacheTextures; ///< +0x8
    unsigned int field_c;              ///< +0xc
    unsigned int m_uiCacheMemory;      ///< +0x10
};

// Constant block of FireGrafix::WaterGlobalData (one copy per camera).
/// size 0x250
struct WaterGlobalConstants {
    D3DXMATRIX m_mViewProj;              ///< +0x0
    D3DXMATRIX m_mOverlayProj;           ///< +0x40
    float m_afScrollOffset[4];           ///< +0x80
    float m_afTileScale[4];              ///< +0x90
    float m_afBumpWeights[4];            ///< +0xa0
    float m_fScrollTile;                 ///< +0xb0
    char unk_b4[0xc];                    ///< +0xb4
    float m_fWaterHeight;                ///< +0xc0
    char unk_c4[0xc];                    ///< +0xc4
    float m_fWaveStrength;               ///< +0xd0
    char unk_d4[0xc];                    ///< +0xd4
    FGXVector3 m_vDir0;                  ///< +0xe0
    char unk_ec[0x4];                    ///< +0xec
    FGXVector3 m_vDir1;                  ///< +0xf0
    char unk_fc[0x4];                    ///< +0xfc
    FGXVector3 m_vSpecColorNarrow;       ///< +0x100
    float m_fSpecPowerNarrow;            ///< +0x10c
    FGXVector3 m_vSpecColorWide;         ///< +0x110
    float m_fSpecPowerWide;              ///< +0x11c
    FGXVector3 m_vDir2;                  ///< +0x120
    float m_fDir2W;                      ///< +0x12c
    FGXVector3 m_vDir3;                  ///< +0x130
    float m_fDir3W;                      ///< +0x13c
    float m_fFogNegFalloff;              ///< +0x140
    float m_fFogIntegral;                ///< +0x144
    float field_148;                     ///< +0x148
    float m_fFogZoom;                    ///< +0x14c
    FGXVector3 m_vCameraPos;             ///< +0x150
    char unk_15c[0x4];                   ///< +0x15c
    D3DXMATRIX m_mReflectionViewProj;    ///< +0x160
    D3DXMATRIX m_mShadowViewProj;        ///< +0x1a0
    D3DXMATRIX m_mWorldToHeightmap;      ///< +0x1e0
    float m_fShadowFade;                 ///< +0x220
    char unk_224[0xc];                   ///< +0x224
    float m_afFogColor[4];               ///< +0x230
    float m_fReflectAmount;              ///< +0x240
    char unk_244[0xc];                   ///< +0x244
};

// Constant block of FireGrafix::WorldCurrentsData (0x40 bytes locked).
/// size 0x40
struct WorldCurrentsConstants {
    float m_afGridSize[2];               ///< +0x0
    char unk_8[0x8];                     ///< +0x8
    float m_fCurrentParamB;              ///< +0x10
    char unk_14[0xc];                    ///< +0x14
    float m_fCurrentParamC;              ///< +0x20
    char unk_24[0xc];                    ///< +0x24
    float m_fCurrentParamA;              ///< +0x30
    char unk_34[0xc];                    ///< +0x34
};

// Embedded in TerrainSystem at +0x2772c; 0xec is the distance to the next subsystem.
/// size 0xec
class WaterSystem {
public:
    FireGrafix::CCommandStream* m_pCommandStream;                  ///< +0x0
    bool m_bWatchSkyFiles;                                         ///< +0x4
    unsigned long long m_ullSkyFileTime;                           ///< +0x8
    unsigned int m_hRiverAlpha;                                    ///< +0x10
    unsigned int m_hDepthColor;                                    ///< +0x14
    unsigned int m_hRiverDiffuse;                                  ///< +0x18
    unsigned int m_hBumpA;                                         ///< +0x1c
    unsigned int m_hBumpB;                                         ///< +0x20
    unsigned int m_ahRiverBumpA[2];                                ///< +0x24
    unsigned int m_ahRiverBumpB[2];                                ///< +0x2c
    unsigned int m_auConstantSetIds[4];                            ///< +0x34
    unsigned int field_44;                                         ///< +0x44
    unsigned int m_hCurrentSurface;                                ///< +0x48
    FireGrafix::WaterShader* m_pWaterShader;                       ///< +0x4c
    FireGrafix::WorldCurrents* m_pWorldCurrents;                   ///< +0x50
    unsigned int m_hQuadVB;                                        ///< +0x54
    unsigned int m_hQuadIB;                                        ///< +0x58
    unsigned int m_uMaxQuads;                                      ///< +0x5c
    float m_fCurrentParamA;                                        ///< +0x60
    float m_fCurrentParamB;                                        ///< +0x64
    float m_fCurrentParamC;                                        ///< +0x68
    float m_fReflectAmount;                                        ///< +0x6c
    FGXVector3 m_vSpecColorNarrow;                                 ///< +0x70
    float m_fSpecPowerNarrow;                                      ///< +0x7c
    FGXVector3 m_vSpecColorWide;                                   ///< +0x80
    float m_fSpecPowerWide;                                        ///< +0x8c
    FGXVector3 m_vWaterDir0;                                       ///< +0x90
    FGXVector3 m_vWaterDir1;                                       ///< +0x9c
    FGXVector3 m_vWaterDir2;                                       ///< +0xa8
    float m_fWaterDir2W;                                           ///< +0xb4
    FGXVector3 m_vWaterDir3;                                       ///< +0xb8
    float m_fWaterDir3W;                                           ///< +0xc4
    float m_fSeaLevel;                                             ///< +0xc8
    float field_cc;                                                ///< +0xcc
    float m_fWaveStrength;                                         ///< +0xd0
    float m_fWaveTiling;                                           ///< +0xd4
    float m_fScrollSpeedA;                                         ///< +0xd8
    float m_fScrollPhaseA;                                         ///< +0xdc
    float m_fScrollSpeedB;                                         ///< +0xe0
    float m_fScrollPhaseB;                                         ///< +0xe4
    unsigned int m_uPass;                                          ///< +0xe8

    /// 0x0868ae38
    UNK_RET Startup(unsigned short a1);
    /// 0x0868b536
    UNK_RET LoadSunSky();
    /// 0x0868b832
    UNK_RET Shutdown();
    /// 0x0868b966
    UNK_RET StartupGame();
    /// 0x0868b9ee
    UNK_RET RenderOceanCurrentMap();
    /// 0x0868bb38
    UNK_RET GenerateBumpTextures();
    /// 0x0868bb3a
    UNK_RET ShutdownGame();
    /// 0x0868bb6a
    UNK_RET ReloadTextures();
    /// 0x0868bbae
    UNK_RET Update(float fDeltaTime);
    /// 0x0868bc02
    UNK_RET Render(unsigned int a1);
};

// Record the event (un)subscribe helpers take: the owner and its method pointer in Itanium form.
/// size 0x10
struct TerrainEventDelegate {
    unsigned int field_0;   ///< +0x0
    void* m_pObject;        ///< +0x4
    void* m_pfnMethod;      ///< +0x8
    int m_iAdjust;          ///< +0xc
};

/// size 0x75fd8
class TerrainSystem {
public:
    /// 0x0867e2c0
    TerrainSystem();
    /// 0x0867e770
    UNK_RET CompactTerrain(char const* a1, char const* a2, bool a3);
    /// 0x0867e9be
    UNK_RET Startup();
    /// 0x0867ef40
    UNK_RET BuildShadowBuffers();
    /// 0x0867f416
    UNK_RET OnTerrainCached(EventTemplate<Event_Int2Type<28>, TerrainCacheData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0867f496
    UNK_RET Shutdown();
    /// 0x0867f6f2
    UNK_RET OnResolutionChanged();
    /// 0x0867f706
    UNK_RET RegisterDebugMenu(LuaContext* pDebugMenu);
    /// 0x0867fb06
    UNK_RET TerrainSystemToggle(unsigned int a1, unsigned int uiToggle);
    /// 0x0867fd46
    UNK_RET SunYawButton(unsigned int a1, unsigned int a2);
    /// 0x0867fdaa
    UNK_RET SunPitchButton(unsigned int a1, unsigned int a2);
    /// 0x0867fe0e
    UNK_RET ReflectPitchButton(unsigned int a1, unsigned int a2);
    /// 0x0867fe72
    UNK_RET ReflectYawButton(unsigned int a1, unsigned int a2);
    /// 0x0867fed6
    UNK_RET TessellationToggle(unsigned int a1, unsigned int a2);
    /// 0x0867fef4
    UNK_RET WireframeToggle(unsigned int a1, unsigned int a2);
    /// 0x0867ff00
    UNK_RET TessLODButton(unsigned int a1, unsigned int a2);
    /// 0x0867ff32
    UNK_RET TessLevelButton(unsigned int a1, unsigned int a2);
    /// 0x0867ff7a
    UNK_RET UpdateSettings();
    /// 0x0868041a
    UNK_RET ReflectAmount(unsigned int a1, unsigned int a2);
    /// 0x0868046a
    UNK_RET Init(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>& kGrid, bool a2, unsigned int uiRandomSeed);
    /// 0x08682312
    UNK_RET CreateTerrain(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& kGrid);
    /// 0x086825e4
    UNK_RET StartGPUInitWork(bool a1);
    /// 0x0868263e
    UNK_RET PerformGPUInitWork();
    /// 0x0868269a
    UNK_RET StartupGame();
    /// 0x086826da
    UNK_RET ShutdownGame();
    /// 0x086828b4
    UNK_RET PerformShadowBlur();
    /// 0x086829c8
    UNK_RET Render(unsigned int a1);
    /// 0x08682aa6
    UNK_RET RenderOverlay();
    /// 0x08682af4
    UNK_RET LaunchJobs();
    /// 0x08682b50
    UNK_RET Update(float fDeltaTime);
    /// 0x08682ba0
    UNK_RET SetupShadowFrameGlobals();
    /// 0x0868300a
    UNK_RET ToggleCellUpdates();
    /// 0x08683028
    UNK_RET ToggleWater();
    /// 0x08683034
    UNK_RET ToggleIceRendering();
    // Event handler code at 0x086831a6; no function symbol covers it.
    /// inline
    UNK_RET SuspendCellUpdates();
    // Event handler code at 0x086831b6; no function symbol covers it.
    /// inline
    UNK_RET ResumeCellUpdates();

    TerrainDecalLayer m_DecalLayer;                       ///< +0x0
    // cvRiverSimSystem; its size is not known yet.
    char unk_50c[0x271cc];                                ///< +0x50c
    // cvMarshSimSystem; its size is not known yet.
    char unk_276d8[0x40];                                 ///< +0x276d8
    Terrain* field_27718;                                 ///< +0x27718
    cvTerrainSampler* field_2771c;                        ///< +0x2771c
    TerrainData* field_27720;                             ///< +0x27720
    int m_iHeightMapWidth;                                ///< +0x27724
    int m_iHeightMapHeight;                               ///< +0x27728
    WaterSystem m_WaterSystem;                            ///< +0x2772c
    // cvFloodplainsSystem; its size is not known yet.
    char unk_27818[0x2b1a4];                              ///< +0x27818
    // cvTerrainIceSystem; its size is not known yet.
    char unk_529bc[0x1001c];                              ///< +0x529bc
    FireGrafix::ShadowShader* m_pShadowShader;            ///< +0x629d8
    TerrainStats m_Stats;                                 ///< +0x629dc
    unsigned int m_uiNum64Textures;                       ///< +0x629f0
    unsigned int m_uiNum128Textures;                      ///< +0x629f4
    unsigned int m_uiNum256Textures;                      ///< +0x629f8
    unsigned int m_uiNum512Textures;                      ///< +0x629fc
    unsigned int m_uiNumBCTextures;                       ///< +0x62a00
    unsigned int m_ah64Textures[0x1fa0];                  ///< +0x62a04
    unsigned int m_ah128Textures[0x400];                  ///< +0x6a884
    unsigned int m_ah256Textures[0x400];                  ///< +0x6b884
    unsigned int m_ah512Textures[0x400];                  ///< +0x6c884
    unsigned int m_ahBCTextures[0x1fa0];                  ///< +0x6d884
    FireGrafix::CCommandStream* m_pCommandStream;         ///< +0x75704
    FireGrafix::CCommandStream* m_pTextureCommandStream;  ///< +0x75708
    FireGrafix::CCommandStream* m_pOverlayCommandStream;  ///< +0x7570c
    FJobSet* m_pJobSet;                                   ///< +0x75710
    float m_fSunYaw;                                      ///< +0x75714
    float m_fSunPitch;                                    ///< +0x75718
    float m_fReflectYaw;                                  ///< +0x7571c
    float m_fReflectPitch;                                ///< +0x75720
    float m_fReflectAmount;                               ///< +0x75724
    FGXVector3 m_vShadowDir;                              ///< +0x75728
    FGXVector3 m_vSunDir;                                 ///< +0x75734
    FGXVector3 m_vReflectDir;                             ///< +0x75740
    TerrainData m_TerrainData;                            ///< +0x7574c
    unsigned int m_ahSunlightCubes[5];                    ///< +0x75f70
    unsigned int m_ahShadowlightCubes[5];                 ///< +0x75f84
    unsigned int m_hShadowSurface;                        ///< +0x75f98
    int m_iInitialized;                                   ///< +0x75f9c
    Terrain* m_pTerrain;                                  ///< +0x75fa0
    FTimer m_InitTimer;                                   ///< +0x75fa4
    FTimer m_GPUInitTimer;                                ///< +0x75fb0
    FTimer m_StageTimer;                                  ///< +0x75fbc
    bool m_bDiffuse;                                      ///< +0x75fc8
    bool m_bSpecular;                                     ///< +0x75fc9
    bool m_bRenderWater;                                  ///< +0x75fca
    bool m_bRenderIce;                                    ///< +0x75fcb
    bool m_bGPUInitStarted;                               ///< +0x75fcc
    bool m_bWireframe;                                    ///< +0x75fcd
    char unk_75fce[2];                                    ///< +0x75fce
    LuaContext* m_pDebugMenu;                             ///< +0x75fd0
    int m_iSuspendCellUpdates;                            ///< +0x75fd4
};

extern "C" {
    /// 0x0a35283c
    extern TerrainSystem g_TerrainSystem;
}

/// 0x08660236
UNK_RET Fun_08660236(TerrainEventDelegate* a1);
/// 0x08660726
UNK_RET Fun_08660726(TerrainEventDelegate* a1);
// Appends a 0x20-byte record to the event sequencer queue passed in.
/// 0x086839fe
unsigned char* Fun_086839fe(void* a1);
/// 0x0868969e
UNK_RET Fun_0868969e(TerrainEventDelegate* a1);
/// 0x086898ca
UNK_RET Fun_086898ca(TerrainEventDelegate* a1);
/// 0x08689af6
UNK_RET Fun_08689af6(TerrainEventDelegate* a1);
/// 0x08689e88
UNK_RET Fun_08689e88(TerrainEventDelegate* a1);
// Frees the grid at +0x84..+0x8c of the road network manager and resets its pools.
/// 0x0868a26e
UNK_RET Fun_0868a26e(void* a1);
/// 0x0867e992
UNK_RET Identity(FGXVector4* a1, FGXVector3 const& a2, FGXVector3 const& a3, void* a4);
/// 0x08a2305d POLY_push_back_simplify<PlotPolygon>
/// 0x08685d38 POLY_push_back_simplify<Road>
template <class T0>
void POLY_push_back_simplify(T0& a1, FGXVector2 a2, float a3);
/// 0x08686041 POLY_push_front_simplify<Road>
template <class T0>
void POLY_push_front_simplify(T0& a1, FGXVector2 a2, float a3);

namespace B_SPLINE {
/// 0x088ebf0a EvalGradient<CoastCP, CoastCP const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x087dfbdf EvalGradient<FGXVector2, FGXVector2 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x08683676 EvalGradient<FGXVector2, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x088e5e0e EvalGradient<FGXVector2, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)1>
/// 0x087c5bc9 EvalGradient<FGXVector2, FUnrolledList_Core<FGXVector2, NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator>, FUnrolledList_Tail_Pointer<NodeArray<FGXVector2, 30u, 14u>, FFastAllocator<NodeArray<FGXVector2, 30u, 14u>, false, 55u, 0u, BaseVector<NodeArray<FGXVector2, 30u, 14u>, false>::FDefaultFastVectorAllocator> > >::iterator, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)1>
/// 0x08845af2 EvalGradient<FGXVector3, FGXVector3 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x08845d6b EvalGradient<FGXVector3, FGXVector3 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)1>
/// 0x087c96cb EvalGradient<FGXVector3, FGXVector3*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x087fb2c5 EvalGradient<FGXVector3, FGXVector3*, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x0893e7e2 EvalGradient<GameSpline::ControlPoint, GameSpline::ControlPoint const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
/// 0x08750444 EvalGradient<GameSpline::ControlPoint, GameSpline::ControlPoint*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
template <class T0, class T1, B_SPLINE::BSPLINE_BEGIN_TYPES T2, B_SPLINE::BSPLINE_END_TYPES T3>
T0 EvalGradient(int a1, T1 a2, float a3, int a4);
}
