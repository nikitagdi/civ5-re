/// origin guess
#include <math.h>
#include <Windows.h>
#include "Civ5/Src/App/Engine/GameSplineDatabase.h"
#include "Civ5/Src/App/Engine/cvRiverSimSystem.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/CoastGrid.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvCoastSplineSystem.h"
#include "Civ5/Src/App/cvCityBuildingSim.h"
#include <stdio.h>
#include <string.h>
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FTextFile.h"
#include "FirePlace/FireWorks/FXmlAttribute.h"
#include "FirePlace/FireWorks/FXmlDocument.h"
#include "FirePlace/FireWorks/FXmlElement.h"

namespace {

// A spline needs at least this many control points to be kept.
const unsigned int kMinSplinePoints = 4;
// Control points reserved for every new spline.
const unsigned int kSplinePointReserve = 0x800;
// Deepest noise pyramid, base of the amplitude falloff and the noise scale in texture units.
const unsigned int kMaxNoiseLevels = 4;
const float kNoiseFalloff = 0.78f;
const float kNoiseScale = 35.0f;
const int kNoiseResolution = 1000;
// Splines with fewer points get no noise.
const unsigned int kMinNoisePoints = 6;

}  // namespace

/// 0x088edcb2
void Fun_088edcb2(CoastWavelet* pWavelet) {
    const int iSize = pWavelet->m_iSize;
    const int iHalf = iSize / 2 + 1;
    int iSource;
    int iWaveletTarget;
    int iScalingTarget;
    if (pWavelet->m_iLevel & 1) {
        iSource = iSize * 2 - 2;
        iWaveletTarget = iSize + 2;
        iScalingTarget = 0;
    } else {
        iSource = 0;
        iWaveletTarget = iHalf + iSize;
        iScalingTarget = iSize;
    }
    FGXVector2* pData = pWavelet->m_pData;
    B_SPLINE::DecomposeWavelet<FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(pData + iSource, pData + iSource + iSize, pData + iWaveletTarget);
    pData = pWavelet->m_pData;
    B_SPLINE::DecomposeScalingFunc<FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(pData + iSource, pData + iSource + iSize, pData + iScalingTarget);
    pWavelet->m_iSize = iHalf;
    pWavelet->m_iOffset = iScalingTarget;
    --pWavelet->m_iLevel;
}

/// 0x088edd64
void Fun_088edd64(CoastWavelet* pWavelet) {
    if (pWavelet->m_iLevel == 0) {
        const unsigned int uiOldSize = pWavelet->m_iSize;
        const unsigned int uiNewSize = uiOldSize * 2 - 2;
        const unsigned int uiNeeded = uiOldSize * 4 - 3;
        if (pWavelet->m_uiMaxData < uiNeeded) {
            Fun_088ea67c(&pWavelet->m_pData, uiNeeded);
        }
        pWavelet->m_uiNumData = (uiNeeded < pWavelet->m_uiMaxData) ? uiNeeded : pWavelet->m_uiMaxData;
        FGXVector2* pData = pWavelet->m_pData;
        B_SPLINE::ReconstructScalingFunc<FGXVector2, FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(pData, pData + uiOldSize, pData + uiNewSize);
        pWavelet->m_iSize = uiNewSize;
    } else {
        const int iLevel = pWavelet->m_iLevel + 1;
        pWavelet->m_iLevel = iLevel;
        pWavelet->m_iOffset = 0;
        const int iOldSize = pWavelet->m_iSize;
        const int iNewSize = iOldSize * 2 - 2;
        pWavelet->m_iSize = iNewSize;
        const int iHalf = (iNewSize >> 1) + 1;
        int iWaveletSource;
        int iTarget;
        int iScalingSource;
        if ((iLevel & 1) == 0) {
            iWaveletSource = iNewSize + iHalf;
            iTarget = 0;
            iScalingSource = iNewSize;
        } else {
            iWaveletSource = iOldSize * 2;
            iTarget = iOldSize * 2 - 4 + iNewSize;
            iScalingSource = 0;
        }
        FGXVector2* pData = pWavelet->m_pData;
        B_SPLINE::ReconstructWavelet<FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(pData + iWaveletSource, pData + (iHalf - 2 + iWaveletSource), pData + iTarget);
        pData = pWavelet->m_pData;
        B_SPLINE::ReconstructScalingFunc_ADD<FGXVector2, FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(pData + iScalingSource, pData + iHalf + iScalingSource, pData + iNewSize + iTarget);
        pWavelet->m_iOffset = iTarget;
    }
}

/// 0x088ec17c
void cvCoastSplineSystem::Startup(CoastGrid const& kGrid) {
    m_iGridWidth = kGrid.m_iWidth;
    m_iGridHeight = kGrid.m_iHeight;
    ModifyAndExtractCoastlines(kGrid);
    m_bStarted = true;
}

/// 0x088ec1b8
void cvCoastSplineSystem::ModifyAndExtractCoastlines(CoastGrid const& kGrid) {
    m_iGridWidth = kGrid.m_iWidth;
    m_iGridHeight = kGrid.m_iHeight;
    // The texture-space shift of one grid width: the hex at (width, 0) against the origin hex.
    const HEXVec_i kWidthHex = {kGrid.m_iWidth, 0, -kGrid.m_iWidth};
    const HEXVec_i kEndGameHex = Fun_086616f0(kGrid.m_mxInverse, kWidthHex);
    D3DXVECTOR4 kEndHex = {(float)kEndGameHex.x, (float)kEndGameHex.y, (float)-(kEndGameHex.y + kEndGameHex.x), 1.0f};
    D3DXMATRIX const* pHexToTex = reinterpret_cast<D3DXMATRIX const*>(WorldGlobals::g_HexToTex);
    D3DXVECTOR4 kEndTex;
    D3DXVec4Transform(&kEndTex, &kEndHex, pHexToTex);
    const HEXVec_i kZeroHex = {0, 0, 0};
    const HEXVec_i kOriginGameHex = Fun_086616f0(kGrid.m_mxInverse, kZeroHex);
    D3DXVECTOR4 kOriginHex = {(float)kOriginGameHex.x, (float)kOriginGameHex.y, (float)-(kOriginGameHex.y + kOriginGameHex.x), 1.0f};
    D3DXVECTOR4 kOriginTex;
    D3DXVec4Transform(&kOriginTex, &kOriginHex, pHexToTex);
    m_vWrapShift.y = kEndTex.y - kOriginTex.y;
    m_vWrapShift.x = kEndTex.x - kOriginTex.x;

    // Adds a spline starting at one edge slot of a cell; it is dropped again when it ends up too short.
    struct SplineAdder {
        static void Add(cvCoastSplineSystem& kSystem, CoastGrid const& kGrid, int iCol, int iRow, int iSlot) {
            if (kSystem.m_uiNumSplines == kSystem.m_uiMaxSplines) {
                Fun_088ee572(&kSystem.m_pSplines, kSystem.m_uiNumSplines + 1);
            }
            ++kSystem.m_uiNumSplines;
            CoastlineSpline* pNew = &kSystem.m_pSplines[kSystem.m_uiNumSplines - 1];
            pNew->m_pControlPoints = 0;
            pNew->m_uiNumPoints = 0;
            pNew->m_uiMaxPoints = 0;
            pNew->unk_c = 0;
            pNew->m_ucStartEdge = 0;
            pNew->unk_e[0] = 0;
            pNew->unk_e[1] = 0;
            pNew->m_iStartCol = 0;
            pNew->m_iStartRow = 0;
            pNew->m_bClosed = false;
            pNew->m_vWrapOffset.x = 0.0f;
            pNew->m_vWrapOffset.y = 0.0f;
            pNew->m_eType = 0;
            pNew->m_iContinent = 0;
            CoastlineSpline* pSpline = &kSystem.m_pSplines[kSystem.m_uiNumSplines - 1];
            if (pSpline->m_uiMaxPoints < kSplinePointReserve) {
                Fun_088e9ff8(reinterpret_cast<CoastControlPointVector*>(pSpline), kSplinePointReserve);
            }
            pSpline->m_iStartRow = iRow;
            pSpline->m_iStartCol = iCol;
            pSpline->m_ucStartEdge = (unsigned char)iSlot;
            kSystem.ExtractSingleCoastlineFromGrid(*pSpline, kGrid);
            if (pSpline->m_uiNumPoints < kMinSplinePoints) {
                --kSystem.m_uiNumSplines;
                Fun_0868a412(&kSystem.m_pSplines[kSystem.m_uiNumSplines]);
            }
        }
    };

    // Open coastlines first: a start link that nothing leads into.
    for (int iRow = 0; iRow < m_iGridHeight; ++iRow) {
        for (int iCol = 0; iCol < m_iGridWidth; ++iCol) {
            CoastSplineHex* pCell = Fun_088e5718(kGrid, iCol, iRow);
            for (int iSlot = 0; iSlot < 3; ++iSlot) {
                if (pCell->m_aNextLinks[iSlot].m_ucPeerEdge != 0xff && pCell->m_aPrevLinks[iSlot].m_ucPeerEdge == 0xff && !pCell->m_aNextLinks[iSlot].m_bMarked) {
                    SplineAdder::Add(*this, kGrid, iCol, iRow, iSlot);
                }
            }
        }
    }
    // What is left are closed loops.
    for (int iRow = 0; iRow < m_iGridHeight; ++iRow) {
        for (int iCol = 0; iCol < m_iGridWidth; ++iCol) {
            CoastSplineHex* pCell = Fun_088e5718(kGrid, iCol, iRow);
            for (int iSlot = 0; iSlot < 3; ++iSlot) {
                if (pCell->m_aNextLinks[iSlot].m_ucPeerEdge != 0xff && !pCell->m_aNextLinks[iSlot].m_bMarked) {
                    SplineAdder::Add(*this, kGrid, iCol, iRow, iSlot);
                }
            }
        }
    }
    ClearMarks(kGrid);
    for (unsigned int i = 0; i < m_uiNumSplines; ++i) {
        AddNoiseToSingleCoastline(m_pSplines[i]);
    }
}

/// 0x088ec7a2
void cvCoastSplineSystem::ExtractSingleCoastlineFromGrid(CoastlineSpline& kSpline, CoastGrid const& kGrid) const {
    auto PushPoint = [](CoastlineSpline& kTarget, FGXVector2 const& vPos, float fWeight, unsigned char ucStyleA, unsigned char ucStyleB) {
        CoastCP kPoint;
        kPoint.m_vPos = vPos;
        kPoint.m_fBlendWeight = fWeight;
        kPoint.m_ucStyleA = ucStyleA;
        kPoint.m_ucStyleB = ucStyleB;
        Fun_088edbc0(kTarget, kPoint);
    };
    // How many whole grid widths a column lies outside the grid (negative: left of it).
    auto GridWrapCount = [](int iColumn, int iWidth) {
        int iWraps = 0;
        int iShifted = iColumn;
        if (iShifted < 0) {
            do {
                --iWraps;
                iShifted += iWidth;
            } while (iShifted < 0);
        }
        if (iWidth <= iShifted) {
            iWraps = iShifted / iWidth;
        }
        return iWraps;
    };
    kSpline.m_iContinent = 1;
    kSpline.m_bClosed = false;
    unsigned char ucEdge = kSpline.m_ucStartEdge;
    int iCol = kSpline.m_iStartCol;
    int iRow = kSpline.m_iStartRow;
    CoastSplineHex* pCell = Fun_088e5718(kGrid, iCol, iRow);
    CoastEdgeLink* pLink = &pCell->m_aNextLinks[ucEdge];
    CoastEdgeSegment const& kSegment = pCell->m_aSegments[ucEdge];
    const bool bShore = (pCell->m_aucEdges[ucEdge] % 12) < 6;
    FGXVector2 const& vNearEnd = bShore ? kSegment.m_vFirst : kSegment.m_vSecond;
    FGXVector2 const& vFarEnd = bShore ? kSegment.m_vSecond : kSegment.m_vFirst;

    const unsigned char ucFirstKind = pCell->m_aucEdgeKind[ucEdge];
    PushPoint(kSpline, vNearEnd, 1.0f, ucFirstKind, 0);
    const unsigned char ucSecondKind = pCell->m_aucEdgeKind[ucEdge];
    float fWeight = (ucFirstKind == ucSecondKind) ? 1.0f : 0.0f;
    PushPoint(kSpline, vFarEnd, fWeight, ucSecondKind, ucFirstKind);
    unsigned char ucPreviousKind = ucSecondKind;
    unsigned char ucCurrentKind = ucFirstKind;
    // The terrain class of the first cell decides which side of the coastline is land.
    if (bShore) {
        if (pCell->m_ucTerrainClass == 1) {
            kSpline.m_eType = 1;
        } else if (pCell->m_ucTerrainClass == 0) {
            kSpline.m_eType = 0;
        }
    } else {
        if (pCell->m_ucTerrainClass == 2) {
            kSpline.m_eType = 1;
        } else if (pCell->m_ucTerrainClass == 1) {
            kSpline.m_eType = 0;
        }
    }

    if (!pLink->m_bMarked) {
        float fWrapShift = 0.0f;
        do {
            ucEdge = pLink->m_ucPeerEdge;
            iCol = pLink->m_iCol;
            iRow = pLink->m_iRow;
            CoastSplineHex* pNext = Fun_088e5718(kGrid, iCol, iRow);
            fWrapShift = fWrapShift + (float)GridWrapCount(iCol, m_iGridWidth) * m_vWrapShift.x;
            CoastEdgeSegment const& kNextSegment = pNext->m_aSegments[ucEdge];
            const bool bNextShore = (pNext->m_aucEdges[ucEdge] % 12) < 6;
            FGXVector2 const& vNextNear = bNextShore ? kNextSegment.m_vFirst : kNextSegment.m_vSecond;
            FGXVector2 const& vNextFar = bNextShore ? kNextSegment.m_vSecond : kNextSegment.m_vFirst;
            const unsigned char ucKind = pNext->m_aucEdgeKind[ucEdge];
            // The blend weight flips between 0 and 1 whenever the style changes along the coastline.
            if (ucCurrentKind != ucKind) {
                fWeight = (fWeight == 1.0f) ? 0.0f : 1.0f;
                ucCurrentKind = ucKind;
            }
            FGXVector2 vNear = {vNextNear.x + fWrapShift, vNextNear.y};
            PushPoint(kSpline, vNear, fWeight, ucKind, ucPreviousKind);
            FGXVector2 vFar = {vNextFar.x + fWrapShift, vNextFar.y};
            PushPoint(kSpline, vFar, fWeight, ucKind, ucKind);
            ucPreviousKind = ucKind;
            pLink->m_bMarked = true;
            pLink = &pNext->m_aNextLinks[ucEdge];
        } while (!pLink->m_bMarked);
    }

    int iStartCol = kSpline.m_iStartCol;
    if (kGrid.field_9c) {
        const int iWidth = kGrid.m_iWidth;
        while (iStartCol < 0) {
            iStartCol += iWidth;
        }
        iStartCol = iStartCol % iWidth;
        while (iCol < 0) {
            iCol += iWidth;
        }
        iCol = iCol % iWidth;
    }
    if (iCol == iStartCol && iRow == kSpline.m_iStartRow && kSpline.m_ucStartEdge == ucEdge) {
        kSpline.m_bClosed = true;
        CoastCP const* pPoints = kSpline.m_pControlPoints;
        const float fLastX = pPoints[kSpline.m_uiNumPoints - 1].m_vPos.x;
        const float fSecondX = pPoints[1].m_vPos.x;
        kSpline.m_vWrapOffset.y = pPoints[kSpline.m_uiNumPoints - 1].m_vPos.y - pPoints[1].m_vPos.y;
        kSpline.m_vWrapOffset.x = fLastX - fSecondX;
    }
}

/// 0x088ecda6
void cvCoastSplineSystem::ClearMarks(CoastGrid const& kGrid) {
    for (int iRow = 0; iRow < kGrid.m_iHeight; ++iRow) {
        for (int iCol = 0; iCol < kGrid.m_iWidth; ++iCol) {
            CoastSplineHex* pCell = Fun_088e5718(kGrid, iCol, iRow);
            for (int iSlot = 0; iSlot < 3; ++iSlot) {
                pCell->m_aNextLinks[iSlot].m_bMarked = false;
                pCell->m_aPrevLinks[iSlot].m_bMarked = false;
            }
        }
    }
}

/// 0x088ece2e
void cvCoastSplineSystem::AddNoiseToSingleCoastline(CoastlineSpline& kSpline) const {
    // Moves a point along the left normal of its spline gradient.
    auto OffsetAlongNormal = [](FGXVector2& vPoint, FGXVector2 const& vGradient, float fDistance) {
        const float fLength = sqrtf(vGradient.x * vGradient.x + vGradient.y * vGradient.y);
        vPoint.x = (vGradient.y / fLength) * fDistance + vPoint.x;
        vPoint.y = vPoint.y - (vGradient.x / fLength) * fDistance;
    };
    auto EvalSplineGradient = [](int iCount, FGXVector2 const* pPoints, float fT, int iBegin) {
        return B_SPLINE::EvalGradient<FGXVector2, FGXVector2 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(iCount, pPoints, fT, iBegin);
    };
    // One draw: a displacement in [-0.5, 0.5) of the given extent.
    auto RandomDisplacement = [](float fExtent) {
        const int iRandom = (int)rand_wincompatible();
        return ((float)(iRandom % kNoiseResolution) / (float)kNoiseResolution + -0.5f) * fExtent;
    };
    const unsigned int uiNumPoints = kSpline.m_uiNumPoints;
    unsigned int uiCoarse = uiNumPoints;
    unsigned int uiLevels = 0;
    unsigned int uiPadded = uiNumPoints;
    // The points are padded so that uiLevels halvings reach uiCoarse points; at most kMaxNoiseLevels levels.
    if (uiNumPoints >= kMinNoisePoints) {
        do {
            uiCoarse = ((uiCoarse & 1) + uiCoarse) / 2 + 1;
            ++uiLevels;
        } while (uiLevels <= kMaxNoiseLevels - 1 && uiCoarse > 5);
        uiPadded = (uiCoarse - 2) * (1u << uiLevels) + 2;
    }

    if (uiLevels != 0) {
        CoastWavelet kWavelet;
        FGXVector2 kFill = {0.0f, 0.0f};
        Fun_088edc1a(&kWavelet, (int)uiPadded, &kFill);
        FGXVector2* pData = kWavelet.m_pData + kWavelet.m_iOffset;
        for (unsigned int i = 0; i < uiNumPoints; ++i) {
            pData[i] = kSpline.m_pControlPoints[i].m_vPos;
        }
        for (unsigned int i = uiNumPoints; i < uiPadded; ++i) {
            pData[i] = pData[uiNumPoints - 1];
        }
        for (unsigned int uiStep = 0; uiStep < uiLevels; ++uiStep) {
            Fun_088edcb2(&kWavelet);
        }
        float fAmplitude = powf(kNoiseFalloff, (float)(5 - (int)uiLevels));
        // Each level refines the pyramid, then displaces every point of the new level along its normal.
        for (unsigned int uiLevel = 0; uiLevel < uiLevels; ++uiLevel) {
            Fun_088edd64(&kWavelet);
            const float fExtent = fAmplitude * kNoiseScale;
            const int iCount = kWavelet.m_iSize;
            FGXVector2* pLevel = kWavelet.m_pData + kWavelet.m_iOffset;
            FGXVector2 const* pCurve = pLevel + uiLevel;
            OffsetAlongNormal(pLevel[0], EvalSplineGradient(iCount, pCurve, 0.0f, (int)uiLevel), RandomDisplacement(fExtent));
            const int iLast = iCount - 1;
            if ((unsigned int)iLast >= 2) {
                float fT = 0.5f;
                for (int i = 1; (unsigned int)i < (unsigned int)iLast; ++i) {
                    const float fDistance = RandomDisplacement(fExtent);
                    OffsetAlongNormal(pLevel[i], EvalSplineGradient(iCount, pCurve, fT, (int)uiLevel), fDistance);
                    fT = fT + 1.0f;
                }
            }
            // The last point draws a number it does not use, and moves by the plain amplitude.
            rand_wincompatible();
            OffsetAlongNormal(pLevel[iLast], EvalSplineGradient(iCount, pLevel + iCount, (float)(iCount - 2), iCount), fAmplitude);
            fAmplitude = fAmplitude * fAmplitude;
        }
        FGXVector2 const* pResult = kWavelet.m_pData + kWavelet.m_iOffset;
        for (unsigned int i = 0; i < kSpline.m_uiNumPoints; ++i) {
            kSpline.m_pControlPoints[i].m_vPos = pResult[i];
        }
        Fun_08683534(&kWavelet.m_pData);
    }

    // A closed coastline repeats its last two points in front of the first two.
    if (kSpline.m_bClosed) {
        CoastCP* pPoints = kSpline.m_pControlPoints;
        const unsigned int uiBeforeLast = kSpline.m_uiNumPoints - 2;
        pPoints[0].m_vPos.x = pPoints[uiBeforeLast].m_vPos.x - kSpline.m_vWrapOffset.x;
        pPoints[0].m_vPos.y = pPoints[uiBeforeLast].m_vPos.y - kSpline.m_vWrapOffset.y;
        pPoints[1].m_vPos.x = pPoints[kSpline.m_uiNumPoints - 1].m_vPos.x - kSpline.m_vWrapOffset.x;
        pPoints[1].m_vPos.y = pPoints[kSpline.m_uiNumPoints - 1].m_vPos.y - kSpline.m_vWrapOffset.y;
    }
}

/// 0x088ed458
void cvCoastSplineSystem::AddSingleCoastlineToDatabase(CoastlineSpline const& kSpline, CoastGrid const& kGrid) const {
    if (kSpline.m_eType != 0) {
        return;
    }
    // How many whole grid widths a column lies outside the grid (negative: left of it).
    auto GridWrapCount = [](int iColumn, int iWidth) {
        int iWraps = 0;
        int iShifted = iColumn;
        if (iShifted < 0) {
            do {
                --iWraps;
                iShifted += iWidth;
            } while (iShifted < 0);
        }
        if (iWidth <= iShifted) {
            iWraps = iShifted / iWidth;
        }
        return iWraps;
    };
    GameSplineDatabase* pDatabase = reinterpret_cast<GameSplineDatabase*>(g_GameSplineDatabase);
    int iCol = kSpline.m_iStartCol;
    int iRow = kSpline.m_iStartRow;
    unsigned char ucEdge = kSpline.m_ucStartEdge;
    CoastSplineHex* pCell = Fun_088e5718(kGrid, iCol, iRow);
    HexEdge kEdge;
    kEdge.m_kHex.x = 0;
    kEdge.m_kHex.y = 0;
    kEdge.m_kHex.z = 0;
    kEdge.m_iEdge = pCell->m_aucEdges[ucEdge];
    CoastCP const* pPoints = kSpline.m_pControlPoints;
    // The database takes the segment end first, then its start.
    FGXVector2 avSegment[2] = {pPoints[1].m_vPos, pPoints[0].m_vPos};
    const int iHexX = iCol - (iRow >> 1);
    const HEXVec_i kCubeHex = {iHexX, iRow, -(iHexX + iRow)};
    const HEXVec_i kGameHex = Fun_086616f0(kGrid.m_mxInverse, kCubeHex);
    kEdge.m_kHex.x = kGameHex.x;
    kEdge.m_kHex.y = kGameHex.y;
    kEdge.m_kHex.z = -(kGameHex.y + kGameHex.x);
    pDatabase->SubmitSegmentToDatabase(kEdge, avSegment, 2);

    CoastEdgeLink* pLink = &pCell->m_aNextLinks[ucEdge];
    if (!pLink->m_bMarked) {
        float fWrapShift = 0.0f;
        unsigned int uiSegment = 1;
        do {
            ucEdge = pLink->m_ucPeerEdge;
            iCol = pLink->m_iCol;
            iRow = pLink->m_iRow;
            CoastSplineHex* pNext = Fun_088e5718(kGrid, iCol, iRow);
            kEdge.m_iEdge = pNext->m_aucEdges[ucEdge];
            fWrapShift = fWrapShift + (float)GridWrapCount(iCol, m_iGridWidth) * m_vWrapShift.x;
            CoastCP const& kStart = pPoints[uiSegment * 2];
            CoastCP const& kEnd = pPoints[uiSegment * 2 + 1];
            avSegment[1].x = kStart.m_vPos.x - fWrapShift;
            avSegment[1].y = kStart.m_vPos.y;
            avSegment[0].y = kEnd.m_vPos.y;
            avSegment[0].x = kEnd.m_vPos.x - fWrapShift;
            const int iNextHexX = iCol - (iRow >> 1);
            const HEXVec_i kNextCubeHex = {iNextHexX, iRow, -(iNextHexX + iRow)};
            const HEXVec_i kNextGameHex = Fun_086616f0(kGrid.m_mxInverse, kNextCubeHex);
            kEdge.m_kHex.x = kNextGameHex.x;
            kEdge.m_kHex.y = kNextGameHex.y;
            kEdge.m_kHex.z = -(kNextGameHex.y + kNextGameHex.x);
            Fun_0865f23e(kEdge.m_kHex);
            pDatabase->SubmitSegmentToDatabase(kEdge, avSegment, 2);
            pLink->m_bMarked = true;
            pLink = &pNext->m_aNextLinks[ucEdge];
            ++uiSegment;
        } while (!pLink->m_bMarked);
    }
}

/// 0x088ef232
int cvCoastMeshLibrary::LoadSplineFromFile(char const* pszFile) {
    // Samples per profile; the file's points are smoothed down two levels and back, and the difference is stored.
    const unsigned int kNumSamples = 98;
    FTextFile kFile(pszFile, FIFile::modeRead, 0, 0);
    FStringA kLine;
    FGXVector2 aRaw[kNumSamples] = {};
    unsigned int uiNumRaw = 0;
    while (kFile.ReadString(kLine) && uiNumRaw <= kNumSamples - 1) {
        FGXVector2 vRaw;
        float fUnused;
        sscanf(static_cast<char const*>(kLine), "[%f,%f,%f]", &vRaw.x, &vRaw.y, &fUnused);
        aRaw[uiNumRaw] = vRaw;
        ++uiNumRaw;
    }
    if (m_uiNumSplines == m_uiMaxSplines) {
        Fun_088ef866(&m_pSplines, m_uiNumSplines + 1);
    }
    const unsigned int uiProfile = m_uiNumSplines;
    m_uiNumSplines = uiProfile + 1;
    CoastCharacterSpline* pProfile = &m_pSplines[uiProfile];
    pProfile->m_pSamples = 0;
    pProfile->m_uiNumSamples = 0;
    pProfile->m_uiMaxSamples = 0;

    // aSmooth[0] and aSmooth[99] are extrapolated end points, aSmooth[1..98] the smoothed samples.
    // The coarse levels live in the same storage; the reconstruction runs backwards in place.
    FGXVector2 aSmooth[kNumSamples + 2];
    FGXVector2* pLevel1 = aSmooth + 50;
    FGXVector2* pLevel2 = aSmooth + 1;
    B_SPLINE::DecomposeScalingFunc<FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(aRaw, aRaw + kNumSamples, pLevel1);
    B_SPLINE::DecomposeScalingFunc<FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(pLevel1, pLevel1 + 50, pLevel2);
    B_SPLINE::ReconstructScalingFunc<FGXVector2, FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(pLevel2, pLevel2 + 26, aSmooth + 51);
    B_SPLINE::ReconstructScalingFunc<FGXVector2, FGXVector2*, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(pLevel2, aSmooth + 51, aSmooth + 99);
    const float fStep = aSmooth[2].x - aSmooth[1].x;
    aSmooth[0].x = aSmooth[1].x - fStep;
    aSmooth[0].y = aSmooth[98].y;
    aSmooth[99].x = fStep + aSmooth[98].x;
    aSmooth[99].y = aSmooth[98].y;
    if (pProfile->m_uiMaxSamples < kNumSamples) {
        Fun_088ef7c2(pProfile, kNumSamples);
    }
    for (unsigned int i = 0; i < kNumSamples; ++i) {
        const float fDeltaX = aSmooth[i + 1].x - aRaw[i].x;
        const float fDeltaY = aSmooth[i + 1].y - aRaw[i].y;
        FGXVector2 vTangent = B_SPLINE::EvalGradient<FGXVector2, FGXVector2 const*, (B_SPLINE::BSPLINE_BEGIN_TYPES)0, (B_SPLINE::BSPLINE_END_TYPES)0>(100, aSmooth, (float)(int)i + 0.5f, 0);
        const float fLength = sqrtf(vTangent.y * vTangent.y + vTangent.x * vTangent.x);
        const float fTangentX = vTangent.x / fLength;
        const float fTangentY = vTangent.y / fLength;
        // The difference in the frame of the tangent: along and across.
        FGXVector2 vSample = {fDeltaY * fTangentY + fDeltaX * fTangentX, fTangentX * fDeltaY - fTangentY * fDeltaX};
        Fun_088ef6aa(pProfile, &vSample);
    }
    return (int)m_uiNumSplines - 1;
}

/// 0x088ef644
void cvCoastMeshLibrary::UnLoadCoastDefinitions() {
    m_uiNumStyles = 0;
    m_uiNumSplines = 0;
}

/// 0x088ee7f4
void cvCoastMeshLibrary::LoadCoastDefinitions(TerrainData const& kTerrainData) {
    // 0xffff in the XML means "none".
    auto NoneToMinusOne = [](int iValue) { return iValue == 0xffff ? -1 : iValue; };
    m_pTerrainData = &kTerrainData;
    FStringA kDirectory("terrain/Coast/");
    FStringA kPath = kDirectory;
    kPath += "CoastStyles.xml";
    FXmlDocument kDocument;
    if (!kDocument.Load(static_cast<char const*>(kPath))) {
        return;
    }
    FXmlElement kList;
    kDocument.GetRootElement(kList);
    if (kList.GetFirstChild(kList, "SplineCharacterList", 19)) {
        FXmlAttribute kAttribute;
        FXmlElement kCharacter(kList);
        if (kCharacter.GetFirstChild(kCharacter, "SplineCharacter", 15)) {
            do {
                FStringA kFilename;
                if (kCharacter.GetFirstAttribute(kAttribute, "Filename", 8) && kAttribute.GetVal(kFilename)) {
                    kPath = kDirectory;
                    kPath += kFilename;
                    LoadSplineFromFile(static_cast<char const*>(kPath));
                }
            } while (kCharacter.GetNextSibling(kCharacter, "SplineCharacter", 15));
        }
    }
    FXmlElement kRoot;
    kDocument.GetRootElement(kRoot);
    if (!kRoot.GetFirstChild(kRoot, "CoastStyleList", 14)) {
        return;
    }
    FXmlAttribute kTypeAttribute;
    FXmlElement kChild;
    FXmlElement kStyleElement(kRoot);
    if (!kStyleElement.GetFirstChild(kStyleElement, "CoastStyle", 10)) {
        return;
    }
    do {
        FStringA kType;
        kStyleElement.GetFirstAttribute(kTypeAttribute, "type", 4);
        kTypeAttribute.GetVal(kType);
        unsigned int uiStyleIndex = 0xffffffff;
        for (unsigned int i = 0; i < 13; ++i) {
            if (strcmp(static_cast<char const*>(kType), g_apszCoastStyleNames[i]) == 0) {
                uiStyleIndex = i;
                break;
            }
        }
        float fHighSide = 0.0f;
        float fLowSide = 0.0f;
        float fHighBeachHeight = 0.0f;
        float fHighBeachOffset = 0.0f;
        int iHighBeachCharacter = 0;
        int iHighBeachTerrain = 0;
        float fLowBeachHeight = 0.0f;
        float fLowBeachOffset = 0.0f;
        int iLowBeachCharacter = 0;
        int iLowBeachTerrain = 0;
        float fWaterlineHeight = 0.0f;
        float fWaterlineOffset = 0.0f;
        int iWaterlineCharacter = 0;
        kStyleElement.GetFirstChild(kChild, "SmoothingOffset", 15);
        kChild.GetFirstAttributeVal(fHighSide, "HighSide", 8);
        kChild.GetFirstAttributeVal(fLowSide, "LowSide", 7);
        kStyleElement.GetFirstChild(kChild, "HighBeach", 9);
        kChild.GetFirstAttributeVal(fHighBeachHeight, "Height", 6);
        kChild.GetFirstAttributeVal(fHighBeachOffset, "Offset", 6);
        kChild.GetFirstAttributeVal(iHighBeachCharacter, "Character", 9);
        kChild.GetFirstAttributeVal(iHighBeachTerrain, "TerrainType", 11);
        kStyleElement.GetFirstChild(kChild, "LowBeach", 8);
        kChild.GetFirstAttributeVal(fLowBeachHeight, "Height", 6);
        kChild.GetFirstAttributeVal(fLowBeachOffset, "Offset", 6);
        kChild.GetFirstAttributeVal(iLowBeachCharacter, "Character", 9);
        kChild.GetFirstAttributeVal(iLowBeachTerrain, "TerrainType", 11);
        kStyleElement.GetFirstChild(kChild, "Waterline", 9);
        kChild.GetFirstAttributeVal(fWaterlineHeight, "Height", 6);
        kChild.GetFirstAttributeVal(fWaterlineOffset, "Offset", 6);
        kChild.GetFirstAttributeVal(iWaterlineCharacter, "Character", 9);

        CoastStyleDef kStyle;
        kStyle.m_aLevels[0].m_fOffset = -fHighSide;
        kStyle.m_aLevels[0].m_fValue = 0.0f;
        kStyle.m_aLevels[0].m_fHeight = fHighBeachHeight;
        kStyle.m_aLevels[0].m_iCharacter = -1;
        kStyle.m_aLevels[1].m_fOffset = -fHighBeachOffset;
        kStyle.m_aLevels[1].m_fValue = 1.0f;
        kStyle.m_aLevels[1].m_fHeight = fHighBeachHeight;
        kStyle.m_aLevels[1].m_iCharacter = NoneToMinusOne(iHighBeachCharacter);
        kStyle.m_aLevels[2].m_fOffset = fWaterlineOffset;
        kStyle.m_aLevels[2].m_fValue = 1.0f;
        kStyle.m_aLevels[2].m_fHeight = fWaterlineHeight;
        kStyle.m_aLevels[2].m_iCharacter = NoneToMinusOne(iWaterlineCharacter);
        kStyle.m_aLevels[3].m_fOffset = fLowBeachOffset;
        kStyle.m_aLevels[3].m_fValue = 1.0f;
        kStyle.m_aLevels[3].m_fHeight = fLowBeachHeight;
        kStyle.m_aLevels[3].m_iCharacter = NoneToMinusOne(iLowBeachCharacter);
        kStyle.m_aLevels[4].m_fOffset = fLowSide;
        kStyle.m_aLevels[4].m_fValue = 0.0f;
        kStyle.m_aLevels[4].m_fHeight = fLowBeachHeight;
        kStyle.m_aLevels[4].m_iCharacter = -1;
        kStyle.m_aiTerrainType[0] = -1;
        kStyle.m_aiTerrainType[1] = NoneToMinusOne(iHighBeachTerrain);
        kStyle.m_aiTerrainType[2] = NoneToMinusOne(iLowBeachTerrain);
        kStyle.m_aiTerrainType[3] = -1;
        kStyle.m_uiStyleIndex = uiStyleIndex;
        Fun_088ef658(&m_pStyles, &kStyle);
    } while (kStyleElement.GetNextSibling(kStyleElement));
}

/// 0x0a2f2dd4
char const* g_apszCoastStyleNames[13] = {"Default", "Grass", "GrassHill", "Plains", "PlainsHill", "Snow", "SnowHill", "Desert", "DesertHill", "Tundra", "TundraHill", "Mountain", "DefaultShallows"};

/// 0x088ed400
void cvCoastSplineSystem::AddAllCoastlinesToDatabase(CoastGrid const& kGrid, FFastVector<FinalLevelSplines, false, 43u, 259u, BaseVector<FinalLevelSplines, false>::FDefaultFastVectorAllocator> const* pFinalLevelSplines) {
    ClearMarks(kGrid);
    for (unsigned int i = 0; i < m_uiNumSplines; ++i) {
        AddSingleCoastlineToDatabase(m_pSplines[i], kGrid);
    }
}
