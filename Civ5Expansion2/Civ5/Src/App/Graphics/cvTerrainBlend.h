/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "Civ5/Src/App/Graphics/CellBlitJob.h"
#include "Civ5/Src/App/Graphics/cvMinimapRenderer.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"

class HexPatternCell;
class SquareGridPolicy;
class TerrainClassGrid;
class TerrainDecalLayer;
template <class T0, class T1, unsigned int T2, unsigned int T3> class DynamicHexGrid;

/// 0x0891b3fa
UNK_RET Fun_0891b3fa(UNK_PARAMS);

/// 0x0891b618
UNK_RET Fun_0891b618(UNK_PARAMS);

/// 0x08683154
UNK_RET Fun_08683154(UNK_PARAMS);

/// 0x08665a1a
UNK_RET Fun_08665a1a(UNK_PARAMS);

/// size 0x1e0
class cvTerrainBlend : public FJobSet {
public:
    /// 0x089180ec
    cvTerrainBlend(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& a1);
    /// 0x08918168
    ~cvTerrainBlend();
    /// 0x08918194
    UNK_RET LoadTerrainPieces(bool a1, char const* a2, unsigned long* a3, unsigned int a4);
    /// 0x08918438
    UNK_RET ClearTerrainPieces();
    /// 0x089184a4
    bool BlendTerrainHeightmap(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>& grid, FGXInt2 mapSize, FFastVector<FGXVector2, true, 43u, 259u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> const& a3, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& a4, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& a5, TerrainDecalLayer& a6);
    /// 0x08918592
    bool LaunchBlendTerrainJobs(FGXInt2 mapSize, FFastVector<FGXVector2, true, 43u, 259u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> const& kSplinePoints, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& kSplinesA, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& kSplinesB, TerrainClassGrid const& classGrid);
    /// 0x08919d3e
    bool LaunchBlendTerrainJobsByRow(FGXInt2 mapSize, FFastVector<FGXVector2, true, 43u, 259u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> const& kSplinePoints, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& kSplinesA, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& kSplinesB, TerrainClassGrid const& classGrid);

    cvCompressedTerrainPiecesLibrary* m_pPiecesLibrary;  ///< +0x4
    HexPatternMatcher m_matcher;   ///< +0x8
    unsigned int m_uiNumPieceIDs;  ///< +0x1cc
    unsigned int field_1d0;        ///< +0x1d0
    unsigned int field_1d4;        ///< +0x1d4
    unsigned int field_1d8;        ///< +0x1d8
    float field_1dc;               ///< +0x1dc
};
