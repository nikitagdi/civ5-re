/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FTempHeap.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"

class LinearEventDispatcher;
class NullEventClass;
class RiverTileAddedData;
class RiverVisSystem;
class RiverVisSystemDataDef;
class SerialContainer;
class TerrainData;
class TerrainStencilVertex;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
class cvTerrainSampler;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
template <class> class RiverVisSystem_Accessor;
namespace B_SPLINE {
enum BSPLINE_BEGIN_TYPES : int;
enum BSPLINE_END_TYPES : int;
}

// One side of a hex; Fun_0865f1be moves sides 3..5 (mod 6) to the neighbour hex.
/// size 0x10
class HexEdge {
public:
    HEXVec_i m_kHex;  ///< +0x0
    // 0..11: six sides, each in two flow directions.
    int m_iEdge;      ///< +0xc
};

// River edge bits of a fixed 200 x 200 hex grid; OnAddRiverTile sets them, IsRiver reads them.
/// size 0x2719c
class RiverEdgeGrid {
public:
    SquareGridPolicy m_kPolicy;      ///< +0x0
    // Bit e set when edge e of the hex carries a river; cell (x, y) of m_mxForward is at y * 200 + x + y / 2.
    unsigned int m_auiEdges[40000];  ///< +0x9c
};

// Control points of one river; the banks reuse the layout.
/// size 0x10
class RiverSimSpline {
public:
    // x, y in height map texels; z is 0 until CreateStencilMeshes puts the river height there.
    FFastVector<FGXVector3, true> m_aPoints;  ///< +0x0
    // Set by MarkRiverMouths when the last edge has no downstream river edge.
    bool m_bEndsAtMouth;                      ///< +0xc
    char unk_d[0x3];                          ///< +0xd
};

// B-spline wavelet levels of 2D points; Fun_087f8f6c drops one level, Fun_087f901e restores it.
/// size 0x18
struct RiverWaveletBuffer {
    int m_iLevel;               ///< +0x0
    // Index in m_pPoints of the first point of the current level.
    unsigned int m_uiStart;     ///< +0x4
    unsigned int m_uiCount;     ///< +0x8
    FGXVector2* m_pPoints;      ///< +0xc
    unsigned int m_uiNumPoints; ///< +0x10
    unsigned int m_uiMaxPoints; ///< +0x14
};

// Starts the buffer with uiCount copies of kFill and room for 2 * uiCount + 1 points.
/// 0x087f8ed4
UNK_RET Fun_087f8ed4(RiverWaveletBuffer* pBuffer, unsigned int uiCount, FGXVector2 const& kFill);
// One decomposition step: the coarse level holds m_uiCount / 2 + 1 points.
/// 0x087f8f6c
UNK_RET Fun_087f8f6c(RiverWaveletBuffer* pBuffer);
// One reconstruction step back to the finer level.
/// 0x087f901e
UNK_RET Fun_087f901e(RiverWaveletBuffer* pBuffer);
// Frees the point store of a RiverWaveletBuffer.
/// 0x087f9dce
UNK_RET Fun_087f9dce(FGXVector2** ppPoints);

// Vertex of the meshes CreateSimMeshes builds; z is a raw height map value (0..65535).
/// size 0x14
struct RiverMeshVertex {
    FGXVector3 m_vPos;  ///< +0x0
    // Bed: 1 on the center line, 0 on the banks; drives the sine dip of the bed blend.
    float m_fAcross;    ///< +0xc
    // Blend weight toward the mesh height: 1 on the banks, 0 at the outer skirt edge.
    float m_fWeight;    ///< +0x10
};

// Triangle lists (three vertices per triangle) of all rivers.
/// size 0x18
class RiverSimSplineMesh {
public:
    FFastVector<RiverMeshVertex, true, 50u, 0u, FTempHeapFastVectorAllocator> m_aBed;     ///< +0x0
    FFastVector<RiverMeshVertex, true, 50u, 0u, FTempHeapFastVectorAllocator> m_aSkirts;  ///< +0xc
};

// One block entry of RiverBlendMask: state 1 is a uniform block holding m_ucValue.
/// size 0x8
struct RiverBlendMaskBlock {
    unsigned short m_usState;  ///< +0x0
    char unk_2[0x2];           ///< +0x2
    unsigned char m_ucValue;   ///< +0x4
    char unk_5[0x3];           ///< +0x5
};

// Paged byte grid in 128 x 128 blocks over the height map; marks the texels a skirt triangle already wrote.
/// size 0x28
struct RiverBlendMask {
    unsigned int m_uiWidth;               ///< +0x0
    unsigned int m_uiHeight;              ///< +0x4
    unsigned int m_uiBlocksX;             ///< +0x8
    unsigned int m_uiBlocksY;             ///< +0xc
    RiverBlendMaskBlock* m_pBlockTable;   ///< +0x10
    // BlendIntoTerrain sets 0xfffffff whenever it resets the mask.
    unsigned int field_14;                ///< +0x14
    unsigned int field_18;                ///< +0x18
    unsigned char* m_pBlocks;             ///< +0x1c
    unsigned int m_uiNumBlocks;           ///< +0x20
    unsigned int m_uiMaxBlocks;           ///< +0x24
};

// Context of the skirt pass of BlendIntoTerrain.
/// size 0x14
struct RiverSkirtBlendContext {
    // Read only by the bed pass; both contexts start with it.
    float m_fBedFloor;                     ///< +0x0
    RiverBlendMask* m_pMask;               ///< +0x4
    cvTerrainSampler* m_pSampler;          ///< +0x8
    // TerrainData indices per continent 1..4; a texel cut low enough with m_aucFromTerrain[i] becomes m_aucToTerrain[i].
    unsigned char m_aucFromTerrain[4];     ///< +0xc
    unsigned char m_aucToTerrain[4];       ///< +0x10
};

// Context of the bed pass of BlendIntoTerrain.
/// size 0x8
struct RiverBedBlendContext {
    // Normalised height; the bed never goes below m_fBedFloor - 0.06 unless the terrain already is.
    float m_fBedFloor;            ///< +0x0
    TerrainData* m_pTerrainData;  ///< +0x4
};

// Resets the mask to kSize rounded up to whole 128-texel blocks, every block uniform *pFill; reserves uiReserve blocks.
/// 0x087f813c
UNK_RET Fun_087f813c(RiverBlendMask* pMask, FGXInt2 const* pSize, unsigned char const* pFill, unsigned int uiReserve);
// Frees the block table and the block storage of the mask.
/// 0x087f8cea
UNK_RET Fun_087f8cea(RiverBlendMask* pMask);
// Mask value at the texel.
/// 0x087fad98
bool Fun_087fad98(RiverBlendMask const* pMask, FGXInt2 const* pPos);
// Sets the mask value at the texel, splitting a uniform block on write.
/// 0x087fade4
UNK_RET Fun_087fade4(RiverBlendMask* pMask, unsigned char const* pValue, FGXInt2 const* pPos);
// Rasterises uiNumTriangles triangles at texel corners and calls Fun_087fa960 for each covered texel.
/// 0x087f8220
UNK_RET Fun_087f8220(FGXInt2 const* pMapSize, RiverMeshVertex const* pTriangles, unsigned int uiNumTriangles, RiverSkirtBlendContext* pContext);
// The same rasteriser for the bed pass; calls Fun_087fa59c.
/// 0x087f875c
UNK_RET Fun_087f875c(FGXInt2 const* pMapSize, RiverMeshVertex const* pTriangles, unsigned int uiNumTriangles, RiverBedBlendContext* pContext);
// Skirt texel: the first triangle to reach it lerps the height toward the mesh and may retexture it.
/// 0x087fa960
UNK_RET Fun_087fa960(RiverSkirtBlendContext* pContext, int iX, int iY, float const* pBary, RiverMeshVertex const* pV0, RiverMeshVertex const* pV1, RiverMeshVertex const* pV2);
// Bed texel: lowers the height toward the mesh plus a sine dip and sets the river bed terrain.
/// 0x087fa59c
UNK_RET Fun_087fa59c(RiverBedBlendContext* pContext, int iX, int iY, float const* pBary, RiverMeshVertex const* pV0, RiverMeshVertex const* pV1, RiverMeshVertex const* pV2);

/// size 0x271cc
class cvRiverSimSystem {
public:
    /// 0x087f27a0
    cvRiverSimSystem();
    /// 0x087f288a
    UNK_RET Startup();
    /// 0x087f2960
    UNK_RET SubscribeToEvents();
    /// 0x087f299c
    UNK_RET Shutdown();
    /// 0x087f29ee
    UNK_RET UnsubscribeToEvents();
    /// 0x087f2a2a
    UNK_RET StartupGame(FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator>& a1);
    /// 0x087f2b8c
    UNK_RET GatherControlPointsFromMap(FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator>& aRiverEdges);
    /// 0x087f30e0
    UNK_RET SmoothLowFrequency();
    /// 0x087f332e
    UNK_RET RandomizeLowFrequency();
    /// 0x087f35c8
    UNK_RET AddSegmentsToGameSplineDatabase(FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> const& a1) const;
    /// 0x087f36ba
    UNK_RET AddRiverEndcaps(FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> const& aRiverEdges);
    /// 0x087f3a8a
    UNK_RET MarkRiverMouths(FFastVector<FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> const& a1);
    /// 0x087f3b2c
    UNK_RET CalculateRiverHeights(FFastVector<FFastVector<float, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator>& a1) const;
    /// 0x087f3d52
    UNK_RET GenerateRiverBankControlPoints();
    /// 0x087f43da
    UNK_RET CreateStencilMeshes(FFastVector<TerrainStencilVertex, true, 50u, 0u, FTempHeapFastVectorAllocator>* pVertices, FFastVector<FFastVector<float, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> const& aHeights) const;
    /// 0x087f4e48
    UNK_RET ShutdownGame();
    /// 0x087f4e78
    UNK_RET ClearInternalBuffers();
    /// 0x087f4f32
    UNK_RET OnAddRiverTile(GameCoreEventTemplate<Event_Int2Type<67>, RiverTileAddedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087f4fb0
    bool IsRiver(HexEdge const& kEdge) const;
    /// 0x087f504c
    bool GetNextEdge(HexEdge const& kEdge, HexEdge& kNext) const;
    /// 0x087f5098
    bool GetPrevEdge(HexEdge const& kEdge, HexEdge& kPrev) const;
    /// 0x087f50e4
    bool GetNextLeftEdge(HexEdge const& kEdge, HexEdge& kNext) const;
    /// 0x087f513c
    bool GetNextRightEdge(HexEdge const& kEdge, HexEdge& kNext) const;
    /// 0x087f5194
    bool TraverseToRiverSource(HexEdge const& kStart, HexEdge& kSource, FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator> const& aVisited) const;
    /// 0x087f5508
    UNK_RET BlendIntoTerrain(TerrainData* pTerrainData) const;
    /// 0x087f5a70
    UNK_RET CreateSimMeshes(RiverSimSplineMesh* a1, FFastVector<FFastVector<float, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> const& a2) const;
    /// 0x087f7a6e
    UNK_RET AddControlPointsAlongRiver(FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator> const& a1);
    /// 0x087f7c1a
    UNK_RET SmoothRivers();

    // The constructor stores 4.
    int field_0;                         ///< +0x0
    FFastVector<RiverSimSpline> m_aRivers;      ///< +0x4
    FFastVector<RiverSimSpline> m_aLeftBanks;   ///< +0x10
    FFastVector<RiverSimSpline> m_aRightBanks;  ///< +0x1c
    // Handle to the RiverVisSystem entry: cvGeneralRefHandle<RiverVisSystem, unsigned int, ...> (base word, then index).
    cvGeneralRefHandleBase m_hRiverVis;  ///< +0x28
    unsigned int m_uiRiverVisIndex;      ///< +0x2c
    RiverEdgeGrid m_kRiverGrid;          ///< +0x30
};

namespace B_SPLINE {
/// 0x087f979c ReconstructWavelet<FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
template <class T0, class T1, B_SPLINE::BSPLINE_BEGIN_TYPES T2, B_SPLINE::BSPLINE_END_TYPES T3>
void ReconstructWavelet(T0 a1, T0 a2, T1 a3);
/// 0x087f9965 ReconstructScalingFunc_ADD<FGXVector2, FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
template <class T0, class T1, class T2, B_SPLINE::BSPLINE_BEGIN_TYPES T3, B_SPLINE::BSPLINE_END_TYPES T4>
void ReconstructScalingFunc_ADD(T1 a1, T1 a2, T2 a3);
/// 0x087f9be4 DecomposeWavelet<FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
template <class T0, class T1, B_SPLINE::BSPLINE_BEGIN_TYPES T2, B_SPLINE::BSPLINE_END_TYPES T3>
void DecomposeWavelet(T0 a1, T0 a2, T1 a3);
/// 0x087f9c97 DecomposeScalingFunc<FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>
template <class T0, class T1, B_SPLINE::BSPLINE_BEGIN_TYPES T2, B_SPLINE::BSPLINE_END_TYPES T3>
void DecomposeScalingFunc(T0 a1, T0 a2, T1 a3);
}
