/// origin guess
#include <string.h>
#include "Civ5/Src/App/Graphics/InitGridJob.h"

namespace {

// HexPatternCell::m_uFlags: every piece class except coast, ocean, reef and the two gibraltar classes is land.
const unsigned int kLandFlags = 0xf3ef9fff;
// Coast, reef and the gibraltar classes make shallow water; plain ocean and an empty cell are deep.
const unsigned int kShallowFlags = 0x0c102000;
const unsigned int kWaterKindFlags = 0x00106000;

const unsigned int kFlagGrass = 0x1;
const unsigned int kFlagPlains = 0x2;
const unsigned int kFlagDesert = 0x4;
const unsigned int kFlagTundra = 0x8;
const unsigned int kFlagSnow = 0x10;
const unsigned int kFlagGrassHills = 0x20;
const unsigned int kFlagPlainsHills = 0x40;
const unsigned int kFlagDesertHills = 0x80;
const unsigned int kFlagTundraHills = 0x100;
const unsigned int kFlagSnowHills = 0x200;
const unsigned int kFlagMountain = 0x400;

const unsigned char kClassLand = 0;
const unsigned char kClassShallow = 1;
const unsigned char kClassDeep = 2;
const unsigned char kKindMountain = 0xb;
const unsigned char kKindWater = 0xc;
// Row stride of CoastGrid::m_aCells.
const int kCoastRowStride = 0xff;

}  // namespace

/// 0x088e6b3c
void InitGridJob::Execute(unsigned int uiThreadIndex) {
    // A cell outside the game grid keeps the fields of the previous one except the class.
    CoastSplineHex kCell;
    memset(&kCell, 0, sizeof(kCell));
    DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const* pGameGrid = ms_pGameGrid;

    for (int iCol = 0; iCol < m_iWidth; ++iCol) {
        kCell.m_ucTerrainClass = kClassDeep;
        if (iCol < pGameGrid->m_iWidth && m_iRow >= 0 && m_iRow < pGameGrid->m_iHeight) {
            const unsigned int uFlags = pGameGrid->m_pCells[pGameGrid->m_iHeight * iCol + m_iRow].m_uFlags;
            if ((uFlags & kLandFlags) != 0) {
                kCell.m_ucTerrainClass = kClassLand;
                kCell.m_ucLandKind = 0;
            } else {
                if ((uFlags & kShallowFlags) != 0) {
                    kCell.m_ucTerrainClass = kClassShallow;
                }
                if ((uFlags & kWaterKindFlags) != 0) {
                    kCell.m_ucLandKind = kKindWater;
                }
            }
            for (int i = 0; i < 3; ++i) {
                kCell.m_aucMountainEdge[i] = 0;
                kCell.m_aucEdgeKind[i] = 0;
            }

            if ((uFlags & kFlagMountain) != 0) {
                kCell.m_ucLandKind = kKindMountain;
                for (int i = 0; i < 3; ++i) {
                    kCell.m_aucMountainEdge[i] = 2;
                }
            } else if ((uFlags & kFlagGrass) != 0) {
                kCell.m_ucLandKind = 1;
            } else if ((uFlags & kFlagGrassHills) != 0) {
                kCell.m_ucLandKind = 2;
            } else if ((uFlags & kFlagPlains) != 0) {
                kCell.m_ucLandKind = 3;
            } else if ((uFlags & kFlagPlainsHills) != 0) {
                kCell.m_ucLandKind = 4;
            } else if ((uFlags & kFlagSnow) != 0) {
                kCell.m_ucLandKind = 5;
            } else if ((uFlags & kFlagSnowHills) != 0) {
                kCell.m_ucLandKind = 6;
            } else if ((uFlags & kFlagDesert) != 0) {
                kCell.m_ucLandKind = 7;
            } else if ((uFlags & kFlagDesertHills) != 0) {
                kCell.m_ucLandKind = 8;
            } else if ((uFlags & kFlagTundra) != 0) {
                kCell.m_ucLandKind = 9;
            } else if ((uFlags & kFlagTundraHills) != 0) {
                kCell.m_ucLandKind = 10;
            }
        }

        int iDestCol = iCol;
        if (ms_pCoastGrid->field_9c) {
            iDestCol = iCol % ms_pCoastGrid->m_iWidth;
        }
        ms_pCoastGrid->m_aCells[iDestCol + m_iRow * kCoastRowStride] = kCell;
    }
}
