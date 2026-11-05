/// origin guess
#include "Civ5/Src/App/Graphics/TerrainClassGrid.h"
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/Graphics/Terrain.h"

static const unsigned int kZeroClassMask = 0xf3ef9fff;
static const unsigned int kSplitClassMask = 0x0c102000;

/// 0x088e3dd4
void TerrainClassGrid::Init(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& source) {
    m_transform.field_0 = source.m_kTransform.field_0;
    m_transform.field_4 = source.m_kTransform.field_4;
    m_transform.field_8 = source.m_kTransform.field_8;
    m_matrix = m_transform.matrix();
    m_invMatrix = m_transform.inv_matrix();
    int sourceWidth = source.m_iWidth;
    int sourceHeight = source.m_iHeight;
    const HexPatternCell* cells = source.m_pCells;
    m_iWidth = sourceWidth;
    m_iHeight = sourceHeight;
    for (int x = 0; x < sourceWidth; ++x) {
        for (int y = 0; y < sourceHeight; ++y) {
            unsigned int flags = cells[sourceHeight * x + y].m_uFlags;
            unsigned char terrainClass;
            if ((flags & kZeroClassMask) == 0) {
                terrainClass = ((flags & kSplitClassMask) == 0) + 1;
            } else {
                terrainClass = 0;
            }
            int column = x;
            if (m_bWrap) {
                column = x % m_iWidth;
            }
            m_aucClass[y * 0xff + column] = terrainClass;
        }
    }
}

/// 0x0891b884
unsigned char const* Fun_0891b884(TerrainClassGrid const* a1, HEXVec_i const* a2) {
    const HEXVec_i kOffset = Fun_086616f0(a1->m_matrix, *a2);
    return Fun_0891ba38(a1, kOffset.x + (kOffset.y >> 1), kOffset.y);
}

/// 0x0891ba38
unsigned char const* Fun_0891ba38(TerrainClassGrid const* a1, int a2, int a3) {
    if ((!a1->m_bWrap && (a2 < 0 || a2 >= a1->m_iWidth)) || a3 < 0 || a3 >= a1->m_iHeight) {
        return &a1->m_ucOutOfRangeClass;
    }
    if (a1->m_bWrap) {
        while (a2 < 0) {
            a2 += a1->m_iWidth;
        }
        a2 %= a1->m_iWidth;
    }
    return &a1->m_aucClass[a3 * 0xff + a2];
}
