/// origin guess
#include <float.h>
#include <Windows.h>
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/TerrainStencil.h"
#include "FirePlace/FireWorks/ForestVis.h"

/// 0x0866fed4
ForestVis::ForestVis(HEXVec_i const& kHex, ForestVis::FOREST_VIS_TYPE eType) {
    m_bUpToDate = true;
    m_fBoundsLeft = 0.0f;
    m_fBoundsTop = 0.0f;
    m_fBoundsRight = 0.0f;
    m_fBoundsBottom = 0.0f;
    m_hVertexBuffer = 0;
    m_hIndexBuffer = 0;
    m_hSurface = 0;
    m_uiTreeList = 0;
    m_Hex = kHex;
    m_eType = eType;
}

/// 0x08670342
UNK_RET ForestVis::AllocateShaders(unsigned int uiNumShaders, ForestShaderPool& kPool) {
    DeleteShaders(kPool);
    for (unsigned int i = 0; i < uiNumShaders; ++i) {
        int iShader = -1;
        if (kPool.m_uiFreeHead != 0xffffffffu) {
            iShader = static_cast<int>(kPool.m_uiFreeHead);
            unsigned int* puiLink = reinterpret_cast<unsigned int*>(
                reinterpret_cast<unsigned char*>(kPool.m_pShaders) + static_cast<unsigned int>(iShader) * 0x34u + 0x30u);
            kPool.m_uiFreeHead = *puiLink;
            *puiLink = 0xffffffffu;
        }
        PushShaderId(m_ShaderIds, static_cast<unsigned int>(iShader));
    }
}

/// 0x0866ff5e
void ForestVis::BuildBuffers(unsigned int hVertexBuffer, unsigned int hIndexBuffer, unsigned int hSurface, FFastVector<ForestVisTreeData, false, 49u, 0u, BaseVector<ForestVisTreeData, false>::FDefaultFastVectorAllocator> const& kTrees, unsigned int uiNumShaders, ForestShaderPool& kPool) {
    AllocateShaders(uiNumShaders, kPool);
    m_hVertexBuffer = hVertexBuffer;
    m_hIndexBuffer = hIndexBuffer;
    m_hSurface = hSurface;
    if (uiNumShaders == 0) {
        return;
    }
    const float fTypeScale = (static_cast<unsigned int>(m_eType) - 3u < 2u) ? 0.75f : 1.0f;
    const float fMinHeight = WorldGlobals::g_TerrainScale.z * 50.583656f;
    for (unsigned int uiSlot = 0; uiSlot < uiNumShaders; ++uiSlot) {
        const unsigned int uiShader = m_ShaderIds[uiSlot];
        if (uiShader >= 2000u) {
            continue;
        }
        FireGrafix::ForestShader* pShader = &kPool.m_pShaders[uiShader];
        float* pRecords = static_cast<float*>(pShader->m_pForestData->LockMutable(0));
        for (unsigned int uiTree = 0; uiTree < 90u; ++uiTree) {
            ForestVisTreeData const& kTree = kTrees.begin()[uiSlot * 90u + uiTree];
            HEXVec kHex;
            kHex.x = static_cast<float>(m_Hex.x) + kTree.m_fHexX;
            kHex.y = static_cast<float>(m_Hex.y) + kTree.m_fHexY;
            kHex.z = -kHex.x - kHex.y;
            kHex.w = 1.0f;
            FGXVector4 kWorld = MapGlobals::g_pTerrainSampler->GetWorldLocation(kHex);
            D3DXVECTOR4 kTexIn = {kHex.x, kHex.y, kHex.z, 1.0f};
            D3DXVECTOR4 kTex;
            D3DXVec4Transform(&kTex, &kTexIn, reinterpret_cast<D3DXMATRIX const*>(WorldGlobals::g_HexToTex));
            float fNormalX = 0.0f;
            float fNormalY = 0.0f;
            float fNormalZ = 1.0f;
            if (g_TerrainSystem.m_pTerrain != nullptr) {
                FGXInt2 kCell = {static_cast<int>(kTex.x), static_cast<int>(kTex.y)};
                FGXVector3 kNormal = MapGlobals::g_pTerrainSampler->GetWorldNormal(kCell);
                fNormalX = kNormal.x * 0.0f;
                fNormalY = kNormal.y * 0.0f;
                fNormalZ = kNormal.z;
            }
            float fScale = kTree.m_fScale * fTypeScale;
            FGXVector2 kWorldXY = {kWorld.x, kWorld.y};
            unsigned char ucStencil = g_TerrainStencil.SampleWorldPosition(kWorldXY, 7);
            if (!(ucStencil == 0 && 0.9f <= fNormalY + fNormalX + fNormalZ && fMinHeight <= kWorld.z)) {
                fScale = 0.0f;
            }
            float* pRecord = pRecords + uiTree * 4u;
            pRecord[0] = kWorld.x;
            pRecord[1] = kWorld.y;
            pRecord[2] = kWorld.z;
            pRecord[3] = fScale;
        }
        pShader->m_pForestData->Unlock(0);
    }
}

/// 0x086703c6
void ForestVis::BuildShadows(FFastVector<ForestVisTreeData, false, 49u, 0u, BaseVector<ForestVisTreeData, false>::FDefaultFastVectorAllocator> const& kTrees) {
    if (m_DecalIds.size() != 0) {
        return;
    }
    TerrainDecalLayer& kLayer = g_TerrainSystem.m_DecalLayer;
    kLayer.m_scratchVertices.m_uiNumVertices = 0;
    kLayer.m_scratchIndices.m_uiNumIndices = 0;
    m_fBoundsTop = FLT_MAX;
    m_fBoundsLeft = FLT_MAX;
    m_fBoundsBottom = -FLT_MAX;
    m_fBoundsRight = -FLT_MAX;
    if (kTrees.size() != 0) {
        const float fHalfSize = (static_cast<unsigned int>(m_eType) - 3u < 2u) ? 3.6f : 6.0f;
        const float fMinHeight = WorldGlobals::g_TerrainScale.z * 50.583656f;
        unsigned short usBaseVertex = 0;
        for (unsigned int uiTree = 0; uiTree < kTrees.size(); ++uiTree) {
            ForestVisTreeData const& kTree = kTrees.begin()[uiTree];
            HEXVec kHex;
            kHex.x = static_cast<float>(m_Hex.x) + kTree.m_fHexX;
            kHex.y = static_cast<float>(m_Hex.y) + kTree.m_fHexY;
            kHex.z = -kHex.x - kHex.y;
            kHex.w = 1.0f;
            D3DXVECTOR4 kTexIn = {kHex.x, kHex.y, kHex.z, 1.0f};
            D3DXVECTOR4 kTex;
            D3DXVec4Transform(&kTex, &kTexIn, reinterpret_cast<D3DXMATRIX const*>(WorldGlobals::g_HexToTex));
            FGXInt2 kCell = {static_cast<int>(kTex.x), static_cast<int>(kTex.y)};
            m_fBoundsLeft = kTex.x < m_fBoundsLeft ? kTex.x : m_fBoundsLeft;
            m_fBoundsTop = kTex.y < m_fBoundsTop ? kTex.y : m_fBoundsTop;
            m_fBoundsRight = kTex.x > m_fBoundsRight ? kTex.x : m_fBoundsRight;
            m_fBoundsBottom = kTex.y > m_fBoundsBottom ? kTex.y : m_fBoundsBottom;
            float fNormalX = 0.0f;
            float fNormalY = 0.0f;
            float fNormalZ = 1.0f;
            if (g_TerrainSystem.m_pTerrain != nullptr) {
                FGXVector3 kNormal = MapGlobals::g_pTerrainSampler->GetWorldNormal(kCell);
                fNormalX = kNormal.x * 0.0f;
                fNormalY = kNormal.y * 0.0f;
                fNormalZ = kNormal.z;
            }
            FGXVector4 kWorld = MapGlobals::g_pTerrainSampler->GetWorldLocation(kHex);
            FGXVector2 kWorldXY = {kWorld.x, kWorld.y};
            unsigned char ucStencil = g_TerrainStencil.SampleWorldPosition(kWorldXY, 7);
            if (ucStencil == 0 && 0.9f <= fNormalY + fNormalX + fNormalZ && fMinHeight <= kWorld.z && kTree.m_fScale != 0.0f) {
                DecalVertex aVertices[4] = {
                    {kTex.x - fHalfSize, kTex.y - fHalfSize, 0.0f, 1.0f},
                    {kTex.x + fHalfSize, kTex.y - fHalfSize, 1.0f, 1.0f},
                    {kTex.x + fHalfSize, kTex.y + fHalfSize, 1.0f, 0.0f},
                    {kTex.x - fHalfSize, kTex.y + fHalfSize, 0.0f, 0.0f},
                };
                AppendDecalVertices(kLayer.m_scratchVertices, aVertices, 4);
                unsigned short ausIndices[6] = {
                    usBaseVertex,
                    static_cast<unsigned short>(usBaseVertex + 1),
                    static_cast<unsigned short>(usBaseVertex + 2),
                    usBaseVertex,
                    static_cast<unsigned short>(usBaseVertex + 2),
                    static_cast<unsigned short>(usBaseVertex + 3),
                };
                AppendDecalIndices(kLayer.m_scratchIndices, ausIndices, 6);
                usBaseVertex = static_cast<unsigned short>(usBaseVertex + 4);
            }
        }
    }
    if (kLayer.m_scratchIndices.m_uiNumIndices != 0) {
        unsigned int uiDecal = kLayer.CreateDecal(3u, kLayer.m_scratchVertices.m_pVertices, kLayer.m_scratchVertices.m_uiNumVertices, kLayer.m_scratchIndices.m_pIndices, kLayer.m_scratchIndices.m_uiNumIndices / 3, true);
        PushDecalId(m_DecalIds, uiDecal);
    }
}
