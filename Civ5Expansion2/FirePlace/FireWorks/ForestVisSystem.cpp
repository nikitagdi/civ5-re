/// origin guess
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/TerrainData.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireEngine2/FTextureResource.h"
#include "FirePlace/FireWorks/FSingleton.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FXmlDocument.h"
#include "FirePlace/FireWorks/FXmlElement.h"
#include "FirePlace/FireWorks/ForestVisSystem.h"

// Trees per ForestShader constant block; the XML counts are rounded to a multiple of it.
static const float kTreesPerShader = 90.0f;
static const unsigned int kNumTreeLists = 20;
static const unsigned int kEndOfVisList = 0xfffffff;
static const unsigned int kMaxPlacementTries = 20;
// Trees beyond this index are placed with scale 0.
static const int kLastVisibleTree = 350;
// The tile mask covers a 96x96 hex-space square with 128 texels per side.
static const float kTileExtent = 96.0f;
static const float kTileMaskMax = 127.0f;
// Each static VB/IB holds 90 variants of one random mesh, one per slot of the shader constant block.
static const unsigned int kNumTreeSlots = 90;
static const unsigned int kNumContinentStyles = 5;
static const unsigned int kForestVertexFormat = 0x11;
static const unsigned int kForestSurfaceType = 0x2c;
static const unsigned int kTextureLoadFlags = 0x20000;
// Colour-key system row used for each continent style.
static const unsigned int kColorKeyStyle[kNumContinentStyles] = {0, 3, 2, 4, 1};

/// 0x0866662a
UNK_RET ForestVisSystem::LoadSettingsFromXML() {
    FXmlDocument kDocument;
    FXmlElement kRoot;
    if (!kDocument.Load("Forests.xml")) {
        MessageBoxA(0, "Could not load forest xml file", "Forest error", MB_ICONEXCLAMATION);
    } else {
        kDocument.GetRootElement(kRoot);
        kRoot.GetFirstAttributeVal(m_uiNumTiles, "num_tiles", 9);
        kRoot.GetFirstAttributeVal(m_uiRandomTreesPerTile, "random_trees_per_tile", 0x15);
        kRoot.GetFirstAttributeVal(m_uiGriddedTreesPerTile, "gridded_trees_per_tile", 0x16);
        kRoot.GetFirstAttributeVal(m_fOverlayAlpha, "overlay_alpha", 0xd);
        kRoot.GetFirstAttributeVal(m_fSpaceBetween, "space_between", 0xd);
        m_uiRandomTreesPerTile = (unsigned int)(roundf((float)m_uiRandomTreesPerTile / kTreesPerShader) * kTreesPerShader);
        m_uiGriddedTreesPerTile = (unsigned int)(roundf((float)m_uiGriddedTreesPerTile / kTreesPerShader) * kTreesPerShader);
    }
}

/// 0x0866686e
UNK_RET ForestVisSystem::LoadFilePaths() {
    m_apszForestMeshFiles[0] = "forest_europe_2.gr2";
    m_apszForestMeshFiles[1] = "forest_america_2.gr2";
    m_apszForestMeshFiles[2] = "forest_asia_2.gr2";
    m_apszForestMeshFiles[3] = "forest_africa_2.gr2";
    m_apszForestMeshFiles[4] = "forest_europe_2.gr2";
    m_apszJungleMeshFiles[0] = "jungle_europe_2.gr2";
    m_apszJungleMeshFiles[1] = "jungle_america_2.gr2";
    m_apszJungleMeshFiles[2] = "jungle_asia_2.gr2";
    m_apszJungleMeshFiles[3] = "jungle_africa_2.gr2";
    m_apszJungleMeshFiles[4] = "jungle_europe_2.gr2";
    m_apszForestTextureFiles[0] = "forest_europe_2.dds";
    m_apszForestTextureFiles[1] = "forest_america_2.dds";
    m_apszForestTextureFiles[2] = "forest_asia_2.dds";
    m_apszForestTextureFiles[3] = "forest_africa_2.dds";
    m_apszForestTextureFiles[4] = "forest_europe_2.dds";
    m_apszJungleTextureFiles[0] = "jungle_europe_2.dds";
    m_apszJungleTextureFiles[1] = "jungle_america_2.dds";
    m_apszJungleTextureFiles[2] = "jungle_asia_2.dds";
    m_apszJungleTextureFiles[3] = "jungle_africa_2.dds";
    m_apszJungleTextureFiles[4] = "jungle_europe_2.dds";
    m_apszForestOverlayFiles[0] = "forest_overlay_europe_2.dds";
    m_apszForestOverlayFiles[1] = "forest_overlay_america_2.dds";
    m_apszForestOverlayFiles[2] = "forest_overlay_asia_2.dds";
    m_apszForestOverlayFiles[3] = "forest_overlay_africa_2.dds";
    m_apszForestOverlayFiles[4] = "forest_overlay_europe_2.dds";
    m_apszJungleOverlayFiles[0] = "jungle_overlay_europe_2.dds";
    m_apszJungleOverlayFiles[1] = "jungle_overlay_america_2.dds";
    m_apszJungleOverlayFiles[2] = "jungle_overlay_asia_2.dds";
    m_apszJungleOverlayFiles[3] = "jungle_overlay_africa_2.dds";
    m_apszJungleOverlayFiles[4] = "jungle_overlay_europe_2.dds";
}

/// 0x08666d64
UNK_RET ForestVisSystem::BuildTreeBuffers() {
    // Copies the 0x20 mesh vertex bytes into the 0x24-byte VB vertex and appends the slot byte.
    auto WriteSlotVertices = [](unsigned char* pDst, ForestVisMeshVertex const* pSrc, unsigned int uiNum, unsigned char ucSlot) {
        for (unsigned int uiVertex = 0; uiVertex < uiNum; ++uiVertex) {
            memcpy(pDst, &pSrc[uiVertex], sizeof(ForestVisMeshVertex));
            pDst[sizeof(ForestVisMeshVertex)] = ucSlot;
            pDst += 0x24;
        }
    };
    // Sampler state 5 goes with slots 4 and 5 and the second colour key.
    auto FillTreeSurface = [](FireGrafix::FGXSurface* pSurface, char const* pszTree, char const* pszOverlay, unsigned int uiStyle) {
        pSurface->field_18[0] = (unsigned short)FTextureResourceMgr::LoadTextureHandle(pszTree, kTextureLoadFlags, 0);
        pSurface->field_38[0] = 0;
        pSurface->field_18[1] = (unsigned short)FTextureResourceMgr::LoadTextureHandle(pszOverlay, kTextureLoadFlags, 0);
        pSurface->field_38[1] = 0;
        pSurface->field_18[2] = (unsigned short)RenderGlobals::g_TexHandles[0];
        pSurface->field_38[2] = 0;
        pSurface->field_18[3] = (unsigned short)RenderGlobals::g_TexHandles[1];
        pSurface->field_38[3] = 0;
        pSurface->field_18[4] = (unsigned short)RenderGlobals::g_TexHandles[4];
        pSurface->field_38[4] = 5;
        pSurface->field_18[6] = (unsigned short)RenderGlobals::g_TexHandles[2];
        pSurface->field_38[6] = 0;
        pSurface->field_18[7] = (unsigned short)RenderGlobals::g_TexHandles[3];
        pSurface->field_38[7] = 0;
        pSurface->field_18[5] = (unsigned short)g_ColorKeySystem.m_auKeyHandles[0][uiStyle];
        pSurface->field_38[5] = 5;
        pSurface->field_28[0] = (unsigned short)g_ColorKeySystem.m_auKeyHandles[1][uiStyle];
        pSurface->field_48[0] = 5;
        pSurface->field_60 = 9;
    };
    for (unsigned int uiStyle = 0; uiStyle < kNumContinentStyles; ++uiStyle) {
        unsigned int uiMaxForestVerts = 0;
        unsigned int uiMaxForestIndices = 0;
        for (unsigned int uiMesh = 0; uiMesh < m_aForestMeshes[uiStyle].m_uiNumMeshes; ++uiMesh) {
            ForestVisMeshRange const& kRange = m_MeshRanges[m_aForestMeshes[uiStyle].m_uiFirstMesh + uiMesh];
            if (kRange.m_uiNumVertices > uiMaxForestVerts) uiMaxForestVerts = kRange.m_uiNumVertices;
            if (kRange.m_uiNumIndices > uiMaxForestIndices) uiMaxForestIndices = kRange.m_uiNumIndices;
        }
        unsigned int uiMaxJungleVerts = 0;
        unsigned int uiMaxJungleIndices = 0;
        for (unsigned int uiMesh = 0; uiMesh < m_aJungleMeshes[uiStyle].m_uiNumMeshes; ++uiMesh) {
            ForestVisMeshRange const& kRange = m_MeshRanges[m_aJungleMeshes[uiStyle].m_uiFirstMesh + uiMesh];
            if (kRange.m_uiNumVertices > uiMaxJungleVerts) uiMaxJungleVerts = kRange.m_uiNumVertices;
            if (kRange.m_uiNumIndices > uiMaxJungleIndices) uiMaxJungleIndices = kRange.m_uiNumIndices;
        }
        Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, m_ahForestVB[uiStyle]);
        Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, m_ahJungleVB[uiStyle]);
        Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, m_ahForestIB[uiStyle]);
        Fun_0866fb7e(FireGrafix::CRenderer9::m_IBList, m_ahJungleIB[uiStyle]);
        FireGrafix::g_pRenderer->ReleaseSurface(m_ahForestSurface[uiStyle]);
        FireGrafix::g_pRenderer->ReleaseSurface(m_ahJungleSurface[uiStyle]);
        m_ahForestVB[uiStyle] = FireGrafix::g_pRenderer->CreateStaticVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(kForestVertexFormat), uiMaxForestVerts * kNumTreeSlots, 0, 0);
        m_ahJungleVB[uiStyle] = FireGrafix::g_pRenderer->CreateStaticVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(kForestVertexFormat), uiMaxJungleVerts * kNumTreeSlots, 0, 0);
        m_ahForestIB[uiStyle] = FireGrafix::g_pRenderer->CreateStaticIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(0), uiMaxForestIndices * kNumTreeSlots, 0, 0);
        m_ahJungleIB[uiStyle] = FireGrafix::g_pRenderer->CreateStaticIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(0), uiMaxJungleIndices * kNumTreeSlots, 0, 0);
        unsigned char* pForestVB = static_cast<unsigned char*>(FireGrafix::g_pRenderer->LockStaticVB(m_ahForestVB[uiStyle], 0));
        unsigned char* pJungleVB = static_cast<unsigned char*>(FireGrafix::g_pRenderer->LockStaticVB(m_ahJungleVB[uiStyle], 0));
        unsigned short* pForestIB = static_cast<unsigned short*>(FireGrafix::g_pRenderer->LockStaticIB(m_ahForestIB[uiStyle], 0));
        unsigned short* pJungleIB = static_cast<unsigned short*>(FireGrafix::g_pRenderer->LockStaticIB(m_ahJungleIB[uiStyle], 0));
        unsigned int uiForestVerts = 0;
        unsigned int uiForestIndices = 0;
        unsigned int uiJungleVerts = 0;
        unsigned int uiJungleIndices = 0;
        // Slot i gets a random mesh of the style; the shader picks the slot by the vertex byte.
        for (unsigned int uiSlot = 0; uiSlot < kNumTreeSlots; ++uiSlot) {
            unsigned int uiForestMesh = rand_wincompatible() % m_aForestMeshes[uiStyle].m_uiNumMeshes + m_aForestMeshes[uiStyle].m_uiFirstMesh;
            unsigned int uiJungleMesh = rand_wincompatible() % m_aJungleMeshes[uiStyle].m_uiNumMeshes + m_aJungleMeshes[uiStyle].m_uiFirstMesh;
            ForestVisMeshRange const& kForest = m_MeshRanges[uiForestMesh];
            ForestVisMeshRange const& kJungle = m_MeshRanges[uiJungleMesh];
            WriteSlotVertices(pForestVB + uiForestVerts * 0x24, &m_Vertices[kForest.m_uiFirstVertex], kForest.m_uiNumVertices, (unsigned char)uiSlot);
            WriteSlotVertices(pJungleVB + uiJungleVerts * 0x24, &m_Vertices[kJungle.m_uiFirstVertex], kJungle.m_uiNumVertices, (unsigned char)uiSlot);
            for (unsigned int uiIndex = 0; uiIndex < kForest.m_uiNumIndices; ++uiIndex) {
                pForestIB[uiForestIndices + uiIndex] = (unsigned short)(m_Indices[kForest.m_uiFirstIndex + uiIndex] + uiForestVerts);
            }
            for (unsigned int uiIndex = 0; uiIndex < kJungle.m_uiNumIndices; ++uiIndex) {
                pJungleIB[uiJungleIndices + uiIndex] = (unsigned short)(m_Indices[kJungle.m_uiFirstIndex + uiIndex] + uiJungleVerts);
            }
            uiForestVerts += kForest.m_uiNumVertices;
            uiForestIndices += kForest.m_uiNumIndices;
            uiJungleVerts += kJungle.m_uiNumVertices;
            uiJungleIndices += kJungle.m_uiNumIndices;
        }
        FireGrafix::g_pRenderer->UnlockVB(m_ahForestVB[uiStyle], 0, uiForestVerts);
        FireGrafix::g_pRenderer->UnlockVB(m_ahJungleVB[uiStyle], 0, uiJungleVerts);
        FireGrafix::g_pRenderer->UnlockIB(m_ahForestIB[uiStyle], 0, uiForestIndices);
        FireGrafix::g_pRenderer->UnlockIB(m_ahJungleIB[uiStyle], 0, uiJungleIndices);
        FireGrafix::FGXSurface* pForestSurface;
        FireGrafix::FGXSurface* pJungleSurface;
        FireGrafix::g_pRenderer->CreateSurface(&pForestSurface, &m_ahForestSurface[uiStyle], kForestSurfaceType);
        FireGrafix::g_pRenderer->CreateSurface(&pJungleSurface, &m_ahJungleSurface[uiStyle], kForestSurfaceType);
        pForestSurface->m_hVB = m_ahForestVB[uiStyle];
        pJungleSurface->m_hVB = m_ahJungleVB[uiStyle];
        pForestSurface->m_hIB = m_ahForestIB[uiStyle];
        pJungleSurface->m_hIB = m_ahJungleIB[uiStyle];
        unsigned int uiColorKeyStyle = kColorKeyStyle[uiStyle];
        FillTreeSurface(pForestSurface, m_apszForestTextureFiles[uiStyle], m_apszForestOverlayFiles[uiStyle], uiColorKeyStyle);
        FillTreeSurface(pJungleSurface, m_apszJungleTextureFiles[uiStyle], m_apszJungleOverlayFiles[uiStyle], uiColorKeyStyle);
        pForestSurface->field_c = uiForestIndices / 3;
        pJungleSurface->field_c = uiJungleIndices / 3;
    }
}

/// 0x086676d2
UNK_RET ForestVisSystem::LoadTreeTiles() {
    unsigned int uiFirstTile = m_TileMasks.size();
    m_TileMasks.setsize(uiFirstTile + m_uiNumTiles);
    for (unsigned int uiTile = 0; uiTile < m_uiNumTiles; ++uiTile) {
        FStringA kPath;
        FStringA::FStringAFixedBuffer<256> kPathBuffer(kPath);
        kPath.Format("forest_tile%i.dds", uiTile + 1);
        FileLoadBuffer kBuffer;
        Fun_08669c48(&kBuffer, kPath);
        {
            FireGrafix::TextureData kTexture(kBuffer.m_pData, Fun_08669d48(&kBuffer), 0x15);
            if (kTexture.m_nMips == 0) {
                MessageBoxA(0, "Forest tile texture missing", "Forest Error", MB_OK);
                exit(0);
            }
            if (kTexture.m_ulFormat != 0x32) {
                MessageBoxA(0, "Incorrect tile texture format (L8 required)", "Forest Error", MB_OK);
                exit(0);
            }
            FireGrafix::TextureData::MipLevel const& kMip = kTexture.m_aMips[0];
            unsigned char* pDst = m_TileMasks[uiFirstTile + uiTile].m_aucMask;
            for (unsigned int uiRow = 0; uiRow < kMip.m_uWidth; ++uiRow) {
                for (unsigned int uiCol = 0; uiCol < kMip.m_uHeight; ++uiCol) {
                    pDst[1 + uiCol] = kMip.m_apFaces[0][uiCol + kMip.m_uPitch * uiRow];
                }
                pDst += kMip.m_uHeight;
            }
        }
        Fun_08669d90(&kBuffer);
    }
}

/// 0x08667ba6
UNK_RET ForestVisSystem::BuildTreeLists() {
    for (unsigned int uiList = 0; uiList < m_TreeLists.size(); ++uiList) {
        m_TreeLists[uiList].clear();
    }
    m_TreeLists.clear();
    for (unsigned int uiList = 0; uiList < kNumTreeLists; ++uiList) {
        FFastVector<ForestVisTreeData, false, 49u, 0u, BaseVector<ForestVisTreeData, false>::FDefaultFastVectorAllocator> kEmpty;
        m_TreeLists.push_back(kEmpty);
        AddRandomLocationsInTile(m_TreeLists[uiList], uiList % m_uiNumTiles);
        ForestVisTreeData* pFirst = m_TreeLists[uiList].begin();
        Fun_0866a1c5(pFirst, pFirst + m_TreeLists[uiList].size(), (int)m_TreeLists[uiList].size());
    }
}

/// 0x08667b7e
UNK_RET ForestVisSystem::StartupGame() {
    BuildTreeLists();
    m_bGameStarted = true;
}

/// 0x08667ca0
UNK_RET ForestVisSystem::CreateTreeShadows() {
    for (unsigned int uiNode = m_uiFirstVis; uiNode != kEndOfVisList; uiNode = m_pVisNodes[uiNode].m_uiNext) {
        ForestVis& kVis = m_pVisNodes[uiNode].m_Vis;
        kVis.BuildShadows(m_TreeLists[kVis.m_uiTreeList]);
    }
}

/// 0x08667d04
UNK_RET ForestVisSystem::ShutdownGame() {
    DestroyTreeLists();
    ReleaseAllVises();
    m_bGameStarted = false;
}

/// 0x086682e8
UNK_RET ForestVisSystem::Update() {
    if (!m_bGameStarted) {
        return;
    }
    for (unsigned int uiNode = m_uiFirstVis; uiNode != kEndOfVisList; uiNode = m_pVisNodes[uiNode].m_uiNext) {
        ForestVis& kVis = m_pVisNodes[uiNode].m_Vis;
        HEXVec kHexPos = {(float)kVis.m_Hex.x, (float)kVis.m_Hex.y, (float)kVis.m_Hex.z, 1.0f};
        FGXVector4 kWorldPos = MapGlobals::g_pTerrainSampler->GetWorldLocation(kHexPos);
        bool bHasBuffers = kVis.m_hSurface != 0 && kVis.m_ShaderIds.size() != 0;
        bool bOffScreen = true;
        if (FrameGlobals::g_nCameras != 0) {
            bool bVisible = false;
            for (unsigned int uiCamera = 0; uiCamera < FrameGlobals::g_nCameras; ++uiCamera) {
                if (kVis.IsOnScreen(kWorldPos, (int)uiCamera)) {
                    bVisible = true;
                }
            }
            bOffScreen = !bVisible;
        }
        if ((bHasBuffers && bOffScreen) || !kVis.m_bUpToDate) {
            if (!kVis.m_bUpToDate) {
                kVis.ReleaseShadows();
            }
            kVis.ReleaseBuffers(m_ShaderPool);
            bHasBuffers = false;
        }
        if (!bHasBuffers && !bOffScreen) {
            HEXMatrix_i const& kGridToOffset = *reinterpret_cast<HEXMatrix_i const*>(MapGlobals::g_pTerrainSampler->unk_c8);
            HEXVec_i const kOffset = Fun_086616f0(kGridToOffset, kVis.m_Hex);
            unsigned int uiContinent = MapGlobals::g_pTerrainSampler->m_aucContinentType[(kOffset.y >> 1) + kOffset.x + kOffset.y * 0xff];
            int iType = (int)kVis.m_eType;
            unsigned int uiTreesPerShader = (m_uiGriddedTreesPerTile + m_uiRandomTreesPerTile) / kNumTreeSlots;
            if ((iType | 2) == 3) {
                kVis.BuildBuffers(m_ahForestVB[uiContinent], m_ahForestIB[uiContinent], m_ahForestSurface[uiContinent], m_TreeLists[kVis.m_uiTreeList], uiTreesPerShader, m_ShaderPool);
            }
            if (iType == 2 || iType == 4) {
                kVis.BuildBuffers(m_ahJungleVB[uiContinent], m_ahJungleIB[uiContinent], m_ahJungleSurface[uiContinent], m_TreeLists[kVis.m_uiTreeList], uiTreesPerShader, m_ShaderPool);
            }
            if (!kVis.m_bUpToDate) {
                kVis.BuildShadows(m_TreeLists[kVis.m_uiTreeList]);
            }
            kVis.m_bUpToDate = true;
        }
    }
}

/// 0x08668284
UNK_RET ForestVisSystem::OnRenderView(unsigned int a1) {
    RenderInternal(a1, false);
}

/// 0x086682b6
UNK_RET ForestVisSystem::OnRenderReflection(unsigned int a1) {
    RenderInternal(a1, true);
}

/// 0x08668cda
UNK_RET ForestVisSystem::AddRandomLocationsInTile(FFastVector<ForestVisTreeData, false, 49u, 0u, BaseVector<ForestVisTreeData, false>::FDefaultFastVectorAllocator>& kTrees, unsigned int uiTile) {
    D3DXMATRIX const* pHexToTex = reinterpret_cast<D3DXMATRIX const*>(WorldGlobals::g_HexToTex);
    D3DXMATRIX const* pTexToHex = reinterpret_cast<D3DXMATRIX const*>(WorldGlobals::g_TexToHex);
    unsigned char const* pMask = m_TileMasks.begin()[uiTile].m_aucMask;
    for (int iTree = 0; iTree < (int)m_uiRandomTreesPerTile; ++iTree) {
        float fScale = (float)(int)rand_wincompatible() / 32767.0f * 0.5f + 0.5f;
        float fX;
        float fY;
        unsigned int uiTries = 0;
        // Retry up to kMaxPlacementTries spots until one clears every placed tree by m_fSpaceBetween.
        for (;;) {
            fX = (float)(int)rand_wincompatible() / 32767.0f * kTileExtent;
            fY = (float)(int)rand_wincompatible() / 32767.0f * kTileExtent;
            ++uiTries;
            bool bOverlap = false;
            for (unsigned int uiOther = 0; uiOther < kTrees.size(); ++uiOther) {
                ForestVisTreeData const& kOther = kTrees[uiOther];
                float fDX = fX - kOther.m_fTileX;
                float fDY = fY - kOther.m_fTileY;
                if (sqrtf(fDY * fDY + fDX * fDX) < (kOther.m_fScale + fScale) * m_fSpaceBetween) {
                    bOverlap = true;
                    break;
                }
            }
            if (!bOverlap || uiTries >= kMaxPlacementTries) {
                break;
            }
        }
        if (uiTries >= kMaxPlacementTries && iTree > kLastVisibleTree) {
            fScale = 0.0f;
        }
        unsigned int uiMaskX = (unsigned int)(fX / kTileExtent * kTileMaskMax);
        unsigned int uiMaskY = (unsigned int)((1.0f - fY / kTileExtent) * kTileMaskMax);
        if (uiMaskX < 0x80 && uiMaskY < 0x80 && pMask[uiMaskX + uiMaskY * 0x80] != 0) {
            if (iTree > kLastVisibleTree) {
                fScale = 0.0f;
            }
            // Tile space is centred on the hex: shift by the hex origin and half the tile.
            D3DXVECTOR4 kOrigin = {0.0f, 0.0f, -0.0f, 1.0f};
            D3DXVECTOR4 kOriginTex = {0.0f, 0.0f, 0.0f, 1.0f};
            D3DXVec4Transform(&kOriginTex, &kOrigin, pHexToTex);
            D3DXVECTOR4 kTex = {fX + kOriginTex.x + -48.0f, kOriginTex.y + fY + -48.0f, 0.0f, 1.0f};
            D3DXVECTOR4 kHex = {0.0f, 0.0f, 0.0f, 1.0f};
            D3DXVec4Transform(&kHex, &kTex, pTexToHex);
            ForestVisTreeData kTree;
            kTree.m_fHexX = kHex.x;
            kTree.m_fHexY = kHex.y;
            kTree.m_fHexZ = kHex.z;
            kTree.m_fHexW = 1.0f;
            kTree.m_fTileX = fX;
            kTree.m_fTileY = fY;
            kTree.m_fScale = fScale;
            kTrees.push_back(kTree);
        } else {
            --iTree;
        }
    }
}

/// 0x0866880c
UNK_RET ForestVisSystem::UpdateFromHandle(cvGeneralRefHandle<ForestVisSystem, unsigned int, ForestVisSystemDataDef, ForestVisSystem_Accessor>& a1, ForestVisSystem_PlaceForest a2) {
    if (!m_bGameStarted) {
        return;
    }
    ForestVis kVis(a2.m_Hex, static_cast<ForestVis::FOREST_VIS_TYPE>(a2.m_ucVariant * 2 + 1));
    kVis.m_uiTreeList = rand_wincompatible() % m_TreeLists.size();
    InsertIntoVisList(kVis);
}

/// 0x086688b8
UNK_RET ForestVisSystem::UpdateFromHandle(cvGeneralRefHandle<ForestVisSystem, unsigned int, ForestVisSystemDataDef, ForestVisSystem_Accessor>& a1, ForestVisSystem_PlaceJungle a2) {
    if (!m_bGameStarted) {
        return;
    }
    ForestVis kVis(a2.m_Hex, static_cast<ForestVis::FOREST_VIS_TYPE>((a2.m_bVariant ? 1 : 0) * 2 + 2));
    kVis.m_uiTreeList = rand_wincompatible() % m_TreeLists.size();
    InsertIntoVisList(kVis);
}
