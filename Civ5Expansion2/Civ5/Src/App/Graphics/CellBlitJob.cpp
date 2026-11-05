/// origin guess
#include "Civ5/Src/App/Graphics/CellBlitJob.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/cvCityBuildingSim.h"

/// 0x0891bc02
void CellBlitJob::Execute(unsigned int uiThreadIndex) {
    m_pThreadStorage = &ms_kThreadLocalStorage[uiThreadIndex];
    Fun_0891bc58();
    Fun_0891bcd2(uiThreadIndex);
    Fun_0891bfd0();
    Fun_0891c5e8();
}

/// 0x0891bc58
void CellBlitJob::Fun_0891bc58() {
    for (unsigned int uiY = 0; uiY < ms_BlendCellSize; ++uiY) {
        for (unsigned int uiX = 0; uiX < ms_BlendCellSize; ++uiX) {
            BlendTexel& kTexel = m_pThreadStorage->m_aTexels[uiY][uiX];
            kTexel.m_fHeightSum = 0.0f;
            // Keeps the height average finite for a texel no piece covers.
            kTexel.m_fWeightSum = 1.0e-6f;
            kTexel.m_ucClass = 4;
            kTexel.m_ucPieceID = 3;
            kTexel.m_ucPieceWeight = 0;
            kTexel.field_b = 0;
        }
    }
}

/// 0x0891bcd2
void CellBlitJob::Fun_0891bcd2(unsigned int uiThreadIndex) {
    TerrainGenCallback kClassifier;
    kClassifier.m_kMin = m_kMin;
    kClassifier.m_kMax = m_kMax;
    kClassifier.m_kMapSize = m_kMapSize;
    kClassifier.m_iWidth = m_kMax.x - m_kMin.x;
    kClassifier.m_uiClass = 2;
    kClassifier.m_pTexels = m_pThreadStorage;

    TerrainFloodFill kFloodFill;
    kFloodFill.m_pCallback = &kClassifier;
    kFloodFill.m_pStack = ms_pThreadLocalFloodFillScratch + uiThreadIndex;
    kFloodFill.m_pStack->clear();

    TerrainLineSeedCallback kSeeder;
    kSeeder.m_kMin = m_kMin;
    kSeeder.m_kMax = m_kMax;
    kSeeder.m_kMapSize = m_kMapSize;
    kSeeder.m_pFloodFill = &kFloodFill;
    kSeeder.m_pTexels = m_pThreadStorage;

    for (unsigned int uiSpline = 0; uiSpline < m_uiNumSplinesA; ++uiSpline) {
        SplineRange const& kRange = m_aSplinesA[uiSpline];
        kClassifier.m_uiClass = kRange.field_8;
        if (kRange.m_iBegin + 1 == kRange.m_iEnd) {
            continue;
        }
        int iX0 = static_cast<int>(m_pSplinePoints[kRange.m_iBegin].x);
        int iY0 = static_cast<int>(m_pSplinePoints[kRange.m_iBegin].y);
        for (int iPoint = kRange.m_iBegin + 1; iPoint != kRange.m_iEnd; ++iPoint) {
            const int iX1 = static_cast<int>(m_pSplinePoints[iPoint].x);
            const int iY1 = static_cast<int>(m_pSplinePoints[iPoint].y);
            RasterizeLine(iX0, iY0, iX1, iY1, kClassifier);
            iX0 = iX1;
            iY0 = iY1;
        }
    }

    for (unsigned int uiSpline = 0; uiSpline < m_uiNumSplinesB; ++uiSpline) {
        SplineRange const& kRange = m_aSplinesB[uiSpline];
        kClassifier.m_uiClass = kRange.field_8;
        if (kRange.m_iBegin + 1 != kRange.m_iEnd) {
            int iX0 = static_cast<int>(m_pSplinePoints[kRange.m_iBegin].x);
            int iY0 = static_cast<int>(m_pSplinePoints[kRange.m_iBegin].y);
            for (int iPoint = kRange.m_iBegin + 1; iPoint != kRange.m_iEnd; ++iPoint) {
                const int iX1 = static_cast<int>(m_pSplinePoints[iPoint].x);
                const int iY1 = static_cast<int>(m_pSplinePoints[iPoint].y);
                RasterizeLine(iX0, iY0, iX1, iY1, kSeeder);
                iX0 = iX1;
                iY0 = iY1;
            }
        }
        kFloodFill.Fun_0891c9a6();
    }

    for (unsigned int uiCorner = 0; uiCorner < m_uiNumCornerPositions; ++uiCorner) {
        kClassifier.m_uiClass = m_auiCornerClasses[uiCorner];
        if (!kClassifier.Fun_0891ca60(m_aCornerPositions[uiCorner])) {
            kFloodFill.Fun_0891cad8(m_aCornerPositions[uiCorner]);
            kFloodFill.Fun_0891c9a6();
        }
    }

    for (unsigned int uiX = 0; uiX < ms_BlendCellSize; ++uiX) {
        for (unsigned int uiY = 0; uiY < ms_BlendCellSize; ++uiY) {
            BlendTexel& kTexel = m_pThreadStorage->m_aTexels[uiY][uiX];
            if (kTexel.m_ucClass == 4) {
                kTexel.m_ucClass = 2;
            }
        }
    }
}

/// 0x0891bfd0
void CellBlitJob::Fun_0891bfd0() {
    unsigned short ausHeights[64 * 64];
    unsigned char aucWeights[64 * 64];
    unsigned char aucPriorities[64 * 64];
    unsigned char aucIDs[64 * 64];

    for (unsigned int uiCell = 0; uiCell < m_uiNumBlendCells; ++uiCell) {
        BlendCell const& kCell = (*m_pBlendCells)[m_auiBlendCells[uiCell]];
        const int iStepY = (kCell.m_kCorner0.y < kCell.m_kCorner1.y) ? 1 : -1;
        const int iStepX = (kCell.m_kCorner0.x < kCell.m_kCorner1.x) ? 1 : -1;

        // A piece across the x wrap seam is moved by one map width to overlap this cell.
        int iX0 = kCell.m_kCorner0.x;
        int iX1 = kCell.m_kCorner1.x;
        if (((iX0 < iX1) ? iX0 : iX1) > m_kMax.x) {
            iX0 -= m_kMapSize.x;
            iX1 -= m_kMapSize.x;
        }
        if (((iX0 > iX1) ? iX0 : iX1) < m_kMin.x) {
            iX0 += m_kMapSize.x;
            iX1 += m_kMapSize.x;
        }

        // Destination runs from the first to the end texel by the step; source columns run upwards in both cases.
        int iFirstX, iEndX, iSrcX0, iSrcX1;
        if (iStepX > 0) {
            iFirstX = (m_kMin.x > iX0) ? m_kMin.x : iX0;
            iEndX = (m_kMax.x < iX1) ? m_kMax.x : iX1;
            iSrcX0 = iFirstX - iX0;
            iSrcX1 = iSrcX0 + abs(iFirstX - iEndX);
        } else {
            const int iLow = (m_kMin.x > iX1) ? m_kMin.x : iX1;
            const int iHigh = (m_kMax.x < iX0) ? m_kMax.x : iX0;
            iSrcX0 = iX0 - iHigh;
            iSrcX1 = iSrcX0 + abs(iLow - iHigh);
            iFirstX = iHigh - 1;
            iEndX = iLow - 1;
        }

        const int iY0 = kCell.m_kCorner0.y;
        const int iY1 = kCell.m_kCorner1.y;
        int iFirstY, iEndY, iSrcY0, iSrcY1;
        if (iStepY > 0) {
            iFirstY = (m_kMin.y > iY0) ? m_kMin.y : iY0;
            iEndY = (m_kMax.y < iY1) ? m_kMax.y : iY1;
            iSrcY0 = iFirstY - iY0;
            iSrcY1 = iSrcY0 + abs(iFirstY - iEndY);
        } else {
            const int iLow = (m_kMin.y > iY1) ? m_kMin.y : iY1;
            const int iHigh = (m_kMax.y < iY0) ? m_kMax.y : iY0;
            iSrcY0 = iY0 - iHigh;
            iSrcY1 = iSrcY0 + abs(iLow - iHigh);
            iFirstY = iHigh - 1;
            iEndY = iLow - 1;
        }

        FGXInt2 kSrcStart;
        kSrcStart.x = iSrcX0;
        kSrcStart.y = iSrcY0;
        FGXInt2 kSrcEnd;
        kSrcEnd.x = iSrcX1;
        kSrcEnd.y = iSrcY1;
        ms_pPiecesLibrary->DecompressIDMapBlock(kCell.m_iIDMapIndex, kSrcStart, kSrcEnd, aucIDs, kCell.m_bSmall);
        ms_pPiecesLibrary->DecompressBlendmapBlock(kCell.m_iBlendmap2Index, kSrcStart, kSrcEnd, aucPriorities, kCell.m_bSmall != 0);
        ms_pPiecesLibrary->DecompressBlendmapBlock(kCell.m_iBlendmap1Index, kSrcStart, kSrcEnd, aucWeights, kCell.m_bSmall != 0);
        ms_pPiecesLibrary->DecompressHeightmapBlock(kCell.m_iHeightmapIndex, kSrcStart, kSrcEnd, ausHeights, kCell.m_bSmall != 0);

        const int iRowWidth = iSrcX1 - iSrcX0;
        const int iWrapBias = m_kMapSize.x * 10;
        int iRow = 0;
        for (int iY = iFirstY; iY != iEndY; iY += iStepY, iRow += iRowWidth) {
            int iSrc = iRow;
            for (int iX = iFirstX; iX != iEndX; iX += iStepX, ++iSrc) {
                const int iMapX = (iWrapBias + iX) % m_kMapSize.x;
                if (iMapX < m_kMin.x || iY < m_kMin.y || iMapX >= m_kMax.x || iY >= m_kMax.y) {
                    continue;
                }
                BlendTexel& kTexel = m_pThreadStorage->m_aTexels[static_cast<unsigned int>(iY) % ms_BlendCellSize]
                                                                [static_cast<unsigned int>(iMapX) % ms_BlendCellSize];
                if (kTexel.m_ucClass != kCell.m_uiPatternType) {
                    continue;
                }
                if (aucPriorities[iSrc] > kTexel.m_ucPieceWeight) {
                    kTexel.m_ucPieceWeight = aucPriorities[iSrc];
                    kTexel.m_ucPieceID = aucIDs[iSrc];
                }
                const float fWeight = aucWeights[iSrc] / 255.0f;
                kTexel.m_fHeightSum += (ausHeights[iSrc] / 65535.0f - field_7ec) * fWeight;
                kTexel.m_fWeightSum += fWeight;
            }
        }
    }
}

/// 0x0891c5e8
void CellBlitJob::Fun_0891c5e8() {
    for (unsigned int uiY = 0; uiY < ms_BlendCellSize; ++uiY) {
        for (unsigned int uiX = 0; uiX < ms_BlendCellSize; ++uiX) {
            BlendTexel const& kTexel = m_pThreadStorage->m_aTexels[uiY][uiX];
            unsigned short usHeight = static_cast<unsigned short>(
                static_cast<int>(static_cast<double>(kTexel.m_fHeightSum / kTexel.m_fWeightSum + field_7ec) * 65535.0));
            FGXInt2 kPos;
            kPos.x = m_kMin.x + static_cast<int>(uiX);
            kPos.y = m_kMin.y + static_cast<int>(uiY);
            FGXInt2 kWrapped = m_pTerrainSampler->GetWrappedPosition(kPos);
            Fun_087fa84a(&m_pTerrainSampler->m_iHeightMapWidth, &usHeight, &kWrapped);
            unsigned char ucPieceID = kTexel.m_ucPieceID;
            kWrapped = m_pTerrainSampler->GetWrappedPosition(kPos);
            Fun_087e75d2(m_pTerrainSampler->unk_4c, &ucPieceID, &kWrapped);
        }
    }
}

/// 0x0891ca60
bool TerrainGenCallback::Fun_0891ca60(FGXInt2 kPos) const {
    const int iX = (m_kMapSize.x * 100 + kPos.x) % m_kMapSize.x;
    const int iY = (m_kMapSize.y * 100 + kPos.y) % m_kMapSize.y;
    if (iX < m_kMin.x || iX >= m_kMax.x || iY < m_kMin.y || iY >= m_kMax.y) {
        return true;
    }
    return m_pTexels->m_aTexels[iY % 64][iX % 64].m_ucClass != 4;
}

/// 0x0891cb4c
void TerrainGenCallback::Fun_0891cb4c(FGXInt2 kPos) {
    const int iX = (m_kMapSize.x * 100 + kPos.x) % m_kMapSize.x;
    const int iY = (m_kMapSize.y * 100 + kPos.y) % m_kMapSize.y;
    if (iX < m_kMin.x || iX >= m_kMax.x || iY < m_kMin.y || iY >= m_kMax.y) {
        return;
    }
    m_pTexels->m_aTexels[iY % 64][iX % 64].m_ucClass = static_cast<unsigned char>(m_uiClass);
}

/// 0x0891cad8
void TerrainFloodFill::Fun_0891cad8(FGXInt2 const& kPos) {
    if (m_pCallback->Fun_0891ca60(kPos)) {
        return;
    }
    m_pCallback->Fun_0891cb4c(kPos);
    m_pStack->push_back(kPos);
}

/// 0x0891c9a6
void TerrainFloodFill::Fun_0891c9a6() {
    while (m_pStack->size() != 0) {
        const FGXInt2 kPos = m_pStack->back();
        m_pStack->pop_back();
        FGXInt2 kNext;
        kNext.x = kPos.x + 1;
        kNext.y = kPos.y;
        Fun_0891cad8(kNext);
        kNext.x = kPos.x - 1;
        kNext.y = kPos.y;
        Fun_0891cad8(kNext);
        kNext.x = kPos.x;
        kNext.y = kPos.y + 1;
        Fun_0891cad8(kNext);
        kNext.x = kPos.x;
        kNext.y = kPos.y - 1;
        Fun_0891cad8(kNext);
    }
}

/// 0x0891ccc6
void TerrainLineSeedCallback::Fun_0891ccc6(FGXInt2 kPos) {
    kPos.x = (m_kMapSize.x * 100 + kPos.x) % m_kMapSize.x;
    kPos.y = (m_kMapSize.y * 100 + kPos.y) % m_kMapSize.y;
    if (kPos.x < m_kMin.x || kPos.x >= m_kMax.x || kPos.y < m_kMin.y || kPos.y >= m_kMax.y) {
        return;
    }
    if (m_pTexels->m_aTexels[kPos.y % 64][kPos.x % 64].m_ucClass == 4) {
        m_pFloodFill->Fun_0891cad8(kPos);
    }
}

/// 0x0a30fcd0
unsigned int CellBlitJob::ms_BlendCellSize = 128;

extern "C" {
/// 0x0a30fcd4
unsigned int g_mTextureMask = 0x00ff0000;
/// 0x0a30fcd8
unsigned int g_aTTPostfix[13] = {2, 39, 38, 40, 48, 2, 39, 38, 40, 48, 16, 0, 0};
/// 0x0a30fd0c
unsigned int g_aContinentPrefix[5] = {0x00100000, 0x00110000, 0x00120000, 0x00130000, 0x00140000};
/// 0x0a30fd20
unsigned int g_pHexColorCodes[31] = {0x0060f030, 0x00e0f030, 0x00d07040, 0x00b0a000, 0x00ffffff, 0x0040c020, 0x00b0c020, 0x00b05030, 0x00807000, 0x00a0a0a0, 0x0090a0b0, 0x00ffffff, 0x00ffffff, 0x00f0c000, 0x00202080, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff, 0x00ffffff};
/// 0x0a30fd9c
char const* g_pBaseTypeNames[31] = {
    "Grass",
    "Plains",
    "Desert",
    "Tundra",
    "Snow",
    "Grass Hill",
    "Plains Hill",
    "Desert Hill",
    "Tundra Hill",
    "Snow Hill",
    "Mountain",
    "Wonder_Kilimanjaro",
    "Wonder_Lake_Victoria",
    "Coast",
    "Ocean",
    "Wonder_Geyser",
    "Wonder_Crater",
    "Wonder_Potosi",
    "Wonder_Fuji",
    "Wonder_Mesa",
    "Wonder_Reef",
    "Wonder_Krakatoa",
    "Wonder_Uluru",
    "Wonder_Mt_Sinai",
    "Wonder_Mt_Kailash",
    "Wonder_Sri_Pada",
    "Wonder_Gibraltar",
    "Empty Water Wonder 1",
    "Generic",
    "Generic Land",
    "Generic Water",
};
/// 0x0a30fe18
char const* g_pContinentNames[5] = {
    "Generic",
    "America",
    "Asia",
    "Africa",
    "Europe",
};
/// 0x0a30fe2c
char const* g_pTerrainPatternClassNames[5] = {
    "Sys_Val",
    "Low",
    "Medium",
    "High",
    "Wonder",
};
/// 0x0a30ff78
char const* TEXTURE_PATHS[6] = {
    "assets/Overlays/arrow.dds",
    "assets/Overlays/arrow_food.dds",
    "assets/Overlays/arrow_prod.dds",
    "assets/Overlays/arrow_popup.dds",
    "assets/Overlays/arrow_popup_food.dds",
    "assets/Overlays/arrow_popup_prod.dds",
};
/// 0x0a30ff90
char const* g_pDynamicGeometryLibFilename[1] = {
    "DynamicGeometryLib.fxsb",
};
}
