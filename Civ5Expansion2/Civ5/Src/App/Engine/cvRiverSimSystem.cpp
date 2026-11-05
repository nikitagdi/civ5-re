/// origin guess
#include <math.h>
#include <Windows.h>
#include "Civ5/Src/App/Engine/cvRiverSimSystem.h"
#include "Civ5/Src/App/Engine/GameSplineDatabase.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/CellBlitJob.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/TerrainStencil.h"
#include "Civ5/Src/App/cvFloodplainsSystem.h"

namespace {

typedef FFastVector<HexEdge, true, 50u, 0u, FTempHeapFastVectorAllocator> HexEdgeVector;
typedef FFastVector<HexEdgeVector, false, 50u, 0u, FTempHeapFastVectorAllocator> HexEdgeVectorList;
typedef FFastVector<FFastVector<float, true, 50u, 0u, FTempHeapFastVectorAllocator>, false, 50u, 0u, FTempHeapFastVectorAllocator> HeightVectorList;
typedef FFastVector<FGXVector3, true, 50u, 0u, FTempHeapFastVectorAllocator> SplinePointVector;
typedef FFastVector<TerrainStencilVertex, true, 50u, 0u, FTempHeapFastVectorAllocator> StencilVertexVector;

const B_SPLINE::BSPLINE_BEGIN_TYPES kSplineBegin = (B_SPLINE::BSPLINE_BEGIN_TYPES)1;
const B_SPLINE::BSPLINE_END_TYPES kSplineEnd = (B_SPLINE::BSPLINE_END_TYPES)0;

// Endcap points past a river mouth, this many texels apart along the last hex side.
const unsigned int kNumEndcapPoints = 4;
const float kEndcapSpacing = 16.0f;

// Stencil ribbon: spline steps of 0.1, half width 5 texels; the side facing -y grows by up to 1.25 half widths.
const float kStencilStep = 0.1f;
const float kStencilEndEpsilon = 1.0e-4f;
const float kStencilHalfWidth = 5.0f;
const float kStencilSouthGrow = 1.25f;
const D3DXVECTOR2 kStencilSouth = {0.0f, -1.0f};

// Height map values are 16-bit; the blends work on value / 65535.
const float kHeightScale = 65535.0f;
// Bed floor of the bed blend, as a normalised height (immediate 0x3e70fcf1).
const float kBedFloor = 0.23533989f;
const float kBedFloorOffset = -0.06f;
// Bed dip: sin of the across coordinate times 3 mapped onto a half period, scaled by -0.05 / 3.
const float kBedAcrossScale = 3.0f;
const float kBedHalfPi = 1.57075f;
const float kBedPi = 3.1415f;
const float kBedDipDepth = -0.05f;
// A skirt texel at or below this raw height takes the continent's river bank terrain.
const unsigned int kSkirtRetextureHeight = 35000;
// Terrain postfixes: the skirt replaces 0x35 with 0x10; the bed takes continent 4's 0x30.
const unsigned int kSkirtFromPostfix = 0x35;
const unsigned int kSkirtToPostfix = 0x10;
const unsigned int kBedPostfix = 0x30;
const unsigned int kBedContinent = 4;
const unsigned int kNumBlendContinents = 4;
const unsigned int kMaskBlockReserve = 0x180;
const unsigned int kMaskFreeListEnd = 0xfffffff;
const double kMinTriangleArea = 1.0e-5;

const int kNumEdgeCodes = 12;
const int kNumHexSides = 6;
const int kRiverGridWidth = 200;

// The same hex side seen from the neighbouring hex across it (0x09149c44).
const unsigned char kTwinEdge[kNumEdgeCodes] = {9, 10, 11, 6, 7, 8, 3, 4, 5, 0, 1, 2};

// Coarse wavelet level offsets are rand() % 10 - 5, control point offsets rand() % 15 - 7.5, in texels.
const int kCoarseJitterRange = 10;
const float kCoarseJitterBias = -5.0f;
const int kFineJitterRange = 15;
const float kFineJitterBias = -7.5f;
// RandomizeLowFrequency leaves rivers with fewer (even-rounded) control points alone.
const unsigned int kMinRandomizedPoints = 7;

// Bank half widths in texels: base plus the taper, then by control point index.
const float kBankBaseWidth = 0.405f;
const float kBankTaperStep = 0.75f;
const float kBankTaperMax = 1.0f;
const float kBankWidthAtEnds = 0.0f;
const float kBankWidthSecondPair = 0.3f;
const float kBankWidthThirdPair = 1.2f;
const int kBankWidthRandomRange = 5;
const int kBankEndRandomRange = 3;
// Points within this many of the end of a river that ends at a mouth flare out.
const unsigned int kMouthFlarePoints = 4;
const unsigned int kRiverEndPoints = 6;
const float kMouthFlareScale = 0.35f;
const float kMouthFlareDivisor = 1.2f;
const unsigned int kMinBankPoints = 4;

}  // namespace

/// 0x087f2b8c
UNK_RET cvRiverSimSystem::GatherControlPointsFromMap(HexEdgeVectorList& aRiverEdges) {
    // Inlined in the binary: kEdge or its twin from the neighbouring hex is in aVisited, both wrapped into the game grid.
    const auto IsEdgeVisited = [](HexEdgeVector const& aVisited, HexEdge const& kEdge) -> bool {
        HexEdge kSame = kEdge;
        HexEdge kTwin;
        const HEXVec_i& kStep = HEXVec_i::TRANSLATIONS[kEdge.m_iEdge % kNumHexSides];
        kTwin.m_kHex.x = kStep.x + kEdge.m_kHex.x;
        kTwin.m_kHex.y = kStep.y + kEdge.m_kHex.y;
        kTwin.m_kHex.z = kStep.z + kEdge.m_kHex.z;
        kTwin.m_iEdge = kTwinEdge[kEdge.m_iEdge];
        Fun_0865f23e(kSame.m_kHex);
        Fun_0865f23e(kTwin.m_kHex);
        for (unsigned int i = 0; i < aVisited.size(); ++i) {
            const HexEdge& kSeen = aVisited[i];
            if ((kSeen.m_kHex.x == kSame.m_kHex.x && kSeen.m_kHex.y == kSame.m_kHex.y && kSeen.m_kHex.z == kSame.m_kHex.z && kSeen.m_iEdge == kSame.m_iEdge) ||
                (kSeen.m_kHex.x == kTwin.m_kHex.x && kSeen.m_kHex.y == kTwin.m_kHex.y && kSeen.m_kHex.z == kTwin.m_kHex.z && kSeen.m_iEdge == kTwin.m_iEdge)) {
                return true;
            }
        }
        return false;
    };

    HexEdgeVector aVisited;
    const SquareGridPolicy& kGameGrid = *reinterpret_cast<const SquareGridPolicy*>(WorldGlobals::g_GameGrid);
    const int iWidth = kGameGrid.m_iWidth;
    const int iHeight = kGameGrid.m_iHeight;
    for (int iX = 0; iX < iWidth; ++iX) {
        for (int iY = 0; iY < iHeight; ++iY) {
            for (int iEdge = 0; iEdge < kNumEdgeCodes; ++iEdge) {
                // Sides 0..2 in both directions name every side of the map once.
                if (iEdge % kNumHexSides >= 3) {
                    continue;
                }
                HEXVec_i kGridHex;
                kGridHex.x = iX - (iY >> 1);
                kGridHex.y = iY;
                kGridHex.z = -(iY + kGridHex.x);
                const HEXVec_i kWorldHex = Fun_086616f0(m_kRiverGrid.m_kPolicy.m_mxInverse, kGridHex);
                HexEdge kEdge;
                kEdge.m_kHex.x = kWorldHex.x;
                kEdge.m_kHex.y = kWorldHex.y;
                kEdge.m_kHex.z = -(kWorldHex.y + kWorldHex.x);
                kEdge.m_iEdge = iEdge;
                if (!IsRiver(kEdge) || IsEdgeVisited(aVisited, kEdge)) {
                    continue;
                }
                if (!TraverseToRiverSource(kEdge, kEdge, aVisited)) {
                    continue;
                }
                HexEdgeVector& aRiver = aRiverEdges[aRiverEdges.push_back()];
                do {
                    Fun_0865f1be(kEdge, kEdge);
                    if (IsEdgeVisited(aVisited, kEdge)) {
                        break;
                    }
                    aRiver.push_back(kEdge);
                    HexEdge kKey = kEdge;
                    Fun_0865f23e(kKey.m_kHex);
                    aVisited.push_back(kKey);
                } while (GetNextEdge(kEdge, kEdge));
                AddControlPointsAlongRiver(aRiver);
            }
        }
    }
}

/// 0x087f332e
UNK_RET cvRiverSimSystem::RandomizeLowFrequency() {
    const unsigned int uiNumRivers = m_aRivers.size();
    for (unsigned int uiRiver = 0; uiRiver < uiNumRivers; ++uiRiver) {
        RiverSimSpline& kRiver = m_aRivers[uiRiver];
        const unsigned int uiNumPoints = kRiver.m_aPoints.size() & ~1u;
        if (uiNumPoints < kMinRandomizedPoints) {
            continue;
        }
        // Every point is overwritten below, so the fill value is never read.
        FGXVector2 kFill;
        RiverWaveletBuffer kWavelet;
        Fun_087f8ed4(&kWavelet, uiNumPoints, kFill);
        for (unsigned int i = 0; i < uiNumPoints; ++i) {
            FGXVector2& kPoint = kWavelet.m_pPoints[kWavelet.m_uiStart + i];
            kPoint.x = kRiver.m_aPoints[i].x;
            kPoint.y = kRiver.m_aPoints[i].y;
        }

        Fun_087f8f6c(&kWavelet);
        FGXVector2* pCoarse = &kWavelet.m_pPoints[kWavelet.m_uiStart];
        for (unsigned int i = 1; i < kWavelet.m_uiCount - 1; ++i) {
            if ((rand_wincompatible() & 1) == 0) {
                pCoarse[i].x = (float)((int)rand_wincompatible() % kCoarseJitterRange) + kCoarseJitterBias + pCoarse[i].x;
                pCoarse[i].y = (float)((int)rand_wincompatible() % kCoarseJitterRange) + kCoarseJitterBias + pCoarse[i].y;
            }
        }
        Fun_087f901e(&kWavelet);

        for (unsigned int i = 0; i < uiNumPoints; ++i) {
            const FGXVector2& kPoint = kWavelet.m_pPoints[kWavelet.m_uiStart + i];
            kRiver.m_aPoints[i].x = kPoint.x;
            kRiver.m_aPoints[i].y = kPoint.y;
        }
        for (unsigned int i = 0; i < uiNumPoints; ++i) {
            if ((rand_wincompatible() & 1) == 0) {
                FGXVector3& kPoint = kRiver.m_aPoints[i];
                kPoint.x = (float)((int)rand_wincompatible() % kFineJitterRange) + kFineJitterBias + kPoint.x;
                kPoint.y = (float)((int)rand_wincompatible() % kFineJitterRange) + kFineJitterBias + kPoint.y;
            }
        }
        Fun_087f9dce(&kWavelet.m_pPoints);
    }
}

/// 0x087f3d52
UNK_RET cvRiverSimSystem::GenerateRiverBankControlPoints() {
    m_aLeftBanks.clear();
    m_aRightBanks.clear();
    for (unsigned int uiRiver = 0; uiRiver < m_aRivers.size(); ++uiRiver) {
        RiverSimSpline& kLeft = m_aLeftBanks[m_aLeftBanks.push_back()];
        RiverSimSpline& kRight = m_aRightBanks[m_aRightBanks.push_back()];
        const RiverSimSpline& kRiver = m_aRivers[uiRiver];
        const unsigned int uiNumPoints = kRiver.m_aPoints.size();
        kLeft.m_aPoints.reserve(uiNumPoints + 16);
        kRight.m_aPoints.reserve(uiNumPoints + 16);
        for (unsigned int i = 0; i < uiNumPoints; ++i) {
            kLeft.m_aPoints.push_back(kRiver.m_aPoints[i]);
            kRight.m_aPoints.push_back(kRiver.m_aPoints[i]);
        }
    }

    const D3DXVECTOR3 vUp = {0.0f, 0.0f, 1.0f};
    for (unsigned int uiRiver = 0; uiRiver < m_aRivers.size(); ++uiRiver) {
        const RiverSimSpline& kRiver = m_aRivers[uiRiver];
        RiverSimSpline& kLeft = m_aLeftBanks[uiRiver];
        RiverSimSpline& kRight = m_aRightBanks[uiRiver];
        float fTaper = 0.0f;
        for (unsigned int i = 0; i < kRiver.m_aPoints.size(); ++i) {
            const unsigned int uiNumPoints = kRiver.m_aPoints.size();
            if (uiNumPoints < kMinBankPoints) {
                continue;
            }
            const FGXVector3* pPoints = kRiver.m_aPoints.begin();
            D3DXVECTOR3 vDir = {0.0f, 0.0f, 0.0f};
            if (i == 0) {
                vDir.x = pPoints[1].x - pPoints[0].x;
                vDir.y = pPoints[1].y - pPoints[0].y;
                vDir.z = 0.0f;
            } else if (i == uiNumPoints - 1) {
                vDir.x = pPoints[i].x - pPoints[i - 1].x;
                vDir.y = pPoints[i].y - pPoints[i - 1].y;
                vDir.z = 0.0f;
            } else {
                const D3DXVECTOR3 vCenter = {pPoints[i].x, pPoints[i].y, 0.0f};
                D3DXVECTOR3 vToPrev = {pPoints[i - 1].x - vCenter.x, pPoints[i - 1].y - vCenter.y, 0.0f};
                D3DXVECTOR3 vToNext = {pPoints[i + 1].x - vCenter.x, pPoints[i + 1].y - vCenter.y, 0.0f};
                D3DXVec3Normalize(&vToPrev, &vToPrev);
                D3DXVec3Normalize(&vToNext, &vToNext);
                vDir.x = (vCenter.x + vToNext.x) - (vToPrev.x + vCenter.x);
                vDir.y = (vCenter.y + vToNext.y) - (vToPrev.y + vCenter.y);
                vDir.z = (vToNext.z + vCenter.z) - (vToPrev.z + vCenter.z);
            }
            D3DXVec3Normalize(&vDir, &vDir);
            // vDir x up: the right-hand side of the flow.
            D3DXVECTOR3 vSide = {vDir.y * vUp.z - vDir.z * vUp.y, vDir.z * vUp.x - vDir.x * vUp.z, vDir.x * vUp.y - vDir.y * vUp.x};
            D3DXVec3Normalize(&vSide, &vSide);

            fTaper = fTaper + kBankTaperStep;
            fTaper = fTaper < kBankTaperMax ? fTaper : kBankTaperMax;
            const bool bMouth = kRiver.m_bEndsAtMouth;
            float fLeftWidth;
            float fRightWidth;
            if (i < 2 || (i >= uiNumPoints - kMouthFlarePoints && bMouth)) {
                fLeftWidth = kBankBaseWidth + fTaper + kBankWidthAtEnds;
                fRightWidth = fLeftWidth;
            } else if (i < 4) {
                fLeftWidth = kBankBaseWidth + fTaper + kBankWidthSecondPair;
                fRightWidth = fLeftWidth;
            } else if (i < 6) {
                fLeftWidth = kBankBaseWidth + fTaper + kBankWidthThirdPair;
                fRightWidth = fLeftWidth;
            } else {
                const float fBase = kBankBaseWidth + fTaper;
                fLeftWidth = (float)((int)rand_wincompatible() % kBankWidthRandomRange) + fBase;
                fRightWidth = (float)((int)rand_wincompatible() % kBankWidthRandomRange) + fBase;
            }
            if (!bMouth) {
                if (i >= uiNumPoints - kRiverEndPoints && i != 0) {
                    const float fBase = kBankBaseWidth + fTaper;
                    fLeftWidth = (float)((int)rand_wincompatible() % kBankEndRandomRange) + fBase;
                    fRightWidth = (float)((int)rand_wincompatible() % kBankEndRandomRange) + fBase;
                }
            } else if (i >= uiNumPoints - kMouthFlarePoints) {
                const unsigned int uiFromFlareStart = i - uiNumPoints + 8;
                const float fFlare = (float)(uiFromFlareStart * uiFromFlareStart) * kMouthFlareScale / kMouthFlareDivisor;
                fLeftWidth += fFlare;
                fRightWidth += fFlare;
            }

            const FGXVector3& kCenter = kRiver.m_aPoints[i];
            FGXVector3& kLeftPoint = kLeft.m_aPoints[i];
            FGXVector3& kRightPoint = kRight.m_aPoints[i];
            kLeftPoint.x = kCenter.x - fLeftWidth * vSide.x;
            kLeftPoint.y = kCenter.y - fLeftWidth * vSide.y;
            kRightPoint.x = vSide.x * fRightWidth + kCenter.x;
            kRightPoint.y = fRightWidth * vSide.y + kCenter.y;
        }
    }
}

/// 0x087f4fb0
bool cvRiverSimSystem::IsRiver(HexEdge const& kEdge) const {
    HexEdge kSide = kEdge;
    Fun_0865f1be(kSide, kSide);
    Fun_0865f23e(kSide.m_kHex);
    const HEXVec_i kCell = Fun_086616f0(m_kRiverGrid.m_kPolicy.m_mxForward, kSide.m_kHex);
    const unsigned int uiBits = m_kRiverGrid.m_auiEdges[kCell.y * kRiverGridWidth + kCell.x + ((unsigned int)kCell.y >> 1)];
    return ((uiBits >> kSide.m_iEdge) & 1) != 0;
}

/// 0x087f5194
bool cvRiverSimSystem::TraverseToRiverSource(HexEdge const& kStart, HexEdge& kSource, HexEdgeVector const& aVisited) const {
    // Same inlined test as in GatherControlPointsFromMap.
    const auto IsEdgeVisited = [](HexEdgeVector const& aVisited, HexEdge const& kEdge) -> bool {
        HexEdge kSame = kEdge;
        HexEdge kTwin;
        const HEXVec_i& kStep = HEXVec_i::TRANSLATIONS[kEdge.m_iEdge % kNumHexSides];
        kTwin.m_kHex.x = kStep.x + kEdge.m_kHex.x;
        kTwin.m_kHex.y = kStep.y + kEdge.m_kHex.y;
        kTwin.m_kHex.z = kStep.z + kEdge.m_kHex.z;
        kTwin.m_iEdge = kTwinEdge[kEdge.m_iEdge];
        Fun_0865f23e(kSame.m_kHex);
        Fun_0865f23e(kTwin.m_kHex);
        for (unsigned int i = 0; i < aVisited.size(); ++i) {
            const HexEdge& kSeen = aVisited[i];
            if ((kSeen.m_kHex.x == kSame.m_kHex.x && kSeen.m_kHex.y == kSame.m_kHex.y && kSeen.m_kHex.z == kSame.m_kHex.z && kSeen.m_iEdge == kSame.m_iEdge) ||
                (kSeen.m_kHex.x == kTwin.m_kHex.x && kSeen.m_kHex.y == kTwin.m_kHex.y && kSeen.m_kHex.z == kTwin.m_kHex.z && kSeen.m_iEdge == kTwin.m_iEdge)) {
                return true;
            }
        }
        return false;
    };

    if (!IsRiver(kStart) || IsEdgeVisited(aVisited, kStart)) {
        return false;
    }
    HexEdge kEdge = kStart;
    HexEdge kPrev;
    bool bMore;
    do {
        kPrev = kEdge;
        bMore = GetPrevEdge(kPrev, kEdge);
        // Stop below a stretch another river already claimed.
        if (IsEdgeVisited(aVisited, kEdge)) {
            bMore = false;
        }
    } while (bMore);
    kSource = kPrev;
    return true;
}

/// 0x087f36ba
UNK_RET cvRiverSimSystem::AddRiverEndcaps(HexEdgeVectorList const& aRiverEdges) {
    const GameSplineDatabase& kDatabase = *reinterpret_cast<const GameSplineDatabase*>(g_GameSplineDatabase);
    const unsigned int uiNumRivers = m_aRivers.size();
    for (unsigned int uiRiver = 0; uiRiver < uiNumRivers; ++uiRiver) {
        const HexEdgeVector& aEdges = aRiverEdges[uiRiver];
        if (aEdges.size() == 0) {
            continue;
        }
        GameSpline::Segment kSegment;
        const HexEdge kLast = aEdges[aEdges.size() - 1];
        HexEdge kNext;
        kNext.m_kHex.x = 0;
        kNext.m_kHex.y = 0;
        kNext.m_kHex.z = 0;
        if (GetNextEdge(kLast, kNext)) {
            // A tributary: run on along the side it flows into.
            kDatabase.QueryHexSegment(kNext, &kSegment);
            for (unsigned int i = 0; i < kSegment.size(); ++i) {
                const GameSpline::ControlPoint& kPoint = kSegment[i];
                const FGXVector3 kExtra = {kPoint.m_vPos.x, kPoint.m_vPos.y, kPoint.field_8};
                m_aRivers[uiRiver].m_aPoints.push_back(kExtra);
            }
            continue;
        }

        // A mouth: run on into the water along the direction of the last side.
        kDatabase.QueryGeneratedHexSegment(kLast, &kSegment);
        if (kSegment.size() < 2) {
            continue;
        }
        RiverSimSpline& kRiver = m_aRivers[uiRiver];
        const unsigned int uiNumPoints = kRiver.m_aPoints.size();
        if (uiNumPoints < 2) {
            continue;
        }
        const GameSpline::ControlPoint kSideStart = kSegment[0];
        const GameSpline::ControlPoint kSideNext = kSegment[1];
        const FGXVector3 kEnd = kRiver.m_aPoints[uiNumPoints - 1];
        D3DXVECTOR2 vStep = {kSideNext.m_vPos.x - kSideStart.m_vPos.x, kSideNext.m_vPos.y - kSideStart.m_vPos.y};
        D3DXVec2Normalize(&vStep, &vStep);
        vStep.x = vStep.x * kEndcapSpacing;
        vStep.y = kEndcapSpacing * vStep.y;
        FGXVector3 aCap[kNumEndcapPoints];
        float fX = kEnd.x;
        float fY = kEnd.y;
        for (unsigned int i = 0; i < kNumEndcapPoints; ++i) {
            fX = fX + vStep.x;
            fY = fY + vStep.y;
            aCap[i].x = fX;
            aCap[i].y = fY;
            aCap[i].z = 0.0f;
        }
        for (unsigned int i = 0; i < kNumEndcapPoints; ++i) {
            kRiver.m_aPoints.push_back(aCap[i]);
        }
    }
}

/// 0x087f43da
UNK_RET cvRiverSimSystem::CreateStencilMeshes(StencilVertexVector* pVertices, HeightVectorList const& aHeights) const {
    const unsigned int uiNumRivers = m_aRivers.size();
    pVertices->clear();
    const D3DXVECTOR3 vUp = {0.0f, 0.0f, 1.0f};
    for (unsigned int uiRiver = 0; uiRiver < uiNumRivers; ++uiRiver) {
        const unsigned int uiNumPoints = m_aRivers[uiRiver].m_aPoints.size();
        if (uiNumPoints < kMinBankPoints) {
            continue;
        }
        SplinePointVector aCenter;
        SplinePointVector aLeft;
        SplinePointVector aRight;
        aCenter.setsize(uiNumPoints);
        aLeft.setsize(uiNumPoints);
        aRight.setsize(uiNumPoints);
        const float* pHeights = aHeights[uiRiver].begin();
        const FGXVector3* pCenter = m_aRivers[uiRiver].m_aPoints.begin();
        const FGXVector3* pLeft = m_aLeftBanks[uiRiver].m_aPoints.begin();
        const FGXVector3* pRight = m_aRightBanks[uiRiver].m_aPoints.begin();
        for (unsigned int i = 0; i < uiNumPoints; ++i) {
            aCenter[i].x = pCenter[i].x;
            aCenter[i].y = pCenter[i].y;
            aCenter[i].z = pHeights[i];
            aLeft[i].x = pLeft[i].x;
            aLeft[i].y = pLeft[i].y;
            aLeft[i].z = pHeights[i];
            aRight[i].x = pRight[i].x;
            aRight[i].y = pRight[i].y;
            aRight[i].z = pHeights[i];
        }

        const float fLastT = (float)(int)(uiNumPoints - 2);
        const float fEndT = fLastT + kStencilEndEpsilon;
        float fT = kStencilStep;
        while (fT <= fEndT) {
            const float fT1 = fLastT < fT ? fLastT : fT;
            const float fT0 = -kStencilStep + fT1;
            const FGXVector3 kCenter1 = B_SPLINE::EvalScalingFunc<FGXVector3, FGXVector3*, kSplineBegin, kSplineEnd>(aCenter.size(), aCenter.begin(), fT1, 0);
            // The bank splines are evaluated too, but only the center line shapes the stencil.
            B_SPLINE::EvalScalingFunc<FGXVector3, FGXVector3*, kSplineBegin, kSplineEnd>(aLeft.size(), aLeft.begin(), fT1, 0);
            B_SPLINE::EvalScalingFunc<FGXVector3, FGXVector3*, kSplineBegin, kSplineEnd>(aRight.size(), aRight.begin(), fT1, 0);
            const FGXVector3 kCenter0 = B_SPLINE::EvalScalingFunc<FGXVector3, FGXVector3*, kSplineBegin, kSplineEnd>(aCenter.size(), aCenter.begin(), fT0, 0);
            B_SPLINE::EvalScalingFunc<FGXVector3, FGXVector3*, kSplineBegin, kSplineEnd>(aLeft.size(), aLeft.begin(), fT0, 0);
            B_SPLINE::EvalScalingFunc<FGXVector3, FGXVector3*, kSplineBegin, kSplineEnd>(aRight.size(), aRight.begin(), fT0, 0);
            FGXVector3 kGrad1 = B_SPLINE::EvalGradient<FGXVector3, FGXVector3*, kSplineBegin, kSplineEnd>(aCenter.size(), aCenter.begin(), fT1, 0);
            FGXVector3 kGrad0 = B_SPLINE::EvalGradient<FGXVector3, FGXVector3*, kSplineBegin, kSplineEnd>(aCenter.size(), aCenter.begin(), fT0, 0);
            D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&kGrad1), reinterpret_cast<const D3DXVECTOR3*>(&kGrad1));
            D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&kGrad0), reinterpret_cast<const D3DXVECTOR3*>(&kGrad0));

            // up x gradient: the left-hand side of the flow.
            D3DXVECTOR3 vSide1 = {vUp.y * kGrad1.z - vUp.z * kGrad1.y, vUp.z * kGrad1.x - vUp.x * kGrad1.z, vUp.x * kGrad1.y - vUp.y * kGrad1.x};
            D3DXVec3Normalize(&vSide1, &vSide1);
            D3DXVECTOR3 vSide0 = {vUp.y * kGrad0.z - vUp.z * kGrad0.y, vUp.z * kGrad0.x - vUp.x * kGrad0.z, vUp.x * kGrad0.y - vUp.y * kGrad0.x};
            D3DXVec3Normalize(&vSide0, &vSide0);
            vSide0.x = vSide0.x * kStencilHalfWidth;
            vSide0.y = vSide0.y * kStencilHalfWidth;
            vSide0.z = vSide0.z * kStencilHalfWidth;
            vSide1.x = vSide1.x * kStencilHalfWidth;
            vSide1.y = vSide1.y * kStencilHalfWidth;
            vSide1.z = kStencilHalfWidth * vSide1.z;

            TerrainStencilVertex kA1 = {kCenter1.x - vSide1.x, kCenter1.y - vSide1.y};
            TerrainStencilVertex kB1 = {vSide1.x + kCenter1.x, vSide1.y + kCenter1.y};
            TerrainStencilVertex kA0 = {kCenter0.x - vSide0.x, kCenter0.y - vSide0.y};
            TerrainStencilVertex kB0 = {kCenter0.x + vSide0.x, kCenter0.y + vSide0.y};

            // Each corner moves out by its side's facing toward -y, clamped to [0, 1].
            D3DXVECTOR2 vOut = {-vSide0.x, -vSide0.y};
            D3DXVECTOR2 vFacing0;
            D3DXVec2Normalize(&vFacing0, &vOut);
            vOut.x = -vSide1.x;
            vOut.y = -vSide1.y;
            D3DXVECTOR2 vFacing1;
            D3DXVec2Normalize(&vFacing1, &vOut);
            float fDot = vFacing0.y * kStencilSouth.y + vFacing0.x * kStencilSouth.x;
            const float fGrowA0 = fDot < 0.0f ? 0.0f : (fDot > 1.0f ? 1.0f : fDot);
            fDot = vFacing1.y * kStencilSouth.y + vFacing1.x * kStencilSouth.x;
            const float fGrowA1 = fDot < 0.0f ? 0.0f : (fDot > 1.0f ? 1.0f : fDot);
            kA0.x = kA0.x - fGrowA0 * vSide0.x * kStencilSouthGrow;
            kA0.y = kA0.y - fGrowA0 * vSide0.y * kStencilSouthGrow;
            kA1.x = kA1.x - vSide1.x * fGrowA1 * kStencilSouthGrow;
            kA1.y = kA1.y - fGrowA1 * vSide1.y * kStencilSouthGrow;

            vOut.x = vSide0.x;
            vOut.y = vSide0.y;
            D3DXVec2Normalize(&vFacing0, &vOut);
            vOut.x = vSide1.x;
            vOut.y = vSide1.y;
            D3DXVec2Normalize(&vFacing1, &vOut);
            fDot = vFacing0.y * kStencilSouth.y + vFacing0.x * kStencilSouth.x;
            const float fGrowB0 = fDot < 0.0f ? 0.0f : (fDot > 1.0f ? 1.0f : fDot);
            fDot = vFacing1.y * kStencilSouth.y + vFacing1.x * kStencilSouth.x;
            const float fGrowB1 = fDot < 0.0f ? 0.0f : (fDot > 1.0f ? 1.0f : fDot);
            kB0.x = vSide0.x * fGrowB0 * kStencilSouthGrow + kB0.x;
            kB0.y = fGrowB0 * vSide0.y * kStencilSouthGrow + kB0.y;
            kB1.x = vSide1.x * fGrowB1 * kStencilSouthGrow + kB1.x;
            kB1.y = fGrowB1 * vSide1.y * kStencilSouthGrow + kB1.y;

            pVertices->push_back(kA0);
            pVertices->push_back(kA1);
            pVertices->push_back(kB1);
            pVertices->push_back(kA0);
            pVertices->push_back(kB1);
            pVertices->push_back(kB0);
            fT = fT1 + kStencilStep;
        }
    }
}

/// 0x087f5508
UNK_RET cvRiverSimSystem::BlendIntoTerrain(TerrainData* pTerrainData) const {
    HeightVectorList aHeights;
    CalculateRiverHeights(aHeights);
    RiverSimSplineMesh kMesh;
    CreateSimMeshes(&kMesh, aHeights);

    RiverBlendMask kMask = {0, 0, 0, 0, NULL, kMaskFreeListEnd, 0, NULL, 0, 0};
    cvTerrainSampler* pSampler = MapGlobals::g_pTerrainSampler;
    const FGXInt2 kMapSize = {pSampler->m_iHeightMapWidth, pSampler->m_iHeightMapHeight};
    const unsigned char ucUnwritten = 0;
    Fun_087f813c(&kMask, &kMapSize, &ucUnwritten, kMaskBlockReserve);

    RiverSkirtBlendContext kSkirt;
    kSkirt.m_fBedFloor = kBedFloor;
    kSkirt.m_pMask = &kMask;
    kSkirt.m_pSampler = pSampler;
    const unsigned int uiNumTerrains = pTerrainData->m_uiNumTerrains;
    for (unsigned int c = 0; c < kNumBlendContinents; ++c) {
        kSkirt.m_aucFromTerrain[c] = 0xff;
        kSkirt.m_aucToTerrain[c] = 0xff;
        if (uiNumTerrains == 0) {
            continue;
        }
        const unsigned int uiPrefix = g_aContinentPrefix[c + 1] & g_mTextureMask;
        const unsigned int uiFromCode = uiPrefix | (~g_mTextureMask & kSkirtFromPostfix);
        const unsigned int uiToCode = uiPrefix | (~g_mTextureMask & kSkirtToPostfix);
        unsigned char ucFrom = 0xff;
        for (unsigned int i = 0; i < uiNumTerrains; ++i) {
            if (pTerrainData->m_aHexCode[i] == uiFromCode) {
                ucFrom = (unsigned char)i;
                break;
            }
        }
        unsigned char ucTo = 0xff;
        for (unsigned int i = 0; i < uiNumTerrains; ++i) {
            if (pTerrainData->m_aHexCode[i] == uiToCode) {
                ucTo = (unsigned char)i;
                break;
            }
        }
        // Without a target terrain the source is disabled too.
        if (ucTo != 0xff) {
            kSkirt.m_aucFromTerrain[c] = ucFrom;
            kSkirt.m_aucToTerrain[c] = ucTo;
        }
    }

    RiverBedBlendContext kBed;
    kBed.m_fBedFloor = kBedFloor;
    kBed.m_pTerrainData = pTerrainData;
    const FGXInt2 kRasterSize = kMapSize;
    // Skirts first: they claim texels in the mask, the bed pass then lowers on top.
    const unsigned int uiNumSkirtVertices = kMesh.m_aSkirts.size();
    for (unsigned int i = 0; i < uiNumSkirtVertices; i += 3) {
        RiverMeshVertex aTriangle[3];
        aTriangle[0] = kMesh.m_aSkirts[i];
        aTriangle[1] = kMesh.m_aSkirts[i + 1];
        aTriangle[2] = kMesh.m_aSkirts[i + 2];
        Fun_087f8220(&kRasterSize, aTriangle, 1, &kSkirt);
    }
    const unsigned int uiNumBedVertices = kMesh.m_aBed.size();
    for (unsigned int i = 0; i < uiNumBedVertices; i += 3) {
        RiverMeshVertex aTriangle[3];
        aTriangle[0] = kMesh.m_aBed[i];
        aTriangle[1] = kMesh.m_aBed[i + 1];
        aTriangle[2] = kMesh.m_aBed[i + 2];
        Fun_087f875c(&kRasterSize, aTriangle, 1, &kBed);
    }

    kMask.m_uiWidth = 0;
    kMask.m_uiHeight = 0;
    delete[] kMask.m_pBlockTable;
    kMask.m_pBlockTable = NULL;
    kMask.field_14 = kMaskFreeListEnd;
    kMask.m_uiNumBlocks = 0;
    kMask.field_18 = 0;
    Fun_087f8cea(&kMask);
}

/// 0x087f8220
UNK_RET Fun_087f8220(FGXInt2 const* pMapSize, RiverMeshVertex const* pTriangles, unsigned int uiNumTriangles, RiverSkirtBlendContext* pContext) {
    for (unsigned int uiTriangle = 0; uiTriangle < uiNumTriangles; ++uiTriangle, pTriangles += 3) {
        const FGXVector3& k0 = pTriangles[0].m_vPos;
        const FGXVector3& k1 = pTriangles[1].m_vPos;
        const FGXVector3& k2 = pTriangles[2].m_vPos;
        const float fMinX12 = k2.x <= k1.x ? k2.x : k1.x;
        const float fMinX = floorf((fMinX12 <= k0.x ? fMinX12 : k0.x) + 0.5f);
        const float fMinY12 = k2.y <= k1.y ? k2.y : k1.y;
        const float fMinY = floorf((fMinY12 <= k0.y ? fMinY12 : k0.y) + 0.5f);
        const float fMaxX12 = k1.x <= k2.x ? k2.x : k1.x;
        const float fMaxX = k0.x <= fMaxX12 ? fMaxX12 : k0.x;
        const float fMaxY12 = k1.y <= k2.y ? k2.y : k1.y;
        const float fMaxY = k0.y <= fMaxY12 ? fMaxY12 : k0.y;
        const float fX0 = k0.x - fMinX;
        const float fY0 = k0.y - fMinY;
        const float fX1 = k1.x - fMinX;
        const float fY1 = k1.y - fMinY;
        const float fX2 = k2.x - fMinX;
        const float fY2 = k2.y - fMinY;
        const float fC0 = fX1 * fY2 - fY1 * fX2;
        const float fArea = fY0 * (fX2 - fX1) + fX0 * (fY1 - fY2) + fC0;
        if (!((double)fabsf(fArea) >= kMinTriangleArea)) {
            continue;
        }
        const int iEndY = (int)fMaxY + 1;
        const int iEndX = (int)fMaxX + 1;
        const float fInvArea = 1.0f / fArea;
        // Texel corners relative to the floored minimum, pre-scaled by 1 / area.
        float fV = 0.0f;
        for (int iY = (int)fMinY; iY < iEndY; ++iY) {
            float fU = 0.0f;
            for (int iX = (int)fMinX; iX < iEndX; ++iX) {
                float afBary[3];
                afBary[0] = (fY1 - fY2) * fU + (fX2 - fX1) * fV + fC0 * fInvArea;
                if (afBary[0] >= 0.0f) {
                    afBary[1] = (fY2 - fY0) * fU + (fX0 - fX2) * fV + (fX2 * fY0 - fX0 * fY2) * fInvArea;
                    if (afBary[1] >= 0.0f) {
                        afBary[2] = (fY0 - fY1) * fU + (fX1 - fX0) * fV + (fX0 * fY1 - fY0 * fX1) * fInvArea;
                        if (afBary[2] >= 0.0f) {
                            Fun_087fa960(pContext, iX, iY, afBary, &pTriangles[0], &pTriangles[1], &pTriangles[2]);
                        }
                    }
                }
                fU = fU + fInvArea;
            }
            fV = fV + fInvArea;
        }
    }
}

/// 0x087f875c
UNK_RET Fun_087f875c(FGXInt2 const* pMapSize, RiverMeshVertex const* pTriangles, unsigned int uiNumTriangles, RiverBedBlendContext* pContext) {
    for (unsigned int uiTriangle = 0; uiTriangle < uiNumTriangles; ++uiTriangle, pTriangles += 3) {
        const FGXVector3& k0 = pTriangles[0].m_vPos;
        const FGXVector3& k1 = pTriangles[1].m_vPos;
        const FGXVector3& k2 = pTriangles[2].m_vPos;
        const float fMinX12 = k2.x <= k1.x ? k2.x : k1.x;
        const float fMinX = floorf((fMinX12 <= k0.x ? fMinX12 : k0.x) + 0.5f);
        const float fMinY12 = k2.y <= k1.y ? k2.y : k1.y;
        const float fMinY = floorf((fMinY12 <= k0.y ? fMinY12 : k0.y) + 0.5f);
        const float fMaxX12 = k1.x <= k2.x ? k2.x : k1.x;
        const float fMaxX = k0.x <= fMaxX12 ? fMaxX12 : k0.x;
        const float fMaxY12 = k1.y <= k2.y ? k2.y : k1.y;
        const float fMaxY = k0.y <= fMaxY12 ? fMaxY12 : k0.y;
        const float fX0 = k0.x - fMinX;
        const float fY0 = k0.y - fMinY;
        const float fX1 = k1.x - fMinX;
        const float fY1 = k1.y - fMinY;
        const float fX2 = k2.x - fMinX;
        const float fY2 = k2.y - fMinY;
        const float fC0 = fX1 * fY2 - fY1 * fX2;
        const float fArea = fY0 * (fX2 - fX1) + fX0 * (fY1 - fY2) + fC0;
        if (!((double)fabsf(fArea) >= kMinTriangleArea)) {
            continue;
        }
        const int iEndY = (int)fMaxY + 1;
        const int iEndX = (int)fMaxX + 1;
        const float fInvArea = 1.0f / fArea;
        float fV = 0.0f;
        for (int iY = (int)fMinY; iY < iEndY; ++iY) {
            float fU = 0.0f;
            for (int iX = (int)fMinX; iX < iEndX; ++iX) {
                float afBary[3];
                afBary[0] = (fY1 - fY2) * fU + (fX2 - fX1) * fV + fC0 * fInvArea;
                if (afBary[0] >= 0.0f) {
                    afBary[1] = (fY2 - fY0) * fU + (fX0 - fX2) * fV + (fX2 * fY0 - fX0 * fY2) * fInvArea;
                    if (afBary[1] >= 0.0f) {
                        afBary[2] = (fY0 - fY1) * fU + (fX1 - fX0) * fV + (fX0 * fY1 - fY0 * fX1) * fInvArea;
                        if (afBary[2] >= 0.0f) {
                            Fun_087fa59c(pContext, iX, iY, afBary, &pTriangles[0], &pTriangles[1], &pTriangles[2]);
                        }
                    }
                }
                fU = fU + fInvArea;
            }
            fV = fV + fInvArea;
        }
    }
}

/// 0x087fa960
UNK_RET Fun_087fa960(RiverSkirtBlendContext* pContext, int iX, int iY, float const* pBary, RiverMeshVertex const* pV0, RiverMeshVertex const* pV1, RiverMeshVertex const* pV2) {
    cvTerrainSampler* pSampler = pContext->m_pSampler;
    if (iY < 0 || iY >= pSampler->m_iHeightMapHeight) {
        return;
    }
    const int iWidth = pSampler->m_iHeightMapWidth;
    const int iWrappedX = (iWidth * 100 + iX) % iWidth;
    FGXInt2 kTexel = {iWrappedX, iY};
    FGXInt2 kPos = pSampler->GetWrappedPosition(kTexel);
    const unsigned short usOldHeight = Fun_08679d0a(&pContext->m_pSampler->m_iHeightMapWidth, &kPos);
    pSampler = pContext->m_pSampler;
    kPos = pSampler->GetWrappedPosition(kTexel);
    const unsigned char ucTerrain = Fun_087fb076(pSampler->unk_4c, &kPos);

    const float fWeight = pV0->m_fWeight * pBary[0] + pV1->m_fWeight * pBary[1] + pV2->m_fWeight * pBary[2];
    const float fHeight0 = pBary[0] * pV0->m_vPos.z;
    const float fHeight1 = pBary[1] * pV1->m_vPos.z;
    const float fHeight2 = pBary[2] * pV2->m_vPos.z;
    const float fBlend = fWeight < 0.0f ? 0.0f : (1.0f < fWeight ? 1.0f : fWeight);
    if (Fun_087fad98(pContext->m_pMask, &kTexel)) {
        return;
    }
    const float fMeshHeight = (fHeight0 + fHeight1 + fHeight2) / kHeightScale;
    const unsigned char ucWritten = 1;
    Fun_087fade4(pContext->m_pMask, &ucWritten, &kTexel);
    const float fOld = (float)usOldHeight / kHeightScale;
    const int iNewHeight = (int)(((1.0f - fBlend) * fOld + fMeshHeight * fBlend) * kHeightScale);
    if ((unsigned int)(unsigned short)iNewHeight <= kSkirtRetextureHeight) {
        for (unsigned int c = 0; c < kNumBlendContinents; ++c) {
            if (ucTerrain == pContext->m_aucFromTerrain[c]) {
                const unsigned char ucTo = pContext->m_aucToTerrain[c];
                pSampler = pContext->m_pSampler;
                kPos = pSampler->GetWrappedPosition(kTexel);
                Fun_087e75d2(pSampler->unk_4c, &ucTo, &kPos);
            }
        }
    }
    const unsigned short usNewHeight = (unsigned short)iNewHeight;
    pSampler = pContext->m_pSampler;
    kPos = pSampler->GetWrappedPosition(kTexel);
    Fun_087fa84a(&pSampler->m_iHeightMapWidth, &usNewHeight, &kPos);
}

/// 0x087fa59c
UNK_RET Fun_087fa59c(RiverBedBlendContext* pContext, int iX, int iY, float const* pBary, RiverMeshVertex const* pV0, RiverMeshVertex const* pV1, RiverMeshVertex const* pV2) {
    cvTerrainSampler* pSampler = MapGlobals::g_pTerrainSampler;
    if (iY < 0 || iY >= pSampler->m_iHeightMapHeight) {
        return;
    }
    const int iWidth = pSampler->m_iHeightMapWidth;
    const FGXInt2 kTexel = {(iWidth * 100 + iX) % iWidth, iY};
    const float fWeight = pV0->m_fWeight * pBary[0] + pV1->m_fWeight * pBary[1] + pV2->m_fWeight * pBary[2];
    const float fMeshHeight = (pBary[0] * pV0->m_vPos.z + pBary[1] * pV1->m_vPos.z + pBary[2] * pV2->m_vPos.z) / kHeightScale;
    const float fAcross = (pV0->m_fAcross * pBary[0] + pV1->m_fAcross * pBary[1] + pV2->m_fAcross * pBary[2]) * kBedAcrossScale;
    float fAngle = 0.0f;
    if (!(0.0f > fAcross)) {
        fAngle = 1.0f < fAcross ? kBedHalfPi : 0.5f * fAcross * kBedPi;
    }
    const float fDip = sinf(fAngle) * kBedDipDepth / kBedAcrossScale;

    FGXInt2 kPos = pSampler->GetWrappedPosition(kTexel);
    const float fOld = (float)Fun_08679d0a(&pSampler->m_iHeightMapWidth, &kPos) / kHeightScale;
    float fNew = (1.0f - fWeight) * fOld + (fMeshHeight + fDip) * fWeight;
    fNew = fNew < fOld ? fNew : fOld;
    const float fFloor = pContext->m_fBedFloor + kBedFloorOffset;
    if (fFloor > fNew) {
        fNew = fOld < fFloor ? fOld : fFloor;
    }
    const unsigned short usNewHeight = (unsigned short)(int)(fNew * kHeightScale);
    pSampler = MapGlobals::g_pTerrainSampler;
    kPos = pSampler->GetWrappedPosition(kTexel);
    Fun_087fa84a(&pSampler->m_iHeightMapWidth, &usNewHeight, &kPos);

    const TerrainData* pTerrainData = pContext->m_pTerrainData;
    unsigned char ucBedTerrain = 0xff;
    for (unsigned int i = 0; i < pTerrainData->m_uiNumTerrains; ++i) {
        if (pTerrainData->m_aHexCode[i] == (g_aContinentPrefix[kBedContinent] | kBedPostfix)) {
            ucBedTerrain = (unsigned char)i;
            break;
        }
    }
    pSampler = MapGlobals::g_pTerrainSampler;
    kPos = pSampler->GetWrappedPosition(kTexel);
    Fun_087e75d2(pSampler->unk_4c, &ucBedTerrain, &kPos);
}
