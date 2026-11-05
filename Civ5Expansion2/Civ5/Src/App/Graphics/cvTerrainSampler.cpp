/// origin binary
#include <math.h>
#include <string.h>
#include <Windows.h>
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/Graphics/NormalGenerationBlockJob.h"
#include "FirePlace/FireWorks/FStringTokenizer.h"

namespace {

const float kHeightToUnit = 1.0f / 255.0f;
const float kNegativeZero = -0.0f;
const float kExpScale = -3.3333333f;

// CalcNormal weighs each 5x5 tap by its distance from the height predicted for it.
const float kRangeScale = 0.0078125f;
const float kWeightNorm = 1.0300645f;
const float kSmoothKernel[5] = {0.0228f, 0.2293f, 0.4948f, 0.2293f, 0.0228f};
const float kDerivativeKernel[5] = {0.0702f, 0.3526f, 0.0f, -0.3526f, -0.0702f};

const float kBlockPlaneStep = -2.0f;
const float kBlockRangeScaleSq = 1.0f / 16384.0f;
const float kBlockWeightNorm = 1.03f;

// Side of the height grid: the block, a 2-cell border each side, and the padding of the last 4-wide group.
const int kBlockGrid69 = 76;
const int kBlockGrid12 = 16;

// Blocks of the paged grids: 16x16 cells for the height and terrain grids, 64x64 for the normal grid.
const int kGridBlockSize = 16;
const int kNormalBlockSize = 64;
// Normal job scratch: one 65x65 grid of vectors per job thread.
const unsigned int kNormalScratchVectors = 65 * 65;
const char kNormalJobTag[] = "Port/Civ5/Src/App/Graphics/cvTerrainSampler.cpp";

// Tap lines of the block filters: the centre line is not sampled, the corner taps are cut.
const int kTapLine[4] = {-2, -1, 1, 2};
const int kTapLineAdvance[4] = {0, 1, 2, 1};
const int kTapLineSpan[4] = {1, 2, 2, 1};

}  // namespace

/// 0x08676154
void cvTerrainSampler::Startup(unsigned int uiCount) {
    field_9c = uiCount;
    if (field_30 < uiCount) {
        Fun_0867a6ae(unk_28, uiCount);
    }
    if (field_70 < uiCount) {
        Fun_0867a388(unk_68, uiCount);
    }
}

/// 0x086761a4
void cvTerrainSampler::Shutdown() {
}

/// 0x086761a6
void cvTerrainSampler::StartupGame(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const* pGameGrid, FGXInt2 kHeightMapSize, FGXVector2 vWorldSize, FGXVector3 vWorldCenter, FGXVector3 vTerrainScale, bool a6, float a7) {
    *reinterpret_cast<HEXMatrix_i*>(unk_c8) = m_kTransform.matrix();
    *reinterpret_cast<HEXMatrix_i*>(unk_108) = m_kTransform.inv_matrix();

    m_iSizeX = kHeightMapSize.x;
    m_iSizeY = kHeightMapSize.y;
    field_8 = a6;
    field_0 = a7;
    field_c0 = vWorldSize.x;
    field_c4 = vWorldSize.y;
    field_bc = vWorldCenter.z;
    m_fOffsetX = vWorldCenter.x;
    m_fOffsetY = vWorldCenter.y;
    m_fHeightScale = vTerrainScale.z;
    m_fScaleX = vTerrainScale.x;
    m_fScaleY = vTerrainScale.y;

    // The first three fields of the padded game grid's transform (DynamicHexGrid::m_kTransform at +0x80).
    const HEXTransform_i* pPaddedTransform = reinterpret_cast<const HEXTransform_i*>(WorldGlobals::g_PaddedGameGrid + 0x80);
    m_kTransform.field_0 = pPaddedTransform->field_0;
    m_kTransform.field_4 = pPaddedTransform->field_4;
    m_kTransform.field_8 = pPaddedTransform->field_8;
    *reinterpret_cast<HEXMatrix_i*>(unk_c8) = m_kTransform.matrix();
    *reinterpret_cast<HEXMatrix_i*>(unk_108) = m_kTransform.inv_matrix();

    SetContinentTypes(*pGameGrid);

    const int iBlocksX = kHeightMapSize.x / kGridBlockSize;
    const int iBlocksY = kHeightMapSize.y / kGridBlockSize;
    const unsigned int nBlocks = iBlocksY * iBlocksX;
    const float fWidth = static_cast<float>(kHeightMapSize.x);
    const float fHeight = static_cast<float>(kHeightMapSize.y);

    const unsigned short usHeightFill = 0;
    Fun_0867a5c8(&m_iHeightMapWidth, &kHeightMapSize, &usHeightFill, nBlocks);
    field_34 = 0.0f;
    field_38 = 0.0f;
    field_3c = fWidth;
    field_40 = fHeight;
    field_44 = static_cast<float>(m_iHeightMapWidth - 1) / fWidth;
    field_48 = static_cast<float>(m_iHeightMapHeight - 1) / fHeight;

    unsigned char aPage[0x200];
    memset(aPage, 0, 0x200);
    if (kHeightMapSize.y >= kGridBlockSize) {
        for (int iBlockY = 0; iBlockY < iBlocksY; ++iBlockY) {
            if (kHeightMapSize.x >= kGridBlockSize) {
                for (int iBlockX = 0; iBlockX < iBlocksX; ++iBlockX) {
                    const FGXInt2 kBlock = {iBlockX, iBlockY};
                    Fun_086799fe(&m_iHeightMapWidth, aPage, &kBlock);
                }
            }
        }
    }

    const unsigned char ucTerrainFill = 0;
    Fun_08679ac0(unk_4c, &kHeightMapSize, &ucTerrainFill, nBlocks);
    memset(aPage, 0, 0x100);
    if (kHeightMapSize.y >= kGridBlockSize) {
        for (int iBlockY = 0; iBlockY < iBlocksY; ++iBlockY) {
            if (kHeightMapSize.x >= kGridBlockSize) {
                for (int iBlockX = 0; iBlockX < iBlocksX; ++iBlockX) {
                    const FGXInt2 kBlock = {iBlockX, iBlockY};
                    Fun_08679ba4(unk_4c, aPage, &kBlock);
                }
            }
        }
    }
    m_bGameStarted = true;
}

/// 0x08676548
void cvTerrainSampler::SetContinentTypes(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& a1) {
    for (int x = 0; x < a1.m_iWidth; ++x) {
        for (int y = 0; y < a1.m_iHeight; ++y) {
            m_aucContinentType[y * 255 + x] = a1.m_pCells[a1.m_iHeight * x + y].m_ucContinent;
        }
    }
}

/// 0x086765b4
void cvTerrainSampler::ShutdownGame() {
    m_bGameStarted = false;
    Fun_08679c66(unk_74);
    Fun_08679cb8(unk_4c);
    Fun_0867a198(&m_iHeightMapWidth);
}

/// 0x086765f2
FGXInt2 cvTerrainSampler::GetWrappedPosition(FGXInt2 pos) const {
    if (WorldGlobals::g_eWrapMode == 1) {
        pos.x = (m_iSizeX * 100 + pos.x) % m_iSizeX;
    }
    return pos;
}

/// 0x08676634
FGXVector4 cvTerrainSampler::GetWorldLocation(HEXVec const& hex) const {
    FGXVector4 location;
    D3DXVec4Transform(reinterpret_cast<D3DXVECTOR4*>(&location), reinterpret_cast<const D3DXVECTOR4*>(&hex),
                      reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));
    location.x = location.x * m_fScaleX - m_fOffsetX;
    location.y = location.y * m_fScaleY - m_fOffsetY;
    FGXInt2 cell;
    cell.x = (int)((location.x + m_fOffsetX) / m_fScaleX);
    cell.y = (int)((location.y + m_fOffsetY) / m_fScaleY);
    location.z = GetWorldHeight(cell);
    location.w = 1.0f;
    return location;
}

/// 0x086766ee
float cvTerrainSampler::GetWorldHeight(FGXVector2 const& pos) const {
    FGXInt2 cell;
    cell.x = (int)((pos.x + m_fOffsetX) / m_fScaleX);
    cell.y = (int)((pos.y + m_fOffsetY) / m_fScaleY);
    return GetWorldHeight(cell);
}

/// 0x08676754
FGXVector2 cvTerrainSampler::GetWorldLocation2D(HEXVec const& hex) const {
    float texX = hex.z * WorldGlobals::g_HexToTex[8] + (hex.y * WorldGlobals::g_HexToTex[4] + WorldGlobals::g_HexToTex[0] * hex.x) + WorldGlobals::g_HexToTex[12];
    float texY = hex.z * WorldGlobals::g_HexToTex[9] + (hex.y * WorldGlobals::g_HexToTex[5] + hex.x * WorldGlobals::g_HexToTex[1]) + WorldGlobals::g_HexToTex[13];
    FGXVector2 world;
    world.x = texX * m_fScaleX - m_fOffsetX;
    world.y = texY * m_fScaleY - m_fOffsetY;
    return world;
}

/// 0x086767f2
FGXVector4 cvTerrainSampler::GetHexLocation2D(FGXVector2 const& pos) const {
    D3DXVECTOR4 tex;
    tex.x = (pos.x + m_fOffsetX) / m_fScaleX;
    tex.y = (pos.y + m_fOffsetY) / m_fScaleY;
    tex.z = 0.0f;
    tex.w = 1.0f;
    FGXVector4 hex = {0.0f, 0.0f, 0.0f, 1.0f};
    D3DXVec4Transform(reinterpret_cast<D3DXVECTOR4*>(&hex), &tex, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_TexToHex));
    return hex;
}

/// 0x08676890
FGXVector2 cvTerrainSampler::TexToWorld(FGXVector2 const& tex) const {
    FGXVector2 world;
    world.x = tex.x * m_fScaleX - m_fOffsetX;
    world.y = tex.y * m_fScaleY - m_fOffsetY;
    return world;
}

/// 0x086768d2
bool cvTerrainSampler::IsValidWorldLocation(FGXVector2 const& pos) const {
    int cellX = (int)((pos.x + m_fOffsetX) / m_fScaleX);
    if (cellX >= m_iHeightMapWidth || cellX < 0) {
        return false;
    }
    int cellY = (int)((pos.y + m_fOffsetY) / m_fScaleY);
    if (cellY < 0) {
        return false;
    }
    return cellY < m_iHeightMapHeight;
}

/// 0x08676926
float cvTerrainSampler::GetWorldHeight(FGXInt2 const& cell) const {
    FGXInt2 wrapped = cell;
    if (WorldGlobals::g_eWrapMode == 1) {
        wrapped.x = (m_iSizeX * 100 + wrapped.x) % m_iSizeX;
    }
    float height = 0.0f;
    if (wrapped.x >= 0 && wrapped.x < m_iHeightMapWidth && wrapped.y >= 0 && wrapped.y < m_iHeightMapHeight) {
        unsigned short raw = Fun_08679d0a(const_cast<int*>(&m_iHeightMapWidth), &wrapped);
        height = (float)(int)raw / 255.0f * m_fHeightScale;
    }
    return height;
}

/// 0x086769ba
FGXVector3 cvTerrainSampler::GetWorldNormal(FGXVector2 const& pos) const {
    FGXInt2 cell;
    cell.x = (int)((pos.x + m_fOffsetX) / m_fScaleX);
    cell.y = (int)((pos.y + m_fOffsetY) / m_fScaleY);
    if (m_iNormalFieldReady == 0) {
        return CalcNormal(cell);
    }
    FGXInt2 wrapped = cell;
    if (WorldGlobals::g_eWrapMode == 1) {
        wrapped.x = (m_iSizeX * 100 + wrapped.x) % m_iSizeX;
    }
    unsigned char packed[2];
    Fun_08679d58(packed, const_cast<char*>(unk_74), &wrapped);
    FGXVector3 normal;
    normal.x = (float)((int)packed[0] - 0x80) / 255.0f;
    normal.y = (float)((int)packed[1] - 0x80) / 255.0f;
    normal.z = sqrtf((1.0f - normal.x * normal.x) - normal.y * normal.y);
    return normal;
}

/// 0x08676ae0
FGXVector3 cvTerrainSampler::CalcNormal(FGXInt2 const& cell) const {
    FGXInt2 centerCell = cell;
    if (WorldGlobals::g_eWrapMode == 1) {
        centerCell.x = (m_iSizeX * 100 + cell.x) % m_iSizeX;
    }
    unsigned short centerRaw = Fun_08679d0a(const_cast<int*>(&m_iHeightMapWidth), &centerCell);

    // Taps outside the map keep the center height in raw and are left unset in height.
    unsigned short raw[5][5];
    float height[5][5];
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            raw[i][j] = centerRaw;
            FGXInt2 tap;
            tap.x = cell.x - 2 + i;
            tap.y = cell.y - 2 + j;
            if (WorldGlobals::g_eWrapMode == 1) {
                tap.x = (m_iSizeX * 100 + tap.x) % m_iSizeX;
            }
            if ((tap.x | tap.y) >= 0 && tap.x < m_iSizeX && tap.y < m_iSizeY) {
                raw[i][j] = Fun_08679d0a(const_cast<int*>(&m_iHeightMapWidth), &tap);
                height[i][j] = (float)raw[i][j] * kHeightToUnit;
            }
        }
    }

    const float center = (float)centerRaw * kHeightToUnit;
    const float planeX = (float)(((int)raw[3][2] - (int)raw[1][2]) >> 1) * kHeightToUnit + center;
    const float planeY = (float)(((int)raw[2][3] - (int)raw[2][1]) >> 1) * kHeightToUnit + center;

    float sumX = 0.0f;
    float sumY = 0.0f;
    float weightX = 0.0f;
    float weightY = 0.0f;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 5; ++j) {
            const float h = height[i][j];

            float ux = (planeX - h) * kRangeScale;
            float uy = (planeY - h) * kRangeScale;
            float tx = ux * kExpScale * ux;
            float ty = uy * kExpScale * uy;
            float txx = tx * tx;
            float tyy = ty * ty;
            float wx = (tx * txx / 6.0f + (txx * 0.5f + (tx + 1.0f))) * kWeightNorm * kDerivativeKernel[i] * kSmoothKernel[j];
            float wy = (ty * tyy / 6.0f + (tyy * 0.5f + (ty + 1.0f))) * kWeightNorm * kSmoothKernel[i] * kDerivativeKernel[j];

            sumX = h * wx + sumX;
            sumY = h * wy + sumY;
            weightX = fabsf(wx) + weightX;
            weightY = fabsf(wy) + weightY;
        }
    }

    const float slopeX = (sumX / weightX) * m_fHeightScale;
    const float slopeY = (sumY / weightY) * m_fHeightScale;
    FGXVector3 normal;
    normal.x = slopeX * m_fScaleY + slopeY * kNegativeZero;
    normal.y = slopeX * kNegativeZero + m_fScaleX * slopeY;
    normal.z = m_fScaleX * m_fScaleY;
    D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&normal), reinterpret_cast<const D3DXVECTOR3*>(&normal));
    return normal;
}

/// 0x08676f0e
FGXVector3 cvTerrainSampler::GetWorldNormal(FGXInt2 const& cell) const {
    if (m_iNormalFieldReady == 0) {
        return CalcNormal(cell);
    }
    FGXInt2 wrapped = cell;
    if (WorldGlobals::g_eWrapMode == 1) {
        wrapped.x = (m_iSizeX * 100 + wrapped.x) % m_iSizeX;
    }
    unsigned char packed[2];
    Fun_08679d58(packed, const_cast<char*>(unk_74), &wrapped);
    FGXVector3 normal;
    normal.x = (float)((int)packed[0] - 0x80) / 255.0f;
    normal.y = (float)((int)packed[1] - 0x80) / 255.0f;
    normal.z = sqrtf((1.0f - normal.x * normal.x) - normal.y * normal.y);
    return normal;
}

/// 0x08676ff4
void cvTerrainSampler::GenerateNormalField(bool a1, float a2) {
    m_iNormalFieldReady = 1;
    field_8 = a1;
    field_0 = a2;

    const int iBlocksX = m_iSizeX / kNormalBlockSize;
    const int iBlocksY = m_iSizeY / kNormalBlockSize;
    const int nBlocks = iBlocksY * iBlocksX;
    if (m_uNormalBlocksCapacity < static_cast<unsigned int>(nBlocks / 2)) {
        Fun_0867a0f6(&m_pNormalBlocks, nBlocks / 2);
    }
    const unsigned short usFill = 0;
    Fun_08679db0(unk_74, reinterpret_cast<const FGXInt2*>(&m_iSizeX), &usFill, 0xffffffff);

    FJobSet kJobSet;
    NormalGenerationBlockJob* pJobs = new NormalGenerationBlockJob[nBlocks];
    const unsigned int nThreads = JobManager_GetNumberThreads();
    NormalGenerationBlockJob::pThreadLocalStorage = new FGXVector3[nThreads * kNormalScratchVectors]();

    unsigned char aPage[0x2000];
    if (m_iSizeY >= kNormalBlockSize) {
        int nJobs = 0;
        for (int iBlockY = 0; iBlockY < iBlocksY; ++iBlockY) {
            if (m_iSizeX >= kNormalBlockSize) {
                for (int iBlockX = 0; iBlockX < iBlocksX; ++iBlockX) {
                    const FGXInt2 kBlock = {iBlockX, iBlockY};
                    Fun_08679e96(unk_74, aPage, &kBlock);
                    pJobs[nJobs].m_iBlockX = iBlockX;
                    pJobs[nJobs].m_iBlockY = iBlockY;
                    pJobs[nJobs].m_pSampler = this;
                    ++nJobs;
                }
            }
        }
        for (int iJob = 0; iJob < nJobs; ++iJob) {
            NormalGenerationBlockJob& kJob = pJobs[iJob];
            const unsigned int uStorage = m_pNormalBlockTable[(m_iNormalBlocksX * kJob.m_iBlockY + kJob.m_iBlockX) * 2 + 1];
            kJob.m_pNormals = reinterpret_cast<unsigned short*>(m_pNormalBlocks + uStorage * sizeof(aPage));
            kJobSet.AddJob(&kJob, kNormalJobTag, nullptr);
        }
    }
    kJobSet.Wait();
    delete[] pJobs;
    if (NormalGenerationBlockJob::pThreadLocalStorage != nullptr) {
        delete[] NormalGenerationBlockJob::pThreadLocalStorage;
    }
}

/// 0x086772d8
void cvTerrainSampler::CalcNormalBlockMax69x69(FGXInt2 const& origin, FGXInt2 size, FGXVector3* pNormals) const {
    float height[kBlockGrid69][kBlockGrid69];
    int rawHeight[kBlockGrid69][kBlockGrid69];

    // Row i, column j hold the cell (origin.x - 2 + i, origin.y - 2 + j), clamped to the map.
    for (int i = 0; i < kBlockGrid69; ++i) {
        int cellX = origin.x - 2 + i;
        if (WorldGlobals::g_eWrapMode == 1) {
            cellX = (m_iSizeX * 100 + cellX) % m_iSizeX;
        }
        if (cellX >= m_iSizeX) {
            cellX = m_iSizeX - 1;
        }
        if (cellX < 0) {
            cellX = 0;
        }
        for (int j = 0; j < kBlockGrid69; ++j) {
            FGXInt2 tap;
            tap.x = cellX;
            tap.y = origin.y - 2 + j;
            if (tap.y > m_iSizeY - 1) {
                tap.y = m_iSizeY - 1;
            }
            if (tap.y < 0) {
                tap.y = 0;
            }
            int raw = Fun_08679d0a(const_cast<int*>(&m_iHeightMapWidth), &tap);
            rawHeight[i][j] = raw;
            height[i][j] = (float)raw * kHeightToUnit;
        }
    }

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            const int r = x + 2;
            const int c = y + 2;
            const float center = height[r][c];
            const int gradX = (rawHeight[r + 1][c] - rawHeight[r - 1][c]) >> 1;
            const int gradY = (rawHeight[r][c + 1] - rawHeight[r][c - 1]) >> 1;

            float planeX = (float)gradX * kHeightToUnit * kBlockPlaneStep + center;
            float sumX = 0.0f;
            float weightX = 0.0f;
            for (int line = 0; line < 4; ++line) {
                for (int step = 0; step < kTapLineAdvance[line]; ++step) {
                    planeX += 1.0f;
                }
                const int dx = kTapLine[line];
                for (int dy = -kTapLineSpan[line]; dy <= kTapLineSpan[line]; ++dy) {
                    const float h = height[r + dx][c + dy];
                    float d = planeX - h;
                    float t = d * d * kExpScale * kBlockRangeScaleSq;
                    float w = (t * t * 0.5f + t + 1.0f) * kBlockWeightNorm * m_afNormalWeightX[(dx + 2) * 5 + dy + 2];
                    sumX = h * w + sumX;
                    weightX = fabsf(w) + weightX;
                }
            }

            float planeY = (float)gradY * kHeightToUnit * kBlockPlaneStep + center;
            float sumY = 0.0f;
            float weightY = 0.0f;
            for (int line = 0; line < 4; ++line) {
                for (int step = 0; step < kTapLineAdvance[line]; ++step) {
                    planeY += 1.0f;
                }
                const int dy = kTapLine[line];
                for (int dx = -kTapLineSpan[line]; dx <= kTapLineSpan[line]; ++dx) {
                    const float h = height[r + dx][c + dy];
                    float d = planeY - h;
                    float t = d * d * kExpScale * kBlockRangeScaleSq;
                    float w = (t * t * 0.5f + t + 1.0f) * kBlockWeightNorm * m_afNormalWeightY[(dx + 2) * 5 + dy + 2];
                    sumY = h * w + sumY;
                    weightY = fabsf(w) + weightY;
                }
            }

            const float slopeX = (sumX / weightX) * m_fHeightScale;
            const float slopeY = m_fHeightScale * (sumY / weightY);
            FGXVector3 normal;
            normal.x = slopeX * m_fScaleY + slopeY * kNegativeZero;
            normal.y = slopeX * kNegativeZero + m_fScaleX * slopeY;
            normal.z = m_fScaleX * m_fScaleY;
            D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&normal), reinterpret_cast<const D3DXVECTOR3*>(&normal));
            pNormals[y * size.x + x] = normal;
        }
    }
}

/// 0x08678670
void cvTerrainSampler::CalcNormalBlockMax12x12(FGXInt2 const& origin, FGXInt2 size, FGXVector3* pNormals) const {
    float height[kBlockGrid12][kBlockGrid12];
    int rawHeight[kBlockGrid12][kBlockGrid12];

    // Row i, column j hold the cell (origin.x - 2 + i, origin.y - 2 + j), clamped to the map.
    for (int i = 0; i < kBlockGrid12; ++i) {
        int cellX = origin.x - 2 + i;
        if (WorldGlobals::g_eWrapMode == 1) {
            cellX = (m_iSizeX * 100 + cellX) % m_iSizeX;
        }
        if (cellX >= m_iSizeX) {
            cellX = m_iSizeX - 1;
        }
        if (cellX < 0) {
            cellX = 0;
        }
        for (int j = 0; j < kBlockGrid12; ++j) {
            FGXInt2 tap;
            tap.x = cellX;
            tap.y = origin.y - 2 + j;
            if (tap.y > m_iSizeY - 1) {
                tap.y = m_iSizeY - 1;
            }
            if (tap.y < 0) {
                tap.y = 0;
            }
            int raw = Fun_08679d0a(const_cast<int*>(&m_iHeightMapWidth), &tap);
            rawHeight[i][j] = raw;
            height[i][j] = (float)raw * kHeightToUnit;
        }
    }

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            const int r = x + 2;
            const int c = y + 2;
            const float center = height[r][c];
            const int gradX = (rawHeight[r + 1][c] - rawHeight[r - 1][c]) >> 1;
            const int gradY = (rawHeight[r][c + 1] - rawHeight[r][c - 1]) >> 1;

            float planeX = (float)gradX * kHeightToUnit * kBlockPlaneStep + center;
            float sumX = 0.0f;
            float weightX = 0.0f;
            for (int line = 0; line < 4; ++line) {
                for (int step = 0; step < kTapLineAdvance[line]; ++step) {
                    planeX += 1.0f;
                }
                const int dx = kTapLine[line];
                for (int dy = -kTapLineSpan[line]; dy <= kTapLineSpan[line]; ++dy) {
                    const float h = height[r + dx][c + dy];
                    float d = planeX - h;
                    float t = d * d * kExpScale * kBlockRangeScaleSq;
                    float w = (t * t * 0.5f + t + 1.0f) * kBlockWeightNorm * m_afNormalWeightX[(dx + 2) * 5 + dy + 2];
                    sumX = h * w + sumX;
                    weightX = fabsf(w) + weightX;
                }
            }

            float planeY = (float)gradY * kHeightToUnit * kBlockPlaneStep + center;
            float sumY = 0.0f;
            float weightY = 0.0f;
            for (int line = 0; line < 4; ++line) {
                for (int step = 0; step < kTapLineAdvance[line]; ++step) {
                    planeY += 1.0f;
                }
                const int dy = kTapLine[line];
                for (int dx = -kTapLineSpan[line]; dx <= kTapLineSpan[line]; ++dx) {
                    const float h = height[r + dx][c + dy];
                    float d = planeY - h;
                    float t = d * d * kExpScale * kBlockRangeScaleSq;
                    float w = (t * t * 0.5f + t + 1.0f) * kBlockWeightNorm * m_afNormalWeightY[(dx + 2) * 5 + dy + 2];
                    sumY = h * w + sumY;
                    weightY = fabsf(w) + weightY;
                }
            }

            const float slopeX = (sumX / weightX) * m_fHeightScale;
            const float slopeY = m_fHeightScale * (sumY / weightY);
            FGXVector3 normal;
            normal.x = slopeX * m_fScaleY + slopeY * kNegativeZero;
            normal.y = slopeX * kNegativeZero + m_fScaleX * slopeY;
            normal.z = m_fScaleX * m_fScaleY;
            D3DXVec3Normalize(reinterpret_cast<D3DXVECTOR3*>(&normal), reinterpret_cast<const D3DXVECTOR3*>(&normal));
            pNormals[y * size.x + x] = normal;
        }
    }
}
