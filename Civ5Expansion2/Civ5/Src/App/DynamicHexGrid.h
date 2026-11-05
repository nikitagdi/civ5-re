/// origin guess
#pragma once
#include "Civ5/Src/App/HEXTransform_i.h"
#include "FirePlace/FireGrafix/FGXInt2.h"

// One hex of the gameplay grid; cvTerrainSampler::SetContinentTypes copies m_ucContinent.
/// size 0x8
class HexPatternCell {
public:
    unsigned int m_uFlags;        ///< +0x0
    unsigned char m_ucContinent;  ///< +0x4
    // Set by HexPatternMatcher::FindLinearPacking when a placed piece takes the cell.
    bool m_bCovered;              ///< +0x5
    char unk_6[0x2];              ///< +0x6
};

// Hex-to-grid mapping and size of a square hex grid; WorldGlobals::g_GameGrid is one on its own.
/// size 0x9c
class SquareGridPolicy {
public:
    // World hex to grid cube coordinates; m_mxInverse maps back.
    HEXMatrix_i m_mxForward;     ///< +0x0
    HEXMatrix_i m_mxInverse;     ///< +0x40
    HEXTransform_i m_kTransform; ///< +0x80
    int m_iWidth;                ///< +0x94
    int m_iHeight;               ///< +0x98
};

// Constructs the policy at a1: identity matrices, then size a2 and transform a3, then both matrices from a3.
/// 0x08661752
UNK_RET Fun_08661752(SquareGridPolicy* a1, FGXInt2 a2, HEXTransform_i const& a3);
// Frees the cell array whose pointer is at a1.
/// 0x0865fdc6
UNK_RET Fun_0865fdc6(HexPatternCell** a1);

// Cells are column-major: m_pCells[x * m_iHeight + y].
/// size 0xa8 DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>
template <class T0, class T1, unsigned int T2, unsigned int T3>
class DynamicHexGrid : public T1 {
public:
    /// inline
    ~DynamicHexGrid() { Fun_0865fdc6(&m_pCells); }

    T0* m_pCells;               ///< +0x9c
    unsigned int m_uiNumCells;  ///< +0xa0
    unsigned int m_uiMaxCells;  ///< +0xa4
};

// Constructs the grid at a1: the policy from a2 and a4, then a2.x * a2.y copies of a3.
/// 0x0865f122
UNK_RET Fun_0865f122(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>* a1, FGXInt2 a2, HexPatternCell const& a3, HEXTransform_i const& a4);
