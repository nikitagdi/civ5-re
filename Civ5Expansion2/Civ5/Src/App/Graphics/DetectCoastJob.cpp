/// origin guess
#include <Windows.h>
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/DetectCoastJob.h"
#include "Civ5/Src/App/Graphics/Terrain.h"

namespace {

const unsigned char kClassLand = 0;
const unsigned char kClassShallow = 1;
const unsigned char kClassDeep = 2;
const unsigned char kKindWater = 0xc;
// Edge id bases: shore from the land cell, shore from the shallow cell, shelf edge from the shallow cell, shelf edge from the deep cell.
const unsigned char kEdgeShoreLand = 0;
const unsigned char kEdgeShoreShallow = 6;
const unsigned char kEdgeShelfShallow = 12;
const unsigned char kEdgeShelfDeep = 18;

}  // namespace

/// 0x088e6484
void DetectCoastJob::Execute(unsigned int uiThreadIndex) {
    // The coast edge between a cell and a neighbour in hex direction iDirection; false when the two classes share none.
    auto ClassifyEdge = [](CoastSplineHex const& kCell, CoastSplineHex const& kNeighbor, int iDirection, unsigned char& ucId, unsigned char& ucKind) -> bool {
        if (kCell.m_ucTerrainClass == kClassDeep) {
            if (kNeighbor.m_ucTerrainClass == kClassShallow) {
                ucId = kEdgeShelfDeep + iDirection;
                ucKind = kKindWater;
                return true;
            }
        } else if (kCell.m_ucTerrainClass == kClassShallow) {
            if (kNeighbor.m_ucTerrainClass == kClassLand) {
                ucId = kEdgeShoreShallow + iDirection;
                ucKind = kNeighbor.m_ucLandKind;
                return true;
            }
            if (kNeighbor.m_ucTerrainClass == kClassDeep) {
                ucId = kEdgeShelfShallow + iDirection;
                ucKind = kKindWater;
                return true;
            }
        } else if (kCell.m_ucTerrainClass == kClassLand) {
            if (kNeighbor.m_ucTerrainClass == kClassShallow) {
                ucId = kEdgeShoreLand + iDirection;
                ucKind = kCell.m_ucLandKind;
                return true;
            }
        }
        return false;
    };

    // Same test as the shared grid helper at 0x087bab18, on a hex already converted to column and row.
    auto IsOffsetInGrid = [](CoastGrid const& kGridToTest, HEXVec_i const& kOffset) -> bool {
        if (!kGridToTest.field_9c) {
            const int iNeighborCol = (kOffset.y >> 1) + kOffset.x;
            if (iNeighborCol < 0 || iNeighborCol >= kGridToTest.m_iWidth || kOffset.y < 0) {
                return false;
            }
        } else if (kOffset.y < 0) {
            return false;
        }
        return kOffset.y < kGridToTest.m_iHeight;
    };
    CoastGrid const& kGrid = *ms_pCoastGrid;
    const int iRow = m_iRow;

    for (int iCol = 0; iCol < m_iWidth; ++iCol) {
        const HEXVec_i kCellOffset = {iCol - (iRow >> 1), iRow, -(iCol - (iRow >> 1) + iRow)};
        const HEXVec_i kHex = Fun_086616f0(kGrid.m_mxInverse, kCellOffset);
        const int iHexZ = -(kHex.y + kHex.x);
        CoastSplineHex* pCell = Fun_088e5718(kGrid, iCol, iRow);
        const D3DXVECTOR4 kHexF = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), static_cast<float>(iHexZ), 1.0f};
        D3DXVECTOR4 kTex;
        D3DXVec4Transform(&kTex, &kHexF, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));

        // Directions 0..2: the side's geometry is kept in this cell.
        for (int iDirection = 0; iDirection < 3; ++iDirection) {
            const HEXVec_i kNeighbor = {kHex.x + HEXVec_i::TRANSLATIONS[iDirection].x, kHex.y + HEXVec_i::TRANSLATIONS[iDirection].y,
                                        HEXVec_i::TRANSLATIONS[iDirection].z - (kHex.y + kHex.x)};
            const HEXVec_i kNeighborOffset = Fun_086616f0(kGrid.m_mxForward, kNeighbor);
            if (!IsOffsetInGrid(kGrid, kNeighborOffset)) {
                continue;
            }
            CoastSplineHex const* pNeighbor = Fun_088e5718(kGrid, kNeighborOffset.x + (kNeighborOffset.y >> 1), kNeighborOffset.y);
            unsigned char ucId;
            unsigned char ucKind;
            const unsigned char ucSlot = pCell->m_ucNumEdges;
            if (ClassifyEdge(*pCell, *pNeighbor, iDirection, ucId, ucKind)) {
                CoastEdgeSegment const& kTemplate = ms_pCoastData[iDirection];
                pCell->m_aucEdges[ucSlot] = ucId;
                pCell->m_aSegments[ucSlot].m_vFirst.x = kTemplate.m_vFirst.x + kTex.x;
                pCell->m_aSegments[ucSlot].m_vFirst.y = kTemplate.m_vFirst.y + kTex.y;
                pCell->m_aSegments[ucSlot].m_vSecond.x = kTemplate.m_vSecond.x + kTex.x;
                pCell->m_aSegments[ucSlot].m_vSecond.y = kTemplate.m_vSecond.y + kTex.y;
                pCell->m_aucEdgeKind[ucSlot] = ucKind;
                ++pCell->m_ucNumEdges;
            } else {
                pCell->m_aSegments[ucSlot].m_vFirst.x = 0.0f;
                pCell->m_aSegments[ucSlot].m_vFirst.y = 0.0f;
                pCell->m_aSegments[ucSlot].m_vSecond.x = 0.0f;
                pCell->m_aSegments[ucSlot].m_vSecond.y = 0.0f;
            }
        }

        // Directions 3..5: the neighbour keeps the geometry, this cell only lists the edge.
        for (int iDirection = 3; iDirection < 6; ++iDirection) {
            const HEXVec_i kNeighbor = {kHex.x + HEXVec_i::TRANSLATIONS[iDirection].x, kHex.y + HEXVec_i::TRANSLATIONS[iDirection].y,
                                        HEXVec_i::TRANSLATIONS[iDirection].z - (kHex.y + kHex.x)};
            const HEXVec_i kNeighborOffset = Fun_086616f0(kGrid.m_mxForward, kNeighbor);
            if (!IsOffsetInGrid(kGrid, kNeighborOffset)) {
                continue;
            }
            CoastSplineHex const* pNeighbor = Fun_088e5718(kGrid, kNeighborOffset.x + (kNeighborOffset.y >> 1), kNeighborOffset.y);
            unsigned char ucId;
            unsigned char ucKind;
            if (ClassifyEdge(*pCell, *pNeighbor, iDirection, ucId, ucKind)) {
                pCell->m_aucBackEdges[pCell->m_ucNumBackEdges] = ucId;
                ++pCell->m_ucNumBackEdges;
            }
        }
    }
}
