/// origin guess
#include "Civ5/Src/App/Graphics/CoastMeshBlendJob.h"
#include "Civ5/Src/App/Graphics/CellBlitJob.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include <algorithm>
#include <math.h>

/// 0x088eaed4
void CoastMeshBlendJob::Execute(unsigned int a1) {
    if (m_iHeapTrample != 0) {
        Fun_08665a68(0x80, 1, 0x2b, 0x10b);
    }
    Fun_088eaf32(&m_kMapSize, &m_kBatch, &m_kParams);
}

/// 0x088eaf32
void Fun_088eaf32(FGXInt2 const* pMapSize, CoastMeshBatch const* pBatch, CoastMeshBlendParams const* pParams) {
    unsigned short const* pIndices = pBatch->m_pIndices;
    for (unsigned int uiTri = 0; uiTri < pBatch->m_uiNumTriangles; ++uiTri, pIndices += 3) {
        CoastMeshVertex const& kV0 = pBatch->m_pVertices[pIndices[0]];
        CoastMeshVertex const& kV1 = pBatch->m_pVertices[pIndices[1]];
        CoastMeshVertex const& kV2 = pBatch->m_pVertices[pIndices[2]];

        float fOriginX = floorf(std::min(kV0.m_vPos.x, std::min(kV1.m_vPos.x, kV2.m_vPos.x)) + 0.5f);
        float fOriginY = floorf(std::min(kV0.m_vPos.y, std::min(kV1.m_vPos.y, kV2.m_vPos.y)) + 0.5f);
        float fMaxX = std::max(kV0.m_vPos.x, std::max(kV1.m_vPos.x, kV2.m_vPos.x));
        float fMaxY = std::max(kV0.m_vPos.y, std::max(kV1.m_vPos.y, kV2.m_vPos.y));

        // Vertices relative to the first covered texel.
        float x0 = kV0.m_vPos.x - fOriginX;
        float y0 = kV0.m_vPos.y - fOriginY;
        float x1 = kV1.m_vPos.x - fOriginX;
        float y1 = kV1.m_vPos.y - fOriginY;
        float x2 = kV2.m_vPos.x - fOriginX;
        float y2 = kV2.m_vPos.y - fOriginY;

        float fEdge0 = x1 * y2 - y1 * x2;
        float fArea2 = x0 * (y1 - y2) + y0 * (x2 - x1) + fEdge0;
        if (fabsf(fArea2) < 1.0e-5) {
            continue;
        }
        int iFirstY = static_cast<int>(fOriginY);
        int iEndY = static_cast<int>(fMaxY) + 1;
        if (iFirstY >= iEndY) {
            continue;
        }
        int iFirstX = static_cast<int>(fOriginX);
        int iEndX = static_cast<int>(fMaxX) + 1;

        // Barycentric weight of vertex k at local (dx, dy): (A_k * dx + B_k * dy + C_k) / fArea2.
        float fInvArea2 = 1.0f / fArea2;
        float fC0 = fEdge0 * fInvArea2;
        float fC1 = (x2 * y0 - x0 * y2) * fInvArea2;
        float fC2 = (x0 * y1 - y0 * x1) * fInvArea2;

        // The step is fInvArea2 per texel, so A_k and B_k need no division.
        float fDY = 0.0f;
        for (int iY = iFirstY; iY != iEndY; ++iY, fDY += fInvArea2) {
            if (iFirstX >= iEndX) {
                continue;
            }
            float fRow0 = (x2 - x1) * fDY;
            float fRow1 = (x0 - x2) * fDY;
            float fRow2 = (x1 - x0) * fDY;
            float fDX = 0.0f;
            for (int iX = iFirstX; iX != iEndX; ++iX, fDX += fInvArea2) {
                float fW0 = (y1 - y2) * fDX + fRow0 + fC0;
                if (fW0 < 0.0f) {
                    continue;
                }
                float fW1 = (y2 - y0) * fDX + fRow1 + fC1;
                if (fW1 < 0.0f) {
                    continue;
                }
                float fW2 = (y0 - y1) * fDX + fRow2 + fC2;
                if (fW2 < 0.0f) {
                    continue;
                }
                if (static_cast<unsigned int>(iY) >= static_cast<unsigned int>(pParams->m_kMapSize.y)) {
                    continue;
                }
                FGXInt2 kTexel;
                kTexel.x = iX;
                kTexel.y = iY;

                cvTerrainSampler* pSampler = pParams->m_pSampler;
                FGXInt2 kWrapped = pSampler->GetWrappedPosition(kTexel);
                unsigned short usOldHeight = Fun_08679d0a(&pSampler->m_iHeightMapWidth, &kWrapped);
                kWrapped = pSampler->GetWrappedPosition(kTexel);
                unsigned char ucOldTerrain = Fun_087fb076(pSampler->unk_4c, &kWrapped);

                // The first vertex alone decides the triangle's terrain.
                if (kV0.m_ucTerrainIndex != 0xff) {
                    unsigned char ucTerrain = kV0.m_ucTerrainIndex;
                    kWrapped = pSampler->GetWrappedPosition(kTexel);
                    Fun_087e75d2(pSampler->unk_4c, &ucTerrain, &kWrapped);
                }

                // Quadratic triangle patch: vertex weights on the corners, edge weights doubled on the mixed terms.
                FGXVector3 const& kEdge = pParams->m_pEdgeWeights[uiTri];
                float fBlend = kV0.field_10 * fW0 * fW0 + kV1.field_10 * fW1 * fW1 + kV2.field_10 * fW2 * fW2
                    + 2.0f * (kEdge.x * fW0 * fW1) + 2.0f * (kEdge.y * fW1 * fW2) + 2.0f * (kEdge.z * fW2 * fW0);
                if (fBlend < 0.0f) {
                    fBlend = 0.0f;
                }
                float fMeshHeight = kV0.m_fHeight * fW0 + kV1.m_fHeight * fW1 + kV2.m_fHeight * fW2;
                float fOldHeight = static_cast<float>(usOldHeight) / 65535.0f;
                // No clamp: a blend above 1 or a height outside [0, 1] wraps in the 16-bit store.
                int iNewHeight = static_cast<int>(((1.0f - fBlend) * fOldHeight + fMeshHeight * fBlend) * 65535.0f);
                unsigned short usNewHeight = static_cast<unsigned short>(iNewHeight);
                kWrapped = pSampler->GetWrappedPosition(kTexel);
                Fun_087fa84a(&pSampler->m_iHeightMapWidth, &usNewHeight, &kWrapped);

                if (usNewHeight > 35000) {
                    continue;
                }
                // Compares the terrain read before the vertex terrain was written.
                for (int i = 0; i < 4; ++i) {
                    if (ucOldTerrain == pParams->m_aucFromTerrain[i]) {
                        unsigned char ucTerrain = pParams->m_aucToTerrain[i];
                        kWrapped = pParams->m_pSampler->GetWrappedPosition(kTexel);
                        Fun_087e75d2(pParams->m_pSampler->unk_4c, &ucTerrain, &kWrapped);
                    }
                }
            }
        }
    }
}
