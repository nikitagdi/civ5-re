/// origin guess
#pragma once
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "FirePlace/FireGrafix/FGXVector2.h"

class HEXVec;
class HexPatternCell;
class SquareGridPolicy;
template <class T0, class T1, unsigned int T2, unsigned int T3> class DynamicHexGrid;

// Part of a hex side in texture space: the middle half of the side, first point nearer one corner.
/// size 0x10
struct CoastEdgeSegment {
    FGXVector2 m_vFirst;   ///< +0x0
    FGXVector2 m_vSecond;  ///< +0x8
};

// Where the coastline continues from a segment: a cell by column and row, and an edge slot of it (0xff: none).
/// size 0xc
struct CoastEdgeLink {
    bool m_bMarked;              ///< +0x0
    unsigned char m_ucPeerEdge;  ///< +0x1
    int m_iCol;                  ///< +0x4
    int m_iRow;                  ///< +0x8
};

// One hex of the coast grid. Edge ids: 0..5 shore seen from land, 6..11 shore seen from shallow water,
// 12..17 shelf edge seen from shallow water, 18..23 shelf edge seen from deep water; id % 6 is the neighbour direction.
// The edge slots hold the three sides toward neighbours 0..2 that carry segment geometry; the back slots
// list the three sides toward neighbours 3..5, whose segment belongs to the neighbour.
/// size 0x88
struct CoastSplineHex {
    CoastEdgeLink m_aNextLinks[3];        ///< +0x0
    CoastEdgeLink m_aPrevLinks[3];        ///< +0x24
    CoastEdgeSegment m_aSegments[3];      ///< +0x48
    unsigned char m_ucTerrainClass;       ///< +0x78
    unsigned char m_aucMountainEdge[3];   ///< +0x79
    unsigned char m_ucLandKind;           ///< +0x7c
    unsigned char m_aucEdgeKind[3];       ///< +0x7d
    unsigned char m_ucNumBackEdges;       ///< +0x80
    unsigned char m_aucBackEdges[3];      ///< +0x81
    unsigned char m_ucNumEdges;           ///< +0x84
    unsigned char m_aucEdges[3];          ///< +0x85
};

// A segment of the coastline: the hex, and the slot among that hex's edges (-1: none).
/// size 0x10
struct SplineHandle {
    HEXVec_i m_kHex;  ///< +0x0
    int m_iEdge;      ///< +0xc
};

// TerrainSystem::Init allocates it from the temp heap (0x86f220 bytes).
/// size 0x86f220
class CoastGrid {
public:
    // The six control points of the spline through one segment go to pPoints; bEdgeSlots selects the edge slots
    // (true) or the back slots (false) of kCell, which resolve to the neighbour's segment.
    /// 0x088e3f3a
    void ReconstructCoastSpline(CoastSplineHex const& kCell, HEXVec_i kHex, unsigned int uiEdge, FGXVector2* pPoints, bool bEdgeSlots) const;
    // bForward: the next segment along the coastline; otherwise the previous one.
    /// 0x088e4554
    SplineHandle NextSplineInCoastline(SplineHandle kHandle, bool bForward) const;
    /// 0x088e4cd0
    void Init(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& kGameGrid);
    // Per edge slot (hex direction 0..2) the hex directions that touch the two ends of its side.
    /// 0x092141e4
    static unsigned int ms_CoastConnect[3][2];

    HEXMatrix_i m_mxForward;                   ///< +0x0
    HEXMatrix_i m_mxInverse;                   ///< +0x40
    HEXTransform_i m_kTransform;               ///< +0x80
    int m_iWidth;                              ///< +0x94
    int m_iHeight;                             ///< +0x98
    // Cleared by TerrainSystem::Init when WorldGlobals::g_eWrapMode is 0; set: columns wrap.
    bool field_9c;                             ///< +0x9c
    // Row-major with a fixed row stride of 255 cells.
    CoastSplineHex m_aCells[0xff * 0xff];      ///< +0xa0
    CoastSplineHex m_kOutsideCell;             ///< +0x86f128
    FGXVector2 m_vWrapShift;                   ///< +0x86f1b0
    CoastEdgeSegment m_aEdgeTemplates[3];      ///< +0x86f1b8
    FGXVector2 m_vOriginTex;                   ///< +0x86f1e8
    FGXVector2 m_avNeighborOffset[6];          ///< +0x86f1f0
};

// Out-of-line constructor of CoastGrid (not exported).
/// 0x08683328
UNK_RET Fun_08683328(CoastGrid* pGrid);

// The cell at a column and row; a position outside the grid gives m_kOutsideCell. Const grid, writable cell.
/// 0x088e5718
CoastSplineHex* Fun_088e5718(CoastGrid const& kGrid, int iCol, int iRow);
// The cell of a hex, through m_mxForward.
/// 0x088e56a2
CoastSplineHex* Fun_088e56a2(CoastGrid const& kGrid, HEXVec_i const& kHex);

/// 0x088e4acf
SplineHandle RefactorfoRVS(CoastSplineHex const& a1, unsigned int& a2, CoastSplineHex const& a3, SplineHandle& a4, HEXVec& a5, CoastSplineHex const& a6, unsigned int& a7, HEXVec& a8, bool& a9);

namespace ReplayMap { namespace Constants {
/// 0x0922363c
extern float fTileImageSize;
}}
