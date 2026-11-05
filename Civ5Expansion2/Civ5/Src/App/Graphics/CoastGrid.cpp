/// origin guess
#include <new>
#include <Windows.h>
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/CoastGrid.h"
#include "Civ5/Src/App/Graphics/DetectCoastJob.h"
#include "Civ5/Src/App/Graphics/GenerateCoastConnectivityJob.h"
#include "Civ5/Src/App/Graphics/InitGridJob.h"
#include "Civ5/Src/App/Graphics/SplinePreProcessJob.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/HEXVec.h"

namespace {

const char kJobTag[] = "Port/Civ5/Src/App/Graphics/cvCoastGrid.cpp";
// Row stride of CoastGrid::m_aCells.
const int kCoastRowStride = 0xff;

}  // namespace

/// 0x088e5718
CoastSplineHex* Fun_088e5718(CoastGrid const& kGrid, int iCol, int iRow) {
    if ((!kGrid.field_9c && (iCol < 0 || iCol >= kGrid.m_iWidth)) || iRow < 0 || iRow >= kGrid.m_iHeight) {
        return const_cast<CoastSplineHex*>(&kGrid.m_kOutsideCell);
    }
    if (kGrid.field_9c) {
        while (iCol < 0) {
            iCol += kGrid.m_iWidth;
        }
        iCol = iCol % kGrid.m_iWidth;
    }
    return const_cast<CoastSplineHex*>(&kGrid.m_aCells[iRow * kCoastRowStride + iCol]);
}

/// 0x088e56a2
CoastSplineHex* Fun_088e56a2(CoastGrid const& kGrid, HEXVec_i const& kHex) {
    const HEXVec_i kOffset = Fun_086616f0(kGrid.m_mxForward, kHex);
    return Fun_088e5718(kGrid, (kOffset.y >> 1) + kOffset.x, kOffset.y);
}

/// 0x088e3f3a
void CoastGrid::ReconstructCoastSpline(CoastSplineHex const& kCell, HEXVec_i kHex, unsigned int uiEdge, FGXVector2* pPoints, bool bEdgeSlots) const {
    if (!bEdgeSlots) {
        // A back slot names the neighbour's side; its own geometry lives in the neighbour's edge slot with the mirrored id.
        const unsigned char ucBackId = kCell.m_aucBackEdges[uiEdge];
        const unsigned int uiDirection = ucBackId % 6;
        const HEXVec_i kNeighborHex = {HEXVec_i::TRANSLATIONS[uiDirection].x + kHex.x, HEXVec_i::TRANSLATIONS[uiDirection].y + kHex.y,
                                       HEXVec_i::TRANSLATIONS[uiDirection].z + kHex.z};
        CoastSplineHex const* pNeighbor = Fun_088e56a2(*this, kNeighborHex);
        unsigned int uiNeighborEdge = 0xffffffff;
        if (pNeighbor->m_ucNumEdges != 0) {
            const int iTurn = (uiDirection < 3) ? 3 : -3;
            const unsigned int uiShifted = iTurn + ucBackId;
            const unsigned int uiWantedId = uiShifted + ((uiShifted % 12 < 6) ? 6 : -6);
            uiNeighborEdge = 0;
            while (pNeighbor->m_aucEdges[uiNeighborEdge] != uiWantedId) {
                ++uiNeighborEdge;
                if (uiNeighborEdge >= pNeighbor->m_ucNumEdges) {
                    uiNeighborEdge = 0xffffffff;
                    break;
                }
            }
        }
        ReconstructCoastSpline(*pNeighbor, kNeighborHex, uiNeighborEdge, pPoints, true);
        return;
    }

    const unsigned char ucEdgeId = kCell.m_aucEdges[uiEdge];
    const FGXVector2 vFirst = kCell.m_aSegments[uiEdge].m_vFirst;
    const FGXVector2 vSecond = kCell.m_aSegments[uiEdge].m_vSecond;
    const float fDeltaX = vFirst.x - vSecond.x;
    const float fDeltaY = vFirst.y - vSecond.y;
    const float fHalfX = fDeltaX * 0.5f;
    const float fHalfY = 0.5f * fDeltaY;
    // The segment extended by its own length at both ends.
    pPoints[0].x = vFirst.x + fDeltaX;
    pPoints[0].y = vFirst.y + fDeltaY;
    pPoints[1].x = vFirst.x + fHalfX;
    pPoints[1].y = vFirst.y + fHalfY;
    pPoints[2] = vFirst;
    pPoints[3] = vSecond;
    pPoints[4].x = vSecond.x - fHalfX;
    pPoints[4].y = vSecond.y - fHalfY;
    pPoints[5].x = vSecond.x - fDeltaX;
    pPoints[5].y = vSecond.y - fDeltaY;

    SplineHandle kHandle;
    kHandle.m_kHex = kHex;
    kHandle.m_iEdge = static_cast<int>(uiEdge);
    const SplineHandle kNext = NextSplineInCoastline(kHandle, true);
    const SplineHandle kPrevious = NextSplineInCoastline(kHandle, false);
    const HEXVec_i kNextOffset = Fun_086616f0(m_mxForward, kNext.m_kHex);
    const HEXVec_i kPreviousOffset = Fun_086616f0(m_mxForward, kPrevious.m_kHex);
    const int iNextCol = (kNextOffset.y >> 1) + kNextOffset.x;
    const int iPreviousCol = (kPreviousOffset.y >> 1) + kPreviousOffset.x;

    // A neighbour across the wrapped seam sits one map width of texels away.
    if (kNext.m_iEdge != -1) {
        CoastSplineHex const* pNextCell = Fun_088e5718(*this, iNextCol, kNextOffset.y);
        int iWrappedCol = iNextCol;
        if (field_9c) {
            while (iWrappedCol < 0) {
                iWrappedCol += m_iWidth;
            }
            iWrappedCol = iWrappedCol % m_iWidth;
        }
        const float fShift = static_cast<float>((iNextCol - iWrappedCol) / m_iWidth) * m_vWrapShift.x;
        CoastEdgeSegment const& kSegment = pNextCell->m_aSegments[kNext.m_iEdge];
        pPoints[4] = kSegment.m_vSecond;
        pPoints[5] = kSegment.m_vFirst;
        pPoints[4].x += fShift;
        pPoints[5].x += fShift;
    }
    if (kPrevious.m_iEdge != -1) {
        CoastSplineHex const* pPreviousCell = Fun_088e5718(*this, iPreviousCol, kPreviousOffset.y);
        int iWrappedCol = iPreviousCol;
        if (field_9c) {
            while (iWrappedCol < 0) {
                iWrappedCol += m_iWidth;
            }
            iWrappedCol = iWrappedCol % m_iWidth;
        }
        const float fShift = static_cast<float>((iPreviousCol - iWrappedCol) / m_iWidth) * m_vWrapShift.x;
        CoastEdgeSegment const& kSegment = pPreviousCell->m_aSegments[kPrevious.m_iEdge];
        pPoints[0] = kSegment.m_vSecond;
        pPoints[1] = kSegment.m_vFirst;
        pPoints[0].x += fShift;
        pPoints[1].x += fShift;
    }

    // Outer-side edges run opposite to their stored segment.
    if (ucEdgeId % 12 >= 6) {
        const FGXVector2 vSwap0 = pPoints[0];
        pPoints[0] = pPoints[5];
        pPoints[5] = vSwap0;
        const FGXVector2 vSwap1 = pPoints[1];
        pPoints[1] = pPoints[4];
        pPoints[4] = vSwap1;
        const FGXVector2 vSwap2 = pPoints[2];
        pPoints[2] = pPoints[3];
        pPoints[3] = vSwap2;
    }
}

/// 0x088e4554
SplineHandle CoastGrid::NextSplineInCoastline(SplineHandle kHandle, bool bForward) const {
    CoastSplineHex* pCell = Fun_088e56a2(*this, kHandle.m_kHex);
    const unsigned char ucEdgeId = pCell->m_aucEdges[kHandle.m_iEdge];
    unsigned int uiCornerA = ms_CoastConnect[ucEdgeId % 3][0];
    unsigned int uiCornerB = ms_CoastConnect[ucEdgeId % 3][1];
    if (ucEdgeId >= 12) {
        uiCornerA += 12;
        uiCornerB += 12;
    }
    const unsigned int uiDirectionA = uiCornerA % 6;
    const unsigned int uiDirectionB = uiCornerB % 6;

    // The two hexes that touch the ends of the side, and the one diagonal to this hex that touches both.
    const HEXVec_i kHexA = {HEXVec_i::TRANSLATIONS[uiDirectionA].x + kHandle.m_kHex.x, HEXVec_i::TRANSLATIONS[uiDirectionA].y + kHandle.m_kHex.y,
                            HEXVec_i::TRANSLATIONS[uiDirectionA].z + kHandle.m_kHex.z};
    const HEXVec_i kHexB = {kHandle.m_kHex.x + HEXVec_i::TRANSLATIONS[uiDirectionB].x, kHandle.m_kHex.y + HEXVec_i::TRANSLATIONS[uiDirectionB].y,
                            kHandle.m_kHex.z + HEXVec_i::TRANSLATIONS[uiDirectionB].z};
    const HEXVec_i kHexDiagonal = {kHexB.x + HEXVec_i::TRANSLATIONS[uiDirectionA].x, kHexB.y + HEXVec_i::TRANSLATIONS[uiDirectionA].y,
                                   kHexB.z + HEXVec_i::TRANSLATIONS[uiDirectionA].z};
    CoastSplineHex* pCellA = Fun_088e56a2(*this, kHexA);
    CoastSplineHex* pCellB = Fun_088e56a2(*this, kHexB);
    CoastSplineHex* pCellDiagonal = Fun_088e56a2(*this, kHexDiagonal);

    // An edge id of the same class (shore ids below 12, shelf ids from 12 up) as the corner.
    auto SameClass = [](unsigned int uiId, unsigned int uiCorner) -> bool {
        return (uiId < 12 && uiCorner < 12) || (uiId > 11 && uiCorner > 11);
    };

    if (!bForward) {
        HEXVec kHexAFloat = {static_cast<float>(kHexA.x), static_cast<float>(kHexA.y), static_cast<float>(kHexA.z), 1.0f};
        HEXVec kHexDiagonalFloat = {static_cast<float>(kHexDiagonal.x), static_cast<float>(kHexDiagonal.y), static_cast<float>(kHexDiagonal.z), 1.0f};
        bool bFound = false;
        kHandle = RefactorfoRVS(*pCell, uiCornerA, *pCellA, kHandle, kHexAFloat, *pCellDiagonal, uiCornerB, kHexDiagonalFloat, bFound);
        if (bFound) {
            return kHandle;
        }
    } else {
        // A side listed in the back slots belongs to the neighbour across it; the match is its edge in the opposite direction.
        if (pCell->m_ucNumBackEdges != 0) {
            const int iTurnB = (uiDirectionB < 3) ? 3 : -3;
            const unsigned int uiOppositeB = (iTurnB + uiCornerB) % 6;
            for (unsigned int uiBack = 0; uiBack < pCell->m_ucNumBackEdges; ++uiBack) {
                const unsigned char ucBackId = pCell->m_aucBackEdges[uiBack];
                if (SameClass(ucBackId, uiCornerB) && ucBackId % 6 == uiDirectionB) {
                    for (unsigned int uiEdge = 0; uiEdge < pCellB->m_ucNumEdges; ++uiEdge) {
                        const unsigned char ucNeighborId = pCellB->m_aucEdges[uiEdge];
                        if (SameClass(ucNeighborId, uiCornerB) && ucNeighborId % 6 == uiOppositeB) {
                            kHandle.m_kHex = kHexB;
                            kHandle.m_iEdge = static_cast<int>(uiEdge);
                            return kHandle;
                        }
                    }
                }
            }
        }
        if (pCellDiagonal->m_ucNumEdges != 0) {
            const int iTurnA = (uiDirectionA < 3) ? 3 : -3;
            const unsigned int uiOppositeA = (iTurnA + uiCornerA) % 6;
            for (unsigned int uiEdge = 0; uiEdge < pCellDiagonal->m_ucNumEdges; ++uiEdge) {
                const unsigned char ucDiagonalId = pCellDiagonal->m_aucEdges[uiEdge];
                if (SameClass(ucDiagonalId, uiCornerA) && ucDiagonalId % 6 == uiOppositeA) {
                    kHandle.m_kHex = kHexDiagonal;
                    kHandle.m_iEdge = static_cast<int>(uiEdge);
                    return kHandle;
                }
            }
        }
    }
    kHandle.m_iEdge = -1;
    return kHandle;
}

/// 0x088e4cd0
void CoastGrid::Init(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& kGameGrid) {
    m_kTransform.field_0 = kGameGrid.m_kTransform.field_0;
    m_kTransform.field_4 = kGameGrid.m_kTransform.field_4;
    m_kTransform.field_8 = kGameGrid.m_kTransform.field_8;
    m_mxForward = m_kTransform.matrix();
    m_mxInverse = m_kTransform.inv_matrix();
    m_iWidth = kGameGrid.m_iWidth;
    m_iHeight = kGameGrid.m_iHeight;

    const D3DXMATRIX* pHexToTex = reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex);
    // Texture position of a hex given as an offset from the plot grid origin.
    auto HexToTex = [&kGameGrid, pHexToTex](HEXVec_i const& kOffset) -> D3DXVECTOR4 {
        const HEXVec_i kHex = Fun_086616f0(kGameGrid.m_mxInverse, kOffset);
        const D3DXVECTOR4 kHexF = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), static_cast<float>(-(kHex.y + kHex.x)), 1.0f};
        D3DXVECTOR4 kTex;
        D3DXVec4Transform(&kTex, &kHexF, pHexToTex);
        return kTex;
    };

    const HEXVec_i kZero = {0, 0, 0};
    const D3DXVECTOR4 kOriginTex = HexToTex(kZero);
    m_vOriginTex.x = kOriginTex.x;
    m_vOriginTex.y = kOriginTex.y;

    // Offsets are truncated; the six translations are whole hexes.
    HEXVec const* aTranslations = reinterpret_cast<HEXVec const*>(HEXVec::TRANSLATIONS);
    const HEXVec_i kOriginHex = Fun_086616f0(kGameGrid.m_mxInverse, kZero);
    for (int i = 0; i < 6; ++i) {
        const HEXVec_i kNeighbor = {static_cast<int>(aTranslations[i].x) + kOriginHex.x, static_cast<int>(aTranslations[i].y) + kOriginHex.y,
                                    static_cast<int>(aTranslations[i].z) - (kOriginHex.y + kOriginHex.x)};
        const D3DXVECTOR4 kNeighborHexF = {static_cast<float>(kNeighbor.x), static_cast<float>(kNeighbor.y), static_cast<float>(kNeighbor.z), 1.0f};
        D3DXVECTOR4 kNeighborTex;
        D3DXVec4Transform(&kNeighborTex, &kNeighborHexF, pHexToTex);
        m_avNeighborOffset[i].x = kNeighborTex.x - m_vOriginTex.x;
        m_avNeighborOffset[i].y = kNeighborTex.y - m_vOriginTex.y;
    }

    const HEXVec_i kFarEdge = {m_iWidth, 0, -m_iWidth};
    const D3DXVECTOR4 kFarTex = HexToTex(kFarEdge);
    const D3DXVECTOR4 kNearTex = HexToTex(kZero);
    m_vWrapShift.x = kFarTex.x - kNearTex.x;
    m_vWrapShift.y = kFarTex.y - kNearTex.y;

    // Per hex direction 0..2 the three neighbours that bound the side: before, across, after.
    static const int aaiSideNeighbors[3][3] = {{4, 0, 5}, {5, 1, 3}, {3, 2, 4}};
    for (int i = 0; i < 3; ++i) {
        const FGXVector2& vBefore = m_avNeighborOffset[aaiSideNeighbors[i][0]];
        const FGXVector2& vAcross = m_avNeighborOffset[aaiSideNeighbors[i][1]];
        const FGXVector2& vAfter = m_avNeighborOffset[aaiSideNeighbors[i][2]];
        // The corners of the side are a third of the way to the neighbour pairs; the segment is the middle half.
        const float fCornerAX = (vBefore.x + vAcross.x) / 3.0f;
        const float fCornerAY = (vBefore.y + vAcross.y) / 3.0f;
        const float fCornerBX = (vAcross.x + vAfter.x) / 3.0f;
        const float fCornerBY = (vAcross.y + vAfter.y) / 3.0f;
        m_aEdgeTemplates[i].m_vFirst.x = fCornerAX * 0.75f + fCornerBX * 0.25f;
        m_aEdgeTemplates[i].m_vFirst.y = fCornerAY * 0.75f + fCornerBY * 0.25f;
        m_aEdgeTemplates[i].m_vSecond.x = fCornerAX * 0.25f + fCornerBX * 0.75f;
        m_aEdgeTemplates[i].m_vSecond.y = fCornerAY * 0.25f + fCornerBY * 0.75f;
    }

    const int iWidth = kGameGrid.m_iWidth;
    const int iHeight = kGameGrid.m_iHeight;
    InitGridJob::ms_pCoastGrid = this;
    InitGridJob::ms_pGameGrid = &kGameGrid;

    // The grid is filled on the calling thread: the jobs are run directly and the set stays empty.
    FJobSet kInitJobs;
    InitGridJob* aInitJobs = static_cast<InitGridJob*>(_alloca(iHeight * sizeof(InitGridJob)));
    for (int iRow = 0; iRow < iHeight; ++iRow) {
        new (&aInitJobs[iRow]) InitGridJob();
        aInitJobs[iRow].m_iRow = iRow;
        aInitJobs[iRow].m_iWidth = iWidth;
        aInitJobs[iRow].InitGridJob::Execute(0);
    }
    kInitJobs.Wait();

    FJobSet kJobSet;
    DetectCoastJob::ms_pCoastData = m_aEdgeTemplates;
    DetectCoastJob::ms_pCoastGrid = this;
    DetectCoastJob* aDetectJobs = static_cast<DetectCoastJob*>(_alloca(iHeight * sizeof(DetectCoastJob)));
    for (int iRow = 0; iRow < iHeight; ++iRow) {
        new (&aDetectJobs[iRow]) DetectCoastJob();
        aDetectJobs[iRow].m_iRow = iRow;
        aDetectJobs[iRow].m_iWidth = iWidth;
        kJobSet.AddJob(&aDetectJobs[iRow], kJobTag, nullptr);
    }
    kJobSet.Wait();

    GenerateCoastConnectivityJob::ms_pCoastGrid = this;
    GenerateCoastConnectivityJob* aConnectJobs = static_cast<GenerateCoastConnectivityJob*>(_alloca(iHeight * sizeof(GenerateCoastConnectivityJob)));
    for (int iRow = 0; iRow < iHeight; ++iRow) {
        new (&aConnectJobs[iRow]) GenerateCoastConnectivityJob();
        aConnectJobs[iRow].m_iRow = iRow;
        aConnectJobs[iRow].m_iWidth = iWidth;
        kJobSet.AddJob(&aConnectJobs[iRow], kJobTag, nullptr);
    }
    kJobSet.Wait();

    SplinePreProcessJob::ms_pCoastGrid = this;
    // Every smoothing pass reads the cells and writes this scratch copy; the commit pass copies it back.
    CoastEdgeSegment* aScratch = static_cast<CoastEdgeSegment*>(_alloca(iWidth * iHeight * 3 * sizeof(CoastEdgeSegment)));
    SplinePreProcessJob* aPreProcessJobs = static_cast<SplinePreProcessJob*>(_alloca(iHeight * sizeof(SplinePreProcessJob)));
    for (int iRow = 0; iRow < iHeight; ++iRow) {
        new (&aPreProcessJobs[iRow]) SplinePreProcessJob();
        aPreProcessJobs[iRow].m_iRow = iRow;
        aPreProcessJobs[iRow].m_iWidth = iWidth;
        aPreProcessJobs[iRow].m_pScratch = aScratch + iRow * iWidth * 3;
    }
    for (int iState = 0; iState < SplinePreProcessJob::ms_nStateList; ++iState) {
        SplinePreProcessJob::ms_uCurrState = SplinePreProcessJob::ms_aStateList[iState];
        for (int iRow = 0; iRow < iHeight; ++iRow) {
            kJobSet.AddJob(&aPreProcessJobs[iRow], kJobTag, nullptr);
        }
        kJobSet.Wait();
    }
}

/// 0x092141e4
unsigned int CoastGrid::ms_CoastConnect[3][2] = {{4, 5}, {5, 3}, {3, 4}};
