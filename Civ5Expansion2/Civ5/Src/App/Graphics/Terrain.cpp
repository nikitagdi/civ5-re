/// origin binary
#include <float.h>
#include <math.h>
#include <string.h>
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/UI/UIRenderBuffer.h"
#include "Civ5/Src/App/Graphics/TerrainCell.h"
#include "Civ5/Src/App/Graphics/TerrainCellStatJob.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireWorks/FTempHeap.h"

namespace {

const TerrainCellStatus kStatusNone = static_cast<TerrainCellStatus>(0);
const TerrainCellStatus kStatusCacheOnly = static_cast<TerrainCellStatus>(1);
const TerrainCellStatus kStatus2 = static_cast<TerrainCellStatus>(2);
const TerrainCellStatus kStatus3 = static_cast<TerrainCellStatus>(3);
const TerrainCellStatus kStatusBaseOnly = static_cast<TerrainCellStatus>(4);
const TerrainCellStatus kStatus5 = static_cast<TerrainCellStatus>(5);

const unsigned int kCompositeVertexSize = 0x30;
const unsigned int kCompressedVertexSize = 0x18;
const int kHexesPerTile = 63;
const int kInitBuildsPerCall = 4;
// Equals the read-only global g_fHeightScale (0x0914aba0): heightmap bytes to [0, 1].
const float kHeightScale = 1.0f / 255.0f;

// Separable 5-tap weights; SetBlendWeights builds the normalized 5x5 product.
const float kBlendKernel1D[5] = {0.125f, 0.636056f, 1.0f, 0.636056f, 0.125f};

// Full-screen quad: position, the same position again, texcoord.
const float kQuadVertices[4][8] = {
    { 1.0f,  1.0f, 0.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f},
    {-1.0f,  1.0f, 0.0f, -1.0f,  1.0f, 0.0f, 0.0f, 0.0f},
    { 1.0f, -1.0f, 0.0f,  1.0f, -1.0f, 0.0f, 1.0f, 1.0f},
    {-1.0f, -1.0f, 0.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
};
const unsigned int kQuadIndices[6] = {2, 1, 0, 3, 1, 2};

// g_EventSystemLocalMachine starts with the index of its active machine; machines are 0xc00180 bytes apart.
const unsigned int kEventMachineStride = 0xc00180;
const unsigned int kTerrainCachedEventSlot = 0xc80;

// Heightmap tiles are 1024 samples wide but 128 of them overlap the next tile; 16 tiles per row in the handle table.
const int kHeightTileSize = 0x400;
const int kHeightTileStep = 0x380;
const int kHeightTilesPerRow = 16;
// The mip chain is stored after the full-resolution level.
const unsigned int kHeightMipOffset = 0x100000;
const int kHeightMipLevels = 11;

// Rows of the hex grid are sqrt(3)/2 apart.
const float kHexRowScale = 0.866f;

// Tessellation level the per-cell minimum starts from.
const float kTessLODCeiling = 10.0f;
const float kPatchShiftPerLOD = -0.75f;
const float kTessLODStepBlend = 0.25f;

// 4x4 block of the terrain constant buffer at TerrainGlobalConstants::m_afBasis.
const float kBasis[16] = {
    -0.125f, 0.875f, -0.875f, 0.125f,
    0.375f, -1.5f, 1.125f, 0.0f,
    -0.375f, 0.0f, 0.375f, 0.0f,
    0.125f, 0.75f, 0.125f, 0.0f,
};

// Scales SetTransformWeights applies before truncating to whole world units: axis 0, the other axes.
const float kAxisZeroScale = 0.33f;
const float kOtherAxisScale = 0.66f;

}  // namespace

/// 0x0890b554
Terrain::Terrain() {
    field_0 = 8;
    Fun_08910c2e(&m_kShared);
    field_1adc = 0;
    m_ppCells = nullptr;
    m_ppPriorityInitCells = nullptr;
    m_ppBackgroundInitCells = nullptr;
    m_ppEvictionCandidates = nullptr;
    for (int i = 0; i < 4; ++i) {
        m_appVisibleCells[i] = nullptr;
        m_anVisibleCells[i] = 0;
    }
    m_nCellsX = 0;
    m_nCellsY = 0;
    m_nCells = 0;
    m_nPriorityInitCells = 0;
    m_uiPriorityInitCursor = 0;
    m_nBackgroundInitCells = 0;
    m_uiBackgroundInitCursor = 0;
    field_232c = 0;
    m_nInitBuildsThisFrame = 0;

    const FGXInt2 kNoSize = {0, 0};
    const int iDefaultState = 0;
    Fun_08910e1e(unk_2358, kNoSize, &iDefaultState, &HEXTransform_i::IDENTITY_TRANSFORM);

    m_bTextureInitDone = 0;
    m_kShared.m_hRenderTextureCache = 0;
    m_kShared.m_hHeightTextureCache = 0;
    m_kShared.m_hShadowTextureCache = 0;
    m_kShared.field_69c = 0;
    m_kShared.m_hTempDecalCache = 0;
}

/// 0x0890d374
Terrain::~Terrain() {
    UNK_DATA* pTextures = FireGrafix::CRenderer9::m_TextureList;
    UNK_DATA* pVertexBuffers = FireGrafix::CRenderer9::m_VBList;
    UNK_DATA* pIndexBuffers = FireGrafix::CRenderer9::m_IBList;
    SharedData& kShared = m_kShared;

    m_kJobSet.Wait();
    for (unsigned int i = 0; i < static_cast<unsigned int>(kShared.m_nTextures); ++i) {
        Fun_08672408(pTextures, kShared.field_708[i]);
        Fun_08672408(pTextures, kShared.field_908[i]);
        Fun_08672408(pTextures, kShared.field_808[i]);
    }
    for (unsigned int i = 0; i < 256; ++i) {
        Fun_08672408(pTextures, kShared.m_ahHeightmapTiles[i]);
        Fun_08672408(pTextures, kShared.field_16a4[i]);
    }
    Fun_08672408(pTextures, kShared.m_hRenderTemp);
    Fun_08672408(pTextures, kShared.m_hHeightTemp);
    Fun_08672408(pTextures, kShared.m_hRenderSpecTemp);
    Fun_08672408(pTextures, kShared.m_hShadowTemp);
    Fun_08672408(pTextures, kShared.m_hNormalTemp);
    Fun_08672408(pTextures, kShared.m_hAlbedoTemp);
    Fun_08672408(pTextures, kShared.m_hSpecTemp);
    Fun_08672408(pTextures, kShared.m_hTempNormalCache);
    Fun_08672408(pTextures, kShared.m_hTempAlbedoCache);
    Fun_08672408(pTextures, kShared.m_hRenderTextureCache);
    Fun_08672408(pTextures, kShared.m_hHeightTextureCache);
    Fun_08672408(pTextures, kShared.m_hShadowTextureCache);
    Fun_08672408(pTextures, kShared.field_69c);
    Fun_08672408(pTextures, kShared.m_hTempDecalCache);
    Fun_08672408(pTextures, kShared.m_hBlankTexture);
    Fun_0866fd26(pVertexBuffers, kShared.m_hCompositeVB);
    Fun_0866fd26(pVertexBuffers, kShared.m_hCompressedVB);
    Fun_0866fb7e(pIndexBuffers, kShared.m_hDynamicIB);
    if (GraphicsSettings::TerrainSettings::g_bEnableDXTCompression != 0) {
        for (int i = 0; i < 4; ++i) {
            Fun_08672408(pTextures, kShared.m_ahDXTDiffuse[i]);
            Fun_08672408(pTextures, kShared.m_ahDXTSpec[i]);
            Fun_08672408(pTextures, kShared.m_ahDXTUncompressedDiffuse[i]);
            Fun_08672408(pTextures, kShared.m_ahDXTUncompressedSpec[i]);
        }
    }
    FireGrafix::g_pRenderer->ReleaseDepthStencilResource(kShared.m_hDepthStencil);
    DeleteCells();

    // Both handlers were subscribed in Init as {this, handler} delegates.
    Fun_086dcac6(this, &Terrain::OnFOWChanged);
    Fun_0873d98e(this, &Terrain::OnDeviceReset);

    Fun_0866fd26(pVertexBuffers, kShared.m_hQuadVB);
    Fun_0866fb7e(pIndexBuffers, kShared.m_hQuadIB);
    for (unsigned int i = 0; i < kShared.m_kBaseMeshes.size(); ++i) {
        Fun_0866fb7e(pIndexBuffers, kShared.m_kBaseMeshes[i].m_hIB);
        Fun_0866fd26(pVertexBuffers, kShared.m_kBaseMeshes[i].m_hVB);
        Fun_0866fb7e(pIndexBuffers, kShared.m_kBaseMeshes[i].m_hPatchIB);
    }

    // Destructors of the members the header keeps as raw storage; m_kShared (shader views, base meshes) follows as a member.
    Fun_0886cd80(&m_piFOWGrid);
    Fun_08910f56(&field_1adc);
}

/// 0x0890b818
bool Terrain::Init(DynamicHexGrid<HexPatternCell, SquareGridPolicy, 0u, 0u> const& kGameGrid, int nTextures, ExternalTextureHandles const& kTexHandles, ExternalDataReferences& kExternalData) {
    SharedData& kShared = m_kShared;
    const unsigned int uiTessLevel = GraphicsSettings::TerrainSettings::g_uTerrainTessLevel;
    if (uiTessLevel == 1) {
        kShared.m_iTessLevel = 2;
    } else if (uiTessLevel == 2) {
        kShared.m_iTessLevel = 1;
    } else {
        kShared.m_iTessLevel = 3;
    }
    m_uiHeightmapDirtyFrame = 0;
    m_uiHeightmapBuiltFrame = 0;
    kShared.m_kExternalData = kExternalData;
    kShared.m_kTexHandles = kTexHandles;

    const FGXVector3& vTerrainScale = WorldGlobals::g_TerrainScale;
    kShared.m_iHeightMapWidth = kExternalData.m_pTerrainSampler->m_iHeightMapWidth;
    kShared.m_iHeightMapHeight = kExternalData.m_pTerrainSampler->m_iHeightMapHeight;
    kShared.m_fHalfWorldWidth = static_cast<float>(kShared.m_iHeightMapWidth / 2) * vTerrainScale.x;
    kShared.m_fHalfWorldHeight = static_cast<float>(kShared.m_iHeightMapHeight / 2) * vTerrainScale.y;
    kShared.field_1ab8 = 0.0f;
    kShared.m_fWorldWidth = static_cast<float>(kShared.m_iHeightMapWidth) * vTerrainScale.x;
    kShared.m_fWorldHeight = static_cast<float>(kShared.m_iHeightMapHeight) * vTerrainScale.y;
    kShared.m_vTerrainScale = vTerrainScale;
    m_bTextureInitDone = 0;
    kShared.field_41c = 0;
    kShared.m_nTextures = nTextures;
    field_2354 = 0;
    for (int i = 0; i < 5; ++i) {
        kShared.m_ahSunlightTextures[i] = 0;
        kShared.m_ahShadowlightTextures[i] = 0;
    }
    memset(kShared.m_ahHeightmapTiles, 0, sizeof(kShared.m_ahHeightmapTiles));
    memset(kShared.field_16a4, 0, sizeof(kShared.field_16a4));
    RecreateGPUHeightmapAndIDMap();

    field_232c = 0x20;
    kShared.m_vSunDir.x = 0.707f;
    kShared.m_vSunDir.y = 0.0f;
    kShared.m_vSunDir.z = 0.707f;
    m_nShadowSteps = 20;
    SetBlendWeights();
    for (unsigned int i = 0; i < static_cast<unsigned int>(kShared.m_nTextures); ++i) {
        kShared.field_a08[i] = 2;
        kShared.field_708[i] = 0;
    }

    kShared.m_hDepthStencil = 0;
    FireGrafix::CRenderer9* pRenderer = FireGrafix::g_pRenderer;
    kShared.m_hQuadIB = pRenderer->CreateStaticIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(1), 6, 0, 0);
    kShared.m_hQuadVB = pRenderer->CreateStaticVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(1), 4, 0, 0);
    memcpy(pRenderer->LockStaticVB(kShared.m_hQuadVB, 0), kQuadVertices, sizeof(kQuadVertices));
    pRenderer->UnlockVB(kShared.m_hQuadVB, 0, 0);
    memcpy(pRenderer->LockStaticIB(kShared.m_hQuadIB, 0), kQuadIndices, sizeof(kQuadIndices));
    pRenderer->UnlockIB(kShared.m_hQuadIB, 0, 0);

    kShared.m_afSampleJitter[0][0] = 0.0f;
    kShared.m_afSampleJitter[0][1] = 0.0f;
    // MSVC-rand style sequence, seed 7, scaled to [-0.5, 0.5): sub-texel sample offsets.
    unsigned int uiSeed = 7;
    for (int i = 1; i < 32; ++i) {
        for (int j = 0; j < 2; ++j) {
            uiSeed = (uiSeed * 214013u + 2531011u) % 4294954991u;
            kShared.m_afSampleJitter[i][j] = static_cast<float>(static_cast<double>(uiSeed) * (1.0 / 4294954991.0)) - 0.5f;
        }
    }

    const unsigned int uiTile = GraphicsSettings::TerrainSettings::g_iMaxTileSize;
    kShared.m_hRenderTemp = pRenderer->CreateRenderTexture(uiTile, uiTile, 0, 0x71, "Terrain Render Temp");
    kShared.m_hHeightTemp = pRenderer->CreateRenderTexture(uiTile, uiTile, 0, 0x71, "Terrain Height Temp");
    kShared.m_hRenderSpecTemp = pRenderer->CreateRenderTexture(uiTile, uiTile, 0, 0x71, "Terrain Spec Temp");
    kShared.m_hShadowTemp = pRenderer->CreateRenderTexture(uiTile, uiTile, 0, 0x15, "Terrain Shadow Temp");
    kShared.m_hNormalTemp = pRenderer->CreateRenderTexture(uiTile, uiTile, 0, 0x15, "Terrain Normal Temp");
    kShared.m_hAlbedoTemp = pRenderer->CreateRenderTexture(uiTile, uiTile, 0, 0x15, "Terrain Albiedo Temp");
    kShared.m_hSpecTemp = pRenderer->CreateRenderTexture(uiTile, uiTile, 0, 0x15, "Terrain Spec Temp");
    const unsigned int uiRoot = GraphicsSettings::TerrainSettings::g_iRootCacheSize;
    kShared.m_hTempNormalCache = pRenderer->CreateRenderTexture(uiRoot, uiRoot, 0, 0x15, "Terrain Temp Normal Cache");
    kShared.m_hTempAlbedoCache = pRenderer->CreateRenderTexture(uiRoot, uiRoot, 0, 0x15, "Terrain Temp Albiedo Cache");
    kShared.m_hTempDecalCache = pRenderer->CreateRenderTexture(uiRoot, uiRoot, 0, 0x15, "Terrain Temp Decal Cache");
    kShared.m_hRenderTextureCache = pRenderer->CreateRenderTexture(uiRoot, uiRoot, 0, 0x71, "Terrain Render Texture Cache");
    kShared.m_hHeightTextureCache = pRenderer->CreateRenderTexture(uiRoot, uiRoot, 0, 0x71, "Terrain Height Texture Cache");
    kShared.m_hShadowTextureCache = pRenderer->CreateRenderTexture(uiRoot, uiRoot, 0, 0x15, "Terrain Shadow Texture Cache");

    CalcHeightMap();
    Fun_086dd934(this, &Terrain::OnFOWChanged);
    Fun_0873dbba(this, &Terrain::OnDeviceReset);

    // Per-hex FOW grid over the whole world, one entry per 63-unit hex step.
    const FGXVector3& vWorldMin = WorldGlobals::g_WorldMin;
    const FGXVector3& vWorldMax = WorldGlobals::g_WorldMax;
    const int nHexesX = static_cast<int>((vWorldMax.x - vWorldMin.x) / (vTerrainScale.x * kHexesPerTile));
    const int nHexesY = static_cast<int>((vWorldMax.y - vWorldMin.y) / (kHexesPerTile * vTerrainScale.y));
    const int iZero = 0;
    int nGrow = nHexesY * nHexesX - static_cast<int>(m_nFOWGrid);
    if (nGrow < 0) {
        nGrow = 0;
    }
    Fun_0886c502(&m_piFOWGrid, &iZero, nGrow);
    m_iFOWGridHeight = nHexesY;
    m_iFOWGridWidth = nHexesX;
    for (int x = 0; x < nHexesX; ++x) {
        for (int y = 0; y < nHexesY; ++y) {
            const int iColumn = m_bFOWGridWrapX ? x % m_iFOWGridWidth : x;
            m_piFOWGrid[iColumn * m_iFOWGridHeight + y] = 0;
        }
    }

    BuildCellToHexTable();
    // Each cell collects a bit per continent type of the hexes it covers.
    for (unsigned int i = 0; i < m_nCells; ++i) {
        TerrainCell* pCell = m_ppCells[i];
        for (unsigned int k = 0; k < pCell->m_nHexes; ++k) {
            cvTerrainSampler* pSampler = kExternalData.m_pTerrainSampler;
            const HEXVec_i kHex = pCell->m_aHexes[k];
            const HEXVec_i kOffset = Fun_086616f0(*reinterpret_cast<const HEXMatrix_i*>(pSampler->unk_c8), kHex);
            const unsigned char ucContinent = pSampler->m_aucContinentType[kOffset.x + (kOffset.y >> 1) + kOffset.y * 255];
            pCell->m_uClassMask |= 1u << (ucContinent & 0x1f);
        }
    }

    m_fTessLOD = 0.0f;
    for (int i = 0; i < 4; ++i) {
        kShared.m_ahDXTUncompressedDiffuse[i] = 0;
        kShared.m_ahDXTDiffuse[i] = 0;
        kShared.m_ahDXTUncompressedSpec[i] = 0;
        kShared.m_ahDXTSpec[i] = 0;
    }
    m_uiPatchDivisions = GraphicsSettings::TerrainSettings::g_iTessellationPatchDivisions;
    m_fLastTessLOD = 0.0f;
    m_fTessLODStep = 0.0f;
    if (GraphicsSettings::TerrainSettings::g_bEnableDXTCompression != 0) {
        // One set per cache page size 64..512; the block-compressed side is a quarter wide.
        for (int i = 0; i < 4; ++i) {
            const int iSize = 0x40 << i;
            const unsigned int uiBlocks = iSize / 4;
            const unsigned int uiMips = 4 + i;
            kShared.m_ahDXTDiffuse[i] = pRenderer->CreateRenderTexture(uiBlocks, uiBlocks, uiMips, 2, "DXT Diffuse Intermediate");
            kShared.m_ahDXTSpec[i] = pRenderer->CreateRenderTexture(uiBlocks, uiBlocks, uiMips, 2, "DXT Spec Intermediate");
            kShared.m_ahDXTUncompressedDiffuse[i] = pRenderer->CreateRenderTexture(iSize, iSize, uiMips, 0x15, "DXT uncompressed Diffuse Intermediate");
            kShared.m_ahDXTUncompressedSpec[i] = pRenderer->CreateRenderTexture(iSize, iSize, uiMips, 0x15, "DXT uncompressed Spec Intermediate");
        }
    }

    kShared.m_hCompositeVB = pRenderer->CreateDynamicVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(0x15), 0x9920, 0);
    kShared.m_hCompressedVB = pRenderer->CreateDynamicVB(static_cast<FireGrafix::_FGX_VERTEXFORMATS>(0x10), 0x9920, 0);
    kShared.m_hDynamicIB = pRenderer->CreateDynamicIB(static_cast<FireGrafix::_FGX_INDEXFORMATS>(0), 0x396c0, 0);
    m_nCompressedVBUsed = 0;
    m_nIBUsed = 0;
    m_pCompositeVB = nullptr;
    m_nCompositeVBUsed = 0;
    m_pIB = nullptr;
    m_pCompressedVB = nullptr;
    return true;
}

/// 0x0890c454
void Terrain::RecreateGPUHeightmapAndIDMap() {
    const int iMapWidth = MapGlobals::g_pTerrainSampler->m_iHeightMapWidth;
    const int iMapHeight = MapGlobals::g_pTerrainSampler->m_iHeightMapHeight;
    unsigned char* pBytes = GetTempHeap()->Allocate(0x300000);
    if (pBytes == nullptr) {
        return;
    }
    FireGrafix::TextureData kTexture(0x1c, static_cast<FireGrafix::_TextureType>(0), kHeightTileSize, kHeightTileSize, 1, 1, 1, pBytes, 0x180000);

    SharedData& kShared = m_kShared;
    const int nTilesX = (iMapWidth + kHeightTileStep) / kHeightTileStep;
    const int nTilesY = (iMapHeight + kHeightTileStep) / kHeightTileStep;
    unsigned char abLevelTemp[512 * 512];
    for (int iTileX = 0; iTileX < nTilesX; ++iTileX) {
        for (int iTileY = 0; iTileY < nTilesY; ++iTileY) {
            // Tiles start 64 samples before their nominal origin and wrap around the map; rows are stored bottom-up.
            for (int iRow = 0; iRow < kHeightTileSize; ++iRow) {
                const int iY = (iTileY * kHeightTileStep + iMapHeight * 100 - 0x40 + iRow) % iMapHeight;
                unsigned char* pLine = pBytes + (kHeightTileSize - 1 - iRow) * kHeightTileSize;
                for (int iColumn = 0; iColumn < kHeightTileSize; ++iColumn) {
                    const int iX = (iTileX * kHeightTileStep + iMapWidth * 100 - 0x40 + iColumn) % iMapWidth;
                    cvTerrainSampler* pSampler = MapGlobals::g_pTerrainSampler;
                    const FGXInt2 kPosition = {iX, iY};
                    const FGXInt2 kWrapped = pSampler->GetWrappedPosition(kPosition);
                    pLine[iColumn] = static_cast<unsigned char>(Fun_08679d0a(&pSampler->m_iHeightMapWidth, &kWrapped) >> 8);
                }
            }

            if (GraphicsSettings::TerrainSettings::g_bUseTessellation != 0) {
                const unsigned char* pSource = pBytes;
                unsigned char* pDest = pBytes + kHeightMipOffset;
                for (int iLevel = 0; iLevel < kHeightMipLevels; ++iLevel) {
                    const int nSize = kHeightTileSize >> (iLevel + 1);
                    const int nSourceSize = kHeightTileSize >> iLevel;
                    if (nSize > 0) {
                        const unsigned char* pSourceLine = pSource;
                        unsigned char* pTempLine = abLevelTemp;
                        for (int iLine = 0; iLine < nSize; ++iLine) {
                            for (int i = 0; i < nSize; ++i) {
                                pTempLine[i] = static_cast<unsigned char>((pSourceLine[i * 2] + pSourceLine[i * 2 + 1] +
                                                                          pSourceLine[nSourceSize + i * 2] + pSourceLine[nSourceSize + i * 2 + 1]) >> 2);
                            }
                            pSourceLine += nSourceSize * 2;
                            pTempLine += nSize;
                        }

                        unsigned char* pDestLine = pDest;
                        for (int iLine = 0; iLine < nSize; ++iLine) {
                            const int aiLines[3] = {iLine > 0 ? iLine - 1 : 0, iLine, iLine < nSize - 1 ? iLine + 1 : nSize - 1};
                            for (int i = 0; i < nSize; ++i) {
                                const int aiColumns[3] = {i > 0 ? i - 1 : 0, i, i < nSize - 1 ? i + 1 : nSize - 1};
                                int iSum = 0;
                                for (int a = 0; a < 3; ++a) {
                                    for (int b = 0; b < 3; ++b) {
                                        iSum += abLevelTemp[aiLines[a] * nSize + aiColumns[b]] * g_aiHeightMipKernel[a * 3 + b];
                                    }
                                }
                                const int iCenter = abLevelTemp[iLine * nSize + i];
                                int iValue = ((iSum - iCenter) >> 3) + iCenter;
                                if (iValue < 0) {
                                    iValue = 0;
                                }
                                pDestLine[i] = iValue > 0xff ? 0xff : static_cast<unsigned char>(iValue);
                            }
                            pDestLine += nSize;
                        }
                    }
                    pSource = pDest;
                    pDest += nSize * nSize;
                }
            }

            unsigned int& rhTile = kShared.m_ahHeightmapTiles[iTileY * kHeightTilesPerRow + iTileX];
            if (rhTile == 0) {
                rhTile = FireGrafix::g_pRenderer->CreateTexture(&kTexture);
            } else {
                FireGrafix::g_pRenderer->RecreateTexture(rhTile, &kTexture, true);
            }
            m_uiHeightmapBuiltFrame = FireGrafix::g_pRenderer->m_uFrame;
        }
    }
    GetTempHeap()->DeAllocate(pBytes);
}

/// 0x0890c980
void Terrain::SetBlendWeights() {
    BlendWeights& kWeights = m_kShared.m_kBlendWeights;
    float fTotal = 0.0f;
    for (int y = 0; y < 5; ++y) {
        for (int x = 0; x < 5; ++x) {
            const int i = y * 5 + x;
            const float fWeight = kBlendKernel1D[x] * kBlendKernel1D[y];
            kWeights.m_afWeight[i] = fWeight;
            fTotal = fWeight + fTotal;
            kWeights.m_aiOffsetX[i] = x - 2;
            kWeights.m_aiOffsetY[i] = y - 2;
        }
    }
    kWeights.m_nWeights = 25;
    for (int i = 0; i < 25; ++i) {
        kWeights.m_afWeight[i] = kWeights.m_afWeight[i] / fTotal;
    }
}

/// 0x0890ca28
void Terrain::CalcHeightMap() {
    const int iHeightMapWidth = m_kShared.m_iHeightMapWidth;
    const int iHeightMapHeight = m_kShared.m_iHeightMapHeight;
    Fun_08910f56(&field_1adc);
    // Cells are 64 samples apart and 65 wide, so neighbours share an edge row.
    Fun_0891116e(&field_1adc, (iHeightMapWidth + 0x3eu) >> 6, (iHeightMapHeight + 0x3eu) >> 6);
    int iCell = 0;
    int iRemainingX = iHeightMapWidth;
    for (unsigned int uiCellX = 0; uiCellX < m_nCellsX; ++uiCellX, iRemainingX -= 0x40) {
        const int iWidth = iRemainingX < 0x42 ? iRemainingX : 0x41;
        int iRemainingY = iHeightMapHeight;
        for (unsigned int uiCellY = 0; uiCellY < m_nCellsY; ++uiCellY, iRemainingY -= 0x40) {
            TerrainCell* pCell = m_ppCells[iCell++];
            const float afOrigin[4] = {
                static_cast<float>(static_cast<int>(uiCellX << 6)) - static_cast<float>(iHeightMapWidth) * 0.5f,
                static_cast<float>(static_cast<int>(uiCellY << 6)) - static_cast<float>(iHeightMapHeight) * 0.5f,
                0.0f,
                0.0f,
            };
            Fun_08911328(pCell, this, &m_kShared, &m_kJobSet, afOrigin);
            const int iHeight = iRemainingY < 0x42 ? iRemainingY : 0x41;
            pCell->SetBuildParameters(uiCellX << 6, uiCellY << 6, iWidth, iHeight);
        }
    }
}

/// 0x0890cc00
void Terrain::OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    // HexFOWStateChangedData is not declared yet: +4 short x, +6 short y, +8 state, +9 "whole map".
    const unsigned char* pEvent = reinterpret_cast<const unsigned char*>(&kEvent);
    const short sX = *reinterpret_cast<const short*>(pEvent + 4);
    const short sY = *reinterpret_cast<const short*>(pEvent + 6);
    const unsigned char ucState = pEvent[8];
    const bool bWholeMap = pEvent[9] != 0;

    int iValue = 0;
    if (ucState == 0) {
        iValue = 2;
    } else if (ucState == 1) {
        iValue = 1;
    }

    if (!bWholeMap) {
        const HEXVec_i kHex = {sX, sY, -(sY + sX)};
        Fun_08911508(unk_2358, &kHex, &iValue);
        FGXInt2 kCell = {sX, sY};
        SetGamePlayCell(kCell);
        return;
    }

    const int iWidth = m_iFOWGridWidth;
    const int iHeight = m_iFOWGridHeight;
    for (int x = 0; x < iWidth; ++x) {
        for (int y = 0; y < iHeight; ++y) {
            int iColumn = x;
            if (m_bFOWGridWrapX) {
                while (iColumn < 0) {
                    iColumn += m_iFOWGridWidth;
                }
                iColumn = iColumn % m_iFOWGridWidth;
            }
            m_piFOWGrid[iColumn * m_iFOWGridHeight + y] = iValue;
        }
    }
    for (int x = 0; x < iWidth; ++x) {
        for (int y = 0; y < iHeight; ++y) {
            FGXInt2 kCell = {x, y};
            SetGamePlayCell(kCell);
        }
    }
}

/// 0x0890cde0
void Terrain::OnDeviceReset(EventTemplate<Event_Int2Type<106>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    for (unsigned int i = 0; i < m_nCells; ++i) {
        m_ppCells[i]->DeleteSurfacesAndBuffers(true, true, true);
        m_ppCells[i]->m_eStatus = kStatusNone;
    }
}

/// 0x0890ce4e
void Terrain::BuildCellToHexTable() {
    const int iGridWidth = m_iFOWGridWidth;
    const int iGridHeight = m_iFOWGridHeight;
    // Identity times the row squash, multiplied in last by every hex-to-texture matrix below.
    D3DXMATRIX kIdentity;
    memset(&kIdentity, 0, sizeof(kIdentity));
    kIdentity._11 = 1.0f;
    kIdentity._22 = 1.0f;
    kIdentity._33 = 1.0f;
    kIdentity._44 = 1.0f;
    D3DXMATRIX kScale;
    D3DXMatrixScaling(&kScale, 1.0f, kHexRowScale, 1.0f);
    D3DXMATRIX kRowScale;
    D3DXMatrixMultiply(&kRowScale, &kIdentity, &kScale);
    // Translation to the hex, then g_HexToTex, then the row squash.
    const auto buildHexTexMatrix = [&kRowScale](D3DXMATRIX* pOut, const HEXVec& vPoint) {
        D3DXMATRIX kTranslation;
        HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&kTranslation), vPoint);
        D3DXMATRIX kTranslated;
        D3DXMatrixMultiply(&kTranslated, &kTranslation, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));
        D3DXMatrixMultiply(pOut, &kTranslated, &kRowScale);
    };
    for (int iGridX = 0; iGridX < iGridWidth; ++iGridX) {
        for (int iGridY = 0; iGridY < iGridHeight; ++iGridY) {
            const int iOffsetX = iGridX - (iGridY >> 1);
            const HEXVec_i kGridHex = {iOffsetX, iGridY, -(iOffsetX + iGridY)};
            // g_GameGrid +0x40 is the grid's inverse matrix.
            const HEXMatrix_i& kGameGridInverse = *reinterpret_cast<const HEXMatrix_i*>(WorldGlobals::g_GameGrid + 0x40);
            const HEXVec_i kHexPos = Fun_086616f0(kGameGridInverse, kGridHex);
            const int iHexX = kHexPos.x;
            const int iHexY = kHexPos.y;
            const float fHexX = static_cast<float>(iHexX);
            const float fHexY = static_cast<float>(iHexY);
            const HEXVec vHex = {fHexX, fHexY, -fHexX - fHexY, 1.0f};
            cvTerrainSampler* pSampler = MapGlobals::g_pTerrainSampler;
            const float fSamplerOffsetX = pSampler->m_fOffsetX;
            const float fSamplerOffsetY = pSampler->m_fOffsetY;

            D3DXMATRIX kCenter;
            buildHexTexMatrix(&kCenter, vHex);
            const float fCenterX = kCenter._41;
            const float fCenterY = kCenter._42;

            FGXVector2 aOutline[6];
            for (int i = 0; i < 6; ++i) {
                const float fCornerX = fHexX + g_aHexOutlineOffsets[i].x;
                const float fCornerY = fHexY + g_aHexOutlineOffsets[i].y;
                const HEXVec vCorner = {fCornerX, fCornerY, -fCornerX - fCornerY, 1.0f};
                D3DXMATRIX kCorner;
                buildHexTexMatrix(&kCorner, vCorner);
                aOutline[i].x = kCorner._41 - fSamplerOffsetX;
                aOutline[i].y = kCorner._42 - fSamplerOffsetY;
            }

            // Every cell around the hex center that its outline touches lists the hex.
            const int iCellX = static_cast<int>((fCenterX * static_cast<float>(m_nCellsX)) / m_kShared.m_fWorldWidth);
            const int iCellY = static_cast<int>((fCenterY * static_cast<float>(m_nCellsY)) / m_kShared.m_fWorldHeight);
            for (int x = iCellX - 1; x <= iCellX + 1; ++x) {
                for (int y = iCellY - 1; y <= iCellY + 1; ++y) {
                    TerrainCell* pCell = m_ppCells[m_nCellsY * ((m_nCellsX + x) % m_nCellsX) + (m_nCellsY + y) % m_nCellsY];
                    bool bTouched = false;
                    for (int i = 0; i < 6; ++i) {
                        if (pCell->IntersectLine(&aOutline[i], &aOutline[(i + 1) % 6])) {
                            bTouched = true;
                        }
                    }
                    if (bTouched) {
                        const HEXVec_i kCellHex = {iHexX, iHexY, -(iHexX + iHexY)};
                        pCell->m_aHexes[pCell->m_nHexes++] = kCellHex;
                    }
                }
            }
        }
    }
}

/// 0x0890d8c4
void Terrain::DeleteCells() {
    Fun_08910f56(&field_1adc);
}

/// 0x0890d8ea
void Terrain::GetBaseVBandIB(int iWidth, int iHeight, unsigned int* phVB, unsigned int* phIB, unsigned int* phPatchIB, unsigned int* puiField8) {
    FStaticVector<BaseMesh, 16>& kMeshes = m_kShared.m_kBaseMeshes;
    unsigned int i = 0;
    for (; i < kMeshes.size(); ++i) {
        const BaseMesh& kMesh = kMeshes[i];
        if (kMesh.m_iWidth == iWidth && kMesh.m_iHeight == iHeight) {
            if (phIB != nullptr) {
                *phIB = kMesh.m_hIB;
            }
            if (phVB != nullptr) {
                *phVB = kMesh.m_hVB;
            }
            if (phPatchIB != nullptr) {
                *phPatchIB = kMesh.m_hPatchIB;
            }
            if (puiField8 != nullptr) {
                *puiField8 = kMesh.field_8;
            }
            break;
        }
    }
    if (i != kMeshes.size()) {
        return;
    }

    BaseMesh kMesh;
    kMesh.m_hIB = CreateIndexLattice(iWidth, iHeight, m_kShared.m_iTessLevel, &kMesh.field_8);
    kMesh.m_hVB = CreateBaseTerrainCellVB(iWidth, iHeight, 2);
    kMesh.m_hPatchIB = CreatePatchIndexLattice(iWidth, iHeight);
    if (phIB != nullptr) {
        *phIB = kMesh.m_hIB;
    }
    if (phVB != nullptr) {
        *phVB = kMesh.m_hVB;
    }
    if (phPatchIB != nullptr) {
        *phPatchIB = kMesh.m_hPatchIB;
    }
    if (puiField8 != nullptr) {
        *puiField8 = kMesh.field_8;
    }
    kMesh.m_iWidth = iWidth;
    kMesh.m_iHeight = iHeight;
    Fun_08910eec(&kMeshes, &kMesh);
}

/// 0x0890da3c
void Terrain::GetIndexBufferSpace(unsigned int nIndices, unsigned short** ppIndices, unsigned int* puiFirstIndex) {
    *puiFirstIndex = m_nIBUsed;
    *ppIndices = m_pIB + m_nIBUsed;
    m_nIBUsed += nIndices;
}

/// 0x0890da70
void Terrain::GetVertexBufferSpaceComposite(unsigned int nVertices, FireGrafix::VStruct<21>** ppVertices, unsigned int* puiFirstVertex) {
    *puiFirstVertex = m_nCompositeVBUsed;
    *ppVertices = reinterpret_cast<FireGrafix::VStruct<21>*>(reinterpret_cast<unsigned char*>(m_pCompositeVB) + m_nCompositeVBUsed * kCompositeVertexSize);
    m_nCompositeVBUsed += nVertices;
}

/// 0x0890daa2
void Terrain::GetVertexBufferSpaceCompressed(unsigned int nVertices, FireGrafix::VStruct<16>** ppVertices, unsigned int* puiFirstVertex) {
    *puiFirstVertex = m_nCompressedVBUsed;
    *ppVertices = reinterpret_cast<FireGrafix::VStruct<16>*>(reinterpret_cast<unsigned char*>(m_pCompressedVB) + m_nCompressedVBUsed * kCompressedVertexSize);
    m_nCompressedVBUsed += nVertices;
}

/// 0x0890dad4
void Terrain::LockBuffers() {
    FireGrafix::CRenderer9* pRenderer = FireGrafix::g_pRenderer;
    m_pCompositeVB = static_cast<FireGrafix::VStruct<21>*>(pRenderer->LockDynamicVB(m_kShared.m_hCompositeVB, 0));
    m_pCompressedVB = static_cast<FireGrafix::VStruct<16>*>(pRenderer->LockDynamicVB(m_kShared.m_hCompressedVB, 0));
    m_pIB = static_cast<unsigned short*>(pRenderer->LockDynamicIB(m_kShared.m_hDynamicIB, 0));
    m_nCompressedVBUsed = 0;
    m_nCompositeVBUsed = 0;
    m_nIBUsed = 0;
}

/// 0x0890db7c
void Terrain::UnlockBuffers() {
    FireGrafix::CRenderer9* pRenderer = FireGrafix::g_pRenderer;
    pRenderer->UnlockVB(m_kShared.m_hCompositeVB, 0, m_nCompositeVBUsed);
    pRenderer->UnlockVB(m_kShared.m_hCompressedVB, 0, m_nCompressedVBUsed);
    pRenderer->UnlockIB(m_kShared.m_hDynamicIB, 0, m_nIBUsed);
    m_pCompressedVB = nullptr;
    m_pCompositeVB = nullptr;
    m_pIB = nullptr;
}

/// 0x0890dc30
unsigned int Terrain::GetRenderTexture(unsigned int uiSize, unsigned int* puiActualSize) {
    // Without DXT a missing page falls back to the next smaller size, down to 64.
    unsigned int hTexture = 0;
    do {
        if (hTexture != 0) {
            break;
        }
        TexturePool* pPool;
        switch (uiSize) {
        case 0x40:
            pPool = &m_kFree64Textures;
            break;
        case 0x80:
            pPool = &m_kFree128Textures;
            break;
        case 0x100:
            pPool = &m_kFree256Textures;
            break;
        case 0x200:
            pPool = &m_kFree512Textures;
            break;
        default:
            return 0;
        }
        if (pPool->size() == 0) {
            uiSize >>= 1;
        } else {
            hTexture = pPool->back();
            pPool->pop_back();
        }
    } while (GraphicsSettings::TerrainSettings::g_bEnableDXTCompression == 0);
    *puiActualSize = uiSize;
    return hTexture;
}

/// 0x0890dd44
unsigned int Terrain::GetBCTexture() {
    unsigned int hTexture = 0;
    if (m_kFreeBCTextures.size() != 0) {
        hTexture = m_kFreeBCTextures.back();
        m_kFreeBCTextures.pop_back();
    }
    return hTexture;
}

/// 0x0890dd66
void Terrain::ReleaseBCTexture(unsigned int hTexture) {
    if (hTexture != 0) {
        FireGrafix::g_pRenderer->GetTextureDesc(hTexture);
        m_kFreeBCTextures.push_back(hTexture);
    }
}

/// 0x0890ddb8
void Terrain::SetRenderTextures(unsigned int* pah64, int n64, unsigned int* pah128, int n128, unsigned int* pah256, int n256, unsigned int* pah512, int n512, unsigned int* pahBC, int nBC) {
    m_kFree64Textures.resize(n64);
    m_kFree128Textures.resize(n128);
    m_kFree256Textures.resize(n256);
    m_kFree512Textures.resize(n512);
    m_kFreeBCTextures.resize(nBC);
    if (n64 != 0) {
        memcpy(m_kFree64Textures.m_pData, pah64, n64 * sizeof(unsigned int));
    }
    if (n128 != 0) {
        memcpy(m_kFree128Textures.m_pData, pah128, n128 * sizeof(unsigned int));
    }
    if (n256 != 0) {
        memcpy(m_kFree256Textures.m_pData, pah256, n256 * sizeof(unsigned int));
    }
    if (n512 != 0) {
        memcpy(m_kFree512Textures.m_pData, pah512, n512 * sizeof(unsigned int));
    }
    if (nBC != 0) {
        memcpy(m_kFreeBCTextures.m_pData, pahBC, nBC * sizeof(unsigned int));
    }
}

/// 0x0890df7a
void Terrain::ReleaseRenderTexture(unsigned int hTexture) {
    if (hTexture == 0) {
        return;
    }
    TexturePool* pPool;
    switch (FireGrafix::g_pRenderer->GetTextureDesc(hTexture)->m_uWidth) {
    case 0x40:
        pPool = &m_kFree64Textures;
        break;
    case 0x80:
        pPool = &m_kFree128Textures;
        break;
    case 0x100:
        pPool = &m_kFree256Textures;
        break;
    case 0x200:
        pPool = &m_kFree512Textures;
        break;
    default:
        return;
    }
    pPool->push_back(hTexture);
}

/// 0x0890e008
void Terrain::SetGlobals(D3DXMATRIX* pView, D3DXMATRIX* pInvView, D3DXMATRIX* pProj, D3DXMATRIX* pShadowViewProj, D3DXMATRIX* pOverlayProj, float fShadowFade, FireGrafix::CCommandStream* pStream) {
    FireGrafix::TerrainGlobalData* pGlobalData = FireGrafix::g_pTerrainGlobalData;
    if (pStream != nullptr) {
        pGlobalData = static_cast<FireGrafix::TerrainGlobalData*>(pGlobalData->AliasGlobal(pStream));
    }
    TerrainGlobalConstants* pConstants = static_cast<TerrainGlobalConstants*>(pGlobalData->LockGlobal(0));
    D3DXMatrixMultiply(&pConstants->m_mViewProj, pView, pProj);
    pConstants->m_mOverlayProj = *pOverlayProj;
    pConstants->m_vTerrainScale = m_kShared.m_vTerrainScale;
    pConstants->m_vSunDir = m_kShared.m_vSunDir;
    pConstants->m_vReflectDir = m_kShared.m_vReflectDir;
    pConstants->m_fScreenWidth = static_cast<float>(FireGrafix::CRenderer9::m_nScreenWidth);
    pConstants->m_fScreenHeight = static_cast<float>(FireGrafix::CRenderer9::m_nScreenHeight);
    memcpy(pConstants->m_afBasis, kBasis, sizeof(kBasis));

    static unsigned int s_uMinPatchDivisions = 0x41;
    static unsigned int s_uMaxPatchDivisions = 0;
    if (s_uMinPatchDivisions > m_uiPatchDivisions) {
        s_uMinPatchDivisions = m_uiPatchDivisions;
    }
    if (s_uMaxPatchDivisions < m_uiPatchDivisions) {
        s_uMaxPatchDivisions = m_uiPatchDivisions;
    }
    pConstants->m_fPatchDivisions = static_cast<float>(m_uiPatchDivisions);
    pConstants->field_f0 = 1.0f;
    memcpy(pConstants->m_afFOWBaseScale, &FrameGlobals::g_FOWBaseScale, sizeof(pConstants->m_afFOWBaseScale));

    pConstants->m_vInvViewAxisZ.x = pInvView->_13;
    pConstants->m_vInvViewAxisZ.y = pInvView->_23;
    pConstants->m_vInvViewAxisZ.z = pInvView->_33;
    pConstants->m_vInvViewPos.x = pInvView->_41;
    pConstants->m_vInvViewPos.y = pInvView->_42;
    pConstants->m_vInvViewPos.z = pInvView->_43;
    D3DXVECTOR3 vAxisZPlusReflectDir = {
        pInvView->_13 + m_kShared.m_vReflectDir.x,
        pInvView->_23 + m_kShared.m_vReflectDir.y,
        pInvView->_33 + m_kShared.m_vReflectDir.z,
    };
    D3DXVec3Normalize(&vAxisZPlusReflectDir, &vAxisZPlusReflectDir);
    pConstants->m_vAxisZPlusReflectDir.x = vAxisZPlusReflectDir.x;
    pConstants->m_vAxisZPlusReflectDir.y = vAxisZPlusReflectDir.y;
    pConstants->m_vAxisZPlusReflectDir.z = vAxisZPlusReflectDir.z;

    pConstants->m_mShadowViewProj = *pShadowViewProj;
    pConstants->m_fShadowFade = fShadowFade;
    pConstants->field_160 = 1.0f / 1024.0f;
    pConstants->field_164 = -1.0f / 1024.0f;
    pConstants->m_fHeightMapWidth = static_cast<float>(m_kShared.m_iHeightMapWidth);
    pConstants->m_fHeightMapHeight = static_cast<float>(m_kShared.m_iHeightMapHeight);
    pConstants->field_170 = 1.0f / 512.0f;
    pConstants->field_174 = 1.0f / 512.0f;
    pConstants->field_240 = m_kShared.field_520;
    pConstants->field_1a0 = static_cast<int>(m_eLighting) == 2 ? 1.0f : 0.0f;
    pConstants->field_1d0 = 1.0f;
    pConstants->field_1e0 = field_240c != 0 ? 1.0f : 0.0f;
    pConstants->field_1c0 = field_2418 != 0 ? 1.0f : 0.0f;
    pConstants->field_1b0 = field_241c != 0 ? 1.0f : 0.0f;
    pGlobalData->Unlock(0);

    FireGrafix::FogGlobalData_Terrain* pFogData = FireGrafix::g_pFogGlobalData_Terrain;
    TerrainFogConstants* pFog = static_cast<TerrainFogConstants*>(pFogData->LockGlobal(0));
    memcpy(pFog->m_afColor, &FrameGlobals::g_v4FogColor, sizeof(pFog->m_afColor));
    const float* pFogControl = &FrameGlobals::g_v4FogControl.x;
    pFog->field_10 = -pFogControl[0];
    pFog->field_14 = -(pFogControl[1] * expf(-(pInvView->_43 * pFogControl[0]))) / pFogControl[0];
    pFog->field_18 = 0.0f;
    pFog->field_1c = pFogControl[3];
    pFogData->Unlock(0);

    if (pStream != nullptr) {
        FireGrafix::FGXConstantSetManager::ReleaseConstantSet(pGlobalData->field_34);
    }
}

/// 0x0890e57c
void Terrain::FindCells(int iX, int iY, int iRadius, TerrainCell** ppCells, int* pnCells) {
    int nFound = 0;
    for (unsigned int i = 0; i < m_nCells; ++i) {
        TerrainCell* pCell = m_ppCells[i];
        if (pCell->m_iX <= iX + iRadius && iX - iRadius <= pCell->m_iX + pCell->m_iWidth &&
            pCell->m_iY <= iY + iRadius && iY - iRadius <= pCell->m_iY + pCell->m_iHeight) {
            ppCells[nFound++] = pCell;
        }
    }
    *pnCells = nFound;
}

/// 0x0890e622
void Terrain::FreeTiles(unsigned int nCandidates, unsigned int uiMinFree) {
    // Candidates are sorted by SortLRU; eviction walks them from the least recently used end.
    for (unsigned int i = 0; i < nCandidates; ++i) {
        TerrainCell* pCell = m_ppEvictionCandidates[i];
        if (pCell->m_eStatus == kStatus3) {
            pCell->DemoteToCacheOnly();
        }
        if (pCell->m_eStatus == kStatus5) {
            pCell->m_eStatus = kStatusBaseOnly;
            pCell->RemoveBase();
        }
    }

    int i = static_cast<int>(nCandidates);
    if (m_kFree64Textures.size() < uiMinFree) {
        do {
            if (--i < 0) {
                break;
            }
            TerrainCell* pCell = m_ppEvictionCandidates[i];
            if (pCell->m_eStatus == kStatusCacheOnly) {
                pCell->DeleteSurfacesAndBuffers(true, true, true);
                pCell->m_eStatus = kStatusNone;
            }
        } while (m_kFree64Textures.size() <= uiMinFree);
    }

    i = static_cast<int>(nCandidates);
    if (GraphicsSettings::TerrainSettings::g_bEnableDXTCompression != 0 && m_kFreeBCTextures.size() < uiMinFree) {
        do {
            if (--i < 0) {
                break;
            }
            TerrainCell* pCell = m_ppEvictionCandidates[i];
            if (pCell->m_eStatus == kStatusCacheOnly) {
                pCell->DeleteSurfacesAndBuffers(true, true, true);
                pCell->m_eStatus = kStatusNone;
            }
        } while (m_kFreeBCTextures.size() <= uiMinFree);
    }

    TexturePool* apPools[3] = {&m_kFree128Textures, &m_kFree256Textures, &m_kFree512Textures};
    const unsigned int auiSizes[3] = {0x80, 0x100, 0x200};
    for (int iPool = 0; iPool < 3; ++iPool) {
        i = static_cast<int>(nCandidates);
        if (apPools[iPool]->size() >= uiMinFree) {
            continue;
        }
        do {
            if (--i < 0) {
                break;
            }
            TerrainCell* pCell = m_ppEvictionCandidates[i];
            if (pCell->m_uPageSize == auiSizes[iPool] && pCell->m_eStatus != kStatusNone) {
                pCell->DemoteToCacheOnly();
            }
        } while (apPools[iPool]->size() <= uiMinFree);
    }
}

/// 0x0890e86c
bool Terrain::CachePageAvailable(int iSize) {
    switch (iSize) {
    case 0x40:
        return m_kFree64Textures.size() > 1;
    case 0x80:
        return m_kFree128Textures.size() > 1;
    case 0x100:
        return m_kFree256Textures.size() > 1;
    case 0x200:
        return m_kFree512Textures.size() > 1;
    default:
        return false;
    }
}

/// 0x0890e8c6
bool Terrain::LaunchJobs(bool a1, bool a2, bool a3) {
    const unsigned int nCameras = FrameGlobals::g_nCameras;
    for (int i = 0; i < 4; ++i) {
        m_anVisibleCells[i] = 0;
    }
    const int bMinimizeGrayTiles = GraphicsSettings::TerrainSettings::g_bMinimizeGrayTiles;

    // Cells that pass the frustum test of any camera are visible; the rest with a resident status are eviction candidates.
    int nEvict = 0;
    for (unsigned int iCell = 0; iCell < m_nCells; ++iCell) {
        TerrainCell* pCell = m_ppCells[iCell];
        if (static_cast<unsigned int>(pCell->field_328 - 1) < 2) {
            continue;
        }
        if (pCell->m_iFOWFlags != 0 && nCameras != 0) {
            bool bVisible = false;
            for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
                if (m_bFrustumCull == 0 || pCell->IsVisible(FrameGlobals::g_ViewClipPlanes[uCamera])) {
                    m_appVisibleCells[uCamera][m_anVisibleCells[uCamera]++] = pCell;
                    bVisible = true;
                }
            }
            if (bVisible) {
                continue;
            }
        }
        if (pCell->m_eStatus != kStatusNone) {
            m_ppEvictionCandidates[nEvict++] = pCell;
        }
    }
    ASL_Msvc_qsort(m_ppEvictionCandidates, nEvict, sizeof(TerrainCell*), SortLRU);
    FreeTiles(nEvict, 8);

    for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
        D3DXMATRIX kViewProj;
        D3DXMatrixMultiply(&kViewProj, &FrameGlobals::g_View[uCamera], &FrameGlobals::g_Proj);
        for (unsigned int i = 0; i < m_anVisibleCells[uCamera]; ++i) {
            TerrainCell* pCell = m_appVisibleCells[uCamera][i];
            pCell->m_uMipSizeNeeded = pCell->CalcMIPLevelNeeded(&kViewProj);
            pCell->m_bLaunchedThisFrame = false;
            switch (pCell->m_eStatus) {
            case kStatusNone:
                // Out of cache pages: evict up to 16 more candidates and retry, at most 20 times.
                for (int iTry = 0; iTry < 0x14; ++iTry) {
                    if (pCell->RequestCacheTexture()) {
                        pCell->m_eTargetStatus = kStatus3;
                        break;
                    }
                    const int iFloor = nEvict - 0x10 < 0 ? 0 : nEvict - 0x10;
                    while (nEvict > iFloor) {
                        --nEvict;
                        m_ppEvictionCandidates[nEvict]->DeleteSurfacesAndBuffers(true, true, true);
                        m_ppEvictionCandidates[nEvict]->m_eStatus = kStatusNone;
                    }
                }
                break;
            case kStatusCacheOnly:
                pCell->m_eTargetStatus = kStatus3;
                break;
            case kStatusBaseOnly:
                pCell->m_eTargetStatus = kStatus5;
                break;
            case kStatus5: {
                if (pCell->m_uBuiltFrame < pCell->m_uInvalidatedFrame) {
                    pCell->CalculateTextureContributions();
                    pCell->m_eTargetStatus = kStatus2;
                }
                if (pCell->m_uPageSize != pCell->m_uMipSizeNeeded && CachePageAvailable(static_cast<int>(pCell->m_uMipSizeNeeded))) {
                    pCell->m_eTargetStatus = kStatus2;
                }
                break;
            }
            default:
                break;
            }
        }
    }
    for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
        ASL_Msvc_qsort(m_appVisibleCells[uCamera], m_anVisibleCells[uCamera], sizeof(TerrainCell*), SortDistanceAndPriority);
    }

    const bool bFullBudget = bMinimizeGrayTiles != 0 || a1;
    const unsigned int nCandidateBudget = bFullBudget ? GraphicsSettings::TerrainSettings::g_uTilesToCalcPerFrame : GraphicsSettings::TerrainSettings::g_uTilesToCalcPerFrameMoving;
    const unsigned int nLaunchBudget = bFullBudget ? 8 : GraphicsSettings::TerrainSettings::g_uTilesToCalcPerFrameMoving;
    FStaticVector<TerrainCell*, 32> kCandidates;
    unsigned int nExamined = 0;
    bool bLaunchedAny = false;
    if (nCameras != 0) {
        // Resident cells first; a cell that is already heading for the final status ends its camera's scan.
        for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
            for (unsigned int i = 0; i < m_anVisibleCells[uCamera]; ++i) {
                if (kCandidates.size() >= nCandidateBudget) {
                    break;
                }
                TerrainCell* pCell = m_appVisibleCells[uCamera][i];
                const TerrainCellStatus eStatus = pCell->m_eStatus;
                if (eStatus == kStatusNone) {
                    continue;
                }
                if ((eStatus == kStatusBaseOnly || eStatus == kStatus5) && pCell->m_eTargetStatus == kStatus5) {
                    break;
                }
                kCandidates.push_back(pCell);
            }
        }
        for (nExamined = 0; nExamined < kCandidates.size(); ++nExamined) {
            if (nExamined >= nLaunchBudget) {
                break;
            }
            TerrainCell* pCell = kCandidates[nExamined];
            if (pCell->RequestUncompressedTextures()) {
                pCell->m_eTargetStatus = kStatus5;
                pCell->LaunchJobs(&m_kJobSet);
                bLaunchedAny = true;
                m_kBuildList.push_back(pCell);
            }
        }
    }

    bool bResult = true;
    if (!bLaunchedAny && m_uiBackgroundInitCursor < m_nBackgroundInitCells) {
        TerrainCell* pCell = m_ppBackgroundInitCells[m_uiBackgroundInitCursor];
        if (pCell->m_eStatus == kStatusNone && pCell->m_eTargetStatus == kStatusNone && nExamined < nLaunchBudget) {
            ++nExamined;
            if (pCell->RequestCacheTexture()) {
                pCell->m_eTargetStatus = kStatusCacheOnly;
                pCell->LaunchJobs(&m_kJobSet);
                m_kBuildList.push_back(pCell);
            }
        }
        ++m_uiBackgroundInitCursor;
        bResult = false;
    }

    for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
        for (unsigned int i = 0; i < m_anVisibleCells[uCamera]; ++i) {
            TerrainCell* pCell = m_appVisibleCells[uCamera][i];
            const TerrainCellStatus eStatus = pCell->m_eStatus;
            bool bLaunch = false;
            if (pCell->m_eTargetStatus == kStatus3) {
                if (eStatus == kStatusCacheOnly) {
                    bLaunch = true;
                } else if (eStatus != kStatus3) {
                    if (nExamined < nLaunchBudget) {
                        ++nExamined;
                        bLaunch = true;
                    } else if (eStatus == kStatusNone) {
                        pCell->DeleteSurfacesAndBuffers(false, true, true);
                        pCell->m_eTargetStatus = kStatusNone;
                    }
                }
            } else if (eStatus == kStatusBaseOnly) {
                bLaunch = true;
            }
            if (bLaunch) {
                if (!pCell->m_bLaunchedThisFrame) {
                    pCell->LaunchJobs(&m_kJobSet);
                    pCell->m_bLaunchedThisFrame = true;
                    m_kBuildList.push_back(pCell);
                }
                bResult = false;
            }
        }
    }
    return bResult;
}

/// 0x0890f10e
void Terrain::SetTransformWeights() {
    float* pWeights = static_cast<float*>(FireGrafix::g_pTerrainGlobalTextureData->LockGlobal(0));
    SharedData& kShared = m_kShared;
    if (kShared.m_nProjections != 0) {
        const float fWorldSpan = WorldGlobals::g_WorldMax.x - WorldGlobals::g_WorldMin.x;
        for (unsigned int i = 0; i < kShared.m_nProjections; ++i) {
            const float fTextureScale = kShared.field_e10[kShared.m_aiProjectionTexture[i]];
            const unsigned int uAxis = kShared.m_aiProjectionAxis[i];
            const float fQuantum = uAxis == 0 ? kAxisZeroScale : kOtherAxisScale;
            pWeights[i * 4] = (static_cast<float>(static_cast<int>(fTextureScale * fQuantum * fWorldSpan)) / fWorldSpan) / fQuantum;
            pWeights[i * 4 + 1] = static_cast<float>(uAxis);
            pWeights[i * 4 + 2] = 0.0f;
            pWeights[i * 4 + 3] = 0.0f;
        }
    }
    FireGrafix::g_pTerrainGlobalTextureData->Unlock(0);
}

/// 0x0890f21e
void Terrain::Render(FireGrafix::CCommandStream* pStream, FireGrafix::CCommandStream* pBuildStream, unsigned int a3, bool a4, bool a5, bool a6) {
    const unsigned int nCameras = FrameGlobals::g_nCameras;
    const float fShadowFade = FrameGlobals::fShadowFade;
    m_kJobSet.Wait();

    D3DXMATRIX kIdentity;
    memset(&kIdentity, 0, sizeof(kIdentity));
    kIdentity._11 = 1.0f;
    kIdentity._22 = 1.0f;
    kIdentity._33 = 1.0f;
    kIdentity._44 = 1.0f;
    SetGlobals(&kIdentity, &kIdentity, &kIdentity, &kIdentity, &kIdentity, 0.0f, nullptr);
    SetTransformWeights();
    if (m_kBuildList.size() != 0) {
        LockBuffers();
        for (unsigned int i = 0; i < m_kBuildList.size(); ++i) {
            m_kBuildList[i]->Build(FireGrafix::g_pRenderer->m_uFrame, pBuildStream);
        }
        m_kBuildList.clear();
        UnlockBuffers();
    }

    if (GraphicsSettings::TerrainSettings::g_bUseTessellation != 0 && nCameras != 0 && m_anVisibleCells[0] != 0) {
        D3DXMATRIX kViewProj;
        D3DXMatrixMultiply(&kViewProj, &FrameGlobals::g_View[0], &FrameGlobals::g_Proj);
        float fTessLOD = kTessLODCeiling;
        for (unsigned int i = 0; i < m_anVisibleCells[0]; ++i) {
            const float fNeeded = m_appVisibleCells[0][i]->CalcTessLODNeeded(&kViewProj);
            if (fNeeded < fTessLOD) {
                fTessLOD = fNeeded;
            }
        }
        // Move to the new level only when it differs from the last one by more than a step, or keeps the step's direction.
        const float fLastTessLOD = m_fLastTessLOD;
        const float fDelta = fTessLOD - fLastTessLOD;
        if (fabsf(fDelta) > fabsf(m_fTessLODStep) || (m_fTessLODStep * fDelta >= 0.0f && fDelta != 0.0f)) {
            float fFloorSource;
            if (fTessLOD >= 0.0f) {
                m_uiPatchDivisions = GraphicsSettings::TerrainSettings::g_iTessellationPatchDivisions;
                fFloorSource = fTessLOD;
            } else {
                m_uiPatchDivisions = GraphicsSettings::TerrainSettings::g_iTessellationPatchDivisions << static_cast<int>(kPatchShiftPerLOD * fTessLOD);
                fFloorSource = 0.0f;
            }
            m_fTessLOD = floorf(fFloorSource);
            const float fFloorDelta = floorf(fTessLOD) - floorf(fLastTessLOD);
            if (fFloorDelta != 0.0f) {
                m_fTessLODStep = fFloorDelta * kTessLODStepBlend;
            }
            m_fLastTessLOD = fTessLOD;
        }
    }

    unsigned int nCellsRendered = 0;
    for (unsigned int uCamera = 0; uCamera < nCameras; ++uCamera) {
        const FGXVector4 vClear = {0.0f, 0.0f, 0.0f, 0.0f};
        const unsigned int uCommandSet = pStream->BeginCommandSet(a3, 0, 0, static_cast<unsigned int>(a6) << 4, "Terrain Cell Render", vClear, 1.0f, 0, 0, 0);
        SetGlobals(&FrameGlobals::g_View[uCamera], &FrameGlobals::g_InvView[uCamera], &FrameGlobals::g_Proj, &FrameGlobals::g_ShadowViewProj[uCamera], &FrameGlobals::g_OverlayProjection[uCamera], fShadowFade, pStream);
        for (unsigned int i = 0; i < m_anVisibleCells[uCamera]; ++i) {
            m_appVisibleCells[uCamera][i]->Render(pStream, &FrameGlobals::g_View[uCamera], &FrameGlobals::g_Proj);
        }
        nCellsRendered += m_anVisibleCells[uCamera];
        pStream->EndCommandSet();
        pStream->AddCommandSetToRenderQueue(uCommandSet);
    }
    if (m_uiHeightmapBuiltFrame < m_uiHeightmapDirtyFrame) {
        RecreateGPUHeightmapAndIDMap();
    }
    m_nCellsRendered = nCellsRendered;
}

/// 0x0890f78c
void Terrain::RenderOverlay(FireGrafix::CCommandStream* pStream) {
    const unsigned int nCameras = FrameGlobals::g_nCameras;
    const float fShadowFade = FrameGlobals::fShadowFade;
    // The second handle of RenderGlobals::g_TexHandles is the overlay render target.
    const unsigned int hOverlayTarget = RenderGlobals::g_TexHandles[1];

    FGXVector4 vClear = {0.0f, 0.0f, 0.0f, 0.0f};
    unsigned int uiCommandSet = pStream->BeginCommandSet(hOverlayTarget, 0, 9, 4, "Terrain Cell Overlay Render Clear", vClear, 1.0f, 0, 0, 0);
    pStream->EndCommandSet();
    pStream->AddCommandSetToRenderQueue(uiCommandSet);

    D3DXMATRIX* pProj = &FrameGlobals::g_Proj;
    D3DXMATRIX* pShadowViewProj = FrameGlobals::g_ShadowViewProj;
    for (unsigned int uiCamera = 0; uiCamera < nCameras; ++uiCamera) {
        D3DXMATRIX* pView = &FrameGlobals::g_View[uiCamera];
        D3DXMATRIX* pInvView = &FrameGlobals::g_InvView[uiCamera];
        D3DXMATRIX* pOverlayProj = &FrameGlobals::g_OverlayProjection[uiCamera];

        FGXVector4 vNoClear = {0.0f, 0.0f, 0.0f, 0.0f};
        uiCommandSet = pStream->BeginCommandSet(hOverlayTarget, 0, 9, 0, "Terrain Screen Space Overlay Render", vNoClear, 1.0f, 0, 0, 0);
        SetGlobals(pView, pInvView, pProj, pShadowViewProj, pOverlayProj, fShadowFade, pStream);
        for (unsigned int i = 0; i < m_anVisibleCells[uiCamera]; ++i) {
            m_appVisibleCells[uiCamera][i]->RenderOverlay(pStream, pView, pProj);
        }
        pStream->EndCommandSet();
        pStream->AddCommandSetToRenderQueue(uiCommandSet);
    }
}

/// 0x0890f9c6
bool Terrain::IntersectRay(FGXVector3 vOrigin, FGXVector3 vDir, FGXVector2* pTexCoord, FGXInt2* pHit, float* pfDistance) {
    *pfDistance = FLT_MAX;
    if (m_nCells == 0) {
        return false;
    }
    for (unsigned int i = 0; i < m_nCells; ++i) {
        TerrainCell* pCell = m_ppCells[i];
        if (static_cast<unsigned int>(pCell->field_328 - 1) < 2) {
            continue;
        }
        FGXVector2 vTexCoord;
        FGXInt2 kHit = {0, 0};
        if (pCell->IntersectRay(vOrigin, vDir, &vTexCoord, &kHit, pfDistance)) {
            *pTexCoord = vTexCoord;
            *pHit = kHit;
        }
    }
    return *pfDistance != FLT_MAX;
}

/// 0x0890fa9c
void Terrain::PopulateTextureBuildList(bool bUseVisibility) {
    // Cells with FOW flags (all cells under g_bBlockOnLoad) are built first, the rest in the background.
    m_nPriorityInitCells = 0;
    m_nBackgroundInitCells = 0;
    const bool bBlockOnLoad = GraphicsSettings::TerrainSettings::g_bBlockOnLoad != 0;
    const unsigned int nCameras = FrameGlobals::g_nCameras;
    for (unsigned int i = 0; i < m_nCells; ++i) {
        TerrainCell* pCell = m_ppCells[i];
        if (pCell->field_328 == 1) {
            continue;
        }
        bool bPriority = bBlockOnLoad;
        if (!bBlockOnLoad && pCell->m_iFOWFlags != 0) {
            if (bUseVisibility) {
                for (unsigned int uiCamera = nCameras; uiCamera-- > 0;) {
                    if (pCell->IsVisible(FrameGlobals::g_ViewClipPlanes[uiCamera])) {
                        bPriority = true;
                        break;
                    }
                }
            } else {
                bPriority = true;
            }
        }
        if (bPriority) {
            m_ppPriorityInitCells[m_nPriorityInitCells++] = pCell;
        } else {
            m_ppBackgroundInitCells[m_nBackgroundInitCells++] = pCell;
        }
    }
    m_uiPriorityInitCursor = 0;
    m_uiBackgroundInitCursor = 0;
}

/// 0x0890fc1a
void Terrain::NonBlockingTextureInit(FireGrafix::CCommandStream* pStream) {
    const int iMachine = *reinterpret_cast<const int*>(g_EventSystemLocalMachine);
    UNK_DATA* pCachedEventQueue = g_EventSystemLocalMachine + iMachine * kEventMachineStride + kTerrainCachedEventSlot;
    if (m_uiPriorityInitCursor == 0 && m_nPriorityInitCells == 0) {
        *Fun_08911626(pCachedEventQueue) = 1.0f;
    } else if (m_uiPriorityInitCursor != m_nPriorityInitCells) {
        float afIdentity[16] = {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
        };
        D3DXMATRIX* pIdentity = reinterpret_cast<D3DXMATRIX*>(afIdentity);
        SetTransformWeights();
        SetGlobals(pIdentity, pIdentity, pIdentity, pIdentity, pIdentity, 0.0f, nullptr);
        m_nInitBuildsThisFrame = 0;
        FJobSet kJobSet;
        LockBuffers();
        // At most kInitBuildsPerCall cells per call, each built synchronously.
        while (m_uiPriorityInitCursor < m_nPriorityInitCells) {
            TerrainCell* pCell = m_ppPriorityInitCells[m_uiPriorityInitCursor];
            if (!pCell->RequestCacheTexture()) {
                m_uiPriorityInitCursor = m_nPriorityInitCells;
                break;
            }
            pCell->m_eTargetStatus = kStatusCacheOnly;
            pCell->LaunchJobs(&kJobSet);
            kJobSet.Wait();
            pCell->Build(FireGrafix::g_pRenderer->m_uFrame, pStream);
            ++m_uiPriorityInitCursor;
            if (++m_nInitBuildsThisFrame >= static_cast<unsigned int>(kInitBuildsPerCall)) {
                break;
            }
        }
        UnlockBuffers();
        *Fun_08911626(pCachedEventQueue) = static_cast<float>(m_uiPriorityInitCursor) / static_cast<float>(m_nPriorityInitCells);
        return;
    }
    m_bTextureInitDone = 1;
}

/// 0x0890fec0
void Terrain::InvalidateRegion(FGXVector2& vMin, FGXVector2& vMax, int bHeightChanged, int a4) {
    const SharedData& kShared = m_kShared;
    const float fCellsX = static_cast<float>(m_nCellsX);
    const float fCellsY = static_cast<float>(m_nCellsY);
    int iMinX = static_cast<int>((vMin.x * fCellsX * kShared.m_vTerrainScale.x) / kShared.m_fWorldWidth);
    int iMinY = static_cast<int>((vMin.y * fCellsY * kShared.m_vTerrainScale.y) / kShared.m_fWorldHeight);
    int iMaxX = static_cast<int>((fCellsX * vMax.x * kShared.m_vTerrainScale.x) / kShared.m_fWorldWidth);
    int iMaxY = static_cast<int>((fCellsY * vMax.y * kShared.m_vTerrainScale.y) / kShared.m_fWorldHeight);
    if (iMinX < 0) {
        iMinX = 0;
    }
    if (iMinY < 0) {
        iMinY = 0;
    }
    if (iMaxX > static_cast<int>(m_nCellsX) - 1) {
        iMaxX = m_nCellsX - 1;
    }
    if (iMaxY > static_cast<int>(m_nCellsY) - 1) {
        iMaxY = m_nCellsY - 1;
    }
    for (int x = iMinX; x <= iMaxX; ++x) {
        for (int y = iMinY; y <= iMaxY; ++y) {
            TerrainCell* pCell = m_ppCells[m_nCellsY * x + y];
            pCell->m_uInvalidatedFrame = FireGrafix::g_pRenderer->m_uFrame;
            pCell->m_nTextures = 0;
            pCell->field_300 = 1;
            pCell->LaunchStatsBuild(&m_kJobSet);
        }
    }
    m_kJobSet.Wait();
    if (bHeightChanged != 0) {
        m_uiHeightmapDirtyFrame = FireGrafix::g_pRenderer->m_uFrame;
    }
}

/// 0x0891008e
void Terrain::PruneWork() {
    for (unsigned int i = 0; i < m_nCells; ++i) {
        m_ppCells[i]->LaunchStatsBuild(&m_kJobSet);
    }
    m_kJobSet.Wait();
}

/// 0x089100ee
void Terrain::SetTextures(unsigned int const* a1, unsigned int const* a2, unsigned int const* a3, float const* a4, float const* a5, TextureProjectionUserMode const* paeProjection, int nTextures) {
    SharedData& kShared = m_kShared;
    kShared.m_nTextures = nTextures;
    if (m_nCells != 0) {
        const unsigned int uFrame = FireGrafix::g_pRenderer->m_uFrame;
        for (unsigned int i = 0; i < m_nCells; ++i) {
            m_ppCells[i]->m_uInvalidatedFrame = uFrame;
        }
    }
    kShared.m_nProjections = 0;
    const size_t nBytes = kShared.m_nTextures * sizeof(unsigned int);
    memcpy(kShared.field_708, a1, nBytes);
    memcpy(kShared.field_808, a2, nBytes);
    memcpy(kShared.field_908, a3, nBytes);
    memcpy(kShared.field_d10, a4, nBytes);
    memcpy(kShared.field_e10, a5, nBytes);
    memcpy(kShared.m_aeProjectionMode, paeProjection, nBytes);
    memset(kShared.m_aiProjectionTexture, 0, sizeof(kShared.m_aiProjectionTexture));

    // A texture with a non-zero projection mode takes three axis entries, the others one.
    for (int i = 0; i < nTextures; ++i) {
        kShared.m_aiFirstProjection[i] = kShared.m_nProjections;
        const int nAxes = static_cast<int>(kShared.m_aeProjectionMode[i]) == 0 ? 1 : 3;
        bool bFull = false;
        for (int iAxis = 0; iAxis < nAxes; ++iAxis) {
            if (kShared.m_nProjections == 64) {
                bFull = true;
                break;
            }
            kShared.m_aiProjectionTexture[kShared.m_nProjections] = i;
            kShared.m_aiProjectionAxis[kShared.m_nProjections] = iAxis;
            ++kShared.m_nProjections;
        }
        if (bFull) {
            break;
        }
    }

    UNK_DATA* pTextures = FireGrafix::CRenderer9::m_TextureList;
    for (int i = 0; i < nTextures; ++i) {
        Fun_0868a21a(pTextures, kShared.field_708[i]);
        Fun_0868a21a(pTextures, kShared.field_908[i]);
        Fun_0868a21a(pTextures, kShared.field_808[i]);
    }

    FireGrafix::TextureData kBlank(0x15, static_cast<FireGrafix::_TextureType>(0), 0x20, 0x20, 1, 1, 0, nullptr, 0);
    memset(kBlank.m_aMips[0].m_apFaces[0], 0, 0x1000);
    kShared.m_hBlankTexture = FireGrafix::g_pRenderer->CreateTexture(&kBlank);
}

/// 0x0891042c
void Terrain::Invalidate() {
    if (m_nCells == 0) {
        return;
    }
    const unsigned int uiFrame = FireGrafix::g_pRenderer->m_uFrame;
    for (unsigned int i = 0; i < m_nCells; ++i) {
        m_ppCells[i]->m_uInvalidatedFrame = uiFrame;
    }
}

/// 0x08910470
void Terrain::SetLightingEnv(unsigned int const* pahSunlight, unsigned int const* pahShadowlight) {
    for (int i = 0; i < 5; ++i) {
        m_kShared.m_ahSunlightTextures[i] = pahSunlight[i];
        m_kShared.m_ahShadowlightTextures[i] = pahShadowlight[i];
    }
}

/// 0x0891049e
void Terrain::SetSunDir(FGXVector3 vSunDir, FGXVector3 vReflectDir) {
    m_kShared.m_vSunDir = vSunDir;
    m_kShared.m_vReflectDir = vReflectDir;
}

/// 0x089104d8
float Terrain::InShadow(FGXInt2 kPos) {
    // Four rays offset by 1.1 texels march along -m_vSunDir; returns the unoccluded fraction.
    // Each ray starts where the previous one stopped: iX/iY carry over, as in the binary.
    const SharedData& kShared = m_kShared;
    const FGXVector3& vSunDir = kShared.m_vSunDir;
    const FGXVector3& vScale = kShared.m_vTerrainScale;
    float fMaxXY = fabsf(vSunDir.y);
    if (fMaxXY <= fabsf(vSunDir.x)) {
        fMaxXY = fabsf(vSunDir.x);
    }
    const float fStepX = -vSunDir.x / fMaxXY;
    const float fStepY = -vSunDir.y / fMaxXY;
    float fShadowed = 0.0f;
    float fSamples = 0.0f;
    float fAngle = 0.0f;
    int iX = kPos.x;
    int iY = kPos.y;
    for (;;) {
        cvTerrainSampler* pGlobalSampler = MapGlobals::g_pTerrainSampler;
        FGXInt2 kStart = {iX, iY};
        const FGXInt2 kStartWrapped = pGlobalSampler->GetWrappedPosition(kStart);
        const unsigned short usStartHeight = Fun_08679d0a(&pGlobalSampler->m_iHeightMapWidth, &kStartWrapped);
        const float fTheta = (fAngle + fAngle) * 3.14159f;
        const float fSin = sinf(fTheta);
        const float fCos = cosf(fTheta);
        if (m_nShadowSteps != 0) {
            float fX = static_cast<float>(iX) + fSin * 1.1f;
            float fY = fCos * 1.1f + static_cast<float>(iY);
            const float fStartX = vScale.x * fX;
            const float fStartY = vScale.y * fY;
            for (unsigned int uiStep = 0; uiStep < m_nShadowSteps; ++uiStep) {
                fX += fStepX;
                fY += fStepY;
                iX = static_cast<int>(fX);
                iY = static_cast<int>(fY);
                if ((iX | iY) < 0 || kShared.m_iHeightMapWidth <= iX || kShared.m_iHeightMapHeight <= iY) {
                    break;
                }
                cvTerrainSampler* pSampler = kShared.m_kExternalData.m_pTerrainSampler;
                FGXInt2 kSample = {iX, iY};
                const FGXInt2 kSampleWrapped = pSampler->GetWrappedPosition(kSample);
                const unsigned short usHeight = Fun_08679d0a(&pSampler->m_iHeightMapWidth, &kSampleWrapped);
                const float fDot = (static_cast<float>(usHeight) * kHeightScale * vScale.z - static_cast<float>(usStartHeight) * kHeightScale * vScale.z) * vSunDir.z +
                                   (vScale.y * fY - fStartY) * vSunDir.y +
                                   (vScale.x * fX - fStartX) * vSunDir.x;
                if (0.0f < fDot) {
                    fShadowed += 1.0f;
                    break;
                }
            }
        }
        fSamples += 1.0f;
        fAngle += 0.3f;
        if (1.0f <= fAngle) {
            return 1.0f - fShadowed / fSamples;
        }
    }
}

/// 0x0891088c
void Terrain::SetOptions(LIGHTINGOPTS eLighting, SHADOWOPTS eShadow, int a3, int bFrustumCull, int a5, int a6, float a7, int a8, int a9) {
    m_eLighting = eLighting;
    m_eShadow = eShadow;
    field_240c = a3;
    m_bFrustumCull = bFrustumCull;
    field_2420 = a7;
    field_2414 = a6;
    field_2418 = a8;
    field_241c = a9;
    m_kShared.field_1aa4 = a5;
}

/// 0x089108f0
void Terrain::CalcStats(TerrainStats* pStats) {
    pStats->field_c = m_nCellsRendered;
    pStats->m_uiCacheMemory = 0;
    pStats->m_uiNumCacheTextures = 0;
    if (m_nCells == 0) {
        return;
    }
    const unsigned int uiCacheSize = GraphicsSettings::TerrainSettings::g_iCacheSize;
    const unsigned int uiBytesPerCell = (uiCacheSize * uiCacheSize * 8) / 3;
    pStats->m_uiCacheMemory = uiBytesPerCell * m_nCells;
    pStats->m_uiNumCacheTextures = m_nCells;
}

/// 0x08910958
void Terrain::SetGamePlayCell(FGXInt2& kHex) {
    D3DXMATRIX kIdentity;
    memset(&kIdentity, 0, sizeof(kIdentity));
    kIdentity._11 = 1.0f;
    kIdentity._22 = 1.0f;
    kIdentity._33 = 1.0f;
    kIdentity._44 = 1.0f;
    D3DXMATRIX kScale;
    D3DXMatrixScaling(&kScale, 1.0f, kHexRowScale, 1.0f);
    D3DXMATRIX kRowScale;
    D3DXMatrixMultiply(&kRowScale, &kIdentity, &kScale);
    const float fHexX = static_cast<float>(kHex.x);
    const float fHexY = static_cast<float>(kHex.y);
    const HEXVec vHex = {fHexX, fHexY, -fHexX - fHexY, 1.0f};
    D3DXMATRIX kTranslation;
    HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&kTranslation), vHex);
    D3DXMATRIX kTranslated;
    D3DXMatrixMultiply(&kTranslated, &kTranslation, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));
    D3DXMATRIX kMatrix;
    D3DXMatrixMultiply(&kMatrix, &kTranslated, &kRowScale);

    // The window is asymmetric: columns -1..+1 and rows -2..0 around the hex's cell.
    const int iCellX = static_cast<int>((static_cast<float>(m_nCellsX) * kMatrix._41) / m_kShared.m_fWorldWidth);
    const int iCellY = static_cast<int>((static_cast<float>(m_nCellsY) * kMatrix._42) / m_kShared.m_fWorldHeight);
    for (int x = iCellX - 1; x <= iCellX + 1; ++x) {
        for (int y = iCellY - 2; y <= iCellY; ++y) {
            TerrainCell* pCell = m_ppCells[m_nCellsY * ((m_nCellsX + x) % m_nCellsX) + (m_nCellsY + y) % m_nCellsY];
            pCell->m_iFOWFlags = 0;
            for (unsigned int k = 0; k < pCell->m_nHexes; ++k) {
                const int iFOWState = *Fun_0891159a(unk_2358, &pCell->m_aHexes[k]);
                if (iFOWState == 1) {
                    pCell->m_iFOWFlags |= 2;
                } else if (iFOWState == 0) {
                    pCell->m_iFOWFlags |= 1;
                }
            }
        }
    }
}
