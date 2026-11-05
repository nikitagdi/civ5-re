/// origin guess
#include <math.h>
#include <string.h>
#include "Civ5/Src/App/Graphics/SplinePreProcessJob.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"

namespace {

const unsigned int kStateSmooth = 1;
const unsigned int kStateCommit = 2;
const unsigned int kStateShiftMountains = 3;

// Texels a mountain shore is pushed along its normal.
const float kMountainShift = 4.0f;
const int kNumSplinePoints = 6;

}  // namespace

/// 0x088e578e
void SplinePreProcessJob::Execute(unsigned int uiThreadIndex) {
    CoastGrid const& kGrid = *ms_pCoastGrid;
    const int iRow = m_iRow;

    for (int iCol = 0; iCol < m_iWidth; ++iCol) {
        const HEXVec_i kCellOffset = {iCol - (iRow >> 1), iRow, -(iCol - (iRow >> 1) + iRow)};
        const HEXVec_i kCellHex = Fun_086616f0(kGrid.m_mxInverse, kCellOffset);
        const HEXVec_i kHex = {kCellHex.x, kCellHex.y, -(kCellHex.y + kCellHex.x)};
        CoastSplineHex* pCell = Fun_088e5718(kGrid, iCol, iRow);
        CoastEdgeSegment* pScratch = m_pScratch + iCol * 3;
        FGXVector2 aPoints[kNumSplinePoints];

        if (ms_uCurrState == kStateShiftMountains) {
            for (unsigned int uiEdge = 0; uiEdge < pCell->m_ucNumEdges; ++uiEdge) {
                kGrid.ReconstructCoastSpline(*pCell, kHex, uiEdge, aPoints, true);
                pScratch[uiEdge] = pCell->m_aSegments[uiEdge];
                if (pCell->m_aucMountainEdge[uiEdge] == 0) {
                    continue;
                }

                const FGXVector2 vGradientA = B_SPLINE::EvalGradient<FGXVector2, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)1>(
                    kNumSplinePoints, aPoints, 1.5f, 0);
                const FGXVector2 vGradientB = B_SPLINE::EvalGradient<FGXVector2, FGXVector2*, (B_SPLINE::BSPLINE_BEGIN_TYPES)1, (B_SPLINE::BSPLINE_END_TYPES)1>(
                    kNumSplinePoints, aPoints, 2.5f, 0);
                const float fLengthA = sqrtf(vGradientA.x * vGradientA.x + vGradientA.y * vGradientA.y);
                const FGXVector2 vNormalA = {vGradientA.y / fLengthA, -vGradientA.x / fLengthA};
                const float fLengthB = sqrtf(vGradientB.x * vGradientB.x + vGradientB.y * vGradientB.y);
                const FGXVector2 vNormalB = {vGradientB.y / fLengthB, -vGradientB.x / fLengthB};

                CoastEdgeSegment& kOut = pScratch[uiEdge];
                if (pCell->m_aucEdges[uiEdge] % 12 < 6) {
                    kOut.m_vSecond.x = vNormalA.x * kMountainShift + kOut.m_vSecond.x;
                    kOut.m_vSecond.y = vNormalA.y * kMountainShift + kOut.m_vSecond.y;
                    kOut.m_vFirst.x = vNormalB.x * kMountainShift + kOut.m_vFirst.x;
                    kOut.m_vFirst.y = vNormalB.y * kMountainShift + kOut.m_vFirst.y;
                } else {
                    kOut.m_vSecond.x = vNormalB.x * kMountainShift + kOut.m_vSecond.x;
                    kOut.m_vSecond.y = vNormalB.y * kMountainShift + kOut.m_vSecond.y;
                    kOut.m_vFirst.x = vNormalA.x * kMountainShift + kOut.m_vFirst.x;
                    kOut.m_vFirst.y = vNormalA.y * kMountainShift + kOut.m_vFirst.y;
                }
            }
        } else if (ms_uCurrState == kStateCommit) {
            memcpy(pCell->m_aSegments, pScratch, sizeof(pCell->m_aSegments));
        } else if (ms_uCurrState == kStateSmooth) {
            for (unsigned int uiEdge = 0; uiEdge < pCell->m_ucNumEdges; ++uiEdge) {
                kGrid.ReconstructCoastSpline(*pCell, kHex, uiEdge, aPoints, true);
                // Weights 1-2-1 over the spline points around each end of the segment.
                const float fFirstX = aPoints[1].x * 0.25f + aPoints[2].x * 0.5f + aPoints[3].x * 0.25f;
                const float fFirstY = aPoints[1].y * 0.25f + aPoints[2].y * 0.5f + aPoints[3].y * 0.25f;
                const float fSecondX = aPoints[2].x * 0.25f + aPoints[3].x * 0.5f + aPoints[4].x * 0.25f;
                const float fSecondY = aPoints[2].y * 0.25f + aPoints[3].y * 0.5f + aPoints[4].y * 0.25f;
                m_afSmoothed[0] = fFirstX;
                m_afSmoothed[1] = fFirstY;
                m_afSmoothed[2] = fSecondX;
                m_afSmoothed[3] = fSecondY;
                CoastEdgeSegment& kOut = pScratch[uiEdge];
                kOut.m_vFirst.x = fFirstX;
                kOut.m_vFirst.y = fFirstY;
                kOut.m_vSecond.x = fSecondX;
                kOut.m_vSecond.y = fSecondY;
                if (pCell->m_aucEdges[uiEdge] % 12 >= 6) {
                    kOut.m_vFirst.x = fSecondX;
                    kOut.m_vFirst.y = fSecondY;
                    kOut.m_vSecond.x = fFirstX;
                    kOut.m_vSecond.y = fFirstY;
                }
            }
        }
    }
}

/// 0x0a30fca0
int SplinePreProcessJob::ms_aStateList[10] = {1, 2, 3, 2, 1, 2, 3, 2, 1, 2};
/// 0x0a30fcc8
int SplinePreProcessJob::ms_nStateList = 10;
