/// origin guess
#include <string.h>
#include "Civ5/Src/App/Graphics/cvMinimapData.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/HEXVec_i.h"

// The cvMinimapData instance lies 0x4d8 bytes after g_MinimapSystem (0x0c03d760).
static const unsigned int kMinimapDataOffset = 0x4d8;

/// 0x088a71a6
void cvMinimapSystem::OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const HexFOWStateChangedData& kData = reinterpret_cast<const HexFOWStateChangedData&>(kEvent);
    if ((m_ucDeferEvents & 1) != 0) {
        MinimapEventCallback kCallback = {this, nullptr, 0};
        void (cvMinimapSystem::*pfnHandler)(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>&) = &cvMinimapSystem::OnFOWChanged;
        memcpy(&kCallback.m_pfnHandler, &pfnHandler, sizeof(kCallback.m_pfnHandler));
        Fun_087508be(m_aucEventQueue, &kData, &kCallback, 0);
        return;
    }

    cvMinimapData& kMinimap = *reinterpret_cast<cvMinimapData*>(reinterpret_cast<unsigned char*>(g_MinimapSystem) + kMinimapDataOffset);
    cvMinimapData::PlayerData& kGrid = *kMinimap.GetPlayerData(kMinimap.m_eActivePlayer);

    if ((kData.m_ucState & 3) != 0) {
        if (kData.m_bWholeMap) {
            for (int iX = 0; iX < kGrid.m_iWidth; ++iX) {
                for (int iY = 0; iY < kGrid.m_iHeight; ++iY) {
                    // Everything explored: no unexplored bit and no fog edges left.
                    Fun_088a9b1c(&kGrid, iX, iY)->m_ucFog = 0;
                }
            }
            kGrid.m_iMaxX = kGrid.m_iWidth - 1;
            kGrid.m_iMaxY = kGrid.m_iHeight - 1;
            kGrid.m_iMinX = 0;
            kGrid.m_iMinY = 0;
            m_uiDirtyFlags |= MINIMAP_DIRTY_EXPLORED_REGION;
        } else {
            const int iHexX = kData.m_sHexX;
            const int iHexY = kData.m_sHexY;
            const HEXVec_i kHex = {iHexX, iHexY, -(iHexY + iHexX)};
            const HEXVec_i kOffset = Fun_086616f0(kGrid.m_mxForward, kHex);
            const int iColumn = (kOffset.y >> 1) + kOffset.x;
            const int iRow = kOffset.y;
            MinimapCell* pCell = Fun_088a9b1c(&kGrid, iColumn, iRow);
            pCell->m_ucFog &= 0xfe;

            if (kGrid.m_bNothingExplored) {
                kGrid.m_iAnchorX = iColumn;
                kGrid.m_iAnchorY = iRow;
                kGrid.m_iMinY = iRow;
                kGrid.m_iMinX = iColumn;
                kGrid.m_iMaxY = iRow;
                kGrid.m_iMaxX = iColumn;
                m_uiDirtyFlags |= MINIMAP_DIRTY_EXPLORED_REGION;
                kGrid.m_bNothingExplored = false;
            }
            if (kGrid.m_iMaxY <= iRow) {
                kGrid.m_iMaxY = iRow;
                m_uiDirtyFlags |= MINIMAP_DIRTY_EXPLORED_REGION;
            }
            if (iRow <= kGrid.m_iMinY) {
                kGrid.m_iMinY = iRow;
                m_uiDirtyFlags |= MINIMAP_DIRTY_EXPLORED_REGION;
            }
            if (!kMinimap.m_bWrapX) {
                if (iColumn < kGrid.m_iMinX) {
                    kGrid.m_iMinX = iColumn;
                    m_uiDirtyFlags |= MINIMAP_DIRTY_EXPLORED_REGION;
                }
                if (kGrid.m_iMaxX < iColumn) {
                    kGrid.m_iMaxX = iColumn;
                    m_uiDirtyFlags |= MINIMAP_DIRTY_EXPLORED_REGION;
                }
            } else if (iColumn < kGrid.m_iAnchorX) {
                // Wrapping map: extend the region on the side that costs less, counting a lap of the width.
                if (iColumn <= kGrid.m_iMinX) {
                    const int iLapped = kGrid.m_iWidth + iColumn;
                    if (kGrid.m_iMaxX <= iLapped) {
                        if (kGrid.m_iMinX - iColumn < iLapped - kGrid.m_iMaxX) {
                            kGrid.m_iMinX = iColumn;
                        } else {
                            kGrid.m_iMaxX = iLapped;
                        }
                        m_uiDirtyFlags |= MINIMAP_DIRTY_EXPLORED_REGION;
                    }
                }
            } else if (kGrid.m_iMaxX <= iColumn) {
                const int iLapped = iColumn - kGrid.m_iWidth;
                if (iLapped <= kGrid.m_iMinX) {
                    if (iColumn - kGrid.m_iMaxX < kGrid.m_iMinX - iLapped) {
                        kGrid.m_iMaxX = iColumn;
                    } else {
                        kGrid.m_iMinX = iLapped;
                    }
                    m_uiDirtyFlags |= MINIMAP_DIRTY_EXPLORED_REGION;
                }
            }

            // Side i of a hex is bit i + 2 of the edge mask; the neighbour sees the same edge as side (i + 3) % 6.
            for (int iSide = 0; iSide < 6; ++iSide) {
                const HEXVec_i kNeighborHex = {
                    HEXVec_i::TRANSLATIONS[iSide].x + iHexX,
                    HEXVec_i::TRANSLATIONS[iSide].y + iHexY,
                    HEXVec_i::TRANSLATIONS[iSide].z - (iHexY + iHexX)};
                MinimapCell* pNeighbor = Fun_088a9aa6(&kGrid, &kNeighborHex);
                const unsigned char ucSide = static_cast<unsigned char>(1 << (iSide + 2));
                const unsigned char ucOpposite = static_cast<unsigned char>(1 << ((iSide + 3) % 6 + 2));
                if (((pNeighbor->m_ucFog ^ pCell->m_ucFog) & 1) == 0) {
                    pCell->m_ucFog &= ~ucSide;
                    pNeighbor->m_ucFog &= ~ucOpposite;
                } else {
                    pCell->m_ucFog |= ucSide;
                    pNeighbor->m_ucFog |= ucOpposite;
                }
            }
        }
    }

    // Edge writes for hexes outside the grid land in the shared outside cell; clear them again.
    kGrid.m_kOutsideCell.m_ucFog &= 1;
    m_uiDirtyFlags |= MINIMAP_DIRTY_FOG;
}

/// 0x088a759a
void cvMinimapSystem::OnHexCultureChanged(GameCoreEventTemplate<Event_Int2Type<29>, HexCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const HexCultureChangedData& kData = reinterpret_cast<const HexCultureChangedData&>(kEvent);
    if ((m_ucDeferEvents & 1) != 0) {
        MinimapEventCallback kCallback = {this, nullptr, 0};
        void (cvMinimapSystem::*pfnHandler)(GameCoreEventTemplate<Event_Int2Type<29>, HexCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>&) = &cvMinimapSystem::OnHexCultureChanged;
        memcpy(&kCallback.m_pfnHandler, &pfnHandler, sizeof(kCallback.m_pfnHandler));
        Fun_088a9446(m_aucEventQueue, &kData, &kCallback, 0);
        return;
    }

    cvMinimapData& kMinimap = *reinterpret_cast<cvMinimapData*>(reinterpret_cast<unsigned char*>(g_MinimapSystem) + kMinimapDataOffset);
    const cvMinimapData::PlayerNode* pEnd = reinterpret_cast<const cvMinimapData::PlayerNode*>(&kMinimap.m_pPlayerListEndPrev);
    const int iHexX = kData.m_sHexX;
    const int iHexY = kData.m_sHexY;
    for (cvMinimapData::PlayerNode* pNode = kMinimap.m_pPlayerListFirst; pNode != pEnd; pNode = pNode->m_pNext) {
        const HEXVec_i kHex = {iHexX, iHexY, -(iHexY + iHexX)};
        MinimapCell* pCell = Fun_088a9aa6(&pNode->m_kData, &kHex);
        // Only land hexes carry culture; the stored owner is the player id plus one, 0 meaning none.
        if ((pCell->m_ucTerrain & 3) != 0) {
            continue;
        }
        pCell->m_ucCultureOwner = static_cast<unsigned char>(kData.m_iOwner + 1);
        for (int iSide = 0; iSide < 6; ++iSide) {
            const HEXVec_i kNeighborHex = {
                HEXVec_i::TRANSLATIONS[iSide].x + iHexX,
                HEXVec_i::TRANSLATIONS[iSide].y + iHexY,
                HEXVec_i::TRANSLATIONS[iSide].z - (iHexY + iHexX)};
            MinimapCell* pNeighbor = Fun_088a9aa6(&pNode->m_kData, &kNeighborHex);
            const unsigned char ucSide = static_cast<unsigned char>(1 << (iSide + 2));
            const unsigned char ucOpposite = static_cast<unsigned char>(1 << ((iSide + 3) % 6 + 2));
            if (pNeighbor->m_ucCultureOwner == pCell->m_ucCultureOwner) {
                pCell->m_ucCultureEdges &= ~ucSide;
                pNeighbor->m_ucCultureEdges &= ~ucOpposite;
            } else {
                pCell->m_ucCultureEdges |= ucSide;
                pNeighbor->m_ucCultureEdges |= ucOpposite;
            }
        }
        m_uiDirtyFlags |= MINIMAP_DIRTY_CULTURE;
    }
}
