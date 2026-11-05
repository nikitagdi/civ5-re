/// origin guess
#pragma once
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "Civ5/Src/App/HEXTransform_i.h"

class HEXVec_i;
class HexPatternCell;
class SquareGridPolicy;
template <class T0, class T1, unsigned int T2, unsigned int T3> class DynamicHexGrid;

/// 0x08675cb4
UNK_RET Fun_08675cb4(UNK_PARAMS);

/// size 0xfea0
class TerrainClassGrid {
public:
    /// 0x088e3dd4
    UNK_RET Init(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& source);

    HEXMatrix_i m_matrix;                ///< +0x0
    HEXMatrix_i m_invMatrix;             ///< +0x40
    HEXTransform_i m_transform;          ///< +0x80
    int m_iWidth;                        ///< +0x94
    int m_iHeight;                       ///< +0x98
    bool m_bWrap;                        ///< +0x9c
    unsigned char m_aucClass[0xfe01];    ///< +0x9d
    unsigned char m_ucOutOfRangeClass;   ///< +0xfe9e
};

// Class byte of the hex; the out-of-range byte when the hex is off the grid.
/// 0x0891b884
unsigned char const* Fun_0891b884(TerrainClassGrid const* a1, HEXVec_i const* a2);
// Class byte at the offset coordinates, wrapped in x when the grid wraps.
/// 0x0891ba38
unsigned char const* Fun_0891ba38(TerrainClassGrid const* a1, int a2, int a3);
