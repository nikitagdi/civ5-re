/// origin binary
#include <math.h>
#include <new>
#include <string.h>
#include <Windows.h>
#include "Civ5/Src/App/Graphics/cvTerrainBlend.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainClassGrid.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/NetProxySessionCallbacks.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FStringTokenizer.h"
#include "FirePlace/FireWorks/FTempHeap.h"

static const int kClassGridSize = 0xff;

namespace {

typedef FFastVector<BlendCell, true, 43u, 259u, FTempHeapFastVectorAllocator> BlendCellVector;
typedef FFastVector<FGXVector2, true, 43u, 259u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> SplinePointVector;
typedef FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> SplineRangeVector;
typedef FFastVector<FGXInt2, true, 43u, 259u, FTempHeapFastVectorAllocator> FloodFillScratchVector;

const int kCellSize = 0x40;
// Cells added before the modulo so that negative columns wrap onto the map.
const int kColumnWrapSlack = 100;
const unsigned int kNoNode = 0xfffffff;
const unsigned char kPieceFlagSmall = 1;
const unsigned char kPieceFlagBlend = 4;
const int kSmallPieceSize = 0x80;
const int kLargePieceSize = 0x200;
const int kExtentCorners = 4;
const char kJobTag[] = "Port/Civ5/Src/App/Graphics/cvTerrainBlend.cpp";

// Texel offsets of the four diagonal neighbours of a hex centre.
const int kCornerOffsets[4][2] = {{1, 1}, {-1, 1}, {1, -1}, {-1, -1}};

}  // namespace

/// 0x089180ec
cvTerrainBlend::cvTerrainBlend(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& a1) {
    m_pPiecesLibrary = nullptr;
    Fun_0891b3fa(&m_matcher);
    m_uiNumPieceIDs = 0;
    field_1d0 = 0x3c3f;
    field_1d4 = 0x1de2;
    field_1d8 = 13000;
    field_1dc = 0.23535515f;
}

/// 0x08918168
cvTerrainBlend::~cvTerrainBlend() {
    Fun_0891b618(&m_matcher);
}

/// 0x08918194
UNK_RET cvTerrainBlend::LoadTerrainPieces(bool a1, char const* a2, unsigned long* a3, unsigned int a4) {
    // The library constructor is inlined here; the allocation is 4 bytes larger than the declared class.
    cvCompressedTerrainPiecesLibrary* pLibrary = static_cast<cvCompressedTerrainPiecesLibrary*>(operator new(0x80188));
    pLibrary->m_uiFileSize = 0;
    pLibrary->m_pFileBuffer = nullptr;
    pLibrary->m_bFileLoaded = false;
    pLibrary->m_uiFileCapacity = 0;
    memcpy(pLibrary->unk_114, &WorldGlobals::g_TerrainPieceToHex, sizeof(pLibrary->unk_114));
    pLibrary->m_aPieces.m_pData = nullptr;
    pLibrary->m_aPieces.m_uiSize = 0;
    pLibrary->m_aPieces.m_uiCapacity = 0;
    pLibrary->m_aPieces.m_pData = static_cast<TerrainPiece*>(FireMallocAlignedNoTracking(0x100 * sizeof(TerrainPiece), 4, "Port/FirePlace/FireWorks/FFastVector.h", 0x55));
    pLibrary->m_aPieces.m_uiCapacity = 0x100;
    pLibrary->m_aByteRuns.m_pData = nullptr;
    pLibrary->m_aByteRuns.m_uiSize = 0;
    pLibrary->m_aByteRuns.m_uiCapacity = 0;
    pLibrary->m_aHeightRuns.m_pData = nullptr;
    pLibrary->m_aHeightRuns.m_uiSize = 0;
    pLibrary->m_aHeightRuns.m_uiCapacity = 0;
    pLibrary->m_aRowOffsets.m_pData = nullptr;
    pLibrary->m_aRowOffsets.m_uiSize = 0;
    pLibrary->m_aRowOffsets.m_uiCapacity = 0;
    *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(pLibrary) + 0x80184) = 0;
    m_pPiecesLibrary = pLibrary;

    pLibrary->LoadTerrainPieces(a1, a2, "Resource\\Common\\TerrainPiecesLibrary.fxsb", a3, a4);

    for (unsigned int i = 0; i < m_pPiecesLibrary->m_aPieces.m_uiSize; ++i) {
        TerrainPiece const& kPiece = m_pPiecesLibrary->m_aPieces.m_pData[i];
        if (kPiece.m_ucFlags & kPieceFlagBlend) {
            Fun_0891b67a(&m_matcher, &kPiece);
        }
    }
    m_uiNumPieceIDs = a4;
}

/// 0x08918438
void cvTerrainBlend::ClearTerrainPieces() {
    if (m_pPiecesLibrary != nullptr) {
        Fun_08683154(m_pPiecesLibrary);
        operator delete(m_pPiecesLibrary);
    }
    m_pPiecesLibrary = nullptr;
    m_matcher.m_uiNumPieces = 0;
    m_matcher.m_uiPieceCapacity = 0;
    Fun_08665a1a(m_matcher.m_pPieces);
    m_matcher.m_pPieces = nullptr;
    m_matcher.m_uiNumPieces = 0;
    m_matcher.m_uiMaxPackingPass = 0;
}

/// 0x089184a4
bool cvTerrainBlend::BlendTerrainHeightmap(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u>& grid, FGXInt2 mapSize, FFastVector<FGXVector2, true, 43u, 259u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> const& a3, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& a4, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& a5, TerrainDecalLayer& a6) {
    m_matcher.ComputePatternPositions(grid);
    FGXInt2 classGridSize;
    classGridSize.x = kClassGridSize;
    classGridSize.y = kClassGridSize;
    TerrainClassGrid classGrid;
    Fun_08675cb4(&classGrid, classGridSize, &HEXTransform_i::IDENTITY_TRANSFORM);
    classGrid.m_ucOutOfRangeClass = 0;
    if (WorldGlobals::g_eWrapMode == 0) {
        classGrid.m_bWrap = false;
    }
    classGrid.Init(grid);
    return LaunchBlendTerrainJobs(mapSize, a3, a4, a5, classGrid);
}

/// 0x08918592
bool cvTerrainBlend::LaunchBlendTerrainJobs(FGXInt2 mapSize, FFastVector<FGXVector2, true, 43u, 259u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> const& kSplinePoints, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& kSplinesA, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& kSplinesB, TerrainClassGrid const& classGrid) {
    BlendCellVector kBlendCells;
    const int iCellsX = mapSize.x / kCellSize;
    const int iCellsY = mapSize.y / kCellSize;
    const int iNumJobs = iCellsX * iCellsY;
    if (GetTempHeap()->GetAvailable() < static_cast<unsigned int>(iNumJobs * sizeof(CellBlitJob))) {
        return LaunchBlendTerrainJobsByRow(mapSize, kSplinePoints, kSplinesA, kSplinesB, classGrid);
    }

    CellBlitJob* pJobs = static_cast<CellBlitJob*>(FTempHeapFastVectorAllocator::AllocAligned(iNumJobs * sizeof(CellBlitJob), 4, 43, 259));
    if (pJobs == nullptr) {
        return false;
    }
    for (int i = 0; i < iNumJobs; ++i) {
        new (&pJobs[i]) CellBlitJob();
    }

    const unsigned int uiNumThreads = JobManager_GetNumberThreads();
    CellBlitJob::ms_pThreadLocalFloodFillScratch = new FloodFillScratchVector[uiNumThreads];
    CellBlitJob::ms_pPiecesLibrary = m_pPiecesLibrary;
    for (unsigned int i = 0; i < uiNumThreads; ++i) {
        CellBlitJob::ms_pThreadLocalFloodFillScratch->reserve(0x400);
    }
    CellBlitJob::ms_kThreadLocalStorage.resize(uiNumThreads);
    CellBlitJob::ms_BlendCellSize = BlendCellSize();

    for (int iX = 0; iX < iCellsX; ++iX) {
        for (int iY = 0; iY < iCellsY; ++iY) {
            CellBlitJob& kJob = pJobs[iX + iY * iCellsX];
            kJob.m_uiNumSplinesA = 0;
            kJob.m_uiNumSplinesB = 0;
            kJob.m_uiNumCornerPositions = 0;
            kJob.m_pSplinePoints = kSplinePoints.begin();
            kJob.m_uiNumBlendCells = 0;
            kJob.m_kMin.x = iX * kCellSize;
            kJob.m_kMin.y = iY * kCellSize;
            kJob.m_kMax.x = (iX + 1) * kCellSize;
            kJob.m_kMax.y = (iY + 1) * kCellSize;
            kJob.m_kMapSize = mapSize;
            kJob.field_7ec = field_1dc;
            kJob.m_uiNumPieceIDs = m_uiNumPieceIDs;
            kJob.m_pTerrainSampler = MapGlobals::g_pTerrainSampler;
            kJob.m_pBlendCells = &kBlendCells;
        }
    }

    // Each spline range goes to every cell its points pass through.
    int iLastCellY = 0;
    for (int iList = 0; iList < 2; ++iList) {
        const SplineRangeVector& kRanges = (iList == 0) ? kSplinesA : kSplinesB;
        for (unsigned int i = 0; i < kRanges.size(); ++i) {
            const SplineRange& kRange = kRanges[i];
            int iLastCellX = -1;
            for (int iPoint = kRange.m_iBegin; iPoint < kRange.m_iEnd; ++iPoint) {
                int iX = static_cast<int>(kSplinePoints[iPoint].x);
                int iY = static_cast<int>(kSplinePoints[iPoint].y);
                while (iX < 0) {
                    iX += mapSize.x;
                }
                while (iY < 0) {
                    iY += mapSize.y;
                }
                const int iCellX = iX % mapSize.x / kCellSize;
                const int iCellY = iY % mapSize.y / kCellSize;
                if (iCellX == iLastCellX && iCellY == iLastCellY) {
                    continue;
                }
                CellBlitJob& kJob = pJobs[iCellX + iCellY * iCellsX];
                unsigned int& uiNumRanges = (iList == 0) ? kJob.m_uiNumSplinesA : kJob.m_uiNumSplinesB;
                SplineRange* aRanges = (iList == 0) ? kJob.m_aSplinesA : kJob.m_aSplinesB;
                bool bKnown = false;
                for (int j = static_cast<int>(uiNumRanges) - 1; j >= 0; --j) {
                    if (aRanges[j].m_iBegin == kRange.m_iBegin && aRanges[j].m_iEnd == kRange.m_iEnd) {
                        bKnown = true;
                        break;
                    }
                }
                iLastCellX = iCellX;
                iLastCellY = iCellY;
                if (!bKnown) {
                    aRanges[uiNumRanges] = kRange;
                    ++uiNumRanges;
                }
            }
        }
    }

    // Each hex of the class grid hands its terrain class to the cells around its centre.
    const D3DXMATRIX* pHexToTex = reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex);
    for (int iRow = 0; iRow < classGrid.m_iHeight; ++iRow) {
        for (int iColumn = 0; iColumn < classGrid.m_iWidth; ++iColumn) {
            HEXVec_i kOffsetHex;
            kOffsetHex.x = iColumn - (iRow >> 1);
            kOffsetHex.y = iRow;
            kOffsetHex.z = -(kOffsetHex.x + kOffsetHex.y);
            const HEXVec_i kGridHex = Fun_086616f0(classGrid.m_invMatrix, kOffsetHex);
            HEXVec_i kHex;
            kHex.x = kGridHex.x;
            kHex.y = kGridHex.y;
            kHex.z = -(kGridHex.x + kGridHex.y);
            const unsigned char ucClass = *Fun_0891b884(&classGrid, &kHex);

            const D3DXVECTOR4 vHex = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), static_cast<float>(kHex.z), 1.0f};
            D3DXVECTOR4 vTex;
            D3DXVec4Transform(&vTex, &vHex, pHexToTex);
            const int iTexX = static_cast<int>(vTex.x);
            const int iTexY = static_cast<int>(vTex.y);
            for (int i = 0; i < 4; ++i) {
                const int iX = kCornerOffsets[i][0] + iTexX;
                const int iCellX = iX / kCellSize;
                if (iCellX >= iCellsX) {
                    continue;
                }
                const int iY = kCornerOffsets[i][1] + iTexY;
                const int iCellY = iY / kCellSize;
                if (iCellY >= iCellsY) {
                    continue;
                }
                CellBlitJob& kJob = pJobs[iCellX + iCellY * iCellsX];
                kJob.m_aCornerPositions[kJob.m_uiNumCornerPositions].x = iX;
                kJob.m_aCornerPositions[kJob.m_uiNumCornerPositions].y = iY;
                kJob.m_auiCornerClasses[kJob.m_uiNumCornerPositions] = ucClass;
                ++kJob.m_uiNumCornerPositions;
            }
        }
    }

    // Corners of the small and the large piece in texel space, one set per pattern transform.
    FGXInt2* aCorners = static_cast<FGXInt2*>(_alloca(m_matcher.m_uiNumTransforms * kExtentCorners * sizeof(FGXInt2)));
    const D3DXVECTOR4* aSmallCorners = reinterpret_cast<const D3DXVECTOR4*>(WorldGlobals::g_ahSmallCorners);
    const D3DXVECTOR4* aLargeCorners = reinterpret_cast<const D3DXVECTOR4*>(WorldGlobals::g_ahLargeCorners);
    for (unsigned int i = 0; i < m_matcher.m_uiNumTransforms; ++i) {
        D3DXMATRIX kMatrix;
        for (int k = 0; k < 16; ++k) {
            kMatrix.m[k / 4][k % 4] = static_cast<float>(m_matcher.m_pTransforms[i].m_aiElements[k]);
        }
        const D3DXVECTOR4 vZero = {0.0f, 0.0f, -0.0f, 1.0f};
        D3DXVECTOR4 vOrigin;
        D3DXVec4Transform(&vOrigin, &vZero, pHexToTex);
        for (int c = 0; c < kExtentCorners; ++c) {
            const D3DXVECTOR4* pCorner = ((c < 2) ? aSmallCorners : aLargeCorners) + (c & 1);
            D3DXVECTOR4 vLocal;
            D3DXVec4Transform(&vLocal, pCorner, &kMatrix);
            D3DXVECTOR4 vTex;
            D3DXVec4Transform(&vTex, &vLocal, pHexToTex);
            aCorners[i * kExtentCorners + c].x = static_cast<int>(roundf(vTex.x - vOrigin.x));
            aCorners[i * kExtentCorners + c].y = static_cast<int>(roundf(vTex.y - vOrigin.y));
        }
    }

    // One BlendCell per pattern position, listed in every job whose cell it overlaps.
    unsigned int uiNode = kNoNode;
    unsigned int uiNodeCount = 0;
    unsigned int uiSlot = 0;
    if (m_matcher.m_kPositions.m_uiFirstNode != kNoNode) {
        uiNode = m_matcher.m_kPositions.m_uiFirstNode & 0xffffff;
        uiNodeCount = m_matcher.m_kPositions.m_kNodes[uiNode].m_uiCount & 0x7fffffff;
    }
    while (uiNode != kNoNode || uiSlot != 0) {
        const HexPatternMatcher::PatternPosition& kPosition = m_matcher.m_kPositions.m_kNodes[uiNode].m_aPositions[uiSlot];
        const PatternPiece& kPattern = m_matcher.m_pPieces[kPosition.m_uiPieceIndex];
        const TerrainPiece& kPiece = m_pPiecesLibrary->m_aPieces.m_pData[kPattern.m_iLibraryIndex];

        const D3DXVECTOR4 vHex = {static_cast<float>(kPosition.m_iHexX), static_cast<float>(kPosition.m_iHexY), static_cast<float>(kPosition.m_iHexZ), 1.0f};
        D3DXVECTOR4 vTex;
        D3DXVec4Transform(&vTex, &vHex, pHexToTex);
        const int iTexX = static_cast<int>(roundf(vTex.x));
        const int iTexY = static_cast<int>(roundf(vTex.y));

        const unsigned int uiCellIndex = kBlendCells.size();
        kBlendCells.resize(uiCellIndex + 1);
        BlendCell& kCell = kBlendCells[uiCellIndex];
        memset(&kCell, 0, sizeof(BlendCell));
        kCell.m_uiPatternType = static_cast<unsigned short>(kPiece.m_sPatternType);
        kCell.m_iIDMapIndex = kPiece.m_iIDMapIndex;
        kCell.m_iHeightmapIndex = kPiece.m_iHeightmapIndex;
        kCell.m_iBlendmap2Index = kPiece.m_iBlendmap2Index;
        kCell.m_iBlendmap1Index = kPiece.m_iBlendmap1Index;
        kCell.m_bSmall = kPiece.m_ucFlags & kPieceFlagSmall;

        const FGXInt2* pExtent = &aCorners[kPosition.m_uiTransformIndex * kExtentCorners + ((kPiece.m_ucFlags & kPieceFlagSmall) ? 0 : 2)];
        kCell.m_kCorner0.x = pExtent[0].x + iTexX;
        kCell.m_kCorner0.y = pExtent[0].y + iTexY;
        kCell.m_kCorner1.x = iTexX + pExtent[1].x;
        kCell.m_kCorner1.y = iTexY + pExtent[1].y;
        const int iPieceSize = (kPiece.m_ucFlags & kPieceFlagSmall) ? kSmallPieceSize : kLargePieceSize;
        kCell.m_kSize.x = iPieceSize;
        kCell.m_kSize.y = iPieceSize;

        const int iMinX = (kCell.m_kCorner0.x < kCell.m_kCorner1.x) ? kCell.m_kCorner0.x : kCell.m_kCorner1.x;
        const int iMaxX = (kCell.m_kCorner0.x > kCell.m_kCorner1.x) ? kCell.m_kCorner0.x : kCell.m_kCorner1.x;
        const int iMinY = (kCell.m_kCorner0.y < kCell.m_kCorner1.y) ? kCell.m_kCorner0.y : kCell.m_kCorner1.y;
        const int iMaxY = (kCell.m_kCorner0.y > kCell.m_kCorner1.y) ? kCell.m_kCorner0.y : kCell.m_kCorner1.y;
        const int iEndCellX = iMaxX / kCellSize + 1;
        const int iFirstCellY = iMinY / kCellSize;
        const int iEndCellY = iMaxY / kCellSize + 1;
        for (int iCellX = iMinX / kCellSize; iCellX < iEndCellX; ++iCellX) {
            for (int iCellY = iFirstCellY; iCellY < iEndCellY; ++iCellY) {
                if (iCellY < 0 || iCellY >= iCellsY) {
                    continue;
                }
                CellBlitJob& kJob = pJobs[(iCellX + iCellsX * kColumnWrapSlack) % iCellsX + iCellY * iCellsX];
                kJob.m_auiBlendCells[kJob.m_uiNumBlendCells] = uiCellIndex;
                ++kJob.m_uiNumBlendCells;
            }
        }

        ++uiSlot;
        if (uiSlot >= uiNodeCount) {
            const unsigned int uiNext = m_matcher.m_kPositions.m_kNodes[uiNode].m_uiNext;
            uiSlot = 0;
            if (uiNext == kNoNode) {
                uiNode = kNoNode;
                uiNodeCount = 0;
            } else {
                uiNode = uiNext;
                uiNodeCount = m_matcher.m_kPositions.m_kNodes[uiNext].m_uiCount & 0x7fffffff;
            }
        }
    }

    for (int iX = 0; iX < iCellsX; ++iX) {
        for (int iY = 0; iY < iCellsY; ++iY) {
            AddJob(&pJobs[iX + iY * iCellsX], kJobTag, nullptr);
        }
    }
    Wait();

    delete[] CellBlitJob::ms_pThreadLocalFloodFillScratch;
    CellBlitJob::ms_pThreadLocalFloodFillScratch = nullptr;
    FTempHeapFastVectorAllocator::FreeAligned(pJobs);
    return true;
}

/// 0x08919d3e
bool cvTerrainBlend::LaunchBlendTerrainJobsByRow(FGXInt2 mapSize, FFastVector<FGXVector2, true, 43u, 259u, BaseVector<FGXVector2, true>::FDefaultFastVectorAllocator> const& kSplinePoints, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& kSplinesA, FFastVector<SplineRange, true, 43u, 259u, BaseVector<SplineRange, true>::FDefaultFastVectorAllocator> const& kSplinesB, TerrainClassGrid const& classGrid) {
    BlendCellVector kBlendCells;
    const int iCellsX = mapSize.x / kCellSize;
    CellBlitJob* pJobs = static_cast<CellBlitJob*>(FTempHeapFastVectorAllocator::AllocAligned(iCellsX * sizeof(CellBlitJob), 4, 43, 259));
    if (pJobs == nullptr) {
        return false;
    }
    for (int i = 0; i < iCellsX; ++i) {
        new (&pJobs[i]) CellBlitJob();
    }

    const int iCellsY = mapSize.y / kCellSize;
    const unsigned int uiNumThreads = JobManager_GetNumberThreads();
    CellBlitJob::ms_pThreadLocalFloodFillScratch = new FloodFillScratchVector[uiNumThreads];
    CellBlitJob::ms_pPiecesLibrary = m_pPiecesLibrary;
    for (unsigned int i = 0; i < uiNumThreads; ++i) {
        CellBlitJob::ms_pThreadLocalFloodFillScratch->reserve(0x400);
    }
    CellBlitJob::ms_kThreadLocalStorage.resize(uiNumThreads);
    CellBlitJob::ms_BlendCellSize = BlendCellSize();

    // Corners of the small and the large piece in texel space, one set per pattern transform.
    const D3DXMATRIX* pHexToTex = reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex);
    FGXInt2* aCorners = static_cast<FGXInt2*>(_alloca(m_matcher.m_uiNumTransforms * kExtentCorners * sizeof(FGXInt2)));
    const D3DXVECTOR4* aSmallCorners = reinterpret_cast<const D3DXVECTOR4*>(WorldGlobals::g_ahSmallCorners);
    const D3DXVECTOR4* aLargeCorners = reinterpret_cast<const D3DXVECTOR4*>(WorldGlobals::g_ahLargeCorners);
    for (unsigned int i = 0; i < m_matcher.m_uiNumTransforms; ++i) {
        D3DXMATRIX kMatrix;
        for (int k = 0; k < 16; ++k) {
            kMatrix.m[k / 4][k % 4] = static_cast<float>(m_matcher.m_pTransforms[i].m_aiElements[k]);
        }
        const D3DXVECTOR4 vZero = {0.0f, 0.0f, -0.0f, 1.0f};
        D3DXVECTOR4 vOrigin;
        D3DXVec4Transform(&vOrigin, &vZero, pHexToTex);
        for (int c = 0; c < kExtentCorners; ++c) {
            const D3DXVECTOR4* pCorner = ((c < 2) ? aSmallCorners : aLargeCorners) + (c & 1);
            D3DXVECTOR4 vLocal;
            D3DXVec4Transform(&vLocal, pCorner, &kMatrix);
            D3DXVECTOR4 vTex;
            D3DXVec4Transform(&vTex, &vLocal, pHexToTex);
            aCorners[i * kExtentCorners + c].x = static_cast<int>(roundf(vTex.x - vOrigin.x));
            aCorners[i * kExtentCorners + c].y = static_cast<int>(roundf(vTex.y - vOrigin.y));
        }
    }

    // pJobs holds one row of cells; only data that falls into row iRow is handed out.
    for (int iRow = 0; iRow < iCellsY; ++iRow) {
        kBlendCells.clear();
        for (int iX = 0; iX < iCellsX; ++iX) {
            CellBlitJob& kJob = pJobs[iX];
            kJob.m_uiNumSplinesA = 0;
            kJob.m_uiNumSplinesB = 0;
            kJob.m_uiNumCornerPositions = 0;
            kJob.m_pSplinePoints = kSplinePoints.begin();
            kJob.m_uiNumBlendCells = 0;
            kJob.m_kMin.x = iX * kCellSize;
            kJob.m_kMin.y = iRow * kCellSize;
            kJob.m_kMax.x = (iX + 1) * kCellSize;
            kJob.m_kMax.y = (iRow + 1) * kCellSize;
            kJob.m_kMapSize = mapSize;
            kJob.field_7ec = field_1dc;
            kJob.m_uiNumPieceIDs = m_uiNumPieceIDs;
            kJob.m_pTerrainSampler = MapGlobals::g_pTerrainSampler;
            kJob.m_pBlendCells = &kBlendCells;
        }

        int iLastCellY = 0;
        for (int iList = 0; iList < 2; ++iList) {
            const SplineRangeVector& kRanges = (iList == 0) ? kSplinesA : kSplinesB;
            for (unsigned int i = 0; i < kRanges.size(); ++i) {
                const SplineRange& kRange = kRanges[i];
                int iLastCellX = -1;
                for (int iPoint = kRange.m_iBegin; iPoint < kRange.m_iEnd; ++iPoint) {
                    int iX = static_cast<int>(kSplinePoints[iPoint].x);
                    int iY = static_cast<int>(kSplinePoints[iPoint].y);
                    while (iX < 0) {
                        iX += mapSize.x;
                    }
                    while (iY < 0) {
                        iY += mapSize.y;
                    }
                    const int iCellX = iX % mapSize.x / kCellSize;
                    const int iCellY = iY % mapSize.y / kCellSize;
                    if (iCellX == iLastCellX && iCellY == iLastCellY) {
                        continue;
                    }
                    if (iCellY == iRow) {
                        CellBlitJob& kJob = pJobs[iCellX];
                        unsigned int& uiNumRanges = (iList == 0) ? kJob.m_uiNumSplinesA : kJob.m_uiNumSplinesB;
                        SplineRange* aRanges = (iList == 0) ? kJob.m_aSplinesA : kJob.m_aSplinesB;
                        bool bKnown = false;
                        for (int j = static_cast<int>(uiNumRanges) - 1; j >= 0; --j) {
                            if (aRanges[j].m_iBegin == kRange.m_iBegin && aRanges[j].m_iEnd == kRange.m_iEnd) {
                                bKnown = true;
                                break;
                            }
                        }
                        if (!bKnown) {
                            aRanges[uiNumRanges] = kRange;
                            ++uiNumRanges;
                        }
                    }
                    iLastCellX = iCellX;
                    iLastCellY = iCellY;
                }
            }
        }

        for (int iClassRow = 0; iClassRow < classGrid.m_iHeight; ++iClassRow) {
            for (int iColumn = 0; iColumn < classGrid.m_iWidth; ++iColumn) {
                HEXVec_i kOffsetHex;
                kOffsetHex.x = iColumn - (iClassRow >> 1);
                kOffsetHex.y = iClassRow;
                kOffsetHex.z = -(kOffsetHex.x + kOffsetHex.y);
                const HEXVec_i kGridHex = Fun_086616f0(classGrid.m_invMatrix, kOffsetHex);
                HEXVec_i kHex;
                kHex.x = kGridHex.x;
                kHex.y = kGridHex.y;
                kHex.z = -(kGridHex.x + kGridHex.y);
                const unsigned char ucClass = *Fun_0891b884(&classGrid, &kHex);

                const D3DXVECTOR4 vHex = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), static_cast<float>(kHex.z), 1.0f};
                D3DXVECTOR4 vTex;
                D3DXVec4Transform(&vTex, &vHex, pHexToTex);
                const int iTexX = static_cast<int>(vTex.x);
                const int iTexY = static_cast<int>(vTex.y);
                for (int i = 0; i < 4; ++i) {
                    const int iX = kCornerOffsets[i][0] + iTexX;
                    const int iCellX = iX / kCellSize;
                    if (iCellX >= iCellsX) {
                        continue;
                    }
                    const int iY = kCornerOffsets[i][1] + iTexY;
                    const int iCellY = iY / kCellSize;
                    if (iCellY >= iCellsY || iCellY != iRow) {
                        continue;
                    }
                    CellBlitJob& kJob = pJobs[iCellX];
                    kJob.m_aCornerPositions[kJob.m_uiNumCornerPositions].x = iX;
                    kJob.m_aCornerPositions[kJob.m_uiNumCornerPositions].y = iY;
                    kJob.m_auiCornerClasses[kJob.m_uiNumCornerPositions] = ucClass;
                    ++kJob.m_uiNumCornerPositions;
                }
            }
        }

        unsigned int uiNode = kNoNode;
        unsigned int uiNodeCount = 0;
        unsigned int uiSlot = 0;
        if (m_matcher.m_kPositions.m_uiFirstNode != kNoNode) {
            uiNode = m_matcher.m_kPositions.m_uiFirstNode & 0xffffff;
            uiNodeCount = m_matcher.m_kPositions.m_kNodes[uiNode].m_uiCount & 0x7fffffff;
        }
        while (uiNode != kNoNode || uiSlot != 0) {
            const HexPatternMatcher::PatternPosition& kPosition = m_matcher.m_kPositions.m_kNodes[uiNode].m_aPositions[uiSlot];
            const PatternPiece& kPattern = m_matcher.m_pPieces[kPosition.m_uiPieceIndex];
            const TerrainPiece& kPiece = m_pPiecesLibrary->m_aPieces.m_pData[kPattern.m_iLibraryIndex];

            const D3DXVECTOR4 vHex = {static_cast<float>(kPosition.m_iHexX), static_cast<float>(kPosition.m_iHexY), static_cast<float>(kPosition.m_iHexZ), 1.0f};
            D3DXVECTOR4 vTex;
            D3DXVec4Transform(&vTex, &vHex, pHexToTex);
            const int iTexX = static_cast<int>(roundf(vTex.x));
            const int iTexY = static_cast<int>(roundf(vTex.y));

            const unsigned int uiCellIndex = kBlendCells.size();
            kBlendCells.resize(uiCellIndex + 1);
            BlendCell& kCell = kBlendCells[uiCellIndex];
            memset(&kCell, 0, sizeof(BlendCell));
            kCell.m_uiPatternType = static_cast<unsigned short>(kPiece.m_sPatternType);
            kCell.m_iIDMapIndex = kPiece.m_iIDMapIndex;
            kCell.m_iHeightmapIndex = kPiece.m_iHeightmapIndex;
            kCell.m_iBlendmap2Index = kPiece.m_iBlendmap2Index;
            kCell.m_iBlendmap1Index = kPiece.m_iBlendmap1Index;
            kCell.m_bSmall = kPiece.m_ucFlags & kPieceFlagSmall;

            const FGXInt2* pExtent = &aCorners[kPosition.m_uiTransformIndex * kExtentCorners + ((kPiece.m_ucFlags & kPieceFlagSmall) ? 0 : 2)];
            kCell.m_kCorner0.x = pExtent[0].x + iTexX;
            kCell.m_kCorner0.y = pExtent[0].y + iTexY;
            kCell.m_kCorner1.x = iTexX + pExtent[1].x;
            kCell.m_kCorner1.y = iTexY + pExtent[1].y;
            const int iPieceSize = (kPiece.m_ucFlags & kPieceFlagSmall) ? kSmallPieceSize : kLargePieceSize;
            kCell.m_kSize.x = iPieceSize;
            kCell.m_kSize.y = iPieceSize;

            const int iMinX = (kCell.m_kCorner0.x < kCell.m_kCorner1.x) ? kCell.m_kCorner0.x : kCell.m_kCorner1.x;
            const int iMaxX = (kCell.m_kCorner0.x > kCell.m_kCorner1.x) ? kCell.m_kCorner0.x : kCell.m_kCorner1.x;
            const int iMinY = (kCell.m_kCorner0.y < kCell.m_kCorner1.y) ? kCell.m_kCorner0.y : kCell.m_kCorner1.y;
            const int iMaxY = (kCell.m_kCorner0.y > kCell.m_kCorner1.y) ? kCell.m_kCorner0.y : kCell.m_kCorner1.y;
            const int iEndCellX = iMaxX / kCellSize + 1;
            const int iFirstCellY = iMinY / kCellSize;
            const int iEndCellY = iMaxY / kCellSize + 1;
            for (int iCellX = iMinX / kCellSize; iCellX < iEndCellX; ++iCellX) {
                for (int iCellY = iFirstCellY; iCellY < iEndCellY; ++iCellY) {
                    if (iCellY < 0 || iCellY >= iCellsY || iCellY != iRow) {
                        continue;
                    }
                    CellBlitJob& kJob = pJobs[(iCellX + iCellsX * kColumnWrapSlack) % iCellsX];
                    kJob.m_auiBlendCells[kJob.m_uiNumBlendCells] = uiCellIndex;
                    ++kJob.m_uiNumBlendCells;
                }
            }

            ++uiSlot;
            if (uiSlot >= uiNodeCount) {
                const unsigned int uiNext = m_matcher.m_kPositions.m_kNodes[uiNode].m_uiNext;
                uiSlot = 0;
                if (uiNext == kNoNode) {
                    uiNode = kNoNode;
                    uiNodeCount = 0;
                } else {
                    uiNode = uiNext;
                    uiNodeCount = m_matcher.m_kPositions.m_kNodes[uiNext].m_uiCount & 0x7fffffff;
                }
            }
        }

        for (int iX = 0; iX < iCellsX; ++iX) {
            AddJob(&pJobs[iX], kJobTag, nullptr);
        }
        Wait();
    }

    delete[] CellBlitJob::ms_pThreadLocalFloodFillScratch;
    CellBlitJob::ms_pThreadLocalFloodFillScratch = nullptr;
    FTempHeapFastVectorAllocator::FreeAligned(pJobs);
    return true;
}

/// 0x0891b67a
void Fun_0891b67a(HexPatternMatcher* a1, TerrainPiece const* a2) {
    const unsigned int uiIndex = a1->m_uiNumPieces;
    if (uiIndex == a1->m_uiPieceCapacity) {
        Fun_0891ce36(&a1->m_pPieces, uiIndex + 1);
    }
    a1->m_uiNumPieces = uiIndex + 1;
    PatternPiece& kPiece = a1->m_pPieces[uiIndex];
    memset(&kPiece, 0, sizeof(PatternPiece));
    HEXTransform_i kIdentity = HEXTransform_i::IDENTITY_TRANSFORM;
    Fun_0891cdd0(&kPiece, &kIdentity);
    kPiece.field_3c0 = kPiece.m_iNumRows;
    kPiece.field_3c4 = kPiece.m_iNumColumns;
    memcpy(&kPiece, a2, 0x3d0);

    kPiece.m_ucNumUsedCells = 0;
    for (int iRow = 0; iRow < kPiece.m_iNumRows; ++iRow) {
        for (int iColumn = 0; iColumn < kPiece.m_iNumColumns; ++iColumn) {
            TerrainPieceCell const& kCell = kPiece.m_aCells[iColumn * 10 + iRow];
            if (kCell.m_ulHexCode != 0xffffffff || kCell.m_bValid) {
                kPiece.m_aucUsedCells[kPiece.m_ucNumUsedCells * 2] = static_cast<unsigned char>(iRow);
                kPiece.m_aucUsedCells[kPiece.m_ucNumUsedCells * 2 + 1] = static_cast<unsigned char>(iColumn);
                ++kPiece.m_ucNumUsedCells;
            }
        }
    }

    if (kPiece.field_3c0 < kPiece.m_iNumRows) {
        kPiece.m_iNumRows = kPiece.field_3c0;
    }
    if (kPiece.field_3c4 < kPiece.m_iNumColumns) {
        kPiece.m_iNumColumns = kPiece.field_3c4;
    }
    if (a1->m_uiMaxPackingPass < kPiece.m_uiPackingPass) {
        a1->m_uiMaxPackingPass = kPiece.m_uiPackingPass;
    }
}
