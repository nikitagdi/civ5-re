/// origin guess
#include "Civ5/Src/App/Engine/cvGameplayRoadHandler.h"
#include <stdlib.h>
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/HEXVec.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include <string.h>

namespace {

// 1 / 256: the unit of RoadCell::m_aucVariation.
const float kVariationUnit = 0.00390625f;
// How far from the hex centre the city ring and the spoke ends lie, in hex units.
const float kCityRingRadius = 0.45f;

// Corner i of the hex (x, y), the first two of the cube corner rows at 0x0a7ed7f0 (stride 0x10).
const float kHexCorners[6][2] = {
    {0.6666667f, -0.33333334f}, {0.33333334f, 0.33333334f}, {-0.33333334f, 0.6666667f},
    {-0.6666667f, 0.33333334f}, {-0.33333334f, -0.33333334f}, {0.33333334f, -0.6666667f},
};

}

/// 0x0866167c
RoadCell* RoadGridCellAt(RoadGrid* a1, int iX, int iY) {
    if ((a1->field_9c == 0 && (iX < 0 || iX >= a1->m_iWidth)) || iY < 0 || iY >= a1->m_iHeight) {
        return reinterpret_cast<RoadCell*>(a1 + 1);
    }
    if (a1->field_9c != 0) {
        while (iX < 0) iX += a1->m_iWidth;
        iX = iX % a1->m_iWidth;
    }
    return &a1->m_aCells[iY * 200 + iX];
}

/// 0x0920c448
int cvGameplayRoadHandler::ms_aaiSegmentSides[0xf][0x2] = {
    {0, 3}, {4, 1}, {2, 5}, {0, 2}, {4, 3}, {2, 1}, {3, 5}, {1, 0}, {5, 4}, {0, 4}, {4, 2}, {2, 3}, {3, 1}, {1, 5}, {5, 0},
};

/// 0x087dc998
void cvGameplayRoadHandler::DeleteRoad(HEXVec_i const& kHex) {
    RoadCell* pCell = Fun_08661606(&m_RoadGrid, kHex);
    for (unsigned int uiSegment = 0; uiSegment < 5; uiSegment++) {
        if (pCell->m_auiDecals[uiSegment] != 0xffffffffu) {
            CalculateRoadSpline(kHex, uiSegment);
            UpdateRoadGeometry(m_SplinePoints, pCell->m_aucBits[1] & 7, false);
            g_TerrainSystem.m_DecalLayer.DestroyDecal(pCell->m_auiDecals[uiSegment]);
            pCell->m_auiDecals[uiSegment] = 0xffffffffu;
        }
    }
    Fun_087df262(kHex);
    pCell = Fun_08661606(&m_RoadGrid, kHex);
    pCell->m_ucRoadType = 0xff;
    pCell->m_ucSideMask = 0;
    pCell->m_aucBits[0] &= 0xf0;
    for (unsigned int uiSegment = 0; uiSegment < 5; uiSegment++) {
        pCell->m_auiDecals[uiSegment] = 0xffffffffu;
    }
    for (int iSide = 0; iSide < 6; iSide++) {
        const HEXVec_i kNeighbour = {HEXVec_i::TRANSLATIONS[iSide].x + kHex.x, HEXVec_i::TRANSLATIONS[iSide].y + kHex.y,
                                     HEXVec_i::TRANSLATIONS[iSide].z + kHex.z};
        ReleaseRoadsOnHex(kNeighbour);
        RefreshRoadsOnHex(kNeighbour);
        Fun_087df262(kNeighbour);
    }
}

/// 0x087dce50
void cvGameplayRoadHandler::AddRoad(HEXVec_i const& kHex, unsigned char ucRouteType, unsigned char ucRouteState) {
    RoadCell* pCell = Fun_08661606(&m_RoadGrid, kHex);
    // Route states 4 and 5 are the under-construction ones; they are not drawn.
    if ((ucRouteState & 0xfe) == 4) return;
    pCell->m_ucRoadType = ucRouteType;
    unsigned int uiBits = pCell->m_aucBits[0] | (pCell->m_aucBits[1] << 8);
    uiBits = (uiBits & 0xf8f7) | ((ucRouteState & 7u) << 8) | 8;
    pCell->m_aucBits[0] = (unsigned char)uiBits;
    pCell->m_aucBits[1] = (unsigned char)(uiBits >> 8);
    for (int iSide = 0; iSide < 6; iSide++) {
        const HEXVec_i kNeighbour = {HEXVec_i::TRANSLATIONS[iSide].x + kHex.x, HEXVec_i::TRANSLATIONS[iSide].y + kHex.y,
                                     HEXVec_i::TRANSLATIONS[iSide].z + kHex.z};
        const HEXVec_i kGridPos = Fun_086616f0(m_RoadGrid.m_mxForward, kNeighbour);
        const HEXVec_i kGridCube = {kGridPos.x, kGridPos.y, -(kGridPos.y + kGridPos.x)};
        const HEXVec_i kWrapped = Fun_086616f0(m_RoadGrid.m_mxInverse, kGridCube);
        const HEXVec_i kRefresh = {kWrapped.x, kWrapped.y, -(kWrapped.y + kWrapped.x)};
        ReleaseRoadsOnHex(kRefresh);
        RefreshRoadsOnHex(kRefresh);
        Fun_087df262(kRefresh);
    }
    ReleaseRoadsOnHex(kHex);
    RefreshRoadsOnHex(kHex);
    Fun_087df262(kHex);
}

/// 0x087dd026
void cvGameplayRoadHandler::ReleaseRoadsOnHex(HEXVec_i const& kHex) {
    RoadCell* pCell = Fun_08661606(&m_RoadGrid, kHex);
    for (unsigned int uiSegment = 0; uiSegment < 5; uiSegment++) {
        if (pCell->m_auiDecals[uiSegment] != 0xffffffffu) {
            CalculateRoadSpline(kHex, uiSegment);
            UpdateRoadGeometry(m_SplinePoints, pCell->m_aucBits[1] & 7, false);
            g_TerrainSystem.m_DecalLayer.DestroyDecal(pCell->m_auiDecals[uiSegment]);
            pCell->m_auiDecals[uiSegment] = 0xffffffffu;
        }
    }
    pCell->m_ucSideMask = 0;
    pCell->m_aucBits[0] &= 0xf8;
    for (unsigned int uiSegment = 0; uiSegment < 5; uiSegment++) {
        pCell->m_auiDecals[uiSegment] = 0xffffffffu;
    }
}

/// 0x087dd2f2
void cvGameplayRoadHandler::CalculateRoadSpline(HEXVec_i const& kHex, unsigned int uiSegment) {
    RoadCell* pCell = Fun_08661606(&m_RoadGrid, kHex);
    if ((pCell->m_aucBits[0] & 0xc0) != 0) {
        CalculateRoadSplineCity(kHex, uiSegment);
        return;
    }
    m_SplinePoints.clear();
    HEXVec_i kHexCopy = kHex;
    const int* aiSides = ms_aaiSegmentSides[pCell->m_aucSegments[uiSegment]];
    BuildSideToSideSpline(this, kHexCopy, aiSides[0], aiSides[1]);
    TightenRoadCurve(m_SplinePoints);
}

/// 0x087dd8b8
unsigned int cvGameplayRoadHandler::CalculateRoadCoverage(HEXVec_i const& kHex) {
    RoadCell* pCell = Fun_08661606(&m_RoadGrid, kHex);
    unsigned int uiSideMask = 0;
    for (int iSide = 0; iSide < 6; iSide++) {
        const HEXVec_i kNeighbour = {HEXVec_i::TRANSLATIONS[iSide].x + kHex.x, HEXVec_i::TRANSLATIONS[iSide].y + kHex.y,
                                     HEXVec_i::TRANSLATIONS[iSide].z + kHex.z};
        const RoadCell* pNeighbour = Fun_08661606(&m_RoadGrid, kNeighbour);
        unsigned int uiBit = 1u << iSide;
        if (((pNeighbour->m_aucBits[0] ^ pCell->m_aucBits[0]) & 8) != 0) uiBit = 0;
        uiSideMask = pCell->m_ucSideMask | uiBit;
        pCell->m_ucSideMask = (unsigned char)uiSideMask;
    }
    // A lone road hex in open country gets a random straight line through it.
    if ((pCell->m_aucBits[0] & 0xc0) == 0 && (unsigned char)uiSideMask == 0) {
        const int iStraight = (int)rand_wincompatible() % 3;
        uiSideMask = (1u << ms_aaiSegmentSides[iStraight][1]) | pCell->m_ucSideMask | (1u << ms_aaiSegmentSides[iStraight][0]);
        pCell->m_ucSideMask = (unsigned char)uiSideMask;
    }
    return uiSideMask & 0xff;
}

/// 0x087dd9c8
unsigned int cvGameplayRoadHandler::UpdateRoadTopology(unsigned char* pucSegments, unsigned int uiSideMask, unsigned char ucCityMode) {
    if (ucCityMode != 0) return UpdateCityRoadTopology(pucSegments, uiSideMask, ucCityMode);

    unsigned char aucConnected[6];
    unsigned int uiConnected = 0;
    for (int iSide = 0; iSide < 6; iSide++) {
        if ((m_aiSideBit[iSide] & uiSideMask) != 0) aucConnected[uiConnected++] = (unsigned char)iSide;
    }

    // Index of the unordered side pair in ms_aaiSegmentSides.
    auto FindPair = [](int iSideA, int iSideB) -> unsigned int {
        for (unsigned int uiPair = 0; uiPair < 0xf; uiPair++) {
            const int* aiPair = ms_aaiSegmentSides[uiPair];
            if ((aiPair[0] == iSideA && aiPair[1] == iSideB) || (aiPair[0] == iSideB && aiPair[1] == iSideA)) return uiPair;
        }
        return 0xf;
    };

    unsigned int uiSegments = 0;
    while (__builtin_popcount(uiSideMask) != 0) {
        if (__builtin_popcount(uiSideMask) >= 2) {
            // Join two random sides of those still unconnected.
            int aiLeft[6];
            unsigned int uiLeft = 0;
            for (int iSide = 0; iSide < 6; iSide++) {
                if ((m_aiSideBit[iSide] & uiSideMask) != 0) aiLeft[uiLeft++] = iSide;
            }
            unsigned int uiFirst = rand_wincompatible() % uiLeft;
            const unsigned int uiSecond = rand_wincompatible() % uiLeft;
            if (uiFirst == uiSecond) uiFirst = (uiFirst + 1) % uiLeft;
            const int iSideA = aiLeft[uiFirst];
            const int iSideB = aiLeft[uiSecond];
            pucSegments[uiSegments++] = (unsigned char)FindPair(iSideA, iSideB);
            uiSideMask &= ~m_aiSideBit[iSideA] & ~m_aiSideBit[iSideB];
        } else {
            // One side left: pair it with the opposite side when it is the only connection, else with a random connected one.
            for (int iSide = 0; iSide < 6; iSide++) {
                if ((m_aiSideBit[iSide] & uiSideMask) == 0) continue;
                int iPartner = -1;
                if (uiConnected == 1) {
                    for (int iStraight = 0; iStraight < 3; iStraight++) {
                        if (ms_aaiSegmentSides[iStraight][0] == iSide) iPartner = ms_aaiSegmentSides[iStraight][1];
                        if (ms_aaiSegmentSides[iStraight][1] == iSide) iPartner = ms_aaiSegmentSides[iStraight][0];
                    }
                } else {
                    const unsigned int uiPick = rand_wincompatible() % uiConnected;
                    iPartner = aucConnected[uiPick];
                    if (iPartner == iSide) iPartner = aucConnected[(uiPick + 1) % uiConnected];
                }
                pucSegments[uiSegments++] = (unsigned char)FindPair(iSide, iPartner);
                uiSideMask &= ~m_aiSideBit[iSide] & ~m_aiSideBit[iPartner];
                if (uiSegments == 5) break;
            }
        }
    }
    return uiSegments;
}

/// 0x087ddcb0
void cvGameplayRoadHandler::CalculateRoadSplineCity(HEXVec_i const& kHex, unsigned int uiSegment) {
    RoadCell* pCell = Fun_08661606(&m_RoadGrid, kHex);
    const float fHexX = (float)kHex.x;
    const float fHexY = (float)kHex.y;
    m_SplinePoints.clear();
    const float* afHexToTex = m_kWorldGlobals.m_afHexToTex;

    // Hex plane position to texture position, z = -(x + y).
    const auto HexToTex = [afHexToTex](float fX, float fY) -> FGXVector2 {
        const float fZ = -fX - fY;
        FGXVector2 vTex;
        vTex.x = afHexToTex[8] * fZ + (afHexToTex[4] * fY + afHexToTex[0] * fX) + afHexToTex[12];
        vTex.y = fZ * afHexToTex[9] + (fY * afHexToTex[5] + fX * afHexToTex[1]) + afHexToTex[13];
        return vTex;
    };

    const unsigned int uiCityMode = pCell->m_aucBits[0] & 0xc0;
    unsigned int uiSpokeIndex;
    if (uiCityMode == 0x80) {
        uiSpokeIndex = 0;
    } else if (uiCityMode == 0x40) {
        if (uiSegment == 0) {
            for (int iCorner = 0; iCorner < 6; iCorner++) {
                m_SplinePoints.push_back(HexToTex(kHexCorners[iCorner][0] * kCityRingRadius + fHexX,
                                                  kHexCorners[iCorner][1] * kCityRingRadius + fHexY));
            }
            // Close the ring: the first two points again.
            const FGXVector2 vFirst = m_SplinePoints[0];
            m_SplinePoints.push_back(vFirst);
            const FGXVector2 vSecond = m_SplinePoints[1];
            m_SplinePoints.push_back(vSecond);
            return;
        }
        uiSpokeIndex = 1;
    } else {
        return;
    }

    for (int iSide = 0; iSide < 6; iSide++) {
        if (((pCell->m_ucSideMask >> iSide) & 1) == 0) continue;
        if (uiSpokeIndex != uiSegment) {
            uiSpokeIndex++;
            continue;
        }
        // The binary also looks up the neighbour's grid cell here and drops the result.
        m_SplinePoints.clear();
        HEXVec_i kHexCopy = kHex;
        BuildSideToSideSpline(this, kHexCopy, iSide, (iSide + 3) % 6);
        TightenRoadCurve(m_SplinePoints);
        m_SplinePoints.resize((m_SplinePoints.size() >> 1) - 1);
        const float fEndX = fHexX + HEXVec::TRANSLATIONS[iSide].x * 0.5f * kCityRingRadius;
        const float fEndY = fHexY + 0.5f * HEXVec::TRANSLATIONS[iSide].y * kCityRingRadius;
        m_SplinePoints.push_back(HexToTex(fEndX, fEndY));
        // Push the last point one step further along the spoke.
        FGXVector2* pPoints = m_SplinePoints.begin();
        const unsigned int uiCount = m_SplinePoints.size();
        const float fLastY = pPoints[uiCount - 1].y;
        const float fPrevY = pPoints[uiCount - 2].y;
        pPoints[uiCount - 1].x = (pPoints[uiCount - 1].x - pPoints[uiCount - 2].x) + pPoints[uiCount - 1].x;
        pPoints[uiCount - 1].y = (fLastY - fPrevY) + fLastY;
        return;
    }
}

/// 0x087df2f8
static unsigned int BuildSideToSideSpline(cvGameplayRoadHandler* pHandler, HEXVec_i& kHex, int iSideA, int iSideB) {
    cvGameplayRoadHandler& kRoad = *pHandler;
    const D3DXVECTOR4 vHex = {(float)kHex.x, (float)kHex.y, (float)kHex.z, 1.0f};
    D3DXVECTOR4 vHexTex;
    D3DXVec4Transform(&vHexTex, &vHex, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));
    RoadCell* pCell = Fun_08661606(&kRoad.m_RoadGrid, kHex);

    // Road variation bytes of the two neighbours; 0x80 (middle of the side) when a neighbour is off the grid.
    unsigned char aucVariationA[6] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
    unsigned char aucVariationB[6] = {0x80, 0x80, 0x80, 0x80, 0x80, 0x80};
    const HEXVec_i kNeighbourA = {HEXVec_i::TRANSLATIONS[iSideA].x + kHex.x, HEXVec_i::TRANSLATIONS[iSideA].y + kHex.y,
                                  HEXVec_i::TRANSLATIONS[iSideA].z + kHex.z};
    const HEXVec_i kGridA = Fun_086616f0(kRoad.m_RoadGrid.m_mxForward, kNeighbourA);
    const HEXVec_i kNeighbourB = {HEXVec_i::TRANSLATIONS[iSideB].x + kHex.x, HEXVec_i::TRANSLATIONS[iSideB].y + kHex.y,
                                  HEXVec_i::TRANSLATIONS[iSideB].z + kHex.z};
    const HEXVec_i kGridB = Fun_086616f0(kRoad.m_RoadGrid.m_mxForward, kNeighbourB);
    const int iColumnA = (kGridA.y >> 1) + kGridA.x;
    const int iColumnB = (kGridB.y >> 1) + kGridB.x;
    const bool bWrap = kRoad.m_RoadGrid.field_9c != 0;
    bool bInside = (bWrap || (iColumnA >= 0 && iColumnA < kRoad.m_RoadGrid.m_iWidth)) && kGridA.y >= 0 && kGridA.y < kRoad.m_RoadGrid.m_iHeight;
    if (bInside) {
        if (bWrap) {
            bInside = kGridB.y >= 0;
        } else {
            bInside = iColumnB >= 0 && kGridB.y >= 0 && iColumnB < kRoad.m_RoadGrid.m_iWidth;
        }
    }
    if (bInside && kGridB.y < kRoad.m_RoadGrid.m_iHeight) {
        const RoadCell* pCellA = RoadGridCellAt(&kRoad.m_RoadGrid, iColumnA, kGridA.y);
        const RoadCell* pCellB = RoadGridCellAt(&kRoad.m_RoadGrid, iColumnB, kGridB.y);
        memcpy(aucVariationA, pCellA->m_aucVariation, sizeof(aucVariationA));
        memcpy(aucVariationB, pCellB->m_aucVariation, sizeof(aucVariationB));
    }

    // The point at fraction ucVariation / 256 along side iSide, in texture space, plus an optional neighbour offset.
    const auto SidePoint = [&kRoad, &vHexTex](int iSide, unsigned char ucVariation, const FGXVector2* pNeighbourDelta) -> FGXVector2 {
        const float fAlong = (float)ucVariation * kVariationUnit;
        const float fRest = 1.0f - fAlong;
        FGXVector2 vPoint;
        vPoint.x = kRoad.m_avSideMid[iSide][0].x * fRest + kRoad.m_avSideMid[iSide][1].x * fAlong + vHexTex.x;
        vPoint.y = fRest * kRoad.m_avSideMid[iSide][0].y + fAlong * kRoad.m_avSideMid[iSide][1].y + vHexTex.y;
        if (pNeighbourDelta != NULL) {
            vPoint.x = vPoint.x + pNeighbourDelta->x;
            vPoint.y = vPoint.y + pNeighbourDelta->y;
        }
        return vPoint;
    };

    const int iOppositeA = (iSideA + 3) % 6;
    const int iOppositeB = (iSideB + 3) % 6;
    kRoad.m_SplinePoints.push_back(SidePoint(iOppositeA, aucVariationA[iOppositeA], &kRoad.m_avNeighborDelta[iSideA]));
    kRoad.m_SplinePoints.push_back(SidePoint(iSideA, pCell->m_aucVariation[iSideA], NULL));
    kRoad.m_SplinePoints.push_back(SidePoint(iSideB, pCell->m_aucVariation[iSideB], NULL));
    kRoad.m_SplinePoints.push_back(SidePoint(iOppositeB, aucVariationB[iOppositeB], &kRoad.m_avNeighborDelta[iSideB]));
    return 1;
}

/// 0x087de446
unsigned int cvGameplayRoadHandler::UpdateCityRoadTopology(unsigned char* pucSegments, unsigned int uiSideMask, unsigned char ucCityMode) {
    unsigned int uiSegments;
    if (ucCityMode == 1) {
        uiSegments = 1;
    } else if (ucCityMode == 2) {
        uiSegments = 0;
    } else {
        return 0;
    }
    for (unsigned int uiSide = 0; uiSide < 6; uiSide++) {
        if ((((uiSideMask & 0xff) >> uiSide) & 1) != 0 && uiSegments < 5) uiSegments++;
    }
    return uiSegments;
}
