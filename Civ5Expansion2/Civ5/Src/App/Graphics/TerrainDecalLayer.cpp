/// origin guess
#include <algorithm>
#include <cstring>
#include <new>
#include "Civ5/Src/App/Graphics/TerrainDecalLayer.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "FirePlace/FireEngine2/FTextureResource.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/TerrainDecalShader.h"
#include "FirePlace/FireWorks/FString.h"
#include "FirePlace/FireWorks/FXmlAttribute.h"
#include "FirePlace/FireWorks/FXmlDocument.h"
#include "FirePlace/FireWorks/FXmlTraverser.h"

static const unsigned int kNoIndex = 0xfffffff;
static const unsigned int kInvalidHandle = 0xffffffff;
static const float kGridCellSize = 256.0f;
static const float kRenderMargin = 256.0f;
static const float kBigDecalExtent = 256.0f;
static const float kDefaultPriority = 0.1f;
static const unsigned int kMaxBankVertices = 0xffff;
static const unsigned char kEntryUploaded = 0x1;
static const unsigned char kEntryPassFlag = 0x2;

// Names of the <Decal category> elements of decal_filenames.xml, in TerrainDecalLayer::m_typeRanges order.
static const char kCategoryNames[10][0x80] = {
    "Detail_Road",
    "Main_Road",
    "Crops",
    "Tree_Shadow",
    "Floodplains",
    "Floodplains_EndCap1",
    "Floodplains_EndCap2",
    "Riverbank",
    "Riverbank_EndCap1",
    "Riverbank_EndCap2",
};

/// 0x0867a8dc
void TerrainDecalLayer::Startup() {
    m_hBlankSpec = FireGrafix::g_pRenderer->LoadTexture("blank_texture_spec_decal.dds");
    Fun_0867c02c(&m_grid);
    m_grid.m_pNodePool = &m_gridNodes;
    Fun_0867e1d6(&m_grid);
    m_pShader = new FireGrafix::TerrainDecalShader();
    LoadTerrainDecals("assets/Decals/decal_filenames.xml");
    unsigned short* pIndices = new unsigned short[0xffff];
    for (unsigned int i = 0; i < 0xffff; ++i) {
        pIndices[i] = static_cast<unsigned short>(i);
    }
    m_hIndexBuffer = FireGrafix::g_pRenderer->CreateAndFillStaticIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(0), 0xffff, pIndices, 0, 0);
    delete[] pIndices;
}

/// 0x0867aa1c
UNK_RET TerrainDecalLayer::LoadTerrainDecals(char const* path) {
    if (path == 0) {
        return;
    }
    FXmlDocument document;
    document.Load(path);
    FXmlTraverser root;
    document.GetRootElement(root);
    root.GetChildCount(1);
    FXmlAttribute attribute;
    FStringA name;
    FStringA::FStringAFixedBuffer<256> nameBuffer(name);
    for (unsigned int category = 0; category < 10; ++category) {
        const char* categoryName = kCategoryNames[category];
        name.Copy(static_cast<int>(strlen(categoryName)), categoryName);
        if (!root.GetFirstChild(root, name, name.GetLength())) {
            continue;
        }
        m_typeRanges[category].m_begin = m_numTypes;
        root.GetFirstChild(root);
        do {
            unsigned int hAlbedo = 0;
            if (root.GetFirstAttribute(attribute, "albedo", 6) && attribute.GetVal(name)) {
                hAlbedo = FTextureResourceMgr::LoadTextureHandle(name, 0x20000, 0);
            }
            unsigned int hHeightmap = 0;
            if (root.GetFirstAttribute(attribute, "heightmap", 9) && attribute.GetVal(name)) {
                hHeightmap = FTextureResourceMgr::LoadTextureHandle(name, 0x20000, 0);
            }
            unsigned int hSpec = m_hBlankSpec;
            if (root.GetFirstAttribute(attribute, "spec", 4) && attribute.GetVal(name)) {
                hSpec = FTextureResourceMgr::LoadTextureHandle(name, 0x20000, 0);
            }
            float fPriority = kDefaultPriority;
            float fValue;
            if (root.GetFirstAttribute(attribute, "priority", 8) && attribute.GetVal(fValue)) {
                fPriority = fValue;
            }
            DecalType type;
            type.m_fPriority = fPriority;
            type.m_hAlbedo = hAlbedo;
            type.m_hHeightmap = hHeightmap;
            type.m_hSpec = hSpec;
            Fun_0867c09e(&m_pTypes, &type);
        } while (root.GetNextSibling(root));
        m_typeRanges[category].m_end = m_numTypes;
        root.GetParent(root);
        root.GetParent(root);
    }
}

/// 0x0867ae88
void TerrainDecalLayer::Shutdown() {
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_hBlankSpec);
    if (m_pShader != 0) {
        delete m_pShader;
    }
}

/// 0x0867aed2
void TerrainDecalLayer::StartupGame() {
    const FGXVector3& worldSize = WorldGlobals::g_WorldSize;
    const FGXVector3& terrainScale = WorldGlobals::g_TerrainScale;
    float extentX = worldSize.x / terrainScale.x;
    float extentY = worldSize.y / terrainScale.y;
    int cellsX = static_cast<int>(worldSize.x / (terrainScale.x * kGridCellSize)) + 1;
    int cellsY = static_cast<int>(worldSize.y / (terrainScale.y * kGridCellSize)) + 1;
    Fun_0867c02c(&m_grid);
    m_grid.m_fMinX = 0.0f;
    m_grid.m_fMinY = 0.0f;
    m_grid.m_fMaxX = extentX;
    m_grid.m_fMaxY = extentY;
    m_grid.m_cellsX = cellsX;
    m_grid.m_cellsY = cellsY;
    Fun_0867e1d6(&m_grid);
    Fun_0867d3bc(&m_bigDecals, m_bigDecals.m_usedHead, kNoIndex);
}

/// 0x0867afb6
void TerrainDecalLayer::ShutdownGame() {
    for (unsigned int i = 0; i < m_numBanks; ++i) {
        TerrainDecalBank& bank = m_pBanks[i];
        if (bank.m_hVertexBuffer != 0) {
            Fun_0866fd26(FireGrafix::CRenderer9::m_VBList, bank.m_hVertexBuffer);
            bank.m_hVertexBuffer = 0;
        }
        Fun_0867d294(&bank.m_decals, bank.m_decals.m_usedHead, kNoIndex);
        if (bank.m_pVertices != 0) {
            ::operator delete(bank.m_pVertices);
        }
    }
    Fun_0867d3bc(&m_bigDecals, m_bigDecals.m_usedHead, kNoIndex);
    Fun_0867c02c(&m_grid);
    Fun_0867e0c6(&m_pBanks, m_pBanks, m_numBanks);
    m_numBanks = 0;
}

/// 0x0867b0b2
unsigned int TerrainDecalLayer::CreateDecal(DecalType const& type, DecalVertex const* pVerts, unsigned int numVerts, unsigned short const* pIndices, unsigned int numTriangles, bool bPassFlag) {
    if (numTriangles == 0) {
        return kInvalidHandle;
    }
    unsigned int numIndices = numTriangles * 3;
    if (numIndices > 0xfffe) {
        return kInvalidHandle;
    }

    unsigned int bankIndex = kInvalidHandle;
    for (int i = 0; i < static_cast<int>(m_numBanks); ++i) {
        TerrainDecalBank& candidate = m_pBanks[i];
        if (candidate.m_numVertices + numIndices < kMaxBankVertices) {
            bankIndex = i;
            break;
        }
        if (candidate.m_bNeedsRepack != 0) {
            RepackDirtyBank(candidate);
            --i;
        }
    }
    if (bankIndex == kInvalidHandle) {
        TerrainDecalBank fresh = {};
        fresh.m_decals.m_freeHead = kNoIndex;
        fresh.m_decals.m_usedHead = kNoIndex;
        fresh.m_decals.m_usedTail = kNoIndex;
        fresh.m_pVertices = static_cast<unsigned long long*>(::operator new(0x7fff8));
        memset(fresh.m_pVertices, 0, 0x7fff8);
        fresh.m_bNeedsUpload = 1;
        bankIndex = Fun_0867c100(&m_pBanks, &fresh);
        TerrainDecalBank& added = m_pBanks[bankIndex];
        if (added.m_decals.m_itemCapacity < 0x100) {
            Fun_0867dd5e(&added.m_decals.m_pItems, 0x100);
        }
        Fun_0867e292(&fresh.m_decals.m_pItems);
    }
    TerrainDecalBank& bank = m_pBanks[bankIndex];

    // minX, minY, maxX, maxY of the vertices, clamped to the grid.
    float bounds[4] = {pVerts[0].m_fX, pVerts[0].m_fY, pVerts[0].m_fX, pVerts[0].m_fY};
    for (unsigned int i = 1; i < numVerts; ++i) {
        float x = pVerts[i].m_fX;
        float y = pVerts[i].m_fY;
        if (x > bounds[2]) {
            bounds[2] = x;
        }
        if (y > bounds[3]) {
            bounds[3] = y;
        }
        if (x < bounds[0]) {
            bounds[0] = x;
        }
        if (y < bounds[1]) {
            bounds[1] = y;
        }
    }
    if (m_grid.m_fMinX > bounds[0]) {
        bounds[0] = m_grid.m_fMinX;
    }
    if (m_grid.m_fMinY > bounds[1]) {
        bounds[1] = m_grid.m_fMinY;
    }
    if (bounds[2] > m_grid.m_fMaxX) {
        bounds[2] = m_grid.m_fMaxX;
    }
    if (bounds[3] > m_grid.m_fMaxY) {
        bounds[3] = m_grid.m_fMaxY;
    }

    Terrain* terrain = g_TerrainSystem.m_pTerrain;
    if (terrain != 0) {
        terrain->InvalidateRegion(reinterpret_cast<FGXVector2&>(bounds[0]), reinterpret_cast<FGXVector2&>(bounds[2]), 0, type.m_hSpec != 0 && type.m_hSpec != m_hBlankSpec);
    }

    bank.m_bNeedsUpload = 1;
    TerrainDecalEntry entry = {};
    entry.m_flags = bPassFlag ? kEntryPassFlag : 0;
    entry.m_firstVertex = bank.m_numVertices;
    entry.m_numTriangles = numTriangles;
    entry.m_hAlbedo = type.m_hAlbedo;
    entry.m_hHeightmap = type.m_hHeightmap;
    entry.m_hSpec = type.m_hSpec;
    entry.m_fPriority = type.m_fPriority;
    entry.m_fMinX = bounds[0];
    entry.m_fMinY = bounds[1];
    entry.m_fMaxX = bounds[2];
    entry.m_fMaxY = bounds[3];
    unsigned int decalIndex = Fun_0867d9f8(&bank.m_decals, &entry);
    Fun_0867dabe(&bank.m_decals, decalIndex);

    float width = bounds[2] - bounds[0];
    float height = bounds[3] - bounds[1];
    if (numIndices != 0) {
        float invWidth = 1.0f / width;
        float invHeight = 1.0f / height;
        unsigned long long* out = bank.m_pVertices + static_cast<unsigned short>(bank.m_numVertices);
        for (unsigned int i = 0; i < numIndices; ++i) {
            DecalVertex const& vertex = pVerts[pIndices[i]];
            float uv[2] = {(vertex.m_fX - bounds[0]) * invWidth, (vertex.m_fY - bounds[1]) * invHeight};
            D3DXFLOAT16* halves = reinterpret_cast<D3DXFLOAT16*>(&out[i]);
            D3DXFloat32To16Array(halves, uv, 2);
            D3DXFloat32To16Array(halves + 2, &vertex.field_8, 2);
        }
    }

    TerrainDecalGridNode node;
    node.m_packedNext = 0x1fffffff;
    node.m_prev = kNoIndex;
    node.m_fX = (bounds[2] + bounds[0]) * 0.5f;
    node.m_fY = (bounds[3] + bounds[1]) * 0.5f;
    node.m_bankIndex = bankIndex;
    node.m_decalIndex = decalIndex;
    bank.m_numVertices += numIndices;

    if (width > kBigDecalExtent || height > kBigDecalExtent) {
        TerrainDecalBigItem big = {};
        big.m_bankIndex = bankIndex;
        big.m_decalIndex = decalIndex;
        unsigned int index = Fun_0867d77c(&m_bigDecals, &big);
        Fun_0867d7fa(&m_bigDecals, index);
        return index | 0x80000000;
    }
    unsigned int index = Fun_0867d4e8(m_grid.m_pNodePool, &node);
    Fun_0867d586(&m_grid, index);
    return index;
}

/// 0x0867b79e
void TerrainDecalLayer::RepackDirtyBank(TerrainDecalBank& bank) {
    TerrainDecalEntry* entries = static_cast<TerrainDecalEntry*>(bank.m_decals.m_pItems);
    unsigned long long* vertices = bank.m_pVertices;
    unsigned int writePos = 0;
    unsigned int index = bank.m_decals.m_usedHead;
    while (index != kNoIndex) {
        TerrainDecalEntry& entry = entries[index];
        unsigned int numVertices = entry.m_numTriangles * 3;
        if (writePos != entry.m_firstVertex) {
            for (unsigned int i = 0; i < numVertices; ++i) {
                vertices[writePos + i] = vertices[entry.m_firstVertex + i];
            }
            entry.m_firstVertex = writePos;
        }
        writePos += numVertices;
        index = entry.m_next;
    }
    bank.m_numVertices = writePos;
    bank.m_bNeedsUpload = 1;
    bank.m_bNeedsRepack = 0;
}

/// 0x0867b85e
unsigned int TerrainDecalLayer::CreateDecal(unsigned int typeCategory, DecalVertex const* pVerts, unsigned int numVerts, unsigned short const* pIndices, unsigned int numTriangles, bool bPassFlag) {
    unsigned int typeIndex = m_typeRanges[typeCategory].m_begin;
    unsigned int numTypes = m_typeRanges[typeCategory].m_end - typeIndex;
    if (numTypes >= 2) {
        typeIndex += rand_wincompatible() % numTypes;
    }
    return CreateDecal(m_pTypes[typeIndex], pVerts, numVerts, pIndices, numTriangles, bPassFlag);
}

/// 0x0867b8e6
void TerrainDecalLayer::DestroyDecal(unsigned int handle) {
    unsigned int bankIndex;
    unsigned int decalIndex;
    if (static_cast<int>(handle) < 0) {
        unsigned int index = handle & 0x7fffffff;
        TerrainDecalBigItem& item = static_cast<TerrainDecalBigItem*>(m_bigDecals.m_pItems)[index];
        bankIndex = item.m_bankIndex;
        decalIndex = item.m_decalIndex;
        Fun_0867d3bc(&m_bigDecals, index, item.m_next);
    } else {
        TerrainDecalGridNode& node = m_grid.m_pNodePool->m_pNodes[handle];
        bankIndex = node.m_bankIndex;
        decalIndex = node.m_decalIndex;
        Fun_0867c1a0(&m_grid, handle);
    }
    TerrainDecalBank& bank = m_pBanks[bankIndex];
    TerrainDecalEntry& entry = static_cast<TerrainDecalEntry*>(bank.m_decals.m_pItems)[decalIndex];
    Terrain* terrain = g_TerrainSystem.m_pTerrain;
    if (terrain != 0) {
        terrain->InvalidateRegion(reinterpret_cast<FGXVector2&>(entry.m_fMinX), reinterpret_cast<FGXVector2&>(entry.m_fMaxX), 0, 0);
    }
    Fun_0867d294(&bank.m_decals, decalIndex, entry.m_next);
    bank.m_bNeedsRepack = 1;
}

/// 0x0867ba16
void TerrainDecalLayer::RepackAllDirtyBanks() {
    for (unsigned int i = 0; i < m_numBanks; ++i) {
        if (m_pBanks[i].m_bNeedsRepack != 0) {
            RepackDirtyBank(m_pBanks[i]);
        }
    }
}

/// 0x0867ba6a
void TerrainDecalLayer::Update() {
    for (unsigned int i = 0; i < m_numBanks; ++i) {
        TerrainDecalBank& bank = m_pBanks[i];
        if (bank.m_bNeedsUpload == 0) {
            continue;
        }
        if (bank.m_hVertexBuffer == 0) {
            bank.m_hVertexBuffer = FireGrafix::g_pRenderer->CreateStaticVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(0x12), 0xffff, 0, 1);
        }
        FireGrafix::g_pRenderer->LockAndFillStaticVB(bank.m_hVertexBuffer, bank.m_pVertices, 0);
        TerrainDecalEntry* entries = static_cast<TerrainDecalEntry*>(bank.m_decals.m_pItems);
        for (unsigned int index = bank.m_decals.m_usedHead; index != kNoIndex; index = entries[index].m_next) {
            entries[index].m_flags |= kEntryUploaded;
        }
        bank.m_bNeedsUpload = 0;
    }
}

/// 0x0867bb5c
void TerrainDecalLayer::Render(FireGrafix::CCommandStream* stream, FGXVector2 const& minCorner, FGXVector2 const& maxCorner, D3DXMATRIX* pMatrix, int bPassFlag) {
    float minX = minCorner.x;
    float minY = minCorner.y;
    float maxX = maxCorner.x;
    float maxY = maxCorner.y;
    float searchRect[4] = {minX - kRenderMargin, minY - kRenderMargin, maxX + kRenderMargin, maxY + kRenderMargin};

    TerrainDecalGridIterator it;
    Fun_0867d050(&it, &m_grid, searchRect);
    while (it.m_node != kNoIndex) {
        TerrainDecalGridNode& node = it.m_pGrid->m_pNodePool->m_pNodes[it.m_node];
        unsigned int bankIndex = node.m_bankIndex;
        unsigned int decalIndex = node.m_decalIndex;
        TerrainDecalBank& bank = m_pBanks[bankIndex];
        TerrainDecalEntry& entry = static_cast<TerrainDecalEntry*>(bank.m_decals.m_pItems)[decalIndex];
        if ((entry.m_flags & kEntryUploaded) != 0 && ((entry.m_flags >> 1) & 1) == bPassFlag && bank.m_bNeedsUpload == 0) {
            if (minY <= entry.m_fMaxY && entry.m_fMinX <= maxX && minX <= entry.m_fMaxX && entry.m_fMinY <= maxY) {
                TerrainDecalDrawItem item;
                item.m_bankIndex = bankIndex;
                item.m_decalIndex = decalIndex;
                item.m_fPriority = entry.m_fPriority;
                Fun_0867c24e(&m_pDrawQueue, &item);
            }
        }
        Fun_0867c2a2(&it);
    }

    for (unsigned int index = m_bigDecals.m_usedHead; index != kNoIndex;) {
        TerrainDecalBigItem& big = static_cast<TerrainDecalBigItem*>(m_bigDecals.m_pItems)[index];
        TerrainDecalBank& bank = m_pBanks[big.m_bankIndex];
        TerrainDecalEntry& entry = static_cast<TerrainDecalEntry*>(bank.m_decals.m_pItems)[big.m_decalIndex];
        if ((entry.m_flags & kEntryUploaded) != 0 && ((entry.m_flags >> 1) & 1) == bPassFlag) {
            if (minY <= entry.m_fMaxY && entry.m_fMinX <= maxX && minX <= entry.m_fMaxX && entry.m_fMinY <= maxY) {
                TerrainDecalDrawItem item;
                item.m_bankIndex = big.m_bankIndex;
                item.m_decalIndex = big.m_decalIndex;
                item.m_fPriority = entry.m_fPriority;
                Fun_0867c24e(&m_pDrawQueue, &item);
            }
        }
        index = big.m_next;
    }

    // The queue is dropped without drawing when the renderer is nearly out of surfaces.
    if (m_numDrawQueue < FireGrafix::g_pRenderer->GetFreeSurfaceCount() - 1000) {
        std::sort(m_pDrawQueue, m_pDrawQueue + m_numDrawQueue, [](const TerrainDecalDrawItem& a, const TerrainDecalDrawItem& b) {
            return a.m_fPriority < b.m_fPriority;
        });
        FireGrafix::DecalDynamics* pDynamics = static_cast<FireGrafix::DecalDynamics*>(m_pShader->m_pConstantSet);
        for (unsigned int i = 0; i < m_numDrawQueue; ++i) {
            TerrainDecalBank& bank = m_pBanks[m_pDrawQueue[i].m_bankIndex];
            TerrainDecalEntry& entry = static_cast<TerrainDecalEntry*>(bank.m_decals.m_pItems)[m_pDrawQueue[i].m_decalIndex];

            float* pConstants = static_cast<float*>(pDynamics->LockDynamic(stream));
            memcpy(pConstants, pMatrix, 16 * sizeof(float));
            pConstants[0x10] = 1.0f;
            pConstants[0x11] = 1.0f;
            pConstants[0x12] = 0.0f;
            pConstants[0x13] = 0.0f;
            memcpy(&pConstants[0x14], &entry.m_fMinX, 4 * sizeof(float));
            pConstants[0x18] = entry.m_fPriority;
            m_pShader->m_pConstantSet->UnlockDynamic();

            FireGrafix::FGXSurface* pSurface;
            unsigned int hSurface;
            FireGrafix::g_pRenderer->CreateSurface(&pSurface, &hSurface, 0x4e);
            pSurface->field_60 = 2;
            pSurface->field_18[0] = static_cast<unsigned short>(entry.m_hAlbedo);
            pSurface->field_18[1] = static_cast<unsigned short>(entry.m_hHeightmap);
            pSurface->field_38[0] = 2;
            pSurface->field_38[1] = 0;
            pSurface->field_10 = entry.m_firstVertex;
            pSurface->field_c = entry.m_numTriangles;
            pSurface->m_hVB = bank.m_hVertexBuffer;
            pSurface->m_hIB = m_hIndexBuffer;
            stream->DrawSurface(hSurface, m_pShader->m_hInstance, 0);
            FireGrafix::g_pRenderer->ReleaseSurface(hSurface);
        }
    }
    m_numDrawQueue = 0;
}
