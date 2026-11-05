/// origin binary
#include <float.h>
#include <math.h>
#include <new>
#include <string.h>
#include <Windows.h>
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainCell.h"
#include "Civ5/Src/App/Graphics/TerrainCellBaseIBVBFallbackJob.h"
#include "Civ5/Src/App/Graphics/TerrainCellJob.h"
#include "Civ5/Src/App/Graphics/TerrainCellStatJob.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/UI/UIRenderBuffer.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/core/FImageCoder.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireWorks/FSingleton.h"
#include "FirePlace/FireWorks/FTempHeap.h"

namespace {

const unsigned int kVertexSize = 0x30;
const unsigned int kCompressedVertexSize = 0x18;
const unsigned int kFallbackVertexSize = 0x18;
const int kJobBatchSize = 12;
const char kJobTag[] = "Port/Civ5/Src/App/Graphics/TerrainCell.cpp";
const char kFallbackJobTag[] = "Build Base IB and VB fallback";
const double kLog2E = 1.4426950408889634;

const TerrainCellStatus kStatusCacheOnly = static_cast<TerrainCellStatus>(1);
const TerrainCellStatus kStatusBaseOnly = static_cast<TerrainCellStatus>(4);
const int kBasePoolSlots = 0x600;

// Lowest set bit 1..4 of a cell's class mask, 0 when none is set.
const auto ClassFromMask = [](unsigned int uClassMask) -> int {
    unsigned int uBit = 1;
    do {
        if ((uClassMask >> uBit) & 1) {
            break;
        }
        ++uBit;
    } while (uBit < 5);
    return uBit != 5 ? static_cast<int>(uBit) : 0;
};
}  // namespace

namespace {

// Vertex of the cell's composite buffer (VStruct<21>), stride 0x30.
struct CompositeVertex {
    float fX;                          // +0x0
    float fY;                          // +0x4
    float fZ;                          // +0x8
    float fHexX;                       // +0xc
    float fHexY;                       // +0x10
    float fWorldX;                     // +0x14
    float fWorldY;                     // +0x18
    float fWorldZ;                     // +0x1c
    unsigned char aWeights[8];         // +0x20
    unsigned char aTextures[8];        // +0x28
};

// Vertex of the cell's compressed buffer (VStruct<16>), stride 0x18.
struct CompressedVertex {
    float fX;                          // +0x0
    float fY;                          // +0x4
    float fZ;                          // +0x8
    unsigned int uNormalXY;            // +0xc
    unsigned int uNormalZW;            // +0x10
    unsigned int uTexCoord;            // +0x14
};

// Vertex of the fallback base mesh, stride 0x18.
struct FallbackVertex {
    float fX;                          // +0x0
    float fY;                          // +0x4
    float fZ;                          // +0x8
    unsigned int field_c;              // +0xc
    unsigned int field_10;             // +0x10
    unsigned int uTexCoord;            // +0x14
};

static_assert(sizeof(CompositeVertex) == 0x30, "composite vertex stride");
static_assert(sizeof(CompressedVertex) == 0x18, "compressed vertex stride");
static_assert(sizeof(FallbackVertex) == 0x18, "fallback vertex stride");

const int kBlendBlockSize = 12;
const int kBlendTextures = 8;
const int kFallbackBlockQuads = 6;
const float kFlatNormalZ = 0.9f;
const float kHeightScale = 1.0f / 255.0f;
const float kWeightScale = 255.0f;
const double kMinWeightSum = 0.001;

// Two floats as halves in one dword (D3DXFloat32To16Array per value), low first.
const auto PackHalves = [](float fLow, float fHigh) -> unsigned int {
    D3DXFLOAT16 kLow, kHigh;
    D3DXFloat32To16Array(&kLow, &fLow, 1);
    D3DXFloat32To16Array(&kHigh, &fHigh, 1);
    return static_cast<unsigned int>(kLow.value) | (static_cast<unsigned int>(kHigh.value) << 16);
};
}  // namespace

/// 0x08911a68
void TerrainCell::DeleteSurfacesAndBuffers(bool bBuffers, bool bCacheTextures, bool bRenderTextures) {
    if (bRenderTextures) {
        m_pTerrain->ReleaseRenderTexture(m_uUncompressedTex1);
        m_pTerrain->ReleaseRenderTexture(m_uUncompressedTex0);
        m_uUncompressedTex0 = 0;
        m_uUncompressedTex1 = 0;
    }
    field_188 = 0;
    field_184 = 0;
    field_18c = 0;
    if (bCacheTextures) {
        if (GraphicsSettings::TerrainSettings::g_bEnableDXTCompression == 0) {
            m_pTerrain->ReleaseRenderTexture(m_uCacheTex0);
            m_pTerrain->ReleaseRenderTexture(m_uCacheTex1);
        } else {
            m_pTerrain->ReleaseBCTexture(m_uCacheTex1);
            m_pTerrain->ReleaseBCTexture(m_uCacheTex0);
        }
        m_uCacheTex0 = 0;
        m_uCacheTex1 = 0;
    }
    if (bBuffers) {
        RemoveBase();
    }
}

/// 0x08911b76
void TerrainCell::RemoveBase() {
    const unsigned int hVB = m_uBaseVB;
    if (GraphicsSettings::TerrainSettings::g_uTerrainTessLevel == 0) {
        const unsigned int hIB = m_uBaseIB;
        if (hVB != 0) {
            for (int i = 0; i < kBasePoolSlots; ++i) {
                if (s_auBaseVBHandle[i] == hVB) {
                    s_abBaseVBInUse[i] = 0;
                    break;
                }
            }
        }
        if (hIB != 0) {
            for (int i = 0; i < kBasePoolSlots; ++i) {
                if (s_auBaseIBHandle[i] == hIB) {
                    s_abBaseIBInUse[i] = 0;
                    break;
                }
            }
        }
    } else {
        Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, hVB);
        Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, m_uBaseIB);
    }
    Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, m_uBaseIB2);
    FireGrafix::g_pRenderer->ReleaseSurface(m_uSurface0);
    FireGrafix::g_pRenderer->ReleaseSurface(m_uSurface1);
    FireGrafix::g_pRenderer->ReleaseSurface(m_uTessSurface0);
    FireGrafix::g_pRenderer->ReleaseSurface(m_uTessSurface1);
    m_uBaseIB = 0;
    m_uBaseVB = 0;
    m_uBaseIB2 = 0;
    m_uSurface1 = 0;
    m_uSurface0 = 0;
    m_uTessSurface1 = 0;
    m_uTessSurface0 = 0;
}

/// 0x08911d5a
void TerrainCell::BuildTextureVertexBatch(int iXBegin, int iXEnd, int iYBegin, int iYEnd, FireGrafix::VStruct<16>* a5) {
    const int iWidth = m_iWidth;
    const int iHeight = m_iHeight;

    FGXVector3 aNormals[(kBlendBlockSize + 1) * (kBlendBlockSize + 1)];
    for (int i = 0; i < (kBlendBlockSize + 1) * (kBlendBlockSize + 1); ++i) {
        aNormals[i].x = 0.0f;
        aNormals[i].y = 0.0f;
        aNormals[i].z = 0.0f;
    }
    const FGXInt2 kBlockOrigin = {m_iX + iXBegin, m_iY + iYBegin};
    const int iBlockWidth = iXEnd - iXBegin;
    const FGXInt2 kBlockSize = {iBlockWidth, iYEnd - iYBegin};
    m_pShared->m_kExternalData.m_pTerrainSampler->CalcNormalBlockMax12x12(kBlockOrigin, kBlockSize, aNormals);
    if (iBlockWidth <= 0) {
        return;
    }

    const float fWidthDivisor = static_cast<float>(static_cast<unsigned int>(iWidth + 1));
    const float fHeightDivisor = static_cast<float>(static_cast<unsigned int>(iHeight + 1));
    for (int iX = iXBegin; iX < iXEnd; ++iX) {
        if (iYBegin >= iYEnd) {
            continue;
        }
        const float fU = static_cast<float>(iX + 1) / fWidthDivisor;
        const float fClipX = fU + fU + -1.0f;
        for (int iY = iYBegin; iY < iYEnd; ++iY) {
            const int iHexX = m_iX + iX;
            const int iHexY = m_iY + iY;
            Terrain::SharedData* pShared = m_pShared;
            const FGXInt2 kWrappedHex = {
                static_cast<int>(static_cast<unsigned int>(pShared->m_iHeightMapWidth + iHexX) %
                                 static_cast<unsigned int>(pShared->m_iHeightMapWidth)),
                static_cast<int>(static_cast<unsigned int>(pShared->m_iHeightMapHeight + iHexY) %
                                 static_cast<unsigned int>(pShared->m_iHeightMapHeight))};

            float fNormalX;
            float fNormalY;
            float fNormalZ;
            if (static_cast<unsigned int>(iHexX - kBlockOrigin.x) < static_cast<unsigned int>(kBlendBlockSize + 1) &&
                static_cast<unsigned int>(iHexY - kBlockOrigin.y) < static_cast<unsigned int>(kBlendBlockSize + 1)) {
                const FGXVector3& kNormal = aNormals[(iHexY - kBlockOrigin.y) * iBlockWidth + (iHexX - kBlockOrigin.x)];
                fNormalX = kNormal.x;
                fNormalY = kNormal.y;
                fNormalZ = kNormal.z;
            } else {
                const FGXVector3 kNormal = pShared->m_kExternalData.m_pTerrainSampler->GetWorldNormal(kWrappedHex);
                fNormalX = kNormal.x;
                fNormalY = kNormal.y;
                fNormalZ = kNormal.z;
            }

            int iProjectionMode = 0;
            if (!(fNormalZ > kFlatNormalZ)) {
                iProjectionMode = (fabsf(fNormalY) <= fabsf(fNormalX) ? 1 : 0) + 1;
            }

            cvTerrainSampler* pSampler = m_pShared->m_kExternalData.m_pTerrainSampler;
            const FGXInt2 kWrapped = pSampler->GetWrappedPosition(kWrappedHex);
            const unsigned short usHeight = Fun_08679d0a(&pSampler->m_iHeightMapWidth, &kWrapped);
            pShared = m_pShared;
            const float fWorldZ = static_cast<float>(usHeight) * kHeightScale * pShared->m_vTerrainScale.z;

            const float fV = static_cast<float>(iY + 1) / fHeightDivisor;
            const int iVertex = (iY + 1) * (iWidth + 2) + (iX + 1);
            const float fTexel = 1.0f / static_cast<float>(static_cast<unsigned int>(field_17c));
            const float fClipY = -2.0f * fV + 1.0f;

            CompressedVertex* pCompressed = reinterpret_cast<CompressedVertex*>(m_pCompressedVertices) + iVertex;
            pCompressed->fX = fClipX - fTexel;
            pCompressed->fY = fTexel + fClipY;
            pCompressed->fZ = 0.5f;
            pCompressed->uNormalXY = PackHalves(fNormalX, fNormalY);
            pCompressed->uNormalZW = PackHalves(fNormalZ, 1.0f);
            pCompressed->uTexCoord = PackHalves(fU, fV);

            CompositeVertex* pComposite = reinterpret_cast<CompositeVertex*>(m_pCompositeVertices) + iVertex;
            const float fHexX = static_cast<float>(iHexX);
            const float fHexY = static_cast<float>(iHexY);
            pComposite->fX = fClipX;
            pComposite->fY = fClipY;
            pComposite->fZ = 0.5f;
            pComposite->fHexX = fHexX;
            pComposite->fHexY = fHexY;
            pComposite->fWorldX = pShared->m_vTerrainScale.x * fHexX;
            pComposite->fWorldY = pShared->m_vTerrainScale.y * fHexY;
            pComposite->fWorldZ = fWorldZ;

            if (GraphicsSettings::TerrainSettings::g_bUseGPUWeightBlending == 0) {
                float afBlend[64];
                memset(afBlend, 0, sizeof(afBlend));
                const FGXInt2 kBlendHex = {m_iX + iX, iY + m_iY};
                CalcBlendVector(afBlend, static_cast<TextureProjectionMode>(iProjectionMode), kBlendHex);

                unsigned int auTextureIds[kBlendTextures] = {};
                float afWeights[kBlendTextures] = {};
                const unsigned int nTextures = m_nTextures;
                if (nTextures == 0) {
                    afWeights[0] = 1.0f;
                } else {
                    memcpy(auTextureIds, m_aTextureIds, nTextures * sizeof(unsigned int));
                    float fWeightSum = 0.0f;
                    for (unsigned int i = 0; i < nTextures; ++i) {
                        afWeights[i] = afBlend[m_aTextureIds[i]];
                        fWeightSum += afWeights[i];
                    }
                    if (static_cast<double>(fWeightSum) < kMinWeightSum) {
                        afWeights[0] = 1.0f;
                    }
                }
                for (int i = 0; i < kBlendTextures; ++i) {
                    pComposite->aWeights[i] = static_cast<unsigned char>(static_cast<unsigned int>(afWeights[i] * kWeightScale));
                }
                for (int i = 0; i < kBlendTextures; ++i) {
                    pComposite->aTextures[i] = static_cast<unsigned char>(auTextureIds[i]);
                }
            } else {
                pComposite->aTextures[0] = static_cast<unsigned char>(iProjectionMode);
            }
        }
    }
}

/// 0x089126ec
void TerrainCell::BuildBaseVBandIBFallback(unsigned int a1) {
    const int iStep = m_pShared->m_iTessLevel;
    const int nCols = (m_iWidth - 2 + iStep) / iStep;
    const int nRows = (m_iHeight - 2 + iStep) / iStep;
    const int nVertexCols = nCols + 1;
    const int nVertexRows = nRows + 1;
    m_nBaseVertices = nVertexRows * nVertexCols;

    int nTriangles = 0;
    if (nCols >= 0) {
        const float fWidthDivisor = static_cast<float>(static_cast<unsigned int>(m_iWidth + 1));
        const float fHeightDivisor = static_cast<float>(static_cast<unsigned int>(m_iHeight + 1));
        int iColPos = 0;
        for (int iCol = 0; iCol < nVertexCols; ++iCol) {
            if (iColPos > m_iWidth - 1) {
                iColPos = m_iWidth - 1;
            }
            if (nRows >= 0) {
                int iRowPos = 0;
                for (int iRow = 0; iRow < nVertexRows; ++iRow) {
                    if (iRowPos > m_iHeight - 1) {
                        iRowPos = m_iHeight - 1;
                    }
                    const int iHexX = m_iX + iColPos;
                    const int iHexY = m_iY + iRowPos;
                    cvTerrainSampler* pSampler = m_pShared->m_kExternalData.m_pTerrainSampler;
                    const FGXInt2 kPos = {iHexX, iHexY};
                    const FGXInt2 kWrapped = pSampler->GetWrappedPosition(kPos);
                    const unsigned short usHeight = Fun_08679d0a(&pSampler->m_iHeightMapWidth, &kWrapped);
                    Terrain::SharedData* pShared = m_pShared;
                    FallbackVertex* pVertex = reinterpret_cast<FallbackVertex*>(m_pFallbackVertices) + (iRow * nVertexCols + iCol);
                    pVertex->fX = static_cast<float>(iHexX) * pShared->m_vTerrainScale.x;
                    pVertex->fY = static_cast<float>(iHexY) * pShared->m_vTerrainScale.y;
                    pVertex->fZ = static_cast<float>(usHeight) * kHeightScale * pShared->m_vTerrainScale.z;
                    pVertex->uTexCoord = PackHalves(static_cast<float>(iColPos + 1) / fWidthDivisor,
                                                    static_cast<float>(iRowPos + 1) / fHeightDivisor);
                    iRowPos += iStep;
                }
            }
            iColPos += iStep;
        }

        if (nCols > 0) {
            unsigned short* pIndices = reinterpret_cast<unsigned short*>(m_pFallbackIndices);
            // Quads go out in blocks kFallbackBlockQuads wide, row by row inside a block.
            for (int iBlockX = 0; iBlockX < nCols; iBlockX += kFallbackBlockQuads * iStep) {
                for (int iY0 = 0; iY0 < nRows; iY0 += iStep) {
                    const int iY1 = iY0 + iStep <= nRows ? iY0 + iStep : nRows;
                    for (int iQuad = 0; iQuad < kFallbackBlockQuads; ++iQuad) {
                        const int iX0 = iBlockX + iQuad * iStep;
                        if (iX0 >= nCols) {
                            break;
                        }
                        const int iX1 = iX0 + iStep <= nCols ? iX0 + iStep : nCols;
                        const unsigned short usNear0 = static_cast<unsigned short>(iY0 * nVertexCols + iX0);
                        const unsigned short usNear1 = static_cast<unsigned short>(iY0 * nVertexCols + iX1);
                        const unsigned short usFar0 = static_cast<unsigned short>(iY1 * nVertexCols + iX0);
                        const unsigned short usFar1 = static_cast<unsigned short>(iY1 * nVertexCols + iX1);
                        pIndices[0] = usFar1;
                        pIndices[1] = usNear1;
                        if (((iX0 + 1) & 1) == 0) {
                            pIndices[2] = usNear0;
                            pIndices[3] = usFar0;
                            pIndices[4] = usFar1;
                            pIndices[5] = usNear0;
                        } else {
                            pIndices[2] = usFar0;
                            pIndices[3] = usFar0;
                            pIndices[4] = usNear1;
                            pIndices[5] = usNear0;
                        }
                        pIndices += 6;
                        nTriangles += 2;
                    }
                }
            }
        }
    }
    m_nBaseTriangles = nTriangles;
}

/// 0x08912bba
void TerrainCell::CalcBlendVector(float* afBlend, TextureProjectionMode eMode, FGXInt2 const& kHex) const {
    memset(afBlend, 0, m_pShared->m_nProjections * sizeof(float));
    const int iMode = static_cast<int>(eMode);
    Terrain::SharedData* pShared = m_pShared;
    for (unsigned int iWeight = 0; iWeight < static_cast<unsigned int>(pShared->m_kBlendWeights.m_nWeights); ++iWeight) {
        cvTerrainSampler* pSampler = pShared->m_kExternalData.m_pTerrainSampler;
        const FGXInt2 kTap = {
            static_cast<int>(static_cast<unsigned int>(pShared->m_kBlendWeights.m_aiOffsetX[iWeight] + kHex.x) %
                             static_cast<unsigned int>(pShared->m_iHeightMapWidth)),
            static_cast<int>(static_cast<unsigned int>(pShared->m_kBlendWeights.m_aiOffsetY[iWeight] + kHex.y) %
                             static_cast<unsigned int>(pShared->m_iHeightMapHeight))};
        const FGXInt2 kWrapped = pSampler->GetWrappedPosition(kTap);
        const unsigned char ucTerrain = Fun_087fb076(pSampler->unk_4c, &kWrapped);
        pShared = m_pShared;

        int iProjection = pShared->m_aiFirstProjection[ucTerrain];
        if (iMode != 0 && static_cast<int>(pShared->m_aeProjectionMode[ucTerrain]) == 1) {
            iProjection += iMode == 2 ? 2 : (iMode == 1 ? 1 : 0);
        }
        afBlend[iProjection] += pShared->m_kBlendWeights.m_afWeight[iWeight];
    }

    pShared = m_pShared;
    const unsigned int nProjections = pShared->m_nProjections;
    if (nProjections != 0) {
        float fSum = 0.0f;
        for (unsigned int i = 0; i < nProjections; ++i) {
            fSum += afBlend[i];
        }
        const float fInverse = 1.0f / fSum;
        for (unsigned int i = 0; i < m_pShared->m_nProjections; ++i) {
            afBlend[i] = afBlend[i] * fInverse;
        }
    }
}

/// 0x08912e0a
void TerrainCell::BuildTextureVBs(FJobSet* pJobSet) {
    const int iWidth = m_iWidth;
    const int iHeight = m_iHeight;
    const int nVertices = (iHeight + 2) * (iWidth + 2);

    m_pJobs = reinterpret_cast<TerrainCellJob*>(GetTempHeap()->Allocate(nVertices * sizeof(TerrainCellJob)));
    m_pCompositeVertices = GetTempHeap()->Allocate(nVertices * kVertexSize);
    m_pCompressedVertices = GetTempHeap()->Allocate(nVertices * kCompressedVertexSize);

    if (m_pCompositeVertices == nullptr || m_pCompressedVertices == nullptr || m_pJobs == nullptr) {
        GetTempHeap()->DeAllocate(reinterpret_cast<unsigned char*>(m_pJobs));
        GetTempHeap()->DeAllocate(m_pCompositeVertices);
        GetTempHeap()->DeAllocate(m_pCompressedVertices);
        m_pCompositeVertices = nullptr;
        m_pCompressedVertices = nullptr;
        m_pJobs = nullptr;
        m_bJobsLaunched = false;
        return;
    }

    for (int i = 0; i < nVertices; ++i) {
        new (&m_pJobs[i]) TerrainCellJob();
    }

    FJobSet localJobs;
    if (iHeight >= -1) {
        int iJob = 0;
        int iRow = -1;
        do {
            const int iRowEnd = iRow + kJobBatchSize;
            if (iWidth >= -1) {
                const int iRowLimit = iRowEnd <= iHeight + 1 ? iRowEnd : iHeight + 1;
                int iColEnd = kJobBatchSize - 1;
                bool bMore;
                do {
                    TerrainCellJob* pJob = &m_pJobs[iJob];
                    const int iColLimit = iColEnd <= iWidth + 1 ? iColEnd : iWidth + 1;
                    pJob->Init(this, reinterpret_cast<FireGrafix::VStruct<16>*>(m_pCompressedVertices), iRow, iRowLimit,
                               iColEnd - kJobBatchSize, iColLimit);
                    pJobSet->AddJob(pJob, kJobTag, nullptr);
                    ++iJob;
                    bMore = iColEnd < iWidth + 1;
                    iColEnd += kJobBatchSize;
                } while (bMore);
            }
            iRow = iRowEnd;
        } while (iRow < iHeight + 1);
    }
}

/// 0x08913044
void TerrainCell::CreateTerrainRenderSurface(unsigned int* puHandle, unsigned int hTexture0, unsigned int hTexture1, int iClass, bool bTessellation, unsigned int a6, unsigned int a7, unsigned int a8) {
    FireGrafix::FGXSurface* pSurface = nullptr;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, puHandle, 0x49);
    if (!bTessellation) {
        pSurface->field_c = m_nBaseTriangles;
        pSurface->m_hIB = m_uBaseIB;
        pSurface->m_hVB = m_uBaseVB;
    }
    pSurface->field_60 = 10;
    pSurface->field_18[0] = static_cast<unsigned short>(hTexture0);
    pSurface->field_38[0] = 5;
    pSurface->field_18[1] = static_cast<unsigned short>(m_pShared->m_kTexHandles.m_ahTextures[0]);
    pSurface->field_38[1] = 5;
    pSurface->field_18[2] = static_cast<unsigned short>(m_pShared->m_kTexHandles.m_ahTextures[4]);
    pSurface->field_38[2] = 5;
    pSurface->field_18[3] = static_cast<unsigned short>(m_pShared->m_kTexHandles.m_ahTextures[6]);
    pSurface->field_38[3] = 0;
    pSurface->field_18[4] = static_cast<unsigned short>(m_pShared->m_kTexHandles.m_ahTextures[2]);
    pSurface->field_38[4] = 0;
    pSurface->field_18[5] = static_cast<unsigned short>(m_pShared->m_kTexHandles.m_ahTextures[3]);
    pSurface->field_38[5] = 0;
    pSurface->field_18[6] = static_cast<unsigned short>(g_ColorKeySystem.m_auKeyHandles[1][iClass]);
    pSurface->field_38[6] = 5;
    pSurface->field_18[7] = static_cast<unsigned short>(g_ColorKeySystem.m_auKeyHandles[2][iClass]);
    pSurface->field_38[7] = 5;
    pSurface->field_28[0] = static_cast<unsigned short>(hTexture1);
    pSurface->field_48[0] = 5;
    pSurface->field_28[1] = static_cast<unsigned short>(m_pShared->m_kTexHandles.m_ahTextures[9]);
    pSurface->field_48[1] = 5;
    if (bTessellation) {
        pSurface->field_61 = 1;
        pSurface->field_58 = static_cast<unsigned short>(field_ec);
        pSurface->field_5c = 0;
    } else if (GraphicsSettings::TerrainSettings::g_uTerrainTessLevel != 0) {
        pSurface->field_61 = 1;
        pSurface->field_58 = static_cast<unsigned short>(field_ec);
        pSurface->field_5c = 9;
    }
}

/// 0x08913238
void TerrainCell::BuildBase() {
    if (GraphicsSettings::TerrainSettings::g_uTerrainTessLevel == 0) {
        const int nVertices = m_nBaseVertices;
        unsigned char* pVertices = m_pFallbackVertices;
        unsigned char* pIndices = m_pFallbackIndices;
        const int nIndices = m_nBaseTriangles * 3;

        int iSlot = 0;
        while (s_auBaseVBHandle[iSlot] != 0 && (s_abBaseVBInUse[iSlot] != 0 || s_aiBaseVBSize[iSlot] < nVertices)) {
            ++iSlot;
        }
        if (s_auBaseVBHandle[iSlot] != 0) {
            m_uBaseVB = s_auBaseVBHandle[iSlot];
            s_abBaseVBInUse[iSlot] = 1;
            FireGrafix::g_pRenderer->LockAndFillStaticVB(m_uBaseVB, pVertices, 0);
        } else {
            m_uBaseVB = FireGrafix::g_pRenderer->CreateAndFillStaticVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(0x10), nVertices, pVertices, 0, 1);
            s_auBaseVBHandle[iSlot] = m_uBaseVB;
            s_abBaseVBInUse[iSlot] = 1;
            s_aiBaseVBSize[iSlot] = nVertices;
        }

        iSlot = 0;
        while (s_auBaseIBHandle[iSlot] != 0 && (s_abBaseIBInUse[iSlot] != 0 || s_aiBaseIBSize[iSlot] < nIndices)) {
            ++iSlot;
        }
        if (s_auBaseIBHandle[iSlot] != 0) {
            m_uBaseIB = s_auBaseIBHandle[iSlot];
            s_abBaseIBInUse[iSlot] = 1;
            FireGrafix::g_pRenderer->LockAndFillStaticIB(m_uBaseIB, pIndices, 0);
        } else {
            m_uBaseIB = FireGrafix::g_pRenderer->CreateAndFillStaticIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(0), nIndices, pIndices, 0, 1);
            s_auBaseIBHandle[iSlot] = m_uBaseIB;
            s_abBaseIBInUse[iSlot] = 1;
            s_aiBaseIBSize[iSlot] = nIndices;
        }

        GetTempHeap()->DeAllocate(m_pFallbackVertices);
        GetTempHeap()->DeAllocate(m_pFallbackIndices);
        m_pFallbackVertices = nullptr;
        m_pFallbackIndices = nullptr;
    } else {
        m_pTerrain->GetBaseVBandIB(m_iWidth, m_iHeight, &m_uBaseVB, &m_uBaseIB, &m_uBaseIB2,
                                   reinterpret_cast<unsigned int*>(&m_nBaseTriangles));
        Fun_0866d812(FireGrafix::CRenderer9::m_VBList, m_uBaseVB);
        Fun_0866d866(FireGrafix::CRenderer9::m_IBList, m_uBaseIB2);
        Fun_0866d866(FireGrafix::CRenderer9::m_IBList, m_uBaseIB);
    }

    const int iClass = ClassFromMask(m_uClassMask);
    CreateTerrainRenderSurface(&m_uSurface0, m_uCacheTex0, m_uCacheTex1, iClass, false, 0, 0, 0);
    CreateTerrainRenderSurface(&m_uSurface1, m_uUncompressedTex0, m_uUncompressedTex1, iClass, false, 0, 0, 0);
    RecreateTessellationSurfaces(static_cast<unsigned int>(floorf(m_pTerrain->m_fTessLOD)));
}

/// 0x089135ec
void TerrainCell::RecreateTessellationSurfaces(unsigned int uLevel) {
    if (FireGrafix::DeviceSettings::g_DeviceCaps < 10) {
        return;
    }
    FireGrafix::g_pRenderer->ReleaseSurface(m_uTessSurface0);
    FireGrafix::g_pRenderer->ReleaseSurface(m_uTessSurface1);
    FireGrafix::g_pRenderer->ReleaseSurface(m_uOverlayTessSurface);
    m_uTessSurface0 = 0;
    m_uTessSurface1 = 0;
    m_uOverlayTessSurface = 0;

    unsigned int uBit = 1;
    do {
        if ((m_uClassMask >> uBit) & 1) {
            break;
        }
        ++uBit;
    } while (uBit < 5);
    const int iClass = uBit != 5 ? static_cast<int>(uBit) : 0;
    CreateTerrainRenderSurface(&m_uTessSurface0, m_uCacheTex0, m_uCacheTex1, iClass, true, 0, 0, 0);
    CreateTerrainRenderSurface(&m_uTessSurface1, m_uUncompressedTex0, m_uUncompressedTex1, iClass, true, 0, 0, 0);
    m_iTessLevel = uLevel;
    m_iOverlayTessLevel = uLevel + 1 < 4 ? uLevel + 1 : 4;
    CreateTerrainRenderSurface(&m_uOverlayTessSurface, 0, 0, 0, true, 0, 0, 0);
}

/// 0x08913766
void TerrainCell::CreateBuffers() {
    const int iWidth = m_iWidth;
    const int iHeight = m_iHeight;
    const unsigned int nVertices = (iHeight + 2) * (iWidth + 2);

    FireGrafix::VStruct<21>* pCompositeVertices;
    unsigned short* pIndices;
    m_pTerrain->GetVertexBufferSpaceComposite(nVertices, &pCompositeVertices, &m_uCompositeVBSpace);
    m_pTerrain->GetIndexBufferSpace((iWidth * 6 + 6) * (iHeight + 1), &pIndices, &m_uIBSpace);

    GetTempHeap()->DeAllocate(reinterpret_cast<unsigned char*>(m_pJobs));
    m_pJobs = nullptr;
    FillIndexLattice(pIndices, m_iWidth + 2, m_iHeight + 2, 1, nullptr);

    memcpy(pCompositeVertices, m_pCompositeVertices, nVertices * kVertexSize);
    GetTempHeap()->DeAllocate(m_pCompositeVertices);
    m_pCompositeVertices = nullptr;

    FireGrafix::VStruct<16>* pCompressedVertices;
    m_pTerrain->GetVertexBufferSpaceCompressed(nVertices, &pCompressedVertices, &m_uCompressedVBSpace);
    memcpy(pCompressedVertices, m_pCompressedVertices, nVertices * kCompressedVertexSize);
    GetTempHeap()->DeAllocate(m_pCompressedVertices);
    m_pCompressedVertices = nullptr;
}

/// 0x08913ab4
void TerrainCell::CreateNormalCompositeSurface(unsigned int* puHandle, unsigned int uSize) {
    FireGrafix::FGXSurface* pSurface = nullptr;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, puHandle, 0x4a);
    pSurface->field_60 = 2;
    pSurface->field_18[0] = static_cast<unsigned short>(uSize);
    pSurface->field_38[0] = 5;
    pSurface->field_18[1] = static_cast<unsigned short>(uSize);
    pSurface->field_38[1] = 5;
    pSurface->field_c = 2;
    pSurface->m_hVB = m_pShared->m_hQuadVB;
    pSurface->m_hIB = m_pShared->m_hQuadIB;
}

/// 0x08913b5c
void TerrainCell::CreateResolveSurface(unsigned int* puHandle, unsigned int uSize) {
    FireGrafix::FGXSurface* pSurface = nullptr;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, puHandle, 0x4c);
    pSurface->field_60 = 1;
    pSurface->field_18[0] = static_cast<unsigned short>(uSize);
    pSurface->field_38[0] = 5;
    pSurface->field_c = 2;
    pSurface->m_hVB = m_pShared->m_hQuadVB;
    pSurface->m_hIB = m_pShared->m_hQuadIB;
}

/// 0x08913bf2
void TerrainCell::CreateMicroShadowSurface(unsigned int* a1, unsigned int a2, unsigned int a3) {
}

/// 0x08913bf4
void TerrainCell::CreateCachedCompSurface(unsigned int* puHandle, unsigned int hNormal, unsigned int hRender, unsigned int hSpec, unsigned int hShadow, unsigned int uTriangleCount) {
    FireGrafix::FGXSurface* pSurface = nullptr;
    FireGrafix::g_pRenderer->CreateSurface(&pSurface, puHandle, 0x4b);
    pSurface->field_60 = 12;
    const int iClass = ClassFromMask(m_uClassMask);
    pSurface->field_18[0] = static_cast<unsigned short>(hNormal);
    pSurface->field_38[0] = 5;
    pSurface->field_18[1] = static_cast<unsigned short>(hRender);
    pSurface->field_38[1] = 5;
    pSurface->field_18[2] = static_cast<unsigned short>(hShadow);
    pSurface->field_38[2] = 5;
    pSurface->field_18[3] = static_cast<unsigned short>(hSpec);
    pSurface->field_38[3] = 5;
    pSurface->field_18[4] = static_cast<unsigned short>(m_pShared->m_ahSunlightTextures[iClass]);
    pSurface->field_38[4] = 2;
    pSurface->field_18[5] = static_cast<unsigned short>(m_pShared->m_ahShadowlightTextures[iClass]);
    pSurface->field_38[5] = 2;
    pSurface->field_18[6] = 0;
    pSurface->field_38[6] = 0;
    pSurface->field_18[7] = 0;
    pSurface->field_38[7] = 0;
    pSurface->field_28[0] = static_cast<unsigned short>(g_ColorKeySystem.m_auKeyHandles[0][iClass]);
    pSurface->field_48[0] = 5;
    pSurface->field_28[1] = static_cast<unsigned short>(RenderGlobals::g_TexHandles[2]);
    pSurface->field_48[1] = 5;
    pSurface->field_28[2] = static_cast<unsigned short>(g_ColorKeySystem.m_auKeyHandles[1][iClass]);
    pSurface->field_48[2] = 5;
    pSurface->field_28[3] = static_cast<unsigned short>(m_pShared->m_kTexHandles.m_ahTextures[9]);
    pSurface->field_48[3] = 0;
    pSurface->field_61 = 1;
    pSurface->field_58 = static_cast<unsigned short>(field_ec);
    pSurface->field_5c = 1;
    pSurface->field_c = uTriangleCount;
    pSurface->m_hIB = m_pShared->m_hDynamicIB;
    pSurface->m_hVB = m_pShared->m_hCompressedVB;
    pSurface->field_14 = m_uCompressedVBSpace;
    pSurface->field_10 = m_uIBSpace;
}

namespace {

// Constant buffer of the composite pass, from TerrainCompositeDynamicData::LockDynamic.
struct TerrainCompositeConstants {
    float fTexelOffsetX;               // +0x0
    float fTexelOffsetY;               // +0x4
    char aPad8[0x8];                   // +0x8
    float aTextureParams[8][4];        // +0x10: snapped scale, projection axis
    float aProjectionSlots[64][4];     // +0x90: cell texture slot per axis 0/1/2, projection mode
    float fCellOriginX;                // +0x490
    float fCellOriginY;                // +0x494
    char aPad498[0x18];                // +0x498
    float fShadowJitterX;              // +0x4b0
    float fShadowJitterY;              // +0x4b4
    float fShadowWeight;               // +0x4b8
};

// Constant buffer from NormalGlobs::LockGlobal.
struct NormalGlobsConstants {
    float fInvTileSizeX;               // +0x0
    float fInvTileSizeY;               // +0x4
    float field_8;                     // +0x8
    float field_c;                     // +0xc
    float field_10;                    // +0x10
};

const TerrainCellStatus kStatusBaseAndCache = static_cast<TerrainCellStatus>(5);
const int kMaxCompositeTextures = 8;
const int kMaxProjections = 64;
const float kSnapScale = 0.66f;
const float kSnapScaleAxis0 = 0.33f;
}  // namespace

/// 0x08913dfc
void TerrainCell::BuildTexture(TerrainCellStatus eStatus, FireGrafix::CCommandStream* pStream) {
    const unsigned int uTriangleCount = (m_iWidth * 2 + 2) * (m_iHeight + 1);
    CreateBuffers();

    const unsigned int uMaxTileSize = GraphicsSettings::TerrainSettings::g_iMaxTileSize;
    int iSizeIndex = 0;
    int nCompressPasses = 0;
    switch (field_17c) {
    case 0x40:
        nCompressPasses = 4;
        break;
    case 0x80:
        iSizeIndex = 1;
        nCompressPasses = 5;
        break;
    case 0x100:
        iSizeIndex = 2;
        nCompressPasses = 6;
        break;
    case 0x200:
        iSizeIndex = 3;
        nCompressPasses = 7;
        break;
    }

    Terrain::SharedData* pShared = m_pShared;
    const unsigned int hNormalTemp = pShared->m_hNormalTemp;
    const unsigned int hSpecTemp = m_bHasSpecular != 0 ? pShared->m_hRenderSpecTemp : 0;
    const unsigned int hRenderTemp = pShared->m_hRenderTemp;
    const unsigned int hHeightTemp = pShared->m_hHeightTemp;
    const unsigned int hShadowTemp = pShared->m_hShadowTemp;
    FireGrafix::TerrainCompositeDynamicData* pCompositeConstantSet =
        static_cast<FireGrafix::TerrainCompositeDynamicData*>(pShared->m_kTerrainShaderScreen.m_pConstantSet);

    unsigned int hDXTUncompressedDiffuse = 0;
    unsigned int hDXTDiffuse = 0;
    unsigned int hDXTUncompressedSpec = 0;
    unsigned int hDXTSpec = 0;
    unsigned int hCompressDiffuseSurface = 0;
    unsigned int hCompressSpecSurface = 0;
    FireGrafix::FGXSurface* pSurface;
    if (GraphicsSettings::TerrainSettings::g_bEnableDXTCompression != 0) {
        hDXTUncompressedDiffuse = pShared->m_ahDXTUncompressedDiffuse[iSizeIndex];
        hDXTDiffuse = pShared->m_ahDXTDiffuse[iSizeIndex];
        hDXTUncompressedSpec = pShared->m_ahDXTUncompressedSpec[iSizeIndex];
        hDXTSpec = pShared->m_ahDXTSpec[iSizeIndex];

        FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hCompressDiffuseSurface, 0x29);
        pSurface->field_60 = 1;
        pSurface->field_18[0] = static_cast<unsigned short>(hDXTUncompressedDiffuse);
        pSurface->field_38[0] = 0;
        pSurface->field_c = 2;
        pSurface->m_hIB = 1;
        pSurface->m_hVB = 1;

        FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hCompressSpecSurface, 0x29);
        pSurface->field_60 = 1;
        pSurface->field_18[0] = static_cast<unsigned short>(hDXTUncompressedSpec);
        pSurface->field_38[0] = 0;
        pSurface->field_c = 2;
        pSurface->m_hIB = 1;
        pSurface->m_hVB = 1;
    }

    unsigned int hNormalCompositeSurface;
    CreateNormalCompositeSurface(&hNormalCompositeSurface, hHeightTemp);
    unsigned int hCachedCompositeSurface;
    CreateCachedCompSurface(&hCachedCompositeSurface, hNormalTemp, hRenderTemp, hSpecTemp, hShadowTemp, uTriangleCount);

    const float fMaxTileSize = static_cast<float>(uMaxTileSize);
    const float fHalfTexel = 1.0f / (fMaxTileSize + fMaxTileSize);
    const FGXVector2 vMin = {static_cast<float>(m_iX - 1), static_cast<float>(m_iY - 1)};
    const FGXVector2 vMax = {static_cast<float>(m_iX + m_iWidth), static_cast<float>(m_iY + m_iWidth)};
    D3DXMATRIX kProjection;
    D3DXMATRIX kTranslation;
    D3DXMATRIX kProduct;
    D3DXMatrixOrthoOffCenterRH(&kProjection, vMin.x, vMax.x, vMax.y, vMin.y, -1.0f, 1.0f);
    D3DXMatrixTranslation(&kTranslation, -2.0f * fHalfTexel, fHalfTexel + fHalfTexel, 0.0f);
    D3DXMatrixMultiply(&kProduct, &kProjection, &kTranslation);
    kProjection = kProduct;

    const FGXVector4 vClearZero = {0.0f, 0.0f, 0.0f, 0.0f};
    const unsigned int uCompositeSet = pStream->BeginCommandSetMulti(
        hRenderTemp, hHeightTemp, hSpecTemp, 0, m_bHasSpecular != 0 ? 0 : pShared->m_hDepthStencil, 4, 9, 4, "HeightMap",
        vClearZero, 1.0f, 0, 0, 0);

    TerrainCompositeConstants* pConstants =
        static_cast<TerrainCompositeConstants*>(pCompositeConstantSet->LockDynamic(pStream));
    pConstants->fTexelOffsetX = -fHalfTexel;
    pConstants->fTexelOffsetY = fHalfTexel;
    pConstants->fCellOriginY = static_cast<float>(field_e8);
    pConstants->fCellOriginX = static_cast<float>(field_e4);

    unsigned int aSlotAxis0[96];
    unsigned int aSlotAxis1[96];
    unsigned int aSlotAxis2[96];
    memset(aSlotAxis0, 0, sizeof(aSlotAxis0));
    memset(aSlotAxis1, 0, sizeof(aSlotAxis1));
    memset(aSlotAxis2, 0, sizeof(aSlotAxis2));

    // Per texture: its slot filed under its projection and axis, and the snapped scale of that projection.
    if (m_nTextures != 0) {
        const float fWorldRange = WorldGlobals::g_WorldMax.x - WorldGlobals::g_WorldMin.x;
        for (unsigned int i = 0; i < m_nTextures; ++i) {
            const unsigned int uTextureId = m_aTextureIds[i];
            const int iProjection = pShared->m_aiProjectionTexture[uTextureId];
            const float fScale = pShared->field_e10[iProjection];
            float fSnapped = static_cast<float>(static_cast<int>(fScale * kSnapScale * fWorldRange)) / fWorldRange / kSnapScale;
            const unsigned int uAxis = pShared->m_aiProjectionAxis[uTextureId];
            if (uAxis == 1) {
                aSlotAxis1[iProjection] = i;
            } else if (uAxis == 0) {
                fSnapped = static_cast<float>(static_cast<int>(fScale * kSnapScaleAxis0 * fWorldRange)) / fWorldRange / kSnapScaleAxis0;
                aSlotAxis0[iProjection] = i;
            } else {
                aSlotAxis2[iProjection] = i;
            }
            pConstants->aTextureParams[i][0] = fSnapped;
            pConstants->aTextureParams[i][1] = static_cast<float>(uAxis);
            pConstants->aTextureParams[i][2] = 0.0f;
            pConstants->aTextureParams[i][3] = 0.0f;
        }
    }

    for (int i = 0; i < kMaxProjections; ++i) {
        pConstants->aProjectionSlots[i][0] = static_cast<float>(aSlotAxis0[i]);
        pConstants->aProjectionSlots[i][1] = static_cast<float>(aSlotAxis1[i]);
        pConstants->aProjectionSlots[i][2] = static_cast<float>(aSlotAxis2[i]);
        pConstants->aProjectionSlots[i][3] = static_cast<float>(static_cast<unsigned int>(pShared->m_aeProjectionMode[i]));
    }
    pCompositeConstantSet->UnlockDynamic();

    unsigned int hDrawSurface;
    if (GraphicsSettings::TerrainSettings::g_bSpecEnable == 0) {
        FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hDrawSurface, 0x4c);
        pSurface->field_60 = 0x10;
        unsigned int i = 0;
        for (; i < m_nTextures && i < static_cast<unsigned int>(kMaxCompositeTextures); ++i) {
            const int iProjection = pShared->m_aiProjectionTexture[m_aTextureIds[i]];
            pSurface->field_18[i] = static_cast<unsigned short>(pShared->field_708[iProjection]);
            pSurface->field_28[i] = static_cast<unsigned short>(pShared->field_908[iProjection]);
            pSurface->field_38[i] = 0;
            pSurface->field_48[i] = 0;
        }
        for (; i < static_cast<unsigned int>(kMaxCompositeTextures); ++i) {
            pSurface->field_18[i] = 0;
            pSurface->field_28[i] = 0;
        }
        pSurface->field_c = uTriangleCount;
        pSurface->m_hIB = pShared->m_hDynamicIB;
        pSurface->m_hVB = pShared->m_hCompositeVB;
        pSurface->field_14 = m_uCompositeVBSpace;
        pSurface->field_10 = m_uIBSpace;
        pStream->DrawSurface(hDrawSurface, pShared->m_kTerrainShaderScreen.m_hInstance, 0);
    } else {
        FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hDrawSurface, 0x4c);
        pSurface->field_60 = 0x18;
        pSurface->field_60 = 3;
        unsigned int i = 0;
        for (; i < m_nTextures && i < static_cast<unsigned int>(kMaxCompositeTextures); ++i) {
            const int iProjection = pShared->m_aiProjectionTexture[m_aTextureIds[i]];
            pSurface->field_18[i] = static_cast<unsigned short>(pShared->field_708[iProjection]);
            pSurface->field_28[i] = static_cast<unsigned short>(pShared->field_908[iProjection]);
            const unsigned int hNormalMap = pShared->field_808[iProjection];
            if (hNormalMap == 0) {
                pSurface->field_38[i] = static_cast<unsigned short>(pShared->m_hBlankTexture);
            } else {
                pSurface->field_38[i] = static_cast<unsigned short>(hNormalMap);
            }
        }
        for (; i < static_cast<unsigned int>(kMaxCompositeTextures); ++i) {
            pSurface->field_18[i] = static_cast<unsigned short>(pShared->m_hBlankTexture);
            pSurface->field_28[i] = static_cast<unsigned short>(pShared->m_hBlankTexture);
            pSurface->field_38[i] = static_cast<unsigned short>(pShared->m_hBlankTexture);
        }
        pSurface->field_38[0] = 0;
        pSurface->field_38[1] = 0;
        pSurface->field_38[2] = 0;
        const bool bGPUWeightBlending = GraphicsSettings::TerrainSettings::g_bUseGPUWeightBlending != 0;
        if (bGPUWeightBlending) {
            pSurface->field_5c = 0;
            pSurface->field_58 = static_cast<unsigned short>(field_f0);
            pSurface->field_61 = 1;
        }
        pSurface->field_c = uTriangleCount;
        pSurface->m_hIB = pShared->m_hDynamicIB;
        pSurface->m_hVB = pShared->m_hCompositeVB;
        pSurface->field_14 = m_uCompositeVBSpace;
        pSurface->field_10 = m_uIBSpace;
        pStream->DrawSurface(hDrawSurface, pShared->m_kTerrainShaderScreen.m_hInstance, bGPUWeightBlending ? 1 : 2);
    }
    FireGrafix::g_pRenderer->ReleaseSurface(hDrawSurface);
    pStream->EndCommandSet();
    pStream->AddCommandSetToRenderQueue(uCompositeSet);

    TerrainSystem* pTerrainSystem = pShared->m_kExternalData.m_pTerrainSystem;
    if (pTerrainSystem != nullptr) {
        const unsigned int uDecalSet = pStream->BeginCommandSetMulti(
            hRenderTemp, hHeightTemp, hSpecTemp, 0, pShared->m_hDepthStencil, 8, 9, 0, "Decal Layer", vClearZero, 1.0f, 0, 0, 0);
        pTerrainSystem->m_DecalLayer.Render(pStream, vMin, vMax, &kProjection, 0);
        pStream->EndCommandSet();
        pStream->AddCommandSetToRenderQueue(uDecalSet);
    }

    NormalGlobsConstants* pNormalGlobs = static_cast<NormalGlobsConstants*>(FireGrafix::g_pNormalGlobs->LockGlobal(0));
    pNormalGlobs->field_10 = 16.0f;
    const float fInvTileSize = 1.0f / fMaxTileSize;
    pNormalGlobs->fInvTileSizeX = fInvTileSize;
    pNormalGlobs->fInvTileSizeY = fInvTileSize;
    pNormalGlobs->field_8 = 0.0f;
    FireGrafix::g_pNormalGlobs->Unlock(0);

    const unsigned int uNormalSet = pStream->BeginCommandSet(hNormalTemp, 0, 9, 0x80, "Normal Calculation", vClearZero, 1.0f, 0, 0, 0);
    pStream->DrawSurface(hNormalCompositeSurface, pShared->m_kNormalCalculate.m_hInstance, 0);
    pStream->EndCommandSet();
    pStream->AddCommandSetToRenderQueue(uNormalSet);

    const unsigned int uShadowSet = pStream->BeginCommandSet(hShadowTemp, 4, 9, 4, "Macro Shadow", vClearZero, 1.0f, 0, 0, 0);
    const unsigned int uShadowQuality = GraphicsSettings::TerrainSettings::g_uShadowQuality;
    if (uShadowQuality != 0) {
        const float fShadowWeight = 1.0f / static_cast<float>(uShadowQuality);
        for (unsigned int i = 0; i < GraphicsSettings::TerrainSettings::g_uShadowQuality; ++i) {
            TerrainCompositeConstants* pShadowConstants =
                static_cast<TerrainCompositeConstants*>(pCompositeConstantSet->LockDynamic(pStream));
            pShadowConstants->fShadowJitterX = pShared->m_afSampleJitter[i][0];
            pShadowConstants->fShadowJitterY = pShared->m_afSampleJitter[i][1];
            pShadowConstants->fShadowWeight = fShadowWeight;
            pShadowConstants->fCellOriginY = static_cast<float>(field_e8);
            pShadowConstants->fCellOriginX = static_cast<float>(field_e4);
            pShadowConstants->fTexelOffsetY = 0.0f;
            pShadowConstants->fTexelOffsetX = 0.0f;
            pCompositeConstantSet->UnlockDynamic();

            unsigned int hShadowSurface;
            FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hShadowSurface, 0x4c);
            pSurface->field_60 = 1;
            pSurface->field_60 = 1;
            pSurface->field_18[0] = static_cast<unsigned short>(field_ec);
            pSurface->field_38[0] = 0;
            pSurface->field_c = uTriangleCount;
            pSurface->m_hIB = pShared->m_hDynamicIB;
            pSurface->m_hVB = pShared->m_hCompositeVB;
            pSurface->field_14 = m_uCompositeVBSpace;
            pSurface->field_10 = m_uIBSpace;
            pStream->DrawSurface(hShadowSurface, pShared->m_kTerrainShaderScreen.m_hInstance, 3);
            FireGrafix::g_pRenderer->ReleaseSurface(hShadowSurface);
        }
    }
    pStream->EndCommandSet();
    pStream->AddCommandSetToRenderQueue(uShadowSet);

    if (pTerrainSystem != nullptr) {
        const unsigned int uDecalShadowSet =
            pStream->BeginCommandSet(hShadowTemp, 4, 9, 0, "Decal Macro Shadow", vClearZero, 1.0f, 0, 0, 0);
        pTerrainSystem->m_DecalLayer.Render(pStream, vMin, vMax, &kProjection, 1);
        pStream->EndCommandSet();
        pStream->AddCommandSetToRenderQueue(uDecalShadowSet);
    }

    const unsigned int hCacheBake = pShared->m_kCacheBake.m_hInstance;
    const unsigned short uHasSpecular = m_bHasSpecular != 0 ? 1 : 0;
    const FGXVector4 vClearDiffuse = {1.0f, 1.0f, 0.0f, 0.0f};
    const FGXVector4 vClearDiffuseSpec = {1.0f, 0.5f, 0.0f, 0.0f};

    if (eStatus == kStatusBaseAndCache) {
        unsigned int uCacheSet;
        if (m_bHasSpecular == 0) {
            uCacheSet = pStream->BeginCommandSet(m_uUncompressedTex0, 0, 9, 0x80, "Terrain Cache", vClearDiffuse, 1.0f, 0, 0, 0);
        } else {
            uCacheSet = pStream->BeginCommandSetMulti(m_uUncompressedTex0, m_uUncompressedTex1, 0, 0, pShared->m_hDepthStencil, 0, 9,
                                                      0x80, "Terrain Cache", vClearDiffuseSpec, 1.0f, 0, 0, 0);
        }
        pStream->DrawSurface(hCachedCompositeSurface, hCacheBake, uHasSpecular);
        pStream->EndCommandSet();
        pStream->AddCommandSetToRenderQueue(uCacheSet);
    }

    if (GraphicsSettings::TerrainSettings::g_bEnableDXTCompression == 0) {
        const unsigned int uCacheSet = pStream->BeginCommandSet(m_uCacheTex0, 0, 9, 4, "Terrain Cache", vClearDiffuse, 1.0f, 0, 0, 0);
        pStream->DrawSurface(hCachedCompositeSurface, hCacheBake, 0);
        pStream->EndCommandSet();
        pStream->AddCommandSetToRenderQueue(uCacheSet);
    } else {
        unsigned int uCacheSet;
        if (m_bHasSpecular == 0) {
            uCacheSet = pStream->BeginCommandSet(hDXTUncompressedDiffuse, 0, 9, 0x80, "Terrain Cache", vClearDiffuse, 1.0f, 0, 0, 0);
        } else {
            uCacheSet = pStream->BeginCommandSetMulti(hDXTUncompressedDiffuse, hDXTUncompressedSpec, 0, 0, pShared->m_hDepthStencil, 0,
                                                      9, 0x80, "Terrain Cache", vClearDiffuseSpec, 1.0f, 0, 0, 0);
        }
        pStream->DrawSurface(hCachedCompositeSurface, hCacheBake, uHasSpecular);
        pStream->EndCommandSet();
        pStream->AddCommandSetToRenderQueue(uCacheSet);

        FireGrafix::BCDynamics* pCompressConstantSet = static_cast<FireGrafix::BCDynamics*>(pShared->m_kCompressBC.m_pConstantSet);
        for (int iMip = 0; iMip < nCompressPasses; ++iMip) {
            const unsigned int uPassSet =
                pStream->BeginCommandSetSubResource(hDXTDiffuse, 0, iMip, 0, 9, 0x20, "DXT Compress", vClearZero, 1.0f, 0, 0, 0);
            *pCompressConstantSet->LockDynamic(pStream) = static_cast<float>(iMip);
            pCompressConstantSet->UnlockDynamic();
            pStream->DrawSurface(hCompressDiffuseSurface, pShared->m_kCompressBC.m_hInstance, 0);
            pStream->EndCommandSet();
            pStream->AddCommandSetToRenderQueue(uPassSet);
        }
        pStream->AddCommandSetToRenderQueue(pStream->CopyResource(hDXTDiffuse, m_uCacheTex0));

        if (m_bHasSpecular != 0) {
            for (int iMip = 0; iMip < nCompressPasses; ++iMip) {
                const unsigned int uPassSet =
                    pStream->BeginCommandSetSubResource(hDXTSpec, 0, iMip, 0, 9, 0x20, "DXT Compress", vClearZero, 1.0f, 0, 0, 0);
                *pCompressConstantSet->LockDynamic(pStream) = static_cast<float>(iMip);
                pCompressConstantSet->UnlockDynamic();
                pStream->DrawSurface(hCompressSpecSurface, pShared->m_kCompressBC.m_hInstance, 0);
                pStream->EndCommandSet();
                pStream->AddCommandSetToRenderQueue(uPassSet);
            }
            pStream->AddCommandSetToRenderQueue(pStream->CopyResource(hDXTSpec, m_uCacheTex1));
        }
    }

    FireGrafix::g_pRenderer->ReleaseSurface(hNormalCompositeSurface);
    FireGrafix::g_pRenderer->ReleaseSurface(hCachedCompositeSurface);
    FireGrafix::g_pRenderer->ReleaseSurface(hCompressDiffuseSurface);
    FireGrafix::g_pRenderer->ReleaseSurface(hCompressSpecSurface);
}

/// 0x0891568c
void TerrainCell::DemoteToCacheOnly() {
    if (m_eStatus != kStatusCacheOnly) {
        DeleteSurfacesAndBuffers(true, false, true);
        m_eStatus = kStatusCacheOnly;
    }
}

/// 0x089156da
float TerrainCell::GetDistanceFromFrustum(D3DXMATRIX* pMatrix, FGXVector4* pPlanes) {
    D3DXVECTOR4 vCenter = {m_fCenterX, m_fCenterY, m_fCenterZ, 1.0f};
    D3DXVec4Transform(&vCenter, &vCenter, pMatrix);

    float fNearest = 100000.0f;
    for (int i = 0; i < 6; ++i) {
        const FGXVector4& kPlane = pPlanes[i];
        const float fDistance = fabsf(kPlane.x * vCenter.x + kPlane.y * vCenter.y + kPlane.z * vCenter.z +
                                      kPlane.w * vCenter.w);
        if (fDistance < fNearest) {
            fNearest = fDistance;
        }
    }
    return fNearest;
}

/// 0x089157c8
void TerrainCell::CalcWeight() {
    if (static_cast<unsigned int>(field_328 - 1) < 2) {
        m_fWeight = 0.0f;
    }

    float fNearest = FLT_MAX;
    const unsigned int nCameras = FrameGlobals::g_nCameras;
    if (nCameras != 0) {
        for (unsigned int i = 0; i < nCameras; ++i) {
            const FGXVector4& kPlaneA = FrameGlobals::g_ViewClipPlanes[i][3];
            const FGXVector4& kPlaneB = FrameGlobals::g_ViewClipPlanes[i][2];

            float fDistA = fabsf(kPlaneA.y * m_fCenterY + kPlaneA.x * m_fCenterX + kPlaneA.z * m_fCenterZ + kPlaneA.w);
            if (fNearest <= fDistA) {
                fDistA = fNearest;
            }
            float fDistB = fabsf(kPlaneB.y * m_fCenterY + kPlaneB.x * m_fCenterX + kPlaneB.z * m_fCenterZ + kPlaneB.w);
            if (fDistA <= fDistB) {
                fDistB = fDistA;
            }
            float fDistY = fabsf(m_fCenterY - FrameGlobals::g_WorldTopLeftVisible[i].y);
            if (fDistB <= fDistY) {
                fDistY = fDistB;
            }
            fNearest = fDistY;
        }
    }
    m_fWeight = fNearest;
}

/// 0x089158ca
bool TerrainCell::IsVisible(FGXVector4* pFrustumPlanes) {
    m_bVisible = true;
    const float* pPlanes = reinterpret_cast<const float*>(pFrustumPlanes);
    const float fNegRadius = -m_fRadius;

    float afDistance[6];
    bool bVisible = true;
    for (int i = 0; i < 6; ++i) {
        const float* pPlane = pPlanes + i * 4;
        afDistance[i] = pPlane[1] * m_fCenterY + pPlane[0] * m_fCenterX + pPlane[2] * m_fCenterZ + pPlane[3];
        if (afDistance[i] < fNegRadius) {
            m_bVisible = false;
            bVisible = false;
        }
    }

    if (bVisible) {
        for (int i = 0; i < 6; ++i) {
            m_afPlaneDistance[i] = afDistance[i];
        }
        return true;
    }
    m_bVisible = false;
    return false;
}

namespace {

// field_328 is 1 when the height maximum of CalculateStats is below 2, 2 below 30 and 3 above.
const float kStatsLowHeight = 2.0f;
const float kStatsMidHeight = 30.0f;

// Height at a hex of the terrain's height map, the hex clamped to the map first.
const auto SampleHeight = [](Terrain::SharedData* pShared, int iX, int iY) -> float {
    if (iX >= pShared->m_iHeightMapWidth) {
        iX = pShared->m_iHeightMapWidth - 1;
    }
    if (iY >= pShared->m_iHeightMapHeight) {
        iY = pShared->m_iHeightMapHeight - 1;
    }
    if (iX < 0) {
        iX = 0;
    }
    if (iY < 0) {
        iY = 0;
    }
    cvTerrainSampler* pSampler = pShared->m_kExternalData.m_pTerrainSampler;
    const FGXInt2 kPos = {iX, iY};
    const FGXInt2 kWrapped = pSampler->GetWrappedPosition(kPos);
    return static_cast<float>(Fun_08679d0a(&pSampler->m_iHeightMapWidth, &kWrapped)) * kHeightScale;
};
}  // namespace

/// 0x089159de
void TerrainCell::CalculateStats() {
    float fMaxStep = 0.0f;
    float fMaxHeight = 0.0f;
    for (int iY = m_iY + 4; iY < m_iY + m_iHeight + 3; ++iY) {
        for (int iX = m_iX + 4; iX < m_iX + m_iWidth + 2; ++iX) {
            const unsigned int uMapHeight = static_cast<unsigned int>(m_pShared->m_iHeightMapHeight);
            const unsigned int uMapWidth = static_cast<unsigned int>(m_pShared->m_iHeightMapWidth);
            const int iRow = static_cast<int>((uMapHeight + iY) % uMapHeight);
            const int iRowNext = static_cast<int>((uMapHeight + 1 + iY) % uMapHeight);
            const int iCol = static_cast<int>((iX + uMapWidth) % uMapWidth);
            const int iColNext = static_cast<int>((iX + 1 + uMapWidth) % uMapWidth);

            const float fHeight = SampleHeight(m_pShared, iCol, iRow);
            const float fHeightRight = SampleHeight(m_pShared, iColNext, iRow);
            const float fHeightUp = SampleHeight(m_pShared, iCol, iRowNext);
            const float fStep = fabsf(fHeight - fHeightUp);
            if (fMaxStep <= fHeightRight) {
                fMaxStep = fHeightRight;
            }
            if (fMaxStep <= fStep) {
                fMaxStep = fStep;
            }
            if (fMaxHeight <= fHeight) {
                fMaxHeight = fHeight;
            }
        }
    }
    field_320 = fMaxStep;
    field_324 = fMaxHeight;
    field_328 = 3;
    if (kStatsLowHeight > fMaxHeight) {
        field_328 = 1;
    } else if (kStatsMidHeight > fMaxHeight) {
        field_328 = 2;
    }
}

/// 0x08915d1c
void TerrainCell::CalculateTextureContributions() {
    const int kNormalGrid = 69;
    const int kMaxTextures = 8;

    memset(m_aTextureIds, 0, sizeof(m_aTextureIds));
    unsigned int aCounts[128];
    memset(aCounts, 0, sizeof(aCounts));

    const int iX = m_iX;
    const int iY = m_iY;
    const int iWidth = m_iWidth;
    const int iHeight = m_iHeight;

    FGXVector3 aNormals[kNormalGrid * kNormalGrid];
    for (int i = 0; i < kNormalGrid * kNormalGrid; ++i) {
        aNormals[i].x = 0.0f;
        aNormals[i].y = 0.0f;
        aNormals[i].z = 0.0f;
    }
    const FGXInt2 kOrigin = {iX, iY};
    const FGXInt2 kSize = {kNormalGrid, kNormalGrid};
    m_pShared->m_kExternalData.m_pTerrainSampler->CalcNormalBlockMax69x69(kOrigin, kSize, aNormals);

    // Count, for every projection, the hexes (with a one hex border) that sample it.
    for (int iSampleY = iY - 2; iSampleY <= iY + iHeight; ++iSampleY) {
        for (int iSampleX = iX - 2; iSampleX <= iX + iWidth; ++iSampleX) {
            Terrain::SharedData* pShared = m_pShared;
            const unsigned int uWrappedY = static_cast<unsigned int>(pShared->m_iHeightMapHeight + iSampleY) %
                                           static_cast<unsigned int>(pShared->m_iHeightMapHeight);
            const unsigned int uWrappedX = static_cast<unsigned int>(iSampleX + pShared->m_iHeightMapWidth) %
                                           static_cast<unsigned int>(pShared->m_iHeightMapWidth);
            cvTerrainSampler* pSampler = pShared->m_kExternalData.m_pTerrainSampler;
            const FGXInt2 kPos = {static_cast<int>(uWrappedX), static_cast<int>(uWrappedY)};
            const FGXInt2 kWrapped = pSampler->GetWrappedPosition(kPos);
            const unsigned char ucTerrain = Fun_087fb076(pSampler->unk_4c, &kWrapped);

            pShared = m_pShared;
            unsigned int uSlot = pShared->m_aiFirstProjection[ucTerrain];

            float fNormalX = 0.0f;
            float fNormalY = 0.0f;
            float fNormalZ = 0.0f;
            const unsigned int uCol = static_cast<unsigned int>(iSampleX + 2 - m_iX);
            const int iRow = iSampleY + 2 - m_iY;
            if (uCol < static_cast<unsigned int>(kNormalGrid) && iRow >= 0 && iRow < kNormalGrid) {
                const FGXVector3& kNormal = aNormals[iRow * kNormalGrid + uCol];
                fNormalX = kNormal.x;
                fNormalY = kNormal.y;
                fNormalZ = kNormal.z;
            }

            if (static_cast<int>(pShared->m_aeProjectionMode[ucTerrain]) == 1) {
                if (!(fNormalZ > kFlatNormalZ)) {
                    if (fabsf(fNormalY) > fabsf(fNormalX)) {
                        uSlot += 1;
                    } else {
                        uSlot += 2;
                    }
                }
                aCounts[uSlot] += 1;
                aCounts[uSlot + 1] += 1;
                aCounts[uSlot + 2] += 1;
            }
            aCounts[uSlot] += 1;
        }
    }

    unsigned int aWeights[64];
    m_nTextures = 0;
    if (m_pShared->m_nProjections != 0) {
        for (unsigned int uProjection = 0; uProjection < m_pShared->m_nProjections; ++uProjection) {
            if (aCounts[uProjection] != 0) {
                m_aTextureIds[m_nTextures] = uProjection;
                aWeights[m_nTextures] = aCounts[uProjection];
                ++m_nTextures;
            }
        }
        if (m_nTextures > static_cast<unsigned int>(kMaxTextures)) {
            // Most-sampled textures first, then keep the first eight.
            bool bSwapped;
            do {
                bSwapped = false;
                for (unsigned int i = 0; i + 1 < m_nTextures; ++i) {
                    if (aWeights[i] < aWeights[i + 1]) {
                        const unsigned int uWeight = aWeights[i];
                        aWeights[i] = aWeights[i + 1];
                        aWeights[i + 1] = uWeight;
                        const unsigned int uTexture = m_aTextureIds[i];
                        m_aTextureIds[i] = m_aTextureIds[i + 1];
                        m_aTextureIds[i + 1] = uTexture;
                        bSwapped = true;
                    }
                }
            } while (bSwapped);
            m_nTextures = kMaxTextures;
        }
    }

    m_bHasSpecular = 0;
    if (GraphicsSettings::TerrainSettings::g_bSpecEnable != 0) {
        if (field_300 != 0) {
            m_bHasSpecular = 1;
        }
        for (unsigned int i = 0; i < m_nTextures; ++i) {
            if (m_pShared->field_808[m_pShared->m_aiProjectionTexture[m_aTextureIds[i]]] != 0) {
                m_bHasSpecular = 1;
            }
        }
    }
}

/// 0x08916132
void TerrainCell::LaunchStatsBuild(FJobSet* pJobSet) {
    m_kStatJob.Init(this);
    pJobSet->AddJob(&m_kStatJob, kJobTag, nullptr);
}

/// 0x0891617a
bool TerrainCell::IntersectRay(FGXVector3& vOrigin, FGXVector3& vDir, FGXVector2* pTexCoord, FGXInt2* pHit, float* pfDistance) {
    float fSphereDistance;
    if (!FGXRaySphereIntersect(reinterpret_cast<FGXVector3*>(&m_fCenterX), m_fRadius, &vOrigin, &vDir, nullptr,
                               &fSphereDistance)) {
        return false;
    }
    float fNearest = *pfDistance;
    if (fSphereDistance > fNearest) {
        return false;
    }
    if (m_iHeight - 1 <= 0) {
        return false;
    }

    bool bHit = false;
    // The ray is tested against two triangles of every hex quad, x stepping over the height and y over the width.
    for (int iStepX = 0; iStepX < m_iHeight - 1; ++iStepX) {
        if (m_iWidth - 1 <= 0) {
            continue;
        }
        for (int iStepY = 0; iStepY < m_iWidth - 1; ++iStepY) {
            const int iCellX = m_iX + iStepX;
            const int iCellY = m_iY + iStepY;

            FGXVector3 aCorner[4] = {};
            for (int iDx = 0; iDx < 2; ++iDx) {
                for (int iDy = 0; iDy < 2; ++iDy) {
                    Terrain::SharedData* pShared = m_pShared;
                    const FGXInt2 kPos = {iCellX + iDx, iCellY + iDy};
                    FGXVector3& kCorner = aCorner[iDx * 2 + iDy];
                    kCorner.x = static_cast<float>(kPos.x) * pShared->m_vTerrainScale.x - pShared->m_fHalfWorldWidth;
                    kCorner.y = static_cast<float>(kPos.y) * pShared->m_vTerrainScale.y - pShared->m_fHalfWorldHeight;
                    kCorner.z = pShared->m_kExternalData.m_pTerrainSampler->GetWorldHeight(kPos);
                }
            }

            const D3DXVECTOR3* pCorners = reinterpret_cast<const D3DXVECTOR3*>(aCorner);
            const D3DXVECTOR3* pOrigin = reinterpret_cast<const D3DXVECTOR3*>(&vOrigin);
            const D3DXVECTOR3* pDir = reinterpret_cast<const D3DXVECTOR3*>(&vDir);
            float fU;
            float fV;
            float fDistance;
            if (D3DXIntersectTri(&pCorners[0], &pCorners[2], &pCorners[1], pOrigin, pDir, &fU, &fV, &fDistance) &&
                fDistance < fNearest) {
                pTexCoord->x = vDir.x * fDistance + vOrigin.x;
                pTexCoord->y = vDir.y * fDistance + vOrigin.y;
                pHit->x = iCellX;
                pHit->y = iCellY;
                bHit = true;
                fNearest = fDistance;
            }
            if (D3DXIntersectTri(&pCorners[2], &pCorners[3], &pCorners[1], pOrigin, pDir, &fU, &fV, &fDistance) &&
                fDistance < fNearest) {
                pTexCoord->x = vDir.x * fDistance + vOrigin.x;
                pTexCoord->y = vDir.y * fDistance + vOrigin.y;
                pHit->x = iCellX;
                pHit->y = iCellY;
                bHit = true;
                fNearest = fDistance;
            }
        }
    }
    if (bHit) {
        *pfDistance = fNearest;
        return true;
    }
    return false;
}

/// 0x08916510
void TerrainCell::SetBuildParameters(int iX, int iY, int iWidth, int iHeight) {
    DeleteSurfacesAndBuffers(true, true, true);
    m_iWidth = iWidth;
    m_iHeight = iHeight;
    m_iX = iX;
    m_iY = iY;

    const float fScaleX = m_pShared->m_vTerrainScale.x;
    const float fScaleY = m_pShared->m_vTerrainScale.y;
    const float fScaleZ = m_pShared->m_vTerrainScale.z;

    // World extent of the cell: the minimum is shifted by half the world size, Z starts at -field_1ab8.
    m_fMinX = iX * fScaleX - m_pShared->m_fHalfWorldWidth;
    m_fMinY = iY * fScaleY - m_pShared->m_fHalfWorldHeight;
    m_fMinZ = -m_pShared->field_1ab8;
    m_fMaxX = iWidth * fScaleX + m_fMinX;
    m_fMaxY = iHeight * fScaleY + m_fMinY;
    m_fMaxZ = 255.0f * fScaleZ + m_fMinZ;

    m_fCenterX = (m_fMaxX + m_fMinX) * 0.5f;
    m_fCenterY = (m_fMaxY + m_fMinY) * 0.5f;
    m_fCenterZ = (m_fMaxZ + m_fMinZ) * 0.5f;

    const float fDx = m_fMaxX - m_fMinX;
    const float fDy = m_fMaxY - m_fMinY;
    const float fDz = m_fMaxZ - m_fMinZ;
    m_fRadius = 0.5f * sqrtf(fDz * fDz + (fDy * fDy + fDx * fDx));

    // Tile-grid key of the cell (multiples of 0x380) and the tile's heightmap entries in the shared data.
    const int iTileX = iX / 0x380;
    const int iTileY = iY / 0x380;
    field_e4 = (iTileX * -0x380) | 0x40;
    field_e8 = (iTileY * -0x380) | 0x40;
    const int iTile = iTileY * 0x10 + iTileX;
    field_ec = m_pShared->m_ahHeightmapTiles[iTile];
    field_f0 = m_pShared->field_16a4[iTile];
}

/// 0x0891674c
void TerrainCell::LaunchJobs(FJobSet* pJobSet) {
    m_bJobsLaunched = true;
    if (GraphicsSettings::TerrainSettings::g_uTerrainTessLevel == 0) {
        const int iPatchStep = m_pShared->m_iTessLevel;
        const int nPatchCols = (m_iWidth + iPatchStep - 2) / iPatchStep;
        const int nPatchRows = (iPatchStep - 2 + m_iHeight) / iPatchStep;
        m_nBaseVertices = (nPatchRows + 1) * (nPatchCols + 1);
        m_nBaseTriangles = nPatchCols * nPatchRows * 2;
        m_pFallbackVertices = GetTempHeap()->Allocate(m_nBaseVertices * kFallbackVertexSize);
        m_pFallbackIndices = GetTempHeap()->Allocate((m_iWidth * 6 - 6) * (m_iHeight - 1));
        if (m_pFallbackVertices == nullptr || m_pFallbackIndices == nullptr) {
            GetTempHeap()->DeAllocate(m_pFallbackVertices);
            GetTempHeap()->DeAllocate(m_pFallbackIndices);
            m_pFallbackVertices = nullptr;
            m_pFallbackIndices = nullptr;
            m_bJobsLaunched = false;
            return;
        }
        m_kFallbackJob.Init(this);
        pJobSet->AddJob(&m_kFallbackJob, kFallbackJobTag, nullptr);
    }

    // Vertex batches are needed only when Build will go on to call BuildTexture.
    const int iTarget = static_cast<int>(m_eTargetStatus);
    if (iTarget != 1) {
        if (iTarget == 5) {
            if (m_eStatus == kStatusBaseOnly) {
                return;
            }
        } else {
            if (iTarget != 3) {
                return;
            }
            if (m_eStatus == kStatusCacheOnly) {
                return;
            }
        }
    }
    BuildTextureVBs(pJobSet);
}

/// 0x089168d0
bool TerrainCell::RequestCacheTexture() {
    Terrain* pTerrain = m_pTerrain;
    const bool bDXT = GraphicsSettings::TerrainSettings::g_bEnableDXTCompression != 0;

    if (m_bHasSpecular == 0) {
        if (m_uCacheTex0 != 0) {
            return true;
        }
        if (bDXT) {
            m_uCacheTex0 = pTerrain->GetBCTexture();
            field_17c = GraphicsSettings::TerrainSettings::g_iCacheSize;
        } else {
            m_uCacheTex0 = pTerrain->GetRenderTexture(GraphicsSettings::TerrainSettings::g_iCacheSize, &field_17c);
        }
        return m_uCacheTex0 != 0;
    }

    if (m_uCacheTex0 != 0 && m_uCacheTex1 != 0) {
        return true;
    }
    if (bDXT) {
        m_uCacheTex1 = pTerrain->GetBCTexture();
        m_uCacheTex0 = pTerrain->GetBCTexture();
        field_17c = GraphicsSettings::TerrainSettings::g_iCacheSize;
        if (m_uCacheTex0 != 0 && m_uCacheTex1 != 0) {
            return true;
        }
        pTerrain->ReleaseBCTexture(m_uCacheTex0);
        pTerrain->ReleaseBCTexture(m_uCacheTex1);
    } else {
        m_uCacheTex1 = pTerrain->GetRenderTexture(GraphicsSettings::TerrainSettings::g_iCacheSize, &field_17c);
        m_uCacheTex0 = pTerrain->GetRenderTexture(GraphicsSettings::TerrainSettings::g_iCacheSize, &field_17c);
        if (m_uCacheTex0 != 0 && m_uCacheTex1 != 0) {
            return true;
        }
        pTerrain->ReleaseRenderTexture(m_uCacheTex0);
        pTerrain->ReleaseRenderTexture(m_uCacheTex1);
    }
    m_uCacheTex1 = 0;
    m_uCacheTex0 = 0;
    return false;
}

/// 0x08916a7e
bool TerrainCell::RequestUncompressedTextures() {
    Terrain* pTerrain = m_pTerrain;
    const bool bDXT = GraphicsSettings::TerrainSettings::g_bEnableDXTCompression != 0;
    const bool bSpecular = m_bHasSpecular != 0;
    unsigned int uSize = field_17c;

    // hCache0 is m_uCacheTex0; hCache1 exists only with specular data.
    unsigned int hCache0 = 0;
    unsigned int hCache1 = 0;
    if (bSpecular) {
        if (bDXT) {
            hCache1 = pTerrain->GetBCTexture();
            hCache0 = pTerrain->GetBCTexture();
            uSize = GraphicsSettings::TerrainSettings::g_iCacheSize;
        } else {
            hCache1 = pTerrain->GetRenderTexture(GraphicsSettings::TerrainSettings::g_iCacheSize, &field_17c);
            hCache0 = pTerrain->GetRenderTexture(GraphicsSettings::TerrainSettings::g_iCacheSize, &field_17c);
        }
    } else if (bDXT) {
        hCache0 = pTerrain->GetBCTexture();
        uSize = GraphicsSettings::TerrainSettings::g_iCacheSize;
    } else {
        hCache0 = pTerrain->GetRenderTexture(GraphicsSettings::TerrainSettings::g_iCacheSize, &field_17c);
    }

    unsigned int uSizeUncompressed0 = 0;
    unsigned int uSizeUncompressed1 = 0;
    const unsigned int hUncompressed0 = pTerrain->GetRenderTexture(m_uMipSizeNeeded, &uSizeUncompressed0);
    unsigned int hUncompressed1 = 0;
    if (bSpecular) {
        hUncompressed1 = pTerrain->GetRenderTexture(m_uMipSizeNeeded, &uSizeUncompressed1);
    }

    const bool bFailed = hCache0 == 0 || hUncompressed0 == 0 ||
                         (bSpecular && (hUncompressed1 == 0 || hCache1 == 0 || uSizeUncompressed0 != uSizeUncompressed1));
    if (bFailed) {
        pTerrain->ReleaseRenderTexture(hUncompressed0);
        pTerrain->ReleaseRenderTexture(hUncompressed1);
        if (bDXT) {
            pTerrain->ReleaseBCTexture(hCache0);
            pTerrain->ReleaseBCTexture(hCache1);
        } else {
            pTerrain->ReleaseRenderTexture(hCache0);
            pTerrain->ReleaseRenderTexture(hCache1);
        }
        return false;
    }

    m_uPageSize = uSizeUncompressed0;
    pTerrain->ReleaseRenderTexture(m_uUncompressedTex0);
    pTerrain->ReleaseRenderTexture(m_uUncompressedTex1);
    if (bDXT) {
        pTerrain->ReleaseBCTexture(m_uCacheTex0);
        pTerrain->ReleaseBCTexture(m_uCacheTex1);
    } else {
        pTerrain->ReleaseRenderTexture(m_uCacheTex0);
        pTerrain->ReleaseRenderTexture(m_uCacheTex1);
    }
    m_uUncompressedTex0 = hUncompressed0;
    m_uUncompressedTex1 = hUncompressed1;
    m_uCacheTex0 = hCache0;
    m_uCacheTex1 = hCache1;
    field_17c = uSize;
    return true;
}

/// 0x08916d20
void TerrainCell::CleanuponFail() {
    GetTempHeap()->DeAllocate(m_pFallbackVertices);
    GetTempHeap()->DeAllocate(m_pFallbackIndices);
    m_pFallbackIndices = nullptr;
    m_pFallbackVertices = nullptr;
    GetTempHeap()->DeAllocate(reinterpret_cast<unsigned char*>(m_pJobs));
    GetTempHeap()->DeAllocate(m_pCompositeVertices);
    GetTempHeap()->DeAllocate(m_pCompressedVertices);
    m_pCompositeVertices = nullptr;
    m_pCompressedVertices = nullptr;
    m_pJobs = nullptr;
}

/// 0x08916de0
void TerrainCell::Build(unsigned int uFrame, FireGrafix::CCommandStream* pStream) {
    if (!m_bJobsLaunched) {
        CleanuponFail();
        return;
    }
    switch (static_cast<int>(m_eTargetStatus)) {
    case 1:
        BuildBase();
        BuildTexture(m_eTargetStatus, pStream);
        DeleteSurfacesAndBuffers(true, false, true);
        break;
    case 3:
        if (m_eStatus != kStatusCacheOnly) {
            DeleteSurfacesAndBuffers(true, false, true);
            BuildTexture(m_eTargetStatus, pStream);
        }
        // Falls through: the base is rebuilt for both 3 and 4.
    case 4:
        BuildBase();
        break;
    case 5:
        field_188 = 0;
        field_184 = 0;
        field_18c = 0;
        RemoveBase();
        BuildBase();
        if (m_eStatus != kStatusBaseOnly) {
            BuildTexture(m_eTargetStatus, pStream);
        }
        break;
    }
    m_eStatus = m_eTargetStatus;
    InterlockedDecrement(&m_lPendingJobs);
    m_uBuiltFrame = uFrame;
}

/// 0x08916f30
float TerrainCell::CalcMaxWidth(D3DXMATRIX* pViewProj) {
    // The four diagonals of the bounding box: aMax[i] and aMin[i] are opposite corners.
    D3DXVECTOR4 aMax[4] = {{m_fMaxX, m_fMaxY, m_fMaxZ, 1.0f},
                           {m_fMaxX, m_fMaxY, m_fMinZ, 1.0f},
                           {m_fMaxX, m_fMinY, m_fMinZ, 1.0f},
                           {m_fMaxX, m_fMinY, m_fMaxZ, 1.0f}};
    D3DXVECTOR4 aMin[4] = {{m_fMinX, m_fMinY, m_fMinZ, 1.0f},
                           {m_fMinX, m_fMinY, m_fMaxZ, 1.0f},
                           {m_fMinX, m_fMaxY, m_fMaxZ, 1.0f},
                           {m_fMinX, m_fMaxY, m_fMinZ, 1.0f}};

    float fMaxLength = 0.0f;
    for (int i = 0; i < 4; ++i) {
        D3DXVec4Transform(&aMax[i], &aMax[i], pViewProj);
        D3DXVec4Transform(&aMin[i], &aMin[i], pViewProj);
        const float fMaxX = aMax[i].x / aMax[i].w;
        const float fMaxY = aMax[i].y / aMax[i].w;
        const float fMinX = aMin[i].x / aMin[i].w;
        const float fMinY = aMin[i].y / aMin[i].w;
        const float fDx = fMaxX - fMinX;
        const float fDy = fMaxY - fMinY;
        const float fLength = sqrtf(fDy * fDy + fDx * fDx) * 0.5f;
        if (fLength > fMaxLength) {
            fMaxLength = fLength;
        }
    }
    return fMaxLength * static_cast<float>(FireGrafix::CRenderer9::m_nScreenWidth) * 0.5f;
}

/// 0x08917166
unsigned int TerrainCell::CalcMIPLevelNeeded(D3DXMATRIX* pViewProj) {
    const float fWidth = CalcMaxWidth(pViewProj);
    unsigned int uSize = 0x200;
    if (!(fWidth > 256.0f)) {
        uSize = 0x100;
        if (!(fWidth > 128.0f)) {
            uSize = fWidth > 64.0f ? 0x80 : 0x40;
        }
    }
    const unsigned int uCacheSize = GraphicsSettings::TerrainSettings::g_iCacheSize;
    if (uSize < uCacheSize) {
        uSize = uCacheSize;
    }
    return uSize;
}

/// 0x089171d6
float TerrainCell::CalcTessLODNeeded(D3DXMATRIX* pViewProj) {
    const float fWidth = CalcMaxWidth(pViewProj);
    const float fPixelsPerTriangle = static_cast<float>(static_cast<double>(GraphicsSettings::TerrainSettings::g_iTessellationPixelsPerTri));
    const float fPatchDivisions = static_cast<float>(static_cast<double>(GraphicsSettings::TerrainSettings::g_iTessellationPatchDivisions));
    const float fPatchesAcross = ((fWidth * 0.5f) / fPixelsPerTriangle) / fPatchDivisions;

    const int iLongSide = m_iHeight <= m_iWidth ? m_iWidth : m_iHeight;
    const double dDivisor = 1.0f <= fPatchesAcross ? static_cast<double>(fPatchesAcross) : 1.0;
    float fLod = static_cast<float>(log(static_cast<double>(iLongSide - 1) / dDivisor) * kLog2E) + 0.5f;
    if (4.0f <= fLod) {
        fLod = 4.0f;
    }
    if (fLod <= -2.0f) {
        fLod = -2.0f;
    }
    return fLod;
}

namespace {

// Constant buffer TerrainDynamicData::LockDynamic locks (0x70 bytes).
struct TerrainDynamicConstants {
    D3DXMATRIX mWorldOffset;           // +0x0
    float fCellX;                      // +0x40
    float fCellY;                      // +0x44
    float fCellOriginX;                // +0x48
    float fCellOriginY;                // +0x4c
    float fInvWidth;                   // +0x50
    float fInvHeight;                  // +0x54
    char aPad58[0x8];                  // +0x58
    float fTessRatioX;                 // +0x60
    float fTessRatioY;                 // +0x64
    float fTessLOD;                    // +0x68
    char aPad6c[0x4];                  // +0x6c
};

static_assert(sizeof(TerrainDynamicConstants) == 0x70, "terrain dynamic constants size");

// Statuses 0, 1 and 4 have no tessellated or composite surface to draw.
const unsigned int kStatusesWithoutSurface = 0x13;
const int kStatusFull = 3;

// Cell size over the size it has after iLevel halvings, and the terrain's tessellation LOD.
const auto StoreTessellationRatios = [](TerrainDynamicConstants* pConstants, TerrainCell* pCell, int iLevel) {
    int iWidth = pCell->m_iWidth;
    int iHeight = pCell->m_iHeight;
    int iReducedWidth = iWidth;
    int iReducedHeight = iHeight;
    for (int i = 0; i < iLevel; ++i) {
        iReducedWidth = (iReducedWidth >> 1) + 1;
        iReducedHeight = (iReducedHeight >> 1) + 1;
    }
    pConstants->fTessRatioX = static_cast<float>(iWidth - 1) / static_cast<float>(iReducedWidth - 1);
    pConstants->fTessRatioY = static_cast<float>(iHeight - 1) / static_cast<float>(iReducedHeight - 1);
    pConstants->fTessLOD = pCell->m_pTerrain->m_fTessLOD;
};
}  // namespace

/// 0x08917300
void TerrainCell::Render(FireGrafix::CCommandStream* pStream, D3DXMATRIX* a2, D3DXMATRIX* a3) {
    const unsigned int uStatus = static_cast<unsigned int>(m_eStatus);
    if (uStatus <= 4 && ((kStatusesWithoutSurface >> uStatus) & 1)) {
        return;
    }

    Terrain::SharedData* pShared = m_pShared;
    D3DXMATRIX kOffset;
    D3DXMatrixTranslation(&kOffset, -pShared->m_fHalfWorldWidth, -pShared->m_fHalfWorldHeight, -pShared->field_1ab8);
    if (FireGrafix::DeviceSettings::g_DeviceCaps >= 10) {
        const unsigned int uLevel = static_cast<unsigned int>(floorf(m_pTerrain->m_fTessLOD));
        if (static_cast<unsigned int>(m_iTessLevel) != uLevel) {
            RecreateTessellationSurfaces(uLevel);
        }
    }

    FireGrafix::TerrainDynamicData* pConstantSet = static_cast<FireGrafix::TerrainDynamicData*>(m_pShared->m_kTerrainShader.m_pConstantSet);
    TerrainDynamicConstants* pConstants = static_cast<TerrainDynamicConstants*>(pConstantSet->LockDynamic(pStream));
    pConstants->mWorldOffset = kOffset;
    pConstants->fCellX = static_cast<float>(m_iX);
    pConstants->fCellY = static_cast<float>(m_iY);
    pConstants->fCellOriginX = static_cast<float>(field_e4);
    pConstants->fCellOriginY = static_cast<float>(field_e8);
    pConstants->fInvWidth = 1.0f / static_cast<float>(m_iWidth + 1);
    pConstants->fInvHeight = 1.0f / static_cast<float>(m_iHeight + 1);
    StoreTessellationRatios(pConstants, this, m_iTessLevel);
    m_pShared->m_kTerrainShader.m_pConstantSet->UnlockDynamic();

    const bool bTessellated = GraphicsSettings::TerrainSettings::g_uTerrainTessLevel != 0;
    const unsigned short usPass = bTessellated ? 1 : 3;
    const bool bUseTessellation = bTessellated && GraphicsSettings::TerrainSettings::g_bUseTessellation != 0;
    field_1d4 = FireGrafix::g_pRenderer->m_uFrame;
    unsigned int* puSurface;
    unsigned int* puTessSurface;
    if (static_cast<int>(m_eStatus) == kStatusFull) {
        puSurface = &m_uSurface0;
        puTessSurface = &m_uTessSurface0;
    } else {
        puSurface = &m_uSurface1;
        puTessSurface = &m_uTessSurface1;
    }
    if (bUseTessellation) {
        puSurface = puTessSurface;
    }
    pStream->DrawSurface(*puSurface, m_pShared->m_kTerrainShader.m_hInstance, m_bHasSpecular == 0 ? usPass : 0);
}

/// 0x0891759c
void TerrainCell::RenderOverlay(FireGrafix::CCommandStream* pStream, D3DXMATRIX* a2, D3DXMATRIX* a3) {
    const unsigned int uStatus = static_cast<unsigned int>(m_eStatus);
    if (uStatus <= 4 && ((kStatusesWithoutSurface >> uStatus) & 1)) {
        return;
    }

    Terrain::SharedData* pShared = m_pShared;
    D3DXMATRIX kOffset;
    D3DXMatrixTranslation(&kOffset, -pShared->m_fHalfWorldWidth, -pShared->m_fHalfWorldHeight, -pShared->field_1ab8);

    FireGrafix::TerrainDynamicData* pConstantSet = static_cast<FireGrafix::TerrainDynamicData*>(m_pShared->m_kTerrainShader.m_pConstantSet);
    TerrainDynamicConstants* pConstants = static_cast<TerrainDynamicConstants*>(pConstantSet->LockDynamic(pStream));
    pConstants->mWorldOffset = kOffset;
    pConstants->fCellX = static_cast<float>(m_iX);
    pConstants->fCellY = static_cast<float>(m_iY);
    pConstants->fCellOriginX = static_cast<float>(field_e4);
    pConstants->fCellOriginY = static_cast<float>(field_e8);
    StoreTessellationRatios(pConstants, this, m_iOverlayTessLevel);
    m_pShared->m_kTerrainShader.m_pConstantSet->UnlockDynamic();

    const bool bTessellated = GraphicsSettings::TerrainSettings::g_uTerrainTessLevel != 0;
    const unsigned short usPass = bTessellated ? 2 : 4;
    const bool bUseTessellation = bTessellated && GraphicsSettings::TerrainSettings::g_bUseTessellation != 0;
    field_1d4 = FireGrafix::g_pRenderer->m_uFrame;
    unsigned int* puSurface = static_cast<int>(m_eStatus) == kStatusFull ? &m_uSurface0 : &m_uSurface1;
    if (bUseTessellation) {
        puSurface = &m_uOverlayTessSurface;
    }
    pStream->DrawSurface(*puSurface, m_pShared->m_kTerrainShader.m_hInstance, usPass);
}

/// 0x08917780
void TerrainCell::CalcTexCoords(FGXVector2* pTexCoord, int iProjection, FGXVector3& kPos) const {
    if (iProjection >= static_cast<int>(m_pShared->m_nProjections)) {
        pTexCoord->x = 0.0f;
        pTexCoord->y = 0.0f;
        return;
    }

    const float fU = kPos.x + m_pShared->m_fHalfWorldWidth;
    const float fV = kPos.y + m_pShared->m_fHalfWorldHeight;
    const float fW = kPos.z + m_pShared->field_1ab8;
    const float fRange = WorldGlobals::g_WorldMax.x - WorldGlobals::g_WorldMin.x;
    const float fTextureScale = m_pShared->field_e10[m_pShared->m_aiProjectionTexture[iProjection]];
    const int iAxis = m_pShared->m_aiProjectionAxis[iProjection];

    if (iAxis == 0) {
        const float fScale = static_cast<float>(static_cast<int>(fTextureScale * 0.33f * fRange)) / fRange;
        pTexCoord->x = fU * fScale;
        pTexCoord->y = -(fV * fScale);
        return;
    }

    const float fScale = static_cast<float>(static_cast<int>(0.66f * fTextureScale * fRange)) / fRange;
    if (iAxis == 1) {
        pTexCoord->x = fU * fScale;
    } else {
        pTexCoord->x = fV * fScale;
    }
    pTexCoord->y = fW * fScale;
}

/// 0x08917f50
bool TerrainCell::IntersectLine(FGXVector2* pStart, FGXVector2* pEnd) {
    const float fMinX = m_fMinX;
    const float fMaxX = m_fMaxX;
    const float fMinY = m_fMinY;
    const float fMaxY = m_fMaxY;
    const bool bStartInside = !(pStart->x < fMinX || fMaxX < pStart->x || pStart->y < fMinY || fMaxY < pStart->y);
    const bool bEndInside = !(pEnd->x < fMinX || fMaxX < pEnd->x || pEnd->y < fMinY || fMaxY < pEnd->y);
    if (bStartInside || bEndInside) {
        return true;
    }

    // Box edges counter-clockwise from the min corner.
    FGXVector2 kCorner[4];
    kCorner[0].x = fMinX;
    kCorner[0].y = fMinY;
    kCorner[1].x = fMaxX;
    kCorner[1].y = fMinY;
    kCorner[2].x = fMaxX;
    kCorner[2].y = fMaxY;
    kCorner[3].x = fMinX;
    kCorner[3].y = fMaxY;

    FGXVector2 kHit;
    if (FGXIntersectLineSegments2D(kHit, *pStart, *pEnd, kCorner[0], kCorner[1]) ||
        FGXIntersectLineSegments2D(kHit, *pStart, *pEnd, kCorner[1], kCorner[2]) ||
        FGXIntersectLineSegments2D(kHit, *pStart, *pEnd, kCorner[2], kCorner[3])) {
        return true;
    }
    return FGXIntersectLineSegments2D(kHit, *pStart, *pEnd, kCorner[3], kCorner[0]);
}
