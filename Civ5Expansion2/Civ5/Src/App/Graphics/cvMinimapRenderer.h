/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FTempHeap.h"
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "FirePlace/FireGrafix/FGXVector2.h"

class FGXInt2;
class HEXTransform_i;
class HEXVec_i;
class HexPatternCell;
class INISettings;
class SquareGridPolicy;
class cvMinimapData;
template <class T0, class T1, class T2, bool T3> class FUnrolledList;
template <class T0, class T1, unsigned int T2, unsigned int T3> class DynamicHexGrid;
template <class T0, unsigned int T1, unsigned int T2> class NodeArray;
namespace FireGrafix {
class CCommandStream;
class FGXSurface;
class MiniMapDynamic;
}
namespace GameSpline {
class Segment;
}
// A terrain piece reduced to the hex cells it covers; the grid base is a 10x10 hex grid of TerrainPieceCell.
/// size 0x49c
class PatternPiece {
public:
    unsigned char unk_0[0x94];            ///< +0x0
    int m_iNumRows;                       ///< +0x94
    int m_iNumColumns;                    ///< +0x98
    TerrainPieceCell m_aCells[100];       ///< +0x9c
    int m_iLibraryIndex;                  ///< +0x3bc
    int field_3c0;                        ///< +0x3c0
    int field_3c4;                        ///< +0x3c4
    // FindLinearPacking places the piece only in the pass with this number; passes run from high to 0.
    unsigned int m_uiPackingPass;         ///< +0x3c8
    int field_3cc;                        ///< +0x3cc
    unsigned char m_aucUsedCells[200];    ///< +0x3d0
    unsigned char m_ucNumUsedCells;       ///< +0x498
    unsigned char unk_499[3];             ///< +0x499
};

/// size 0x1c4
class HexPatternMatcher {
public:
    /// size 0x14
    class PatternPosition {
    public:
        unsigned int m_uiPieceIndex;      ///< +0x0
        unsigned int m_uiTransformIndex;  ///< +0x4
        int m_iHexX;                      ///< +0x8
        int m_iHexY;                      ///< +0xc
        int m_iHexZ;                      ///< +0x10
    };

    // Unrolled-list node: 15 positions behind a header; links are node indices, 0xfffffff ends a chain.
    /// size 0x138
    class PatternNode {
    public:
        unsigned int m_uiPrev;                ///< +0x0
        unsigned int m_uiNext;                ///< +0x4
        // Low 31 bits: positions used; bit 31 marks a node not in use and is cleared when the node is acquired.
        unsigned int m_uiCount;               ///< +0x8
        PatternPosition m_aPositions[15];     ///< +0xc
    };

    // The FUnrolledList<PatternPosition, NodeArray<PatternPosition, 15, 7>, ...> instantiation, field by field.
    /// size 0x24
    class PositionList {
    public:
        unsigned int m_uiFreeNode;            ///< +0x0
        unsigned int m_uiNumAcquiredNodes;    ///< +0x4
        FFastVector<PatternNode, true, 43u, 262u, FTempHeapFastVectorAllocator> m_kNodes;  ///< +0x8
        unsigned int m_uiNumLinkedNodes;      ///< +0x14
        unsigned int m_uiFirstNode;           ///< +0x18
        unsigned int m_uiLastNode;            ///< +0x1c
        unsigned int m_uiSize;                ///< +0x20
    };

    /// 0x08a46f58
    void ComputePatternPositions(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>& grid);
    /// 0x08a46fd6
    void FindLinearPacking(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>& grid, unsigned int uiPass, FUnrolledList<HexPatternMatcher::PatternPosition, NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, FFastAllocator<NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, true, 43u, 262u, FTempHeapFastVectorAllocator>, false>* pPositions);

    PatternPiece* m_pPieces;             ///< +0x0
    unsigned int m_uiNumPieces;          ///< +0x4
    unsigned int m_uiPieceCapacity;      ///< +0x8
    HEXMatrix_i* m_pTransforms;          ///< +0xc
    unsigned int m_uiNumTransforms;      ///< +0x10
    unsigned int m_uiTransformCapacity;  ///< +0x14
    // Inline storage m_pTransforms points at after construction (0x0891b43f).
    HEXMatrix_i m_aTransformStorage[6];  ///< +0x18
    unsigned char field_198;             ///< +0x198
    unsigned char unk_199[3];            ///< +0x199
    unsigned int m_uiMaxPackingPass;     ///< +0x19c
    PositionList m_kPositions;           ///< +0x1a0
};

/// 0x0891b67a
void Fun_0891b67a(HexPatternMatcher* a1, TerrainPiece const* a2);
/// 0x0891ce36
void Fun_0891ce36(PatternPiece** a1, unsigned int a2);
// Piece cell against grid cell: a zero mask on either side matches, else masks must share a bit and continents agree.
/// 0x08a478b6
bool Fun_08a478b6(TerrainPieceCell const* a1, HexPatternCell const* a2);
// List push_back; returns the iterator (last node << 8 | slot), 0xfffffff when the list is empty.
/// 0x08a478fc
unsigned int Fun_08a478fc(FUnrolledList<HexPatternMatcher::PatternPosition, NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, FFastAllocator<NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, true, 43u, 262u, FTempHeapFastVectorAllocator>, false>* a1, HexPatternMatcher::PatternPosition const& a2);
// Inserts a position at a slot of a node with room, shifting the later slots up.
/// 0x08a47b80
bool Fun_08a47b80(HexPatternMatcher::PatternNode* a1, HexPatternMatcher::PatternPosition const& a2, unsigned int a3);
// Takes a node from the free chain or appends one to the pool, copies the template in and returns its index.
/// 0x08a47c0a
unsigned int Fun_08a47c0a(HexPatternMatcher::PositionList* a1, HexPatternMatcher::PatternNode const& a2);
// Makes a node the new last node of the chain.
/// 0x08a47cc0
void Fun_08a47cc0(HexPatternMatcher::PositionList* a1, unsigned int a2);
// Links node a2 after node a3 (0xfffffff: a2 becomes a lone node).
/// 0x08a47d02
void Fun_08a47d02(HexPatternMatcher::PositionList* a1, unsigned int a2, unsigned int a3);

// Growable array of 4-byte values (GPU handles): data, count, capacity.
/// size 0xc
class MinimapHandleArray {
public:
    unsigned int* m_pData;        ///< +0x0
    unsigned int m_uiCount;       ///< +0x4
    unsigned int m_uiCapacity;    ///< +0x8
};

// What one mesh pass appended to the shared vertex and index lists, and the surfaces built from it.
/// size 0x18
class MinimapMeshRange {
public:
    unsigned int m_uiFirstVertex;   ///< +0x0
    unsigned int m_uiFirstIndex;    ///< +0x4
    unsigned int m_uiNumVertices;   ///< +0x8
    unsigned int m_uiNumIndices;    ///< +0xc
    unsigned int m_uiFirstSurface;  ///< +0x10
    unsigned int m_uiNumSurfaces;   ///< +0x14
};

// The hexes of one culture owner. m_uiOwner holds the owner id plus one while the owners are collected and the id once the mesh is built.
/// size 0x1c
class MinimapCultureGroup {
public:
    unsigned int m_uiOwner;         ///< +0x0
    MinimapMeshRange m_kRange;      ///< +0x4
};

/// size 0xc
class MinimapCultureGroupArray {
public:
    MinimapCultureGroup* m_pData;   ///< +0x0
    unsigned int m_uiCount;         ///< +0x4
    unsigned int m_uiCapacity;      ///< +0x8
};

// A surface of the sprite batches and the shader pass to draw it with.
/// size 0x8
class MinimapSpriteSurface {
public:
    unsigned short m_usPass;        ///< +0x0
    unsigned char unk_2[0x2];       ///< +0x2
    unsigned int m_hSurface;        ///< +0x4
};

/// size 0xc
class MinimapSpriteSurfaceArray {
public:
    MinimapSpriteSurface* m_pData;  ///< +0x0
    unsigned int m_uiCount;         ///< +0x4
    unsigned int m_uiCapacity;      ///< +0x8
};

// A locked static vertex or index buffer: its handle and the mapped memory.
/// size 0x8
class MinimapBufferLock {
public:
    unsigned int m_hBuffer;         ///< +0x0
    void* m_pData;                  ///< +0x4
};

// Pushes one handle onto a MinimapHandleArray, growing it when full.
/// 0x08a46492
void Fun_08a46492(MinimapHandleArray* a1, unsigned int const* a2);
// Pushes one culture group onto a MinimapCultureGroupArray, growing it when full.
/// 0x08a463a4
void Fun_08a463a4(MinimapCultureGroupArray* a1, MinimapCultureGroup const* a2);
// Rounds a2[0..a3) cube coordinates to the nearest hex into a1 (float x, y, z, w per element); returns a1.
/// 0x087e9677
HEXVec* Fun_087e9677(HEXVec* a1, HEXVec const* a2, int a3);

// The minimap target and the layers drawn into it; the one instance is the global at 0x0c03d464.
/// size 0x2fc
class cvMinimapRenderer {
public:
    /// 0x08a415a8
    cvMinimapRenderer();
    /// 0x08a41710
    UNK_RET Startup(INISettings const& a1);
    /// 0x08a41a62
    UNK_RET StartupGame(unsigned int a1, unsigned int a2, cvMinimapData* a3);
    /// 0x08a41dfa
    UNK_RET ShutdownGame();
    /// 0x08a41dfc
    UNK_RET Shutdown();
    /// 0x08a4207c
    void RenderMiniMap(unsigned int uiDirtyFlags);
    /// 0x08a42920
    void DeriveCameraParams();
    /// 0x08a42ed8
    void BuildHexMeshes(FFastVector<FGXVector2, true, 63u, 0u, FTempHeapFastVectorAllocator>& kVertices, FFastVector<unsigned int, true, 63u, 1u, FTempHeapFastVectorAllocator>& kIndices);
    /// 0x08a43900
    void ConstructHexBuffersAndSurfaces(FFastVector<FGXVector2, true, 63u, 0u, FTempHeapFastVectorAllocator>& kVertices, FFastVector<unsigned int, true, 63u, 1u, FTempHeapFastVectorAllocator>& kIndices);
    /// 0x08a43f46
    UNK_RET BuildSprites();
    /// 0x08a44860
    UNK_RET CreateCameraRectGeometry();
    /// 0x08a44d32
    UNK_RET SetupGlobals();
    /// 0x08a44dea
    UNK_RET SetupCamera(FGXVector2 const& vMin, FGXVector2 const& vMax, FireGrafix::CCommandStream* pStream);
    /// 0x08a44f08
    UNK_RET BindTextures(FireGrafix::FGXSurface* pSurface);
    /// 0x08a44f6a
    UNK_RET RenderIce();
    /// 0x08a4521e
    UNK_RET RenderRivers();
    /// 0x08a45304
    UNK_RET RenderSprites();
    /// 0x08a45360
    UNK_RET RenderCameraRects();
    /// 0x08a45424
    UNK_RET GetClickedLocation(float a1, float a2, FGXVector2& a3, bool a4);
    /// 0x08a4553c
    UNK_RET PushTessellatedHexToMesh(HEXVec_i const& a1, unsigned int a2, bool a3, FFastVector<FGXVector2, true, 63u, 0u, FTempHeapFastVectorAllocator>& a4, FFastVector<unsigned int, true, 63u, 1u, FTempHeapFastVectorAllocator>& a5);
    /// 0x08a458b8
    MinimapBufferLock GetNewVB(unsigned int uiIndex);
    /// 0x08a45962
    MinimapBufferLock GetNewIB(unsigned int uiIndex);
    /// 0x08a45a0c
    UNK_RET IsHexVisible(FGXVector2 const& a1);
    /// 0x08a45a98
    UNK_RET PushHexToMesh(HEXVec_i const& a1, FFastVector<FGXVector2, true, 63u, 0u, FTempHeapFastVectorAllocator>& a2, FFastVector<unsigned int, true, 63u, 1u, FTempHeapFastVectorAllocator>& a3);
    /// 0x08a45bca
    UNK_RET GetSplineForEdge(HEXVec_i const& a1, unsigned int a2, unsigned int a3, bool a4);
    /// 0x08a4610e
    UNK_RET EvaluateSpline(GameSpline::Segment const* a1, float a2);
    /// 0x08a46180
    UNK_RET IsHexVisible(FGXInt2 const& a1);
    /// 0x08a46256
    UNK_RET IsHexVisible(HEXVec_i const& a1);
    /// 0x08a462d6
    UNK_RET GetSplineSegment(HEXVec_i const& a1, unsigned int a2, bool a3, GameSpline::Segment* a4);
    /// 0x09225744
    static UNK_DATA RECT_IB[0x60];

    unsigned int m_hRenderTexture;                  ///< +0x0
    // Non-zero when the sets draw into a separate target that is resolved into m_hRenderTexture at the end.
    unsigned int m_hResolveSource;                  ///< +0x4
    FireGrafix::CCommandStream* m_pCommandStream;   ///< +0x8
    unsigned char unk_c[0x4];                       ///< +0xc
    unsigned int m_hShaderInstance;                 ///< +0x10
    unsigned char unk_14[0x10];                     ///< +0x14
    FireGrafix::MiniMapDynamic* m_pDynamicConstants; ///< +0x24
    unsigned char unk_28[0x14];                     ///< +0x28
    unsigned int m_hGrassTexture;                   ///< +0x3c
    unsigned int m_hWaterTexture;                   ///< +0x40
    unsigned int m_hFogTexture;                     ///< +0x44
    unsigned int m_hCultureTexture;                 ///< +0x48
    unsigned int m_hShallowWaterTexture;            ///< +0x4c
    unsigned int m_hMountainTexture;                ///< +0x50
    unsigned int m_hIceTexture;                     ///< +0x54
    MinimapHandleArray m_kVertexBuffers;            ///< +0x58
    MinimapHandleArray m_kIndexBuffers;             ///< +0x64
    MinimapHandleArray m_kSurfaces;                 ///< +0x70
    MinimapCultureGroupArray m_kCultureGroups;      ///< +0x7c
    MinimapMeshRange m_kFogRange;                   ///< +0x88
    MinimapMeshRange m_kLandRange;                  ///< +0xa0
    MinimapMeshRange m_kCoastRange;                 ///< +0xb8
    MinimapHandleArray m_kSpriteVertexBuffers;      ///< +0xd0
    MinimapHandleArray m_kSpriteIndexBuffers;       ///< +0xdc
    MinimapSpriteSurfaceArray m_kSpriteSurfaces;    ///< +0xe8
    unsigned int m_hCameraRectVB;                   ///< +0xf4
    unsigned int m_hCameraRectIB;                   ///< +0xf8
    float m_fAspect;                                ///< +0xfc
    unsigned int m_uiWidth;                         ///< +0x100
    unsigned int m_uiHeight;                        ///< +0x104
    unsigned int m_hViewport;                       ///< +0x108
    unsigned int m_uiViewportX;                     ///< +0x10c
    unsigned int m_uiViewportWidth;                 ///< +0x110
    float m_fViewportAspect;                        ///< +0x114
    float m_fCameraRectThickness;                   ///< +0x118
    float m_fCameraRectThreshold;                   ///< +0x11c
    float m_fExploredRegionGutter;                  ///< +0x120
    unsigned int m_uiEdgeSubdivisions;              ///< +0x124
    float m_fEdgeStep;                              ///< +0x128
    FGXVector2 m_akViewMin[3];                      ///< +0x12c
    FGXVector2 m_akViewMax[3];                      ///< +0x144
    unsigned int m_uiNumViews;                      ///< +0x15c
    unsigned char unk_160[0x140];                   ///< +0x160
    FGXVector2 m_vWorldMin;                         ///< +0x2a0
    FGXVector2 m_vWorldMax;                         ///< +0x2a8
    cvMinimapData* m_pData;                         ///< +0x2b0
    FGXVector2 m_akHexCorner[6];                    ///< +0x2b4
    FGXVector2 m_vCornerMin;                        ///< +0x2e4
    FGXVector2 m_vCornerMax;                        ///< +0x2ec
    float m_fHexRadius;                             ///< +0x2f4
    float m_fFogAlpha;                              ///< +0x2f8
};
