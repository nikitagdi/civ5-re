/// origin guess
#pragma once
#include "FirePlace/FireWorks/Diagram.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FTempHeap.h"
#include "FirePlace/FireGrafix/FGXVector2.h"

class CoastGrid;
class CoastlineSpline;
class TerrainData;
namespace FVoronoi {
class NullClass;
template <class T0, class T1, class T2, class T3, int T4, int T5, int T6, int T7> class Diagram;
}
namespace TerrainIce {
class CompareVoronoiVerts;
class CrackLineGraph;
class FractureMesh;
class IceInputGeometry;
class IceSettings;
class KDTree;
}

// The two style bytes are CoastStyle indices of the coast cells on both sides of the point; the weight blends them.
/// size 0x10
class CoastCP {
public:
    FGXVector2 m_vPos;          ///< +0x0
    float m_fBlendWeight;       ///< +0x8
    unsigned char m_ucStyleA;   ///< +0xc
    unsigned char m_ucStyleB;   ///< +0xd
    unsigned char unk_e[2];     ///< +0xe
};

// One coastline as a run of control points (a growing array), filled from the CoastGrid links.
/// size 0x2c
class CoastlineSpline {
public:
    CoastCP* m_pControlPoints;       ///< +0x0
    unsigned int m_uiNumPoints;      ///< +0x4
    unsigned int m_uiMaxPoints;      ///< +0x8
    unsigned char unk_c;             ///< +0xc
    unsigned char m_ucStartEdge;     ///< +0xd
    unsigned char unk_e[2];          ///< +0xe
    int m_iStartCol;                 ///< +0x10
    int m_iStartRow;                 ///< +0x14
    // True when the walk returned to its first segment; m_vWrapOffset is then last point minus second point.
    bool m_bClosed;                  ///< +0x18
    char unk_19[3];                  ///< +0x19
    FGXVector2 m_vWrapOffset;        ///< +0x1c
    int m_eType;                     ///< +0x24
    int m_iContinent;                ///< +0x28
};

// Wavelet pyramid of 2-D points used by the coastline noise: data holds the coarse and the detail coefficients
// of all levels side by side; m_iOffset is where the current level starts.
/// size 0x18
class CoastWavelet {
public:
    int m_iLevel;                    ///< +0x0
    int m_iOffset;                   ///< +0x4
    int m_iSize;                     ///< +0x8
    FGXVector2* m_pData;             ///< +0xc
    unsigned int m_uiNumData;        ///< +0x10
    unsigned int m_uiMaxData;        ///< +0x14
};

// The five offset levels of one coastline; each level is a [begin, end) run of m_pControlPoints.
/// size 0x38
class FinalLevelSplines {
public:
    unsigned int m_auiLevelRange[5][2]; ///< +0x0
    // An FFastVector with the default allocator; its pool and sub-id are not recoverable.
    CoastCP* m_pControlPoints;          ///< +0x28
    unsigned int m_uiNumControlPoints;  ///< +0x2c
    unsigned int m_uiMaxControlPoints;  ///< +0x30
    int m_eType;                        ///< +0x34
};

/// size 0x20
class cvCoastSplineSystem {
public:
    /// 0x088ec0f0
    cvCoastSplineSystem();
    /// 0x088ec158
    ~cvCoastSplineSystem();
    /// 0x088ec17c
    void Startup(CoastGrid const& kGrid);
    /// 0x088ec1b8
    void ModifyAndExtractCoastlines(CoastGrid const& kGrid);
    /// 0x088ec766
    UNK_RET Shutdown();
    /// 0x088ec7a2
    void ExtractSingleCoastlineFromGrid(CoastlineSpline& kSpline, CoastGrid const& kGrid) const;
    /// 0x088ecda6
    void ClearMarks(CoastGrid const& kGrid);
    /// 0x088ece2e
    void AddNoiseToSingleCoastline(CoastlineSpline& kSpline) const;
    /// 0x088ed400
    void AddAllCoastlinesToDatabase(CoastGrid const& kGrid, FFastVector<FinalLevelSplines, false, 43u, 259u, BaseVector<FinalLevelSplines, false>::FDefaultFastVectorAllocator> const* pFinalLevelSplines);
    /// 0x088ed458
    void AddSingleCoastlineToDatabase(CoastlineSpline const& kSpline, CoastGrid const& kGrid) const;
    /// 0x088ed744
    UNK_RET CreateCoastlineDecals();

    bool m_bStarted;                    ///< +0x0
    char unk_1[3];                      ///< +0x1
    int m_iGridWidth;                   ///< +0x4
    int m_iGridHeight;                  ///< +0x8
    // Texture-space size of one full grid width (x) and its row term (y); the shift per wrap of a column.
    FGXVector2 m_vWrapShift;            ///< +0xc
    CoastlineSpline* m_pSplines;        ///< +0x14
    unsigned int m_uiNumSplines;        ///< +0x18
    unsigned int m_uiMaxSplines;        ///< +0x1c
};

// Appends a control point to the spline.
/// 0x088edbc0
void Fun_088edbc0(CoastlineSpline& kSpline, CoastCP const& kPoint);
// The {data, size, capacity} triple that starts CoastlineSpline (+0x0) and sits at FinalLevelSplines +0x28.
/// size 0xc
struct CoastControlPointVector {
    CoastCP* m_pData;           ///< +0x0
    unsigned int m_uiSize;      ///< +0x4
    unsigned int m_uiCapacity;  ///< +0x8
};
// Makes room for uiNeeded splines in the array that starts at ppSplines.
/// 0x088ee572
void Fun_088ee572(CoastlineSpline** ppSplines, unsigned int uiNeeded);
// Reserves uiCapacity control points of the spline.
/// 0x088e9ff8
void Fun_088e9ff8(CoastControlPointVector* pVector, unsigned int uiCapacity);
// Destructor of a CoastlineSpline.
/// 0x0868a412
void Fun_0868a412(CoastlineSpline* pSpline);
// Builds a pyramid of iSize points, all filled from *pFill.
/// 0x088edc1a
void Fun_088edc1a(CoastWavelet* pWavelet, int iSize, FGXVector2 const* pFill);
// One decomposition step: the level counter goes down and m_iSize halves.
/// 0x088edcb2
void Fun_088edcb2(CoastWavelet* pWavelet);
// One reconstruction step: the level counter goes up and m_iSize doubles.
/// 0x088edd64
void Fun_088edd64(CoastWavelet* pWavelet);
// Reserves uiCapacity points of the pyramid data.
/// 0x088ea67c
void Fun_088ea67c(FGXVector2** ppData, unsigned int uiCapacity);
// Frees the pyramid data.
/// 0x08683534
void Fun_08683534(FGXVector2** ppData);
/// 0x088ed78f
UNK_RET CreateDebugDecals(CoastCP const* a1, unsigned int a2);

namespace FGXMeshUtils {
/// 0x088f42e0 WeldVertices<44, 5, unsigned int, FVoronoi::Diagram<FVoronoi::NullClass, FVoronoi::NullClass, FVoronoi::NullClass, FTempHeapFastVectorAllocator, 44, 2, 44, 3>::Vertex, TerrainIce::CompareVoronoiVerts>
template <int T0, int T1, class T2, class T3, class T4>
unsigned int WeldVertices(T3 const* a1, T4& a2, T2 a3, T2* a4);
}

namespace TerrainIce {
/// 0x088efa50
UNK_RET ConstructFractureMesh(TerrainIce::IceInputGeometry const& a1, TerrainIce::KDTree& a2, FVoronoi::Diagram<FVoronoi::NullClass, FVoronoi::NullClass, FVoronoi::NullClass, FTempHeapFastVectorAllocator, 44, 2, 44, 3> const& a3, TerrainIce::FractureMesh& a4);
/// 0x088f357c
UNK_RET ComputeVoronoiSiteCount(TerrainIce::IceSettings const& a1, TerrainIce::IceInputGeometry const& a2);
/// 0x088f35b6
UNK_RET GenerateVoronoiSites(TerrainIce::IceSettings const& a1, TerrainIce::IceInputGeometry const& a2, TerrainIce::KDTree const& a3, FVoronoi::Diagram<FVoronoi::NullClass, FVoronoi::NullClass, FVoronoi::NullClass, FTempHeapFastVectorAllocator, 44, 2, 44, 3>::Site* a4);
/// 0x088f38fc
UNK_RET CreateVoronoi(FVoronoi::Diagram<FVoronoi::NullClass, FVoronoi::NullClass, FVoronoi::NullClass, FTempHeapFastVectorAllocator, 44, 2, 44, 3>::Site* a1, unsigned int a2, FVoronoi::Diagram<FVoronoi::NullClass, FVoronoi::NullClass, FVoronoi::NullClass, FTempHeapFastVectorAllocator, 44, 2, 44, 3>& a3);
/// 0x088f84c8
UNK_RET DoCrackPropagation(float a1, TerrainIce::IceSettings const& a2, TerrainIce::FractureMesh& a3);
/// 0x088f8b24
UNK_RET BuildCrackLineGraph(TerrainIce::FractureMesh const& a1, TerrainIce::CrackLineGraph& a2);
}

// A coast character profile: 98 samples of (along, across) offsets, built by LoadSplineFromFile.
/// size 0xc
class CoastCharacterSpline {
public:
    FGXVector2* m_pSamples;     ///< +0x0
    unsigned int m_uiNumSamples; ///< +0x4
    unsigned int m_uiMaxSamples; ///< +0x8
};

// One of the five offset levels of a style, from the land side to the deep water side: -HighSide, -HighBeach.Offset,
// Waterline.Offset, LowBeach.Offset, LowSide. The value is 0 on the two outer levels and 1 on the inner three;
// the character is -1 on the outer levels.
/// size 0x10
class CoastStyleLevel {
public:
    float m_fOffset;       ///< +0x0
    float m_fValue;        ///< +0x4
    float m_fHeight;       ///< +0x8
    int m_iCharacter;      ///< +0xc
};

// One CoastStyle element of coaststyles.xml. Terrain types per strip between the levels: -1, HighBeach, LowBeach, -1.
/// size 0x64
class CoastStyleDef {
public:
    CoastStyleLevel m_aLevels[5];  ///< +0x0
    int m_aiTerrainType[4];        ///< +0x50
    unsigned int m_uiStyleIndex;   ///< +0x60
};

// Embedded in cvCoastMeshEngine at +0x4; UnLoadCoastDefinitions empties the vectors at +0x4 and +0x10.
/// size 0x1c
class cvCoastMeshLibrary {
public:
    /// 0x088ee7f4
    void LoadCoastDefinitions(TerrainData const& kTerrainData);
    // Returns the index of the profile it added.
    /// 0x088ef232
    int LoadSplineFromFile(char const* pszFile);
    /// 0x088ef644
    void UnLoadCoastDefinitions();

    TerrainData const* m_pTerrainData;     ///< +0x0
    // Two growing arrays: the profiles (+0x4) and the styles (+0x10).
    CoastCharacterSpline* m_pSplines;      ///< +0x4
    unsigned int m_uiNumSplines;           ///< +0x8
    unsigned int m_uiMaxSplines;           ///< +0xc
    CoastStyleDef* m_pStyles;              ///< +0x10
    unsigned int m_uiNumStyles;            ///< +0x14
    unsigned int m_uiMaxStyles;            ///< +0x18
};

// Appends a copy of the style to the array that starts at ppStyles.
/// 0x088ef658
void Fun_088ef658(CoastStyleDef** ppStyles, CoastStyleDef const* pStyle);
// Makes room for uiNeeded profiles in the array that starts at ppSplines.
/// 0x088ef866
void Fun_088ef866(CoastCharacterSpline** ppSplines, unsigned int uiNeeded);
// Reserves uiCapacity samples of the profile.
/// 0x088ef7c2
void Fun_088ef7c2(CoastCharacterSpline* pProfile, unsigned int uiCapacity);
// Appends a sample to the profile.
/// 0x088ef6aa
void Fun_088ef6aa(CoastCharacterSpline* pProfile, FGXVector2 const* pSample);
// Names matched against the type attribute of a CoastStyle element; the style index is the position.
/// 0x0a2f2dd4
extern char const* g_apszCoastStyleNames[13];
