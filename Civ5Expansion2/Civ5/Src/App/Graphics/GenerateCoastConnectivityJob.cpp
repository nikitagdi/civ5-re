/// origin guess
#include "Civ5/Src/App/Graphics/GenerateCoastConnectivityJob.h"
#include "Civ5/Src/App/Graphics/Terrain.h"

namespace {

const unsigned char kNoPeerEdge = 0xff;

}  // namespace

/// 0x088e6038
void GenerateCoastConnectivityJob::Execute(unsigned int uiThreadIndex) {
    CoastGrid const& kGrid = *ms_pCoastGrid;
    const int iRow = m_iRow;

    // The peer edge slot and the peer cell's column and row.
    auto SetLink = [&kGrid](CoastEdgeLink& kLink, SplineHandle const& kPeer) {
        kLink.m_ucPeerEdge = static_cast<unsigned char>(kPeer.m_iEdge);
        const HEXVec_i kPeerOffset = Fun_086616f0(kGrid.m_mxForward, kPeer.m_kHex);
        kLink.m_iRow = kPeerOffset.y;
        kLink.m_iCol = (kPeerOffset.y >> 1) + kPeerOffset.x;
    };

    for (int iCol = 0; iCol < m_iWidth; ++iCol) {
        CoastSplineHex* pCell = Fun_088e5718(kGrid, iCol, iRow);
        const HEXVec_i kCellOffset = {iCol - (iRow >> 1), iRow, -(iCol - (iRow >> 1) + iRow)};
        const HEXVec_i kCellHex = Fun_086616f0(kGrid.m_mxInverse, kCellOffset);
        const HEXVec_i kHex = {kCellHex.x, kCellHex.y, -(kCellHex.y + kCellHex.x)};

        for (unsigned int uiSlot = 0; uiSlot < 3; ++uiSlot) {
            CoastEdgeLink& kNext = pCell->m_aNextLinks[uiSlot];
            CoastEdgeLink& kPrev = pCell->m_aPrevLinks[uiSlot];
            kNext.m_bMarked = false;
            kNext.m_ucPeerEdge = kNoPeerEdge;
            kPrev.m_bMarked = false;
            kPrev.m_ucPeerEdge = kNoPeerEdge;
            if (uiSlot >= pCell->m_ucNumEdges) {
                continue;
            }

            // The edge ids that are 6 or more past a multiple of 12 take the opposite neighbour for the next link.
            const bool bInland = pCell->m_aucEdges[uiSlot] % 12 < 6;
            SplineHandle kHandle;
            kHandle.m_kHex = kHex;
            kHandle.m_iEdge = static_cast<int>(uiSlot);
            SetLink(kNext, kGrid.NextSplineInCoastline(kHandle, bInland));
            SetLink(kPrev, kGrid.NextSplineInCoastline(kHandle, !bInland));
        }
    }
}
