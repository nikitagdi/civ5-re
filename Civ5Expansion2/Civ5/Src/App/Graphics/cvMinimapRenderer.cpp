/// origin guess
#include <algorithm>
#include <math.h>
#include <string.h>
#include "Civ5/Src/App/Graphics/cvMinimapRenderer.h"
#include "Civ5/Src/App/Graphics/cvMinimapData.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FTempHeap.h"

typedef FUnrolledList<HexPatternMatcher::PatternPosition, NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, FFastAllocator<NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, true, 43u, 262u, FTempHeapFastVectorAllocator>, false> PatternPositionList;
typedef FFastVector<HEXVec_i, true, 43u, 259u, FTempHeapFastVectorAllocator> CandidateHexVector;
typedef FFastVector<FGXInt2, true, 43u, 259u, FTempHeapFastVectorAllocator> CoveredCellVector;

static const unsigned int kNoListNode = 0xfffffff;
static const unsigned int kNodeUnusedBit = 0x80000000u;
static const unsigned int kNodeCountMask = 0x7fffffff;
static const unsigned int kNodePositions = 15;

/// 0x08a46f58
void HexPatternMatcher::ComputePatternPositions(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>& grid) {
    PatternPositionList* pPositions = reinterpret_cast<PatternPositionList*>(&m_kPositions);
    for (int iPass = static_cast<int>(m_uiMaxPackingPass); iPass >= 0; --iPass) {
        FindLinearPacking(grid, static_cast<unsigned int>(iPass), pPositions);
        // The binary still walks every grid cell here with an empty body.
        for (int iX = 0; iX < grid.m_iWidth; ++iX) {
            for (int iY = 0; iY < grid.m_iHeight; ++iY) {
            }
        }
    }
}

/// 0x08a46fd6
void HexPatternMatcher::FindLinearPacking(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>& grid, unsigned int uiPass, FUnrolledList<HexPatternMatcher::PatternPosition, NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, FFastAllocator<NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, true, 43u, 262u, FTempHeapFastVectorAllocator>, false>* pPositions) {
    const int iWidth = grid.m_iWidth;
    const int iHeight = grid.m_iHeight;
    CoveredCellVector kCovered;

    const unsigned int uiNumPieces = m_uiNumPieces;
    const unsigned int uiNumTransforms = m_uiNumTransforms;
    unsigned int* aPieceOrder = static_cast<unsigned int*>(_alloca(uiNumPieces * sizeof(unsigned int)));
    for (unsigned int i = 0; i < uiNumPieces; ++i) {
        aPieceOrder[i] = i;
    }
    unsigned int* aTransformOrder = static_cast<unsigned int*>(_alloca(uiNumTransforms * sizeof(unsigned int)));
    for (unsigned int i = 0; i < uiNumTransforms; ++i) {
        aTransformOrder[i] = i;
    }

    // Candidate anchors: every offset cell plus a two-cell border, taken into world hex space.
    CandidateHexVector kCandidates;
    kCandidates.reserve((iWidth + 4) * (iHeight + 4));
    for (int iX = -2; iX < iWidth + 2; ++iX) {
        for (int iY = -2; iY < iHeight + 2; ++iY) {
            HEXVec_i kOffsetHex;
            kOffsetHex.x = iX - (iY >> 1);
            kOffsetHex.y = iY;
            kOffsetHex.z = -kOffsetHex.x - iY;
            const HEXVec_i kWorldHex = Fun_086616f0(grid.m_mxInverse, kOffsetHex);
            HEXVec_i kCandidate;
            kCandidate.x = kWorldHex.x;
            kCandidate.y = kWorldHex.y;
            kCandidate.z = -(kWorldHex.x + kWorldHex.y);
            kCandidates.push_back(kCandidate);
        }
    }
    std::random_shuffle(&kCandidates[0], &kCandidates[0] + kCandidates.size());

    const unsigned int uiNumCandidates = kCandidates.size();
    for (unsigned int uiCandidate = 0; uiCandidate < uiNumCandidates; ++uiCandidate) {
        const HEXVec_i kCandidate = kCandidates[uiCandidate];
        HEXMatrix_i kTranslation = {{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0}};
        HEXMat_iTranslation(&kTranslation, kCandidate);
        std::random_shuffle(aPieceOrder, aPieceOrder + uiNumPieces);
        std::random_shuffle(aTransformOrder, aTransformOrder + uiNumTransforms);

        for (unsigned int uiPiece = 0; uiPiece < uiNumPieces; ++uiPiece) {
            for (unsigned int uiTransform = 0; uiTransform < uiNumTransforms; ++uiTransform) {
                PatternPiece& kPiece = m_pPieces[aPieceOrder[uiPiece]];
                if (kPiece.m_uiPackingPass != uiPass) {
                    continue;
                }
                kCovered.clear();
                HEXMatrix_i kPlacement = m_pTransforms[aTransformOrder[uiTransform]];
                Fun_0868e43a(&kPlacement, kTranslation);
                Fun_0868e43a(&kPlacement, grid.m_mxForward);
                if (kPiece.m_ucNumUsedCells == 0) {
                    continue;
                }

                bool bFits = true;
                for (unsigned int uiCell = 0; uiCell < kPiece.m_ucNumUsedCells; ++uiCell) {
                    const unsigned char ucRow = kPiece.m_aucUsedCells[uiCell * 2];
                    const unsigned char ucColumn = kPiece.m_aucUsedCells[uiCell * 2 + 1];
                    HEXVec_i kPieceHex;
                    kPieceHex.x = ucRow - (ucColumn >> 1);
                    kPieceHex.y = ucColumn;
                    kPieceHex.z = -(kPieceHex.x + ucColumn);
                    const HEXVec_i kGridHex = Fun_086616f0(kPlacement, kPieceHex);
                    FGXInt2 kCell;
                    kCell.x = kGridHex.x + (kGridHex.y >> 1);
                    kCell.y = kGridHex.y;
                    if (kCell.x < 0 || kCell.x >= grid.m_iWidth || kCell.y < 0 || kCell.y >= grid.m_iHeight) {
                        continue;
                    }
                    const TerrainPieceCell& kPieceCell = kPiece.m_aCells[ucColumn * 10 + ucRow];
                    const HexPatternCell& kGridCell = grid.m_pCells[kCell.x * grid.m_iHeight + kCell.y];
                    if (!Fun_08a478b6(&kPieceCell, &kGridCell)) {
                        bFits = false;
                        break;
                    }
                    if (kPieceCell.m_bValid) {
                        kCovered.push_back(kCell);
                        if (kGridCell.m_bCovered) {
                            bFits = false;
                            break;
                        }
                    }
                }
                if (!bFits || kCovered.size() == 0) {
                    continue;
                }

                HexPatternMatcher::PatternPosition kPosition;
                kPosition.m_uiPieceIndex = aPieceOrder[uiPiece];
                kPosition.m_uiTransformIndex = aTransformOrder[uiTransform];
                kPosition.m_iHexX = kCandidate.x;
                kPosition.m_iHexY = kCandidate.y;
                kPosition.m_iHexZ = kCandidate.z;
                Fun_08a478fc(pPositions, kPosition);
                for (unsigned int i = 0; i < kCovered.size(); ++i) {
                    const FGXInt2& kCell = kCovered[i];
                    if (kCell.x >= 0 && kCell.x < grid.m_iWidth && kCell.y >= 0 && kCell.y < grid.m_iHeight) {
                        grid.m_pCells[kCell.x * grid.m_iHeight + kCell.y].m_bCovered = true;
                    }
                }
            }
        }
    }
}

/// 0x08a478b6
bool Fun_08a478b6(TerrainPieceCell const* a1, HexPatternCell const* a2) {
    bool bContinentMatches = true;
    if (a1->m_ucContinent != 0) {
        bContinentMatches = a1->m_ucContinent == a2->m_ucContinent || a2->m_ucContinent == 0;
    }
    return (a2->m_uFlags & a1->m_ulHexCode) != 0 && bContinentMatches || a2->m_uFlags == 0 || a1->m_ulHexCode == 0;
}

/// 0x08a478fc
unsigned int Fun_08a478fc(FUnrolledList<HexPatternMatcher::PatternPosition, NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, FFastAllocator<NodeArray<HexPatternMatcher::PatternPosition, 15u, 7u>, true, 43u, 262u, FTempHeapFastVectorAllocator>, false>* a1, HexPatternMatcher::PatternPosition const& a2) {
    HexPatternMatcher::PositionList* pList = reinterpret_cast<HexPatternMatcher::PositionList*>(a1);
    if (pList->m_uiSize != 0 && (pList->m_kNodes[pList->m_uiLastNode].m_uiCount & kNodeCountMask) < kNodePositions) {
        HexPatternMatcher::PatternNode& kLast = pList->m_kNodes[pList->m_uiLastNode];
        Fun_08a47b80(&kLast, a2, kLast.m_uiCount & kNodeCountMask);
    } else {
        HexPatternMatcher::PatternNode kTemplate;
        memset(&kTemplate, 0, sizeof(kTemplate));
        kTemplate.m_uiPrev = kNoListNode;
        kTemplate.m_uiNext = kNoListNode;
        kTemplate.m_uiCount = kNodeUnusedBit;
        const unsigned int uiNode = Fun_08a47c0a(pList, kTemplate);
        Fun_08a47cc0(pList, uiNode);
        HexPatternMatcher::PatternNode& kNode = pList->m_kNodes[uiNode];
        kNode.m_uiCount = (kNode.m_uiCount & kNodeUnusedBit) | ((kNode.m_uiCount + 1) & kNodeCountMask);
        kNode.m_aPositions[0] = a2;
    }
    if (++pList->m_uiSize == 0) {
        return kNoListNode;
    }
    return (pList->m_uiLastNode << 8) | ((pList->m_kNodes[pList->m_uiLastNode].m_uiCount & kNodeCountMask) - 1);
}

/// 0x08a47b80
bool Fun_08a47b80(HexPatternMatcher::PatternNode* a1, HexPatternMatcher::PatternPosition const& a2, unsigned int a3) {
    for (unsigned int i = a1->m_uiCount & kNodeCountMask; i > a3; --i) {
        a1->m_aPositions[i] = a1->m_aPositions[i - 1];
    }
    a1->m_uiCount = (a1->m_uiCount & kNodeUnusedBit) | ((a1->m_uiCount + 1) & kNodeCountMask);
    a1->m_aPositions[a3] = a2;
    return true;
}

/// 0x08a47c0a
unsigned int Fun_08a47c0a(HexPatternMatcher::PositionList* a1, HexPatternMatcher::PatternNode const& a2) {
    unsigned int uiNode = a1->m_uiFreeNode;
    if (uiNode == kNoListNode) {
        uiNode = a1->m_kNodes.size();
        const unsigned int uiPushed = a1->m_kNodes.push_back(a2);
        a1->m_kNodes[uiPushed].m_uiCount &= kNodeCountMask;
    } else {
        HexPatternMatcher::PatternNode& kNode = a1->m_kNodes[uiNode];
        a1->m_uiFreeNode = kNode.m_uiNext;
        kNode = a2;
        kNode.m_uiCount &= kNodeCountMask;
    }
    ++a1->m_uiNumAcquiredNodes;
    return uiNode;
}

/// 0x08a47cc0
void Fun_08a47cc0(HexPatternMatcher::PositionList* a1, unsigned int a2) {
    const unsigned int uiOldLast = a1->m_uiLastNode;
    a1->m_uiLastNode = a2;
    ++a1->m_uiNumLinkedNodes;
    if (a1->m_uiFirstNode == kNoListNode) {
        a1->m_uiFirstNode = a2;
    }
    Fun_08a47d02(a1, a2, uiOldLast);
}

/// 0x08a47d02
void Fun_08a47d02(HexPatternMatcher::PositionList* a1, unsigned int a2, unsigned int a3) {
    HexPatternMatcher::PatternNode& kNode = a1->m_kNodes[a2];
    if (a3 == kNoListNode) {
        kNode.m_uiNext = kNoListNode;
        kNode.m_uiPrev = kNoListNode;
        return;
    }
    const unsigned int uiNext = a1->m_kNodes[a3].m_uiNext;
    kNode.m_uiPrev = a3;
    kNode.m_uiNext = uiNext;
    if (uiNext != kNoListNode) {
        a1->m_kNodes[uiNext].m_uiPrev = a2;
    }
    a1->m_kNodes[a3].m_uiNext = a2;
}

// CreateSurface type flags shared by every minimap surface.
static const unsigned int kMinimapSurfaceType = 0x42;
// D3DFMT_A8R8G8B8, the format of the minimap render texture.
static const unsigned int kMinimapTargetFormat = 0x15;
// Shader passes of the minimap effect, by what the pass draws.
static const unsigned short kPassFog = 0;
static const unsigned short kPassLand = 1;
static const unsigned short kPassCulture = 2;
static const unsigned short kPassRivers = 3;
static const unsigned short kPassShallowWater = 4;
static const unsigned short kPassWater = 7;
static const unsigned short kPassCameraRect = 8;
// A static vertex buffer takes 0xc00 hex mesh vertices and an index buffer 0x3000 indices; slot 0xbff is never used and
// marks a source vertex that is not in the buffer yet.
static const unsigned int kMaxBufferIndices = 0x3000;
static const unsigned short kUnmappedVertex = 0xbff;

/// 0x08a42920
void cvMinimapRenderer::DeriveCameraParams() {
    cvMinimapData::PlayerData& kPlayer = *m_pData->GetPlayerData(m_pData->m_eActivePlayer);
    const cvTerrainSampler& kSampler = *m_pData->m_pTerrainSampler;

    // Corners of the explored region: offset coordinates, through the inverse matrix, to world hexes and positions.
    HEXVec_i kMinOffset;
    kMinOffset.y = kPlayer.m_iMinY;
    kMinOffset.x = kPlayer.m_iMinX - (kMinOffset.y >> 1);
    kMinOffset.z = -(kMinOffset.x + kMinOffset.y);
    const HEXVec_i kMinCube = Fun_086616f0(kPlayer.m_mxInverse, kMinOffset);
    const HEXVec kMinHex = {static_cast<float>(kMinCube.x), static_cast<float>(kMinCube.y), static_cast<float>(-(kMinCube.y + kMinCube.x)), 1.0f};

    HEXVec_i kMaxOffset;
    kMaxOffset.y = kPlayer.m_iMaxY;
    kMaxOffset.x = kPlayer.m_iMaxX - (kMaxOffset.y >> 1);
    kMaxOffset.z = -(kMaxOffset.x + kMaxOffset.y);
    const HEXVec_i kMaxCube = Fun_086616f0(kPlayer.m_mxInverse, kMaxOffset);
    const HEXVec kMaxHex = {static_cast<float>(kMaxCube.x), static_cast<float>(kMaxCube.y), static_cast<float>(-(kMaxCube.y + kMaxCube.x)), 1.0f};

    const FGXVector2 vMinPos = kSampler.GetWorldLocation2D(kMinHex);
    const FGXVector2 vMaxPos = kSampler.GetWorldLocation2D(kMaxHex);

    // Grow the region by half a hex on every side.
    const float fHalfHexWidth = (m_vCornerMax.x - m_vCornerMin.x) * 0.5f;
    const float fHalfHexHeight = (m_vCornerMax.y - m_vCornerMin.y) * 0.5f;
    const float fRegionMinX = vMinPos.x - fHalfHexWidth;
    const float fRegionMinY = vMinPos.y - fHalfHexHeight;
    const float fRegionMaxX = fHalfHexWidth + vMaxPos.x;
    const float fRegionMaxY = fHalfHexHeight + vMaxPos.y;
    const float fRegionWidth = fRegionMaxX - fRegionMinX;
    const float fRegionHeight = fRegionMaxY - fRegionMinY;

    // Fit the region to the aspect of the viewport, padding the short axis by the gutter in pixels but not past the world.
    float fLeft;
    float fRight;
    float fBottom;
    float fTop;
    if (fRegionWidth / fRegionHeight <= m_fViewportAspect) {
        const float fCenterX = (fRegionMinX + fRegionMaxX) * 0.5f;
        float fPad = m_fExploredRegionGutter * (fRegionHeight / static_cast<float>(m_uiHeight));
        const float fWorldHeight = m_vWorldMax.y - m_vWorldMin.y;
        if (fWorldHeight <= fPad + fPad + fRegionHeight) {
            const float fRoom = (fWorldHeight - fRegionHeight) * 0.5f;
            fPad = 0.0f;
            if (0.0f <= fRoom) {
                fPad = fRoom;
            }
        }
        fTop = fRegionMaxY + fPad;
        fBottom = fRegionMinY - fPad;
        const float fHalfWidth = (fTop - fBottom) * m_fViewportAspect * 0.5f;
        fLeft = fCenterX - fHalfWidth;
        fRight = fHalfWidth + fCenterX;
    } else {
        const float fCenterY = (fRegionMaxY + fRegionMinY) * 0.5f;
        float fPad = m_fExploredRegionGutter * (fRegionWidth / static_cast<float>(m_uiViewportWidth));
        const float fWorldWidth = m_vWorldMax.x - m_vWorldMin.x;
        if (fWorldWidth <= fPad + fPad + fRegionWidth) {
            const float fRoom = (fWorldWidth - fRegionWidth) * 0.5f;
            fPad = 0.0f;
            if (0.0f <= fRoom) {
                fPad = fRoom;
            }
        }
        fRight = fRegionMaxX + fPad;
        fLeft = fRegionMinX - fPad;
        const float fHalfHeight = ((fRight - fLeft) / m_fViewportAspect) * 0.5f;
        fBottom = fCenterY - fHalfHeight;
        fTop = fHalfHeight + fCenterY;
    }
    if (m_vWorldMax.y <= fTop && fBottom <= m_vWorldMin.y) {
        const float fShift = (fTop - fBottom) * 0.5f - (m_vWorldMax.y - m_vWorldMin.y) * 0.5f;
        fBottom -= fShift;
        fTop -= fShift;
    }

    // Subdivisions of a hex edge: a quarter of the pixels one hex radius covers, at least 2.
    const float fPixelsPerUnitY = static_cast<float>(m_uiHeight) / (fTop - fBottom);
    const float fWidth = fRight - fLeft;
    const float fPixelsPerUnitX = static_cast<float>(m_uiViewportWidth) / fWidth;
    const float fPixelsPerUnit = fPixelsPerUnitY <= fPixelsPerUnitX ? fPixelsPerUnitX : fPixelsPerUnitY;
    const float fSubdivisions = ceilf(fPixelsPerUnit * m_fHexRadius * 0.25f);
    m_uiEdgeSubdivisions = 2;
    if (2.0f < fSubdivisions) {
        m_uiEdgeSubdivisions = static_cast<unsigned int>(fSubdivisions);
    }
    m_fEdgeStep = 1.0f / static_cast<float>(m_uiEdgeSubdivisions - 1);

    m_akViewMin[0].x = fLeft;
    m_akViewMax[0].x = fRight;
    m_akViewMin[0].y = fBottom;
    m_akViewMax[0].y = fTop;
    m_uiNumViews = 1;

    // On a wrapping map a view that leaves the world on one side is drawn a second time one world width over.
    const float fWorldLeft = m_vWorldMin.x;
    float fShiftedLeft;
    if (fWorldLeft <= fLeft || !m_pData->m_bWrapX) {
        if (fRight <= m_vWorldMax.x || !m_pData->m_bWrapX) {
            return;
        }
        fShiftedLeft = fWorldLeft - (m_vWorldMax.x - fLeft);
    } else {
        fShiftedLeft = m_vWorldMax.x - (fWorldLeft - fLeft);
    }
    m_akViewMin[1].x = fShiftedLeft;
    m_akViewMax[1].x = fShiftedLeft + fWidth;
    m_akViewMin[1].y = fBottom;
    m_akViewMax[1].y = fTop;
    m_uiNumViews = 2;
}

/// 0x08a42ed8
void cvMinimapRenderer::BuildHexMeshes(FFastVector<FGXVector2, true, 63u, 0u, FTempHeapFastVectorAllocator>& kVertices, FFastVector<unsigned int, true, 63u, 1u, FTempHeapFastVectorAllocator>& kIndices) {
    const cvTerrainSampler& kSampler = *m_pData->m_pTerrainSampler;

    // The hexes under the corners of the view, rounded to whole hexes.
    const FGXVector4 vMinLocation = kSampler.GetHexLocation2D(m_akViewMin[0]);
    const HEXVec kMinLocation = {vMinLocation.x, vMinLocation.y, vMinLocation.z, vMinLocation.w};
    HEXVec kMinHex = {0.0f, 0.0f, 0.0f, 1.0f};
    Fun_087e9677(&kMinHex, &kMinLocation, 1);
    const FGXVector4 vMaxLocation = kSampler.GetHexLocation2D(m_akViewMax[0]);
    const HEXVec kMaxLocation = {vMaxLocation.x, vMaxLocation.y, vMaxLocation.z, vMaxLocation.w};
    HEXVec kMaxHex = {0.0f, 0.0f, 0.0f, 1.0f};
    Fun_087e9677(&kMaxHex, &kMaxLocation, 1);

    cvMinimapData::PlayerData& kPlayer = *m_pData->GetPlayerData(m_pData->m_eActivePlayer);
    const HEXVec_i kMinCube = {static_cast<int>(kMinHex.x), static_cast<int>(kMinHex.y), static_cast<int>(kMinHex.z)};
    const HEXVec_i kMinOffset = Fun_086616f0(kPlayer.m_mxForward, kMinCube);
    const HEXVec_i kMaxCube = {static_cast<int>(kMaxHex.x), static_cast<int>(kMaxHex.y), static_cast<int>(kMaxHex.z)};
    const HEXVec_i kMaxOffset = Fun_086616f0(kPlayer.m_mxForward, kMaxCube);

    // The grid cells covering the view, with one cell of margin.
    const int iFirstColumn = (kMinOffset.y >> 1) - 1 + kMinOffset.x;
    const int iFirstRow = kMinOffset.y - 1;
    const int iLastColumn = (kMaxOffset.y >> 1) + 1 + kMaxOffset.x;
    const int iLastRow = kMaxOffset.y + 1;

    // Fog: unexplored cells, which includes every cell outside the grid.
    m_kFogRange.m_uiFirstVertex = kVertices.size();
    m_kFogRange.m_uiFirstIndex = kIndices.size();
    for (int iColumn = iFirstColumn; iColumn <= iLastColumn; ++iColumn) {
        for (int iRow = iFirstRow; iRow <= iLastRow; ++iRow) {
            const MinimapCell* pCell = Fun_088a9b1c(&kPlayer, iColumn, iRow);
            if ((pCell->m_ucFog & 1) != 0) {
                HEXVec_i kCellHex;
                kCellHex.x = iColumn - (iRow >> 1);
                kCellHex.y = iRow;
                kCellHex.z = -(kCellHex.x + iRow);
                const HEXVec_i kWorld = Fun_086616f0(kPlayer.m_mxInverse, kCellHex);
                const HEXVec_i kHex = {kWorld.x, kWorld.y, -(kWorld.y + kWorld.x)};
                PushTessellatedHexToMesh(kHex, static_cast<unsigned int>(pCell->m_ucFog >> 2), false, kVertices, kIndices);
            }
        }
    }
    m_kFogRange.m_uiNumVertices = kVertices.size() - m_kFogRange.m_uiFirstVertex;
    m_kFogRange.m_uiNumIndices = kIndices.size() - m_kFogRange.m_uiFirstIndex;

    // Shallow water: coast cells.
    m_kCoastRange.m_uiFirstVertex = kVertices.size();
    m_kCoastRange.m_uiFirstIndex = kIndices.size();
    for (int iColumn = iFirstColumn; iColumn <= iLastColumn; ++iColumn) {
        for (int iRow = iFirstRow; iRow <= iLastRow; ++iRow) {
            const MinimapCell* pCell = Fun_088a9b1c(&kPlayer, iColumn, iRow);
            if ((pCell->m_ucTerrain & 3) == 1) {
                HEXVec_i kCellHex;
                kCellHex.x = iColumn - (iRow >> 1);
                kCellHex.y = iRow;
                kCellHex.z = -(kCellHex.x + iRow);
                const HEXVec_i kWorld = Fun_086616f0(kPlayer.m_mxInverse, kCellHex);
                const HEXVec_i kHex = {kWorld.x, kWorld.y, -(kWorld.y + kWorld.x)};
                PushTessellatedHexToMesh(kHex, static_cast<unsigned int>(pCell->m_ucTerrain >> 2), true, kVertices, kIndices);
            }
        }
    }
    m_kCoastRange.m_uiNumVertices = kVertices.size() - m_kCoastRange.m_uiFirstVertex;
    m_kCoastRange.m_uiNumIndices = kIndices.size() - m_kCoastRange.m_uiFirstIndex;

    // Land: grass cells.
    m_kLandRange.m_uiFirstVertex = kVertices.size();
    m_kLandRange.m_uiFirstIndex = kIndices.size();
    for (int iColumn = iFirstColumn; iColumn <= iLastColumn; ++iColumn) {
        for (int iRow = iFirstRow; iRow <= iLastRow; ++iRow) {
            const MinimapCell* pCell = Fun_088a9b1c(&kPlayer, iColumn, iRow);
            if ((pCell->m_ucTerrain & 3) == 0) {
                HEXVec_i kCellHex;
                kCellHex.x = iColumn - (iRow >> 1);
                kCellHex.y = iRow;
                kCellHex.z = -(kCellHex.x + iRow);
                const HEXVec_i kWorld = Fun_086616f0(kPlayer.m_mxInverse, kCellHex);
                const HEXVec_i kHex = {kWorld.x, kWorld.y, -(kWorld.y + kWorld.x)};
                PushTessellatedHexToMesh(kHex, static_cast<unsigned int>(pCell->m_ucTerrain >> 2), true, kVertices, kIndices);
            }
        }
    }
    m_kLandRange.m_uiNumVertices = kVertices.size() - m_kLandRange.m_uiFirstVertex;
    m_kLandRange.m_uiNumIndices = kIndices.size() - m_kLandRange.m_uiFirstIndex;

    // Culture: one mesh per owner found in the view.
    m_kCultureGroups.m_uiCount = 0;
    for (int iColumn = iFirstColumn; iColumn <= iLastColumn; ++iColumn) {
        for (int iRow = iFirstRow; iRow <= iLastRow; ++iRow) {
            const unsigned int uiOwner = Fun_088a9b1c(&kPlayer, iColumn, iRow)->m_ucCultureOwner;
            if (uiOwner == 0) {
                continue;
            }
            bool bKnown = false;
            for (unsigned int i = 0; i < m_kCultureGroups.m_uiCount; ++i) {
                if (m_kCultureGroups.m_pData[i].m_uiOwner == uiOwner) {
                    bKnown = true;
                    break;
                }
            }
            if (!bKnown) {
                MinimapCultureGroup kGroup = {};
                kGroup.m_uiOwner = uiOwner;
                Fun_08a463a4(&m_kCultureGroups, &kGroup);
            }
        }
    }
    for (unsigned int i = 0; i < m_kCultureGroups.m_uiCount; ++i) {
        MinimapCultureGroup& kGroup = m_kCultureGroups.m_pData[i];
        kGroup.m_kRange.m_uiFirstVertex = kVertices.size();
        kGroup.m_kRange.m_uiFirstIndex = kIndices.size();
        for (int iColumn = iFirstColumn; iColumn <= iLastColumn; ++iColumn) {
            for (int iRow = iFirstRow; iRow <= iLastRow; ++iRow) {
                const MinimapCell* pCell = Fun_088a9b1c(&kPlayer, iColumn, iRow);
                if (pCell->m_ucCultureOwner == kGroup.m_uiOwner) {
                    HEXVec_i kCellHex;
                    kCellHex.x = iColumn - (iRow >> 1);
                    kCellHex.y = iRow;
                    kCellHex.z = -(kCellHex.x + iRow);
                    const HEXVec_i kWorld = Fun_086616f0(kPlayer.m_mxInverse, kCellHex);
                    const HEXVec_i kHex = {kWorld.x, kWorld.y, -(kWorld.y + kWorld.x)};
                    PushTessellatedHexToMesh(kHex, static_cast<unsigned int>(pCell->m_ucCultureEdges >> 2), true, kVertices, kIndices);
                }
            }
        }
        kGroup.m_kRange.m_uiNumVertices = kVertices.size() - kGroup.m_kRange.m_uiFirstVertex;
        kGroup.m_kRange.m_uiNumIndices = kIndices.size() - kGroup.m_kRange.m_uiFirstIndex;
        kGroup.m_uiOwner -= 1;
    }
}

/// 0x08a43900
void cvMinimapRenderer::ConstructHexBuffersAndSurfaces(FFastVector<FGXVector2, true, 63u, 0u, FTempHeapFastVectorAllocator>& kVertices, FFastVector<unsigned int, true, 63u, 1u, FTempHeapFastVectorAllocator>& kIndices) {
    // The ranges in the order BuildHexMeshes appended them: fog, coast, land, then one per culture owner.
    const unsigned int uiNumRanges = m_kCultureGroups.m_uiCount + 3;
    FFastVector<MinimapMeshRange*, true, 63u, 2u, FTempHeapFastVectorAllocator> kRanges;
    kRanges.push_back(&m_kFogRange);
    kRanges.push_back(&m_kCoastRange);
    kRanges.push_back(&m_kLandRange);
    for (unsigned int i = 0; i < m_kCultureGroups.m_uiCount; ++i) {
        kRanges.push_back(&m_kCultureGroups.m_pData[i].m_kRange);
    }

    // Where each source vertex went in the vertex buffer being filled.
    FFastVector<unsigned short, true, 63u, 3u, FTempHeapFastVectorAllocator> kSlots;
    kSlots.resize(kVertices.size());
    for (unsigned int i = 0; i < kVertices.size(); ++i) {
        kSlots[i] = kUnmappedVertex;
    }

    for (unsigned int i = 0; i < m_kSurfaces.m_uiCount; ++i) {
        FireGrafix::g_pRenderer->ReleaseSurface(m_kSurfaces.m_pData[i]);
    }
    m_kSurfaces.m_uiCount = 0;

    if (kIndices.size() == 0) {
        for (unsigned int i = 0; i < uiNumRanges; ++i) {
            kRanges[i]->m_uiFirstSurface = 0;
            kRanges[i]->m_uiNumSurfaces = 0;
        }
        return;
    }

    MinimapBufferLock kVertexLock = GetNewVB(0);
    MinimapBufferLock kIndexLock = GetNewIB(0);
    unsigned int uiNextVertexBuffer = 1;
    unsigned int uiNextIndexBuffer = 1;
    kRanges[0]->m_uiFirstSurface = 0;

    unsigned int uiPos = 0;            // next index to place
    unsigned int uiSurfaceStart = 0;   // first index of the surface being collected
    unsigned int uiIndexBufferStart = 0;
    unsigned int uiRangeStart = 0;
    unsigned int uiRange = 0;
    unsigned int uiNumBufferVertices = 0;
    for (;;) {
        // 4: the current range is complete, 1: the next triangle would overflow the vertex buffer, 2: the index buffer is full.
        unsigned char ucFlags = 0;
        if (uiPos - uiRangeStart == kRanges[uiRange]->m_uiNumIndices) {
            ucFlags = 4;
        }
        if (uiPos < kIndices.size() - 3u) {
            const unsigned int uiNewVertices = (kSlots[kIndices[uiPos]] == kUnmappedVertex ? 1u : 0u)
                + uiNumBufferVertices
                + (kSlots[kIndices[uiPos + 1]] == kUnmappedVertex ? 1u : 0u)
                + (kSlots[kIndices[uiPos + 2]] == kUnmappedVertex ? 1u : 0u);
            if (uiNewVertices >= kUnmappedVertex) {
                ucFlags |= 1;
            }
        }
        if (uiPos - uiIndexBufferStart == kMaxBufferIndices) {
            ucFlags |= 2;
        }

        if (ucFlags == 0) {
            for (unsigned int k = 0; k < 3; ++k) {
                const unsigned int uiVertex = kIndices[uiPos + k];
                unsigned short usSlot = kSlots[uiVertex];
                if (usSlot == kUnmappedVertex) {
                    usSlot = static_cast<unsigned short>(uiNumBufferVertices);
                    kSlots[uiVertex] = usSlot;
                    static_cast<FGXVector2*>(kVertexLock.m_pData)[uiNumBufferVertices] = kVertices[uiVertex];
                    ++uiNumBufferVertices;
                }
                static_cast<unsigned short*>(kIndexLock.m_pData)[uiPos - uiIndexBufferStart + k] = usSlot;
            }
            uiPos += 3;
            continue;
        }

        FireGrafix::FGXSurface* pSurface;
        unsigned int hSurface;
        FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hSurface, kMinimapSurfaceType);
        pSurface->m_hIB = kIndexLock.m_hBuffer;
        pSurface->m_hVB = kVertexLock.m_hBuffer;
        pSurface->field_10 = uiSurfaceStart % kMaxBufferIndices;
        pSurface->field_c = (uiPos - uiSurfaceStart) / 3;
        BindTextures(pSurface);
        Fun_08a46492(&m_kSurfaces, &hSurface);

        if ((ucFlags & 4) != 0) {
            kRanges[uiRange]->m_uiNumSurfaces = m_kSurfaces.m_uiCount - kRanges[uiRange]->m_uiFirstSurface;
            ++uiRange;
            if (uiRange == uiNumRanges) {
                FireGrafix::g_pRenderer->UnlockIB(kIndexLock.m_hBuffer, 0, 0);
                FireGrafix::g_pRenderer->UnlockVB(kVertexLock.m_hBuffer, 0, 0);
                return;
            }
            kRanges[uiRange]->m_uiFirstSurface = m_kSurfaces.m_uiCount;
            uiRangeStart = uiPos;
        }
        if ((ucFlags & 2) != 0) {
            FireGrafix::g_pRenderer->UnlockIB(kIndexLock.m_hBuffer, 0, 0);
            kIndexLock = GetNewIB(uiNextIndexBuffer++);
            uiIndexBufferStart = uiPos;
        }
        uiSurfaceStart = uiPos;
        if ((ucFlags & 1) != 0) {
            FireGrafix::g_pRenderer->UnlockVB(kVertexLock.m_hBuffer, 0, 0);
            kVertexLock = GetNewVB(uiNextVertexBuffer++);
            for (unsigned int i = 0; i < kVertices.size(); ++i) {
                kSlots[i] = kUnmappedVertex;
            }
            uiNumBufferVertices = 0;
        }
    }
}

/// 0x08a4207c
void cvMinimapRenderer::RenderMiniMap(unsigned int uiDirtyFlags) {
    const unsigned int hTarget = m_hResolveSource != 0 ? m_hResolveSource : m_hRenderTexture;

    if ((uiDirtyFlags & (MINIMAP_DIRTY_FOG | MINIMAP_DIRTY_CULTURE | MINIMAP_DIRTY_EXPLORED_REGION)) != 0) {
        DeriveCameraParams();
        FFastVector<FGXVector2, true, 63u, 0u, FTempHeapFastVectorAllocator> kVertices;
        FFastVector<unsigned int, true, 63u, 1u, FTempHeapFastVectorAllocator> kIndices;
        kVertices.reserve(0x10000);
        kIndices.reserve(0x20000);
        BuildHexMeshes(kVertices, kIndices);
        ConstructHexBuffersAndSurfaces(kVertices, kIndices);
    }
    if ((uiDirtyFlags & 0x1c) != 0) {
        BuildSprites();
    }
    if ((uiDirtyFlags & MINIMAP_DIRTY_CAMERA) != 0) {
        CreateCameraRectGeometry();
    }
    SetupGlobals();

    // The explored region as laid out above and the rectangle the game camera sees, in world units.
    const float fViewMinX = m_akViewMin[0].x;
    const float fViewMinY = m_akViewMin[0].y;
    const float fViewMaxX = m_akViewMax[0].x;
    const float fViewMaxY = m_akViewMax[0].y;
    const float fCameraMinX = m_pData->m_akViewMin[0].x;
    const float fCameraMinY = m_pData->m_akViewMin[0].y;
    const float fCameraMaxX = m_pData->m_akViewMax[0].x;
    const float fCameraMaxY = m_pData->m_akViewMax[0].y;

    // The target is cleared to black with the fog alpha.
    const FGXVector4 vFogClear = {0.0f, 0.0f, 0.0f, m_fFogAlpha};
    m_pCommandStream->BeginCommandSet(hTarget, 0, 9, 4, "ColorClear", vFogClear, 1.0f, 0, 0, 0);
    m_pCommandStream->EndCommandSet();

    const FGXVector4 vZero = {0.0f, 0.0f, 0.0f, 0.0f};
    m_pCommandStream->BeginCommandSet(hTarget, 8, 9, 1, "Minimap", vZero, 1.0f, 0, 0, m_hViewport);
    SetupCamera(m_akViewMin[0], m_akViewMax[0], m_pCommandStream);

    // Water: a quad over the whole target.
    FireGrafix::FGXSurface* pSurface;
    unsigned int hSurface;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hSurface, kMinimapSurfaceType);
    pSurface->m_hIB = 1;
    pSurface->m_hVB = 1;
    pSurface->field_c = 2;
    pSurface->field_10 = 0;
    BindTextures(pSurface);
    m_pCommandStream->DrawSurface(hSurface, m_hShaderInstance, kPassWater);
    FireGrafix::g_pRenderer->ReleaseSurface(hSurface);

    for (unsigned int i = 0; i < m_kCoastRange.m_uiNumSurfaces; ++i) {
        m_pCommandStream->DrawSurface(m_kSurfaces.m_pData[m_kCoastRange.m_uiFirstSurface + i], m_hShaderInstance, kPassShallowWater);
    }
    for (unsigned int i = 0; i < m_kLandRange.m_uiNumSurfaces; ++i) {
        m_pCommandStream->DrawSurface(m_kSurfaces.m_pData[m_kLandRange.m_uiFirstSurface + i], m_hShaderInstance, kPassLand);
    }
    // Culture: the owner colour goes through a 2.2 gamma into the pass constants.
    for (unsigned int g = 0; g < m_kCultureGroups.m_uiCount; ++g) {
        const MinimapCultureGroup& kGroup = m_kCultureGroups.m_pData[g];
        float* pColor = static_cast<float*>(m_pDynamicConstants->LockDynamic(m_pCommandStream));
        pColor[4] = powf(static_cast<float>(m_pData->m_aaucPlayerColors[kGroup.m_uiOwner][4]) / 255.0f, 2.2f);
        pColor[5] = powf(static_cast<float>(m_pData->m_aaucPlayerColors[kGroup.m_uiOwner][5]) / 255.0f, 2.2f);
        pColor[6] = powf(static_cast<float>(m_pData->m_aaucPlayerColors[kGroup.m_uiOwner][6]) / 255.0f, 2.2f);
        m_pDynamicConstants->UnlockDynamic();
        for (unsigned int i = 0; i < kGroup.m_kRange.m_uiNumSurfaces; ++i) {
            m_pCommandStream->DrawSurface(m_kSurfaces.m_pData[kGroup.m_kRange.m_uiFirstSurface + i], m_hShaderInstance, kPassCulture);
        }
    }
    RenderIce();
    m_pCommandStream->EndCommandSet();

    // Rivers and sprites, once per copy of the view.
    for (unsigned int v = 0; v < m_uiNumViews; ++v) {
        m_pCommandStream->BeginCommandSet(hTarget, 8, 9, 1, "Minimap", vZero, 1.0f, 0, 0, m_hViewport);
        SetupCamera(m_akViewMin[v], m_akViewMax[v], m_pCommandStream);
        RenderRivers();
        RenderSprites();
        m_pCommandStream->EndCommandSet();
    }

    // Fog goes over everything drawn so far.
    m_pCommandStream->BeginCommandSet(hTarget, 0, 9, 1, "Minimap", vZero, 1.0f, 0, 0, m_hViewport);
    SetupCamera(m_akViewMin[0], m_akViewMax[0], m_pCommandStream);
    for (unsigned int i = 0; i < m_kFogRange.m_uiNumSurfaces; ++i) {
        m_pCommandStream->DrawSurface(m_kSurfaces.m_pData[m_kFogRange.m_uiFirstSurface + i], m_hShaderInstance, kPassFog);
    }
    m_pCommandStream->EndCommandSet();

    // The camera rectangle is drawn only while the camera sees clearly less than the explored region.
    const float fRatioX = (fViewMaxX - fViewMinX) / (fCameraMaxX - fCameraMinX);
    const float fRatioY = (fViewMaxY - fViewMinY) / (fCameraMaxY - fCameraMinY);
    const float fRatio = fRatioY <= fRatioX ? fRatioX : fRatioY;
    if (m_fCameraRectThreshold < fRatio && m_uiNumViews != 0) {
        for (unsigned int v = 0; v < m_uiNumViews; ++v) {
            m_pCommandStream->BeginCommandSet(hTarget, 8, 9, 1, "Minimap", vZero, 1.0f, 0, 0, m_hViewport);
            SetupCamera(m_akViewMin[v], m_akViewMax[v], m_pCommandStream);
            RenderCameraRects();
            m_pCommandStream->EndCommandSet();
        }
    }

    if (m_hResolveSource != 0) {
        m_pCommandStream->ResolveRenderTexture(m_hResolveSource, m_hRenderTexture, kMinimapTargetFormat);
    }
    m_pCommandStream->AddAllCommandSetsToRenderQueue();
}
