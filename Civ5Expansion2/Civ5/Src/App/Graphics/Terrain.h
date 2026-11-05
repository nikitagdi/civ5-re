/// origin binary
#pragma once
#include <d3d9.h>
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "FirePlace/FireGrafix/CacheBake.h"
#include "FirePlace/FireGrafix/CompositeShader.h"
#include "FirePlace/FireGrafix/CompressBC.h"
#include "FirePlace/FireGrafix/NormalCalculate.h"
#include "FirePlace/FireGrafix/TerrainShader.h"
#include "FirePlace/FireGrafix/TerrainShaderScreen.h"

class HexEdge;
class HexFOWStateChangedData;
class HexPatternCell;
class RoadCell;
class RoadGrid;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class SquareGridPolicy;
class TerrainCell;
class TerrainStats;
class TerrainSystem;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
class cvTerrainSampler;
enum LIGHTINGOPTS : int;
enum SHADOWOPTS : int;
enum TextureProjectionUserMode : int;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1, unsigned int T2, unsigned int T3> class DynamicHexGrid;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class CCommandStream;
template <int T0> class VStruct;
}

// Copied whole into Terrain::Init; TerrainSystem passes RenderGlobals::g_TexHandles.
/// size 0x28
class ExternalTextureHandles {
public:
    unsigned int m_ahTextures[10];  ///< +0x0
};

/// size 0x8
class ExternalDataReferences {
public:
    TerrainSystem* m_pTerrainSystem;       ///< +0x0
    cvTerrainSampler* m_pTerrainSampler;   ///< +0x4
};

// Contents of the 0x250-byte buffer TerrainGlobalData locks; Terrain::SetGlobals fills it.
/// size 0x250
struct TerrainGlobalConstants {
    D3DXMATRIX m_mViewProj;              ///< +0x0
    D3DXMATRIX m_mOverlayProj;           ///< +0x40
    char unk_80[0x10];                   ///< +0x80
    float m_afFOWBaseScale[4];           ///< +0x90
    float m_afBasis[16];                 ///< +0xa0
    float m_fPatchDivisions;             ///< +0xe0
    char unk_e4[0xc];                    ///< +0xe4
    float field_f0;                      ///< +0xf0
    char unk_f4[0xc];                    ///< +0xf4
    FGXVector3 m_vTerrainScale;          ///< +0x100
    char unk_10c[0x4];                   ///< +0x10c
    FGXVector3 m_vSunDir;                ///< +0x110
    char unk_11c[0x4];                   ///< +0x11c
    FGXVector3 m_vReflectDir;            ///< +0x120
    char unk_12c[0x4];                   ///< +0x12c
    FGXVector3 m_vInvViewPos;            ///< +0x130
    char unk_13c[0x4];                   ///< +0x13c
    FGXVector3 m_vInvViewAxisZ;          ///< +0x140
    char unk_14c[0x4];                   ///< +0x14c
    FGXVector3 m_vAxisZPlusReflectDir;   ///< +0x150
    char unk_15c[0x4];                   ///< +0x15c
    float field_160;                     ///< +0x160
    float field_164;                     ///< +0x164
    char unk_168[0x8];                   ///< +0x168
    float field_170;                     ///< +0x170
    float field_174;                     ///< +0x174
    char unk_178[0x8];                   ///< +0x178
    float m_fHeightMapWidth;             ///< +0x180
    float m_fHeightMapHeight;            ///< +0x184
    char unk_188[0x8];                   ///< +0x188
    float m_fShadowFade;                 ///< +0x190
    char unk_194[0xc];                   ///< +0x194
    float field_1a0;                     ///< +0x1a0
    char unk_1a4[0xc];                   ///< +0x1a4
    float field_1b0;                     ///< +0x1b0
    char unk_1b4[0xc];                   ///< +0x1b4
    float field_1c0;                     ///< +0x1c0
    char unk_1c4[0xc];                   ///< +0x1c4
    float field_1d0;                     ///< +0x1d0
    char unk_1d4[0xc];                   ///< +0x1d4
    float field_1e0;                     ///< +0x1e0
    char unk_1e4[0xc];                   ///< +0x1e4
    D3DXMATRIX m_mShadowViewProj;        ///< +0x1f0
    float m_fScreenWidth;                ///< +0x230
    float m_fScreenHeight;               ///< +0x234
    char unk_238[0x8];                   ///< +0x238
    float field_240;                     ///< +0x240
    char unk_244[0xc];                   ///< +0x244
};

// Contents of the 0x20-byte buffer FogGlobalData_Terrain locks.
/// size 0x20
struct TerrainFogConstants {
    float m_afColor[4];  ///< +0x0
    float field_10;      ///< +0x10
    float field_14;      ///< +0x14
    float field_18;      ///< +0x18
    float field_1c;      ///< +0x1c
};

/// size 0x29d1c
class Terrain {
public:
    // Normalized 2D blur kernel (5x5 by default) used to blend terrain types between hexes.
    /// size 0x3d0
    struct BlendWeights {
        int m_aiOffsetX[81];   ///< +0x0
        int m_aiOffsetY[81];   ///< +0x144
        float m_afWeight[81];  ///< +0x288
        int m_nWeights;        ///< +0x3cc
    };

    // Cached base mesh of a cell of the given size, shared by all cells of that size.
    /// size 0x18
    struct BaseMesh {
        int m_iWidth;              ///< +0x0
        int m_iHeight;             ///< +0x4
        unsigned int field_8;      ///< +0x8
        unsigned int m_hVB;        ///< +0xc
        unsigned int m_hIB;        ///< +0x10
        unsigned int m_hPatchIB;   ///< +0x14
    };

    typedef FStaticVector<unsigned int, 8096> TexturePool;

    // Everything a TerrainCell reads from its terrain (TerrainCell::m_pShared); constructed by Fun_08910c2e.
    /// size 0x1ad0
    struct SharedData {
        BlendWeights m_kBlendWeights;                        ///< +0x0
        unsigned int m_ahSunlightTextures[5];                ///< +0x3d0
        unsigned int m_ahShadowlightTextures[5];             ///< +0x3e4
        FGXVector3 m_vSunDir;                                ///< +0x3f8
        FGXVector3 m_vReflectDir;                            ///< +0x404
        char unk_410[0xc];                                   ///< +0x410
        int field_41c;                                       ///< +0x41c
        float m_afSampleJitter[32][2];                       ///< +0x420
        float field_520;                                     ///< +0x520
        unsigned int m_hQuadVB;                              ///< +0x524
        unsigned int m_hQuadIB;                              ///< +0x528
        unsigned int m_hCompressedVB;                        ///< +0x52c
        unsigned int m_hCompositeVB;                         ///< +0x530
        unsigned int m_hDynamicIB;                           ///< +0x534
        ExternalDataReferences m_kExternalData;              ///< +0x538
        ExternalTextureHandles m_kTexHandles;                ///< +0x540
        FireGrafix::TerrainShader m_kTerrainShader;          ///< +0x568
        FireGrafix::TerrainShaderScreen m_kTerrainShaderScreen; ///< +0x598
        FireGrafix::CompositeShader m_kCompositeShader;      ///< +0x5c8
        FireGrafix::NormalCalculate m_kNormalCalculate;      ///< +0x5f8
        FireGrafix::CacheBake m_kCacheBake;                  ///< +0x628
        FireGrafix::CompressBC m_kCompressBC;                ///< +0x658
        unsigned int m_hDepthStencil;                        ///< +0x688
        unsigned int m_hRenderTemp;                          ///< +0x68c
        unsigned int m_hRenderSpecTemp;                      ///< +0x690
        unsigned int m_hHeightTemp;                          ///< +0x694
        unsigned int m_hShadowTemp;                          ///< +0x698
        unsigned int field_69c;                              ///< +0x69c
        unsigned int m_hNormalTemp;                          ///< +0x6a0
        unsigned int m_hAlbedoTemp;                          ///< +0x6a4
        unsigned int m_hSpecTemp;                            ///< +0x6a8
        unsigned int m_hTempDecalCache;                      ///< +0x6ac
        unsigned int m_hTempNormalCache;                     ///< +0x6b0
        unsigned int m_hTempAlbedoCache;                     ///< +0x6b4
        unsigned int m_hRenderTextureCache;                  ///< +0x6b8
        unsigned int m_hHeightTextureCache;                  ///< +0x6bc
        unsigned int m_hShadowTextureCache;                  ///< +0x6c0
        unsigned int m_ahDXTUncompressedDiffuse[4];          ///< +0x6c4
        unsigned int m_ahDXTDiffuse[4];                      ///< +0x6d4
        unsigned int m_ahDXTUncompressedSpec[4];             ///< +0x6e4
        unsigned int m_ahDXTSpec[4];                         ///< +0x6f4
        // Also the patch step of the base mesh lattice.
        int m_iTessLevel;                                    ///< +0x704
        unsigned int field_708[64];                          ///< +0x708
        unsigned int field_808[64];                          ///< +0x808
        unsigned int field_908[64];                          ///< +0x908
        int field_a08[64];                                   ///< +0xa08
        unsigned int m_hBlankTexture;                        ///< +0xb08
        int m_nTextures;                                     ///< +0xb0c
        TextureProjectionUserMode m_aeProjectionMode[64];    ///< +0xb10
        int m_aiFirstProjection[64];                         ///< +0xc10
        float field_d10[64];                                 ///< +0xd10
        float field_e10[64];                                 ///< +0xe10
        int m_aiProjectionTexture[64];                       ///< +0xf10
        int m_aiProjectionAxis[64];                          ///< +0x1010
        unsigned int m_nProjections;                         ///< +0x1110
        FStaticVector<BaseMesh, 16> m_kBaseMeshes;           ///< +0x1114
        unsigned int m_ahHeightmapTiles[256];                ///< +0x12a4
        unsigned int field_16a4[256];                        ///< +0x16a4
        int field_1aa4;                                      ///< +0x1aa4
        int m_iHeightMapWidth;                               ///< +0x1aa8
        int m_iHeightMapHeight;                              ///< +0x1aac
        float m_fHalfWorldWidth;                             ///< +0x1ab0
        float m_fHalfWorldHeight;                            ///< +0x1ab4
        float field_1ab8;                                    ///< +0x1ab8
        float m_fWorldWidth;                                 ///< +0x1abc
        float m_fWorldHeight;                                ///< +0x1ac0
        FGXVector3 m_vTerrainScale;                          ///< +0x1ac4
    };

    /// 0x0890b554
    Terrain();
    /// 0x0890d374
    ~Terrain();
    /// 0x0890b818
    bool Init(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& kGameGrid, int nTextures, ExternalTextureHandles const& kTexHandles, ExternalDataReferences& kExternalData);
    /// 0x0890c454
    void RecreateGPUHeightmapAndIDMap();
    /// 0x0890c980
    void SetBlendWeights();
    /// 0x0890ca28
    void CalcHeightMap();
    /// 0x0890cc00
    void OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x0890cde0
    void OnDeviceReset(EventTemplate<Event_Int2Type<106>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x0890ce4e
    void BuildCellToHexTable();
    /// 0x0890d8c4
    void DeleteCells();
    /// 0x0890d8ea
    void GetBaseVBandIB(int iWidth, int iHeight, unsigned int* phVB, unsigned int* phIB, unsigned int* phPatchIB, unsigned int* puiField8);
    /// 0x0890da3c
    void GetIndexBufferSpace(unsigned int nIndices, unsigned short** ppIndices, unsigned int* puiFirstIndex);
    /// 0x0890da70
    void GetVertexBufferSpaceComposite(unsigned int nVertices, FireGrafix::VStruct<21>** ppVertices, unsigned int* puiFirstVertex);
    /// 0x0890daa2
    void GetVertexBufferSpaceCompressed(unsigned int nVertices, FireGrafix::VStruct<16>** ppVertices, unsigned int* puiFirstVertex);
    /// 0x0890dad4
    void LockBuffers();
    /// 0x0890db7c
    void UnlockBuffers();
    /// 0x0890dc30
    unsigned int GetRenderTexture(unsigned int uiSize, unsigned int* puiActualSize);
    /// 0x0890dd44
    unsigned int GetBCTexture();
    /// 0x0890dd66
    void ReleaseBCTexture(unsigned int hTexture);
    /// 0x0890ddb8
    void SetRenderTextures(unsigned int* pah64, int n64, unsigned int* pah128, int n128, unsigned int* pah256, int n256, unsigned int* pah512, int n512, unsigned int* pahBC, int nBC);
    /// 0x0890df7a
    void ReleaseRenderTexture(unsigned int hTexture);
    /// 0x0890e008
    void SetGlobals(D3DXMATRIX* pView, D3DXMATRIX* pInvView, D3DXMATRIX* pProj, D3DXMATRIX* pShadowViewProj, D3DXMATRIX* pOverlayProj, float fShadowFade, FireGrafix::CCommandStream* pStream);
    /// 0x0890e57c
    void FindCells(int iX, int iY, int iRadius, TerrainCell** ppCells, int* pnCells);
    /// 0x0890e622
    void FreeTiles(unsigned int nCandidates, unsigned int uiMinFree);
    /// 0x0890e86c
    bool CachePageAvailable(int iSize);
    /// 0x0890e8c6
    bool LaunchJobs(bool a1, bool a2, bool a3);
    /// 0x0890f10e
    void SetTransformWeights();
    /// 0x0890f21e
    void Render(FireGrafix::CCommandStream* pStream, FireGrafix::CCommandStream* pBuildStream, unsigned int a3, bool a4, bool a5, bool a6);
    /// 0x0890f78c
    void RenderOverlay(FireGrafix::CCommandStream* pStream);
    /// 0x0890f9c6
    bool IntersectRay(FGXVector3 vOrigin, FGXVector3 vDir, FGXVector2* pTexCoord, FGXInt2* pHit, float* pfDistance);
    /// 0x0890fa9c
    void PopulateTextureBuildList(bool bUseVisibility);
    /// 0x0890fc1a
    void NonBlockingTextureInit(FireGrafix::CCommandStream* pStream);
    /// 0x0890fec0
    void InvalidateRegion(FGXVector2& vMin, FGXVector2& vMax, int bHeightChanged, int a4);
    /// 0x0891008e
    void PruneWork();
    /// 0x089100ee
    void SetTextures(unsigned int const* a1, unsigned int const* a2, unsigned int const* a3, float const* a4, float const* a5, TextureProjectionUserMode const* paeProjection, int nTextures);
    /// 0x0891042c
    void Invalidate();
    /// 0x08910470
    void SetLightingEnv(unsigned int const* pahSunlight, unsigned int const* pahShadowlight);
    /// 0x0891049e
    void SetSunDir(FGXVector3 vSunDir, FGXVector3 vReflectDir);
    /// 0x089104d8
    float InShadow(FGXInt2 kPos);
    /// 0x0891088c
    void SetOptions(LIGHTINGOPTS eLighting, SHADOWOPTS eShadow, int a3, int bFrustumCull, int a5, int a6, float a7, int a8, int a9);
    /// 0x089108f0
    void CalcStats(TerrainStats* pStats);
    /// 0x08910958
    void SetGamePlayCell(FGXInt2& kHex);

    int field_0;                                         ///< +0x0
    unsigned int m_nCellsRendered;                       ///< +0x4
    SharedData m_kShared;                                ///< +0x8
    FJobSet m_kJobSet;                                   ///< +0x1ad8
    // +0x1adc..+0x2334 is one cell-container object: ~Terrain destroys it with Fun_08910f56 before the build list.
    int field_1adc;                                      ///< +0x1adc
    TerrainCell** m_ppCells;                             ///< +0x1ae0
    TerrainCell** m_ppPriorityInitCells;                 ///< +0x1ae4
    TerrainCell** m_ppBackgroundInitCells;               ///< +0x1ae8
    TerrainCell** m_ppEvictionCandidates;                ///< +0x1aec
    TerrainCell** m_appVisibleCells[4];                  ///< +0x1af0
    FStaticVector<TerrainCell*, 512> m_kBuildList;       ///< +0x1b00
    unsigned int m_nCellsX;                              ///< +0x2310
    unsigned int m_nCellsY;                              ///< +0x2314
    unsigned int m_nCells;                               ///< +0x2318
    unsigned int m_nPriorityInitCells;                   ///< +0x231c
    unsigned int m_uiPriorityInitCursor;                 ///< +0x2320
    unsigned int m_nBackgroundInitCells;                 ///< +0x2324
    unsigned int m_uiBackgroundInitCursor;               ///< +0x2328
    int field_232c;                                      ///< +0x232c
    unsigned int m_nInitBuildsThisFrame;                 ///< +0x2330
    unsigned int m_anVisibleCells[4];                    ///< +0x2334
    unsigned int m_nShadowSteps;                         ///< +0x2344
    unsigned int m_uiHeightmapDirtyFrame;                ///< +0x2348
    unsigned int m_uiHeightmapBuiltFrame;                ///< +0x234c
    int m_bTextureInitDone;                              ///< +0x2350
    int field_2354;                                      ///< +0x2354
    // +0x2358..+0x2404 is one per-hex FOW grid: built by Fun_08910e1e, read and written by Fun_0891159a/Fun_08911508.
    char unk_2358[0x94];                                 ///< +0x2358
    int m_iFOWGridWidth;                                 ///< +0x23ec
    int m_iFOWGridHeight;                                ///< +0x23f0
    bool m_bFOWGridWrapX;                                ///< +0x23f4
    char unk_23f5[0x3];                                  ///< +0x23f5
    int* m_piFOWGrid;                                    ///< +0x23f8
    unsigned int m_nFOWGrid;                             ///< +0x23fc
    unsigned int field_2400;                             ///< +0x2400
    LIGHTINGOPTS m_eLighting;                            ///< +0x2404
    SHADOWOPTS m_eShadow;                                ///< +0x2408
    int field_240c;                                      ///< +0x240c
    int m_bFrustumCull;                                  ///< +0x2410
    int field_2414;                                      ///< +0x2414
    int field_2418;                                      ///< +0x2418
    int field_241c;                                      ///< +0x241c
    float field_2420;                                    ///< +0x2420
    float m_fTessLOD;                                    ///< +0x2424
    unsigned int m_uiPatchDivisions;                     ///< +0x2428
    float m_fLastTessLOD;                                ///< +0x242c
    float m_fTessLODStep;                                ///< +0x2430
    unsigned int m_nIBUsed;                              ///< +0x2434
    unsigned int m_nCompressedVBUsed;                    ///< +0x2438
    unsigned int m_nCompositeVBUsed;                     ///< +0x243c
    FireGrafix::VStruct<21>* m_pCompositeVB;             ///< +0x2440
    FireGrafix::VStruct<16>* m_pCompressedVB;            ///< +0x2444
    unsigned short* m_pIB;                               ///< +0x2448
    TexturePool m_kFree64Textures;                       ///< +0x244c
    TexturePool m_kFree128Textures;                      ///< +0xa2dc
    TexturePool m_kFree256Textures;                      ///< +0x1216c
    TexturePool m_kFree512Textures;                      ///< +0x19ffc
    TexturePool m_kFreeBCTextures;                       ///< +0x21e8c
};

// Comparators of ASL_Msvc_qsort over TerrainCell* arrays.
/// 0x0890e48c
int SortDistanceAndPriority(void const* a1, void const* a2);
/// 0x0890e4fe
UNK_RET SortOnlyDistance(void const* a1, void const* a2);
/// 0x0890e54c
int SortLRU(void const* a1, void const* a2);

// Read-only 3x3 weights of the sharpening pass over each mip of the heightmap tile.
/// 0x092148c8
extern const int g_aiHeightMipKernel[9];
// The six vertices of a hex outline, as offsets from its center in HEXVec space.
/// 0x0c059744
extern HEXVec g_aHexOutlineOffsets[6];

/// 0x08910e1e
UNK_RET Fun_08910e1e(UNK_PARAMS);
/// 0x08910eec
UNK_RET Fun_08910eec(UNK_PARAMS);
/// 0x08910f56
UNK_RET Fun_08910f56(UNK_PARAMS);
/// 0x0891116e
UNK_RET Fun_0891116e(UNK_PARAMS);
/// 0x08910c2e
UNK_RET Fun_08910c2e(Terrain::SharedData* pShared);
// Initializes a cell in place: terrain, shared data, origin (four floats); the job set is not used.
/// 0x08911328
UNK_RET Fun_08911328(TerrainCell* pCell, Terrain* pTerrain, Terrain::SharedData* pShared, FJobSet* pJobSet, float const* pafOrigin);
/// 0x08911508
UNK_RET Fun_08911508(UNK_PARAMS);
/// 0x0891159a
int* Fun_0891159a(UNK_PARAMS);
/// 0x08911626
float* Fun_08911626(UNK_PARAMS);
/// 0x08911a02
UNK_RET Fun_08911a02(UNK_PARAMS);
/// 0x08672408
UNK_RET Fun_08672408(UNK_PARAMS);
/// 0x0868a21a
void Fun_0868a21a(void* list, unsigned int handle);
/// 0x0866fd26
void Fun_0866fd26(void* list, unsigned int handle);
// Releases one reference of an index buffer handle in a CRenderer9 buffer list under its spin lock; returns the references left.
/// 0x0866fb7e
int Fun_0866fb7e(void* pList, unsigned int hHandle);
// Adds one reference to a vertex buffer handle in a CRenderer9 buffer list under its spin lock; returns the new count.
/// 0x0866d812
int Fun_0866d812(void* pList, unsigned int hHandle);
// Same for an index buffer list.
/// 0x0866d866
int Fun_0866d866(void* pList, unsigned int hHandle);
/// 0x086dd934
UNK_RET Fun_086dd934(UNK_PARAMS);
/// 0x086dcac6
UNK_RET Fun_086dcac6(UNK_PARAMS);
/// 0x0873dbba
UNK_RET Fun_0873dbba(UNK_PARAMS);
/// 0x0873d98e
UNK_RET Fun_0873d98e(UNK_PARAMS);
/// 0x0886c502
UNK_RET Fun_0886c502(UNK_PARAMS);
/// 0x0886cd80
UNK_RET Fun_0886cd80(UNK_PARAMS);
/// 0x086616f0
HEXVec_i Fun_086616f0(HEXMatrix_i const& a1, HEXVec_i const& a2);
// a2 is a1 with sides 3..5 moved to the neighbour hex, so each side has one name.
/// 0x0865f1be
UNK_RET Fun_0865f1be(HexEdge const& a1, HexEdge& a2);
// Wraps a1 into WorldGlobals::g_GameGrid in place.
/// 0x0865f23e
UNK_RET Fun_0865f23e(HEXVec_i& a1);
// Cell of the road grid at a1; a fixed spare cell when off the grid.
/// 0x08661606
RoadCell* Fun_08661606(RoadGrid* a1, HEXVec_i const& a2);
