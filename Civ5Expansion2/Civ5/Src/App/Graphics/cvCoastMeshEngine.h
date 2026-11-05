/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FTempHeap.h"
#include "Civ5/Src/App/TerrainStencil.h"
#include "Civ5/Src/App/Graphics/cvCoastSplineSystem.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"

class CoastlineSpline;
class DecalVertex;
class FGXInt2;
class TerrainData;
class TerrainDecalLayer;

// A run of points in the point list of GenerateAllCoastalRoads, laid out as one large road.
/// size 0xc
class CoastalRoad {
public:
    unsigned int m_uiFirstPoint; ///< +0x0
    unsigned int m_uiNumPoints;  ///< +0x4
    bool field_8;                ///< +0x8
    char unk_9[3];               ///< +0x9
};

/// size 0x14
class CoastStencilSurface {
public:
    TerrainStencilSurface m_Surface; ///< +0x0
    unsigned char m_ucStencilBits;   ///< +0x10
    char unk_11[3];                  ///< +0x11
};

/// size 0x24
class CoastStencilMesh {
public:
    // An FFastVector; its template arguments are not recoverable.
    CoastStencilSurface* m_pSurfaces; ///< +0x0
    unsigned int m_uiNumSurfaces;     ///< +0x4
    unsigned int m_uiMaxSurfaces;     ///< +0x8
    char unk_c[0x18];                 ///< +0xc
};

/// size 0x14
class CoastMeshVertex {
public:
    FGXVector2 m_vPos;             ///< +0x0
    // Index into TerrainData::m_aHexCode; 0xff when the style has no terrain for the strip.
    unsigned char m_ucTerrainIndex; ///< +0x8
    char unk_9[3];                 ///< +0x9
    float m_fHeight;               ///< +0xc
    // Blended CoastStyleLevel::m_fValue: 0 on the outer levels, 1 on the inner three.
    float field_10;                ///< +0x10
};

// A run of whole segments; GenerateFinalMesh starts at 64 triangles and adds 8 while the batch count is odd and above 1.
/// size 0x14
class CoastMeshBatch {
public:
    unsigned int m_uiNumVertices;   ///< +0x0
    unsigned int m_uiNumTriangles;  ///< +0x4
    char unk_8[4];                  ///< +0x8
    CoastMeshVertex* m_pVertices;   ///< +0xc
    unsigned short* m_pIndices;     ///< +0x10
};

// Four strips between the five offset levels; each spline sample adds 8 vertices, each segment 24 indices.
/// size 0x30
class CoastMesh {
public:
    FFastVector<CoastMeshVertex, true, 43u, 267u, FTempHeapFastVectorAllocator> m_aVertices; ///< +0x0
    FFastVector<unsigned short, true, 43u, 267u, FTempHeapFastVectorAllocator> m_aIndices;   ///< +0xc
    // Eight per segment: (0,0,0), (1,0,0), four (1,1,1), (0,0,1), (0,0,0); BlendAllCoastMeshes hands it to its jobs.
    FFastVector<FGXVector3, true, 43u, 267u, FTempHeapFastVectorAllocator> field_18;          ///< +0x18
    FFastVector<CoastMeshBatch, true, 43u, 267u, FTempHeapFastVectorAllocator> m_aBatches;   ///< +0x24
};

/// size ?
class cvCoastMeshEngine {
public:
    /// 0x088e6df4
    UNK_RET GenerateAllLevelSplines();
    /// 0x088e7046
    UNK_RET GenerateAllCoastStencil(FFastVector<CoastStencilMesh, false, 43u, 267u, FTempHeapFastVectorAllocator>& a1);
    /// 0x088e7306
    UNK_RET GenerateAllCoastalRoads(FFastVector<FGXVector2, true, 43u, 267u, FTempHeapFastVectorAllocator>& a1, FFastVector<CoastalRoad, true, 43u, 267u, FTempHeapFastVectorAllocator>& a2);
    /// 0x088e7704
    UNK_RET GenerateAllCoastBlendMeshes(FFastVector<CoastMesh, false, 43u, 267u, FTempHeapFastVectorAllocator>& a1);
    /// 0x088e7918
    UNK_RET GenerateLevels(FinalLevelSplines& kLevels, CoastlineSpline const& kSpline) const;
    /// 0x088e8614
    UNK_RET GenerateFinalMesh(CoastMesh& kMesh, FinalLevelSplines const& kLevels, unsigned int uiContinent) const;
    /// 0x088e8d14
    UNK_RET GenerateForestLayoutStencilMesh(CoastStencilMesh& a1, FinalLevelSplines const& a2) const;
    /// 0x088e8ff4
    UNK_RET BlendAllCoastMeshes(FGXInt2 a1, unsigned short a2, FFastVector<CoastMesh, false, 43u, 267u, FTempHeapFastVectorAllocator>& a3, TerrainData const* a4) const;

    bool m_bLibraryLoaded;                       ///< +0x0
    char unk_1[3];                               ///< +0x1
    cvCoastMeshLibrary m_CoastMeshLibrary;       ///< +0x4
    FFastVector<FinalLevelSplines, false, 43u, 259u, BaseVector<FinalLevelSplines, false>::FDefaultFastVectorAllocator> m_aFinalLevelSplines; ///< +0x20
    cvCoastSplineSystem m_CoastSplineSystem;     ///< +0x2c
};

// Out-of-line implicit constructor and destructor of cvCoastMeshEngine (not exported).
/// 0x08683292
UNK_RET Fun_08683292(cvCoastMeshEngine* pEngine);
/// 0x086839b8
UNK_RET Fun_086839b8(cvCoastMeshEngine* pEngine);

// Two triangles of one strip quad: vertices i, i+1 on this sample and i+8, i+9 on the next one.
/// 0x092142b0
extern unsigned int const g_aCoastQuadIndices[6];

// Copy of the style whose m_uiStyleIndex is uiStyle (zeroed if none), terrain types mapped to TerrainData indices.
/// 0x088e96b4
CoastStyleDef Fun_088e96b4(cvCoastMeshLibrary const& kLibrary, unsigned int uiStyle, unsigned int uiContinent);

/// 0x088e948d
UNK_RET CreateDebugDecals(CoastMesh& a1);
/// 0x088e97f6 SplitAndSubmitDecals<DecalVertex*, unsigned short*>
/// 0x088edf4b SplitAndSubmitDecals<FFastVector<DecalVertex, false, 0u, 0u, BaseVector<DecalVertex, false>::FDefaultFastVectorAllocator>, FFastVector<unsigned short, false, 0u, 0u, BaseVector<unsigned short, false>::FDefaultFastVectorAllocator> >
template <class T0, class T1>
void SplitAndSubmitDecals(TerrainDecalLayer& a1, T0 a2, T1 a3, unsigned int a4);
