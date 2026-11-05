/// origin guess
#include <math.h>
#include <stdlib.h>
#include <Windows.h>
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/Graphics/FluidParticles.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/UI/LuaContext.h"
#include "Civ5/Src/App/UI/TextControl.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "FirePlace/FireGrafix/FOWBaseLayer.h"
#include "FirePlace/FireGrafix/FOWDrawMask.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireWorks/FXmlDocument.h"
#include "FirePlace/FireWorks/FXmlTraverser.h"
#include "FirePlace/FireWorks/FluidFOWSystem.h"

namespace {

const int kNumGrids = 3;
// Particle spawn area is rounded to whole 64-unit chunks of the world.
const int kWorldChunk = 0x40;
const int kSpawnAreaPadding = 0x540;
const int kParticleStartupArg = 6;
const unsigned int kMaxSampleTries = 10;
const float kRandMax = 32767.0f;
// Radius below which the polar transform is skipped.
const float kMinRadiusSq = 1.0e-4f;
// Fog height offset in terrain-scale units added to the random height.
const float kHeightBias = 0.19836728f;
// The XML correlation is halved before use.
const float kCorrelationScale = 0.5f;
// Fraction of the world width used for the base layer scale.
const float kBaseScaleFraction = 0.002f;

const int kDefaultSpawnFrequency = 20;
const float kDefaultSizeMean = 1.0f;
const float kDefaultSizeDeviation = 1.0f;
const float kDefaultSizeMin = 0.25f;
const float kDefaultSizeMax = 4.0f;
const float kDefaultMassMean = 1.0f;
const float kDefaultMassDeviation = 1.0f;
const float kDefaultMassMin = 0.125f;
const float kDefaultMassMax = 8.0f;
const float kDefaultCorrelation = 0.0f;
const float kDefaultFogHeight = 0.0f;
const float kDefaultHeightVariation = 60.0f;

const float kDefaultDensity = 50.0f;
const float kDefaultSpeed = 5.0f;
const float kDefaultDrag = 0.001f;
const float kDefaultMasterSize = 1.0f;
const float kDefaultForceStrength = 1.0e-5f;
const float kDefaultMaxRotate = 0.01f;
const int kDefaultIterations = 1;

// Fog layer quads cover 64 x 64 terrain-scale units each.
const int kFogTileUnits = 0x40;
const float kFogTileHalfUnits = 32.0f;
const int kMaxFogTiles = 0x800;
const int kFogTilesPerDraw = 24;
// Heights in multiples of g_TerrainScale.z.
const float kCullHeight = 510.0f;
const float kFogLayer0Height = 56.0f;
const float kFogLayer1Height = 191.25f;
const float kFogLayer2Height = 255.0f;
// Cloud UV offset per second of game view time, before the per-layer scale.
const float kFogScrollRate = -2.0f;
const unsigned int kParticleFogLevel = 3;
// Grid value OnFOWChanged writes for an unrevealed plot.
const int kCellUnrevealed = 2;
const int kCullBorderColumns = 2;

// Hexes within two steps of a cell on the offset grid; Update culls fog only where none is unrevealed.
const int kNumNearOffsets = 21;
const int kNearOffsets[kNumNearOffsets][2] = {
    {0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, 1}, {-1, -1}, {1, -1}, {1, 1}, {0, -2}, {0, 2},
    {-2, 0}, {2, 0}, {-1, -2}, {-1, 2}, {-2, -1}, {2, -1}, {1, -2}, {1, 2}, {-2, 1}, {2, 1},
};

// FOWBaseLayer dynamic constants FOWBaseWVP, FOWBaseWorld, FOWOverlay, FOWBaseScale.
struct FogLayerConstants {
    D3DXMATRIX amWorldViewProj[kFogTilesPerDraw];
    D3DXMATRIX amWorld[kFogTilesPerDraw];
    D3DXMATRIX mOverlayProjection;
    // xy: cloud UV scale (g_FOWBaseScale), zw: scroll offset times the layer's scroll scale.
    float afScaleScroll[4];
};

// Hex rows are squashed to this factor in mask texture space.
const float kHexRowScale = 0.866f;
// Added to the cull quad's world y.
const float kCullQuadOffsetY = -128.0f;
// The cull pass reaches this many columns past the last visible one.
const int kCullColumnMargin = 2;
// Mask hex classes, the FOW grid values: 0 visible, 1 revealed (also visible hexes outside city fog), 2 unrevealed or off the grid.
const int kMaskClassVisible = 0;
const int kMaskClassRevealed = 1;
const int kMaskClassUnrevealed = 2;
const int kNumMaskPasses = 3;

}  // namespace

/// 0x0886aa78
UNK_RET FluidFOWSystem::RenderFOWMask(bool bClear) {
    // Camera visible-range arrays hold one (row, column) int pair per camera.
    const auto CameraRange = [](const void* pArray, unsigned int uCamera) -> const FGXInt2& {
        return reinterpret_cast<const FGXInt2*>(pArray)[uCamera];
    };
    // Packs the matrices 24 per FOWMaskDynamics lock and draws the surface once per lock.
    const auto DrawMaskInstances = [this](const D3DXMATRIX* pInstances, int nInstances, unsigned int hSurface, unsigned short usSubset) {
        FireGrafix::FOWMaskDynamics* pDynamics = static_cast<FireGrafix::FOWMaskDynamics*>(m_pMaskShader->m_pConstantSet);
        int iFirst = 0;
        while (iFirst < nInstances) {
            D3DXMATRIX* pConstants = static_cast<D3DXMATRIX*>(pDynamics->LockDynamic(m_pCommandStream));
            int nBatch = nInstances - iFirst;
            if (nBatch > kFogTilesPerDraw) {
                nBatch = kFogTilesPerDraw;
            }
            for (int i = 0; i < nBatch; ++i) {
                pConstants[i] = pInstances[iFirst + i];
            }
            iFirst += nBatch;
            if (nBatch < kFogTilesPerDraw) {
                memset(&pConstants[nBatch], 0, (kFogTilesPerDraw - nBatch) * sizeof(D3DXMATRIX));
            }
            pDynamics->UnlockDynamic();
            m_pCommandStream->DrawSurface(hSurface, m_pMaskShader->m_hInstance, usSubset);
        }
    };

    if (bClear) {
        const FGXVector4 vWhite = {1.0f, 1.0f, 1.0f, 1.0f};
        const unsigned int hClearSet = m_pCommandStream->BeginCommandSet(RenderGlobals::g_TexHandles[2], m_hAlphaState, 9, 4, "FOW Mask", vWhite, 1.0f, 0, 0, 0);
        m_pCommandStream->EndCommandSet();
        m_pCommandStream->AddCommandSetToRenderQueue(hClearSet);
        return;
    }

    // Row squash, then the inverse of the sampler's world offset.
    D3DXMATRIX mTexTransform;
    memset(&mTexTransform, 0, sizeof(mTexTransform));
    mTexTransform._11 = 1.0f;
    mTexTransform._22 = 1.0f;
    mTexTransform._33 = 1.0f;
    mTexTransform._44 = 1.0f;
    const float fOffsetX = MapGlobals::g_pTerrainSampler->m_fOffsetX;
    const float fOffsetY = MapGlobals::g_pTerrainSampler->m_fOffsetY;
    D3DXMATRIX mStep;
    D3DXMatrixMultiply(&mTexTransform, &mTexTransform, D3DXMatrixScaling(&mStep, 1.0f, kHexRowScale, 1.0f));
    D3DXMatrixMultiply(&mTexTransform, &mTexTransform, D3DXMatrixTranslation(&mStep, -fOffsetX, -fOffsetY, 0.0f));

    const FGXVector4 vBlack = {0.0f, 0.0f, 0.0f, 0.0f};
    const unsigned int hMaskSet = m_pCommandStream->BeginCommandSet(RenderGlobals::g_TexHandles[2], m_hAlphaState, 9, 4, "FOW Mask", vBlack, 1.0f, 0, 0, 0);

    const HEXMatrix_i& mxGameGridInverse = *reinterpret_cast<const HEXMatrix_i*>(WorldGlobals::g_GameGrid + 0x40);
    const int iGameRows = *reinterpret_cast<const int*>(WorldGlobals::g_GameGrid + 0x94);
    const int iGameColumns = *reinterpret_cast<const int*>(WorldGlobals::g_GameGrid + 0x98);
    const FluidFOWGrid& kFog = m_aGrids[0];
    const FluidFOWGrid& kCityFog = m_aGrids[1];
    D3DXMATRIX amInstances[kMaxFogTiles];

    for (int iPass = 0; iPass < kNumMaskPasses; ++iPass) {
        if (FrameGlobals::g_nCameras == 0) {
            continue;
        }
        int nInstances = 0;
        for (unsigned int uCamera = 0; uCamera < FrameGlobals::g_nCameras; ++uCamera) {
            const int iRowBegin = CameraRange(FrameGlobals::g_GameGridTopLeftVisible, uCamera).x;
            const int iRowEnd = CameraRange(FrameGlobals::g_GameGridBottomRightVisible, uCamera).x;
            const int iColumnBegin = CameraRange(FrameGlobals::g_GameGridTopLeftVisibleUnclamped, uCamera).y;
            const int iColumnEnd = CameraRange(FrameGlobals::g_GameGridBottomRightVisibleUnclamped, uCamera).y;
            for (int iRow = iRowBegin; iRow < iRowEnd; ++iRow) {
                for (int iColumn = iColumnBegin; iColumn < iColumnEnd; ++iColumn) {
                    int iClass = kMaskClassUnrevealed;
                    if (iRow >= 0 && iRow < iGameRows && iColumn >= 0 && iColumn < iGameColumns) {
                        iClass = kFog.m_piCells[kFog.m_iWidth * iRow + iColumn];
                        if (iClass != kMaskClassUnrevealed && m_bCityFogActive != 0 && iRow < kCityFog.m_iHeight &&
                            iColumn < kCityFog.m_iWidth && kCityFog.m_piCells[kCityFog.m_iWidth * iRow + iColumn] == 0) {
                            iClass = kMaskClassRevealed;
                        }
                    }
                    // Pass 0 draws revealed hexes, pass 1 visible ones, pass 2 unrevealed ones.
                    const int iPassClass = (iPass == 0) ? kMaskClassRevealed : (iPass == 1 ? kMaskClassVisible : kMaskClassUnrevealed);
                    if (iClass != iPassClass) {
                        continue;
                    }

                    const int iHexX = iRow - (iColumn >> 1);
                    const HEXVec_i kOffsetHex = {iHexX, iColumn, -(iHexX + iColumn)};
                    const HEXVec_i kHex = Fun_086616f0(mxGameGridInverse, kOffsetHex);
                    const HEXVec vHex = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), -static_cast<float>(kHex.x) - static_cast<float>(kHex.y), 1.0f};
                    D3DXMATRIX mHex;
                    HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&mHex), vHex);
                    if (nInstances < kMaxFogTiles) {
                        D3DXMATRIX mTex;
                        D3DXMATRIX mMask;
                        D3DXMatrixMultiply(&mTex, &mHex, reinterpret_cast<const D3DXMATRIX*>(WorldGlobals::g_HexToTex));
                        D3DXMatrixMultiply(&mMask, &mTex, &mTexTransform);
                        D3DXMatrixMultiply(&amInstances[nInstances], &mMask, &FrameGlobals::g_OverlayProjection[uCamera]);
                        ++nInstances;
                    }
                }
            }
        }

        if (iPass == 2) {
            DrawMaskInstances(amInstances, nInstances, m_hMaskWideHexSurface, 0);
        } else if (iPass == 1) {
            DrawMaskInstances(amInstances, nInstances, m_hMaskHexSurface, 1);
        } else {
            DrawMaskInstances(amInstances, nInstances, m_hMaskHexSurface, 2);
        }
    }
    m_pCommandStream->EndCommandSet();
    m_pCommandStream->AddCommandSetToRenderQueue(hMaskSet);
}

/// 0x0886b1c0
UNK_RET FluidFOWSystem::RenderFOWDepthCullSurface(unsigned int hRenderTarget, float fHeight, FGXVector2& a3) {
    const auto CameraRange = [](const void* pArray, unsigned int uCamera) -> const FGXInt2& {
        return reinterpret_cast<const FGXInt2*>(pArray)[uCamera];
    };
    const auto DrawMaskInstances = [this](const D3DXMATRIX* pInstances, int nInstances, unsigned int hSurface, unsigned short usSubset) {
        FireGrafix::FOWMaskDynamics* pDynamics = static_cast<FireGrafix::FOWMaskDynamics*>(m_pMaskShader->m_pConstantSet);
        int iFirst = 0;
        while (iFirst < nInstances) {
            D3DXMATRIX* pConstants = static_cast<D3DXMATRIX*>(pDynamics->LockDynamic(m_pCommandStream));
            int nBatch = nInstances - iFirst;
            if (nBatch > kFogTilesPerDraw) {
                nBatch = kFogTilesPerDraw;
            }
            for (int i = 0; i < nBatch; ++i) {
                pConstants[i] = pInstances[iFirst + i];
            }
            iFirst += nBatch;
            if (nBatch < kFogTilesPerDraw) {
                memset(&pConstants[nBatch], 0, (kFogTilesPerDraw - nBatch) * sizeof(D3DXMATRIX));
            }
            pDynamics->UnlockDynamic();
            m_pCommandStream->DrawSurface(hSurface, m_pMaskShader->m_hInstance, usSubset);
        }
    };

    const FGXVector4 vZero = {0.0f, 0.0f, 0.0f, 0.0f};
    const unsigned int hCullSet = m_pCommandStream->BeginCommandSet(hRenderTarget, 3, 0, 0, "FOW CULL Layer", vZero, 1.0f, 0, 0, 0);

    const HEXMatrix_i& mxGameGridInverse = *reinterpret_cast<const HEXMatrix_i*>(WorldGlobals::g_GameGrid + 0x40);
    const int iGameRows = *reinterpret_cast<const int*>(WorldGlobals::g_GameGrid + 0x94);
    const int iGameColumns = *reinterpret_cast<const int*>(WorldGlobals::g_GameGrid + 0x98);
    const FluidFOWGrid& kCull = m_aGrids[2];
    D3DXMATRIX amInstances[kMaxFogTiles];

    for (unsigned int uCamera = 0; uCamera < FrameGlobals::g_nCameras; ++uCamera) {
        const int iRowBegin = CameraRange(FrameGlobals::g_GameGridTopLeftVisible, uCamera).x;
        const int iColumnBegin = CameraRange(FrameGlobals::g_GameGridTopLeftVisibleUnclamped, uCamera).y;
        const int iRowEnd = CameraRange(FrameGlobals::g_GameGridBottomRightVisible, uCamera).x + 1;
        const int iColumnEnd = CameraRange(FrameGlobals::g_GameGridBottomRightVisibleUnclamped, uCamera).y + kCullColumnMargin;
        const float fQuadHalfX = WorldGlobals::g_TerrainScale.x * kFogTileHalfUnits;
        const float fQuadHalfY = WorldGlobals::g_TerrainScale.y * kFogTileHalfUnits;
        D3DXMATRIX mQuadScale;
        D3DXMatrixScaling(&mQuadScale, fQuadHalfX, fQuadHalfY, 1.0f);

        int nInstances = 0;
        for (int iRow = iRowBegin; iRow < iRowEnd; ++iRow) {
            for (int iColumn = iColumnBegin; iColumn < iColumnEnd; ++iColumn) {
                if (iRow < 0 || iColumn < 0 || iRow >= iGameRows || iColumn >= iGameColumns ||
                    kCull.m_piCells[kCull.m_iWidth * iRow + iColumn] == 0) {
                    continue;
                }
                const int iHexX = iRow - (iColumn >> 1);
                const HEXVec_i kOffsetHex = {iHexX, iColumn, -(iHexX + iColumn)};
                const HEXVec_i kHex = Fun_086616f0(mxGameGridInverse, kOffsetHex);
                const HEXVec vHex = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), static_cast<float>(-(kHex.y + kHex.x)), 1.0f};
                const FGXVector2 vWorld = MapGlobals::g_TerrainSampler.GetWorldLocation2D(vHex);
                D3DXMATRIX mTranslation;
                D3DXMatrixTranslation(&mTranslation, vWorld.x + fQuadHalfX, vWorld.y + fQuadHalfY + kCullQuadOffsetY, fHeight);
                if (nInstances < kMaxFogTiles) {
                    D3DXMATRIX mWorld;
                    D3DXMATRIX mWorldView;
                    D3DXMatrixMultiply(&mWorld, &mQuadScale, &mTranslation);
                    D3DXMatrixMultiply(&mWorldView, &mWorld, &FrameGlobals::g_View[uCamera]);
                    D3DXMatrixMultiply(&amInstances[nInstances], &mWorldView, &FrameGlobals::g_Proj);
                    ++nInstances;
                }
            }
        }
        DrawMaskInstances(amInstances, nInstances, m_hCullSurface, 3);
    }
    m_pCommandStream->EndCommandSet();
    m_pCommandStream->AddCommandSetToRenderQueue(hCullSet);
}

/// 0x0886b784
UNK_RET FluidFOWSystem::Update(float fDeltaSeconds) {
    m_fElapsedTime += fDeltaSeconds;
    float* pfBaseScale = &FrameGlobals::g_FOWBaseScale.x;
    pfBaseScale[0] = m_fBaseScale;
    pfBaseScale[1] = m_fBaseScale;
    pfBaseScale[2] = m_fElapsedTime * kFogScrollRate;
    pfBaseScale[3] = 0.0f;
    if (m_bFOWDirty == 0) {
        return;
    }

    const FluidFOWGrid& kFog = m_aGrids[0];
    FluidFOWGrid& kCull = m_aGrids[2];
    const int iHeight = kFog.m_iHeight;
    const int iWidth = kFog.m_iWidth;
    for (int iRow = 0; iRow < iHeight; ++iRow) {
        for (int iColumn = 0; iColumn < iWidth; ++iColumn) {
            int bCull = 1;
            for (int i = 0; i < kNumNearOffsets; ++i) {
                const int iNearRow = (kNearOffsets[i][0] + iRow + iHeight) % iHeight;
                const int iNearColumn = (kNearOffsets[i][1] + iColumn + iWidth) % iWidth;
                bCull &= kFog.m_piCells[iNearRow * kFog.m_iWidth + iNearColumn] != kCellUnrevealed;
            }
            if (iColumn < kCullBorderColumns || iColumn > iWidth - kCullBorderColumns) {
                bCull = 0;
            }
            kCull.m_piCells[kCull.m_iWidth * iRow + iColumn] = bCull;
        }
    }
    m_bFOWDirty = 0;
}

/// 0x0886ba88
UNK_RET FluidFOWSystem::RenderFogLayer(unsigned int hSurface, float fHeight, FGXVector2& vWorldMin, float fScrollScale) {
    D3DXMATRIX amWorld[kMaxFogTiles];
    D3DXMATRIX amWorldViewProj[kMaxFogTiles];
    const FGXVector3* pBottomRightVisible = reinterpret_cast<const FGXVector3*>(FrameGlobals::g_WorldBottomRightVisible);

    for (unsigned int uCamera = 0; uCamera < FrameGlobals::g_nCameras; ++uCamera) {
        const float fScaleX = WorldGlobals::g_TerrainScale.x;
        const float fScaleY = WorldGlobals::g_TerrainScale.y;
        const int iLeft = (int)roundf((FrameGlobals::g_WorldTopLeftVisible[uCamera].x - vWorldMin.x) / fScaleX);
        const int iTop = (int)roundf((FrameGlobals::g_WorldTopLeftVisibleUnclamped[uCamera].y - vWorldMin.y) / fScaleY);
        const int iRight = (int)roundf((pBottomRightVisible[uCamera].x - vWorldMin.x) / fScaleX);
        const int iBottom = (int)roundf((FrameGlobals::g_WorldBottomRightVisibleUnclamped[uCamera].y - vWorldMin.y) / fScaleY);
        const int iTileLeft = iLeft / kFogTileUnits;
        const int iTileRight = iRight / kFogTileUnits + 1;

        D3DXMATRIX mTileScale;
        D3DXMatrixScaling(&mTileScale, fScaleX * kFogTileHalfUnits, fScaleY * kFogTileHalfUnits, 1.0f);
        if (iTileLeft >= iTileRight) {
            continue;
        }
        const int iTileTop = iTop / kFogTileUnits;
        const int iTileBottom = iBottom / kFogTileUnits + 2;
        const float fHalfTileX = WorldGlobals::g_TerrainScale.x * kFogTileHalfUnits;
        const float fHalfTileY = WorldGlobals::g_TerrainScale.y * kFogTileHalfUnits;

        int nTiles = 0;
        for (int iTileX = iTileLeft; iTileX < iTileRight; ++iTileX) {
            for (int iTileY = iTileTop; iTileY < iTileBottom; ++iTileY) {
                D3DXMATRIX mTranslation;
                D3DXMatrixTranslation(&mTranslation,
                                      WorldGlobals::g_TerrainScale.x * (float)(iTileX * kFogTileUnits) + vWorldMin.x + fHalfTileX,
                                      (float)(iTileY * kFogTileUnits) * WorldGlobals::g_TerrainScale.y + vWorldMin.y + fHalfTileY,
                                      fHeight);
                if (nTiles >= kMaxFogTiles) {
                    continue;
                }
                D3DXMatrixMultiply(&amWorld[nTiles], &mTileScale, &mTranslation);
                D3DXMATRIX mWorld;
                D3DXMATRIX mWorldView;
                D3DXMatrixMultiply(&mWorld, &mTileScale, &mTranslation);
                D3DXMatrixMultiply(&mWorldView, &mWorld, &FrameGlobals::g_View[uCamera]);
                D3DXMatrixMultiply(&amWorldViewProj[nTiles], &mWorldView, &FrameGlobals::g_Proj);
                ++nTiles;
            }
        }

        const float* pfBaseScale = &FrameGlobals::g_FOWBaseScale.x;
        int iTile = 0;
        while (iTile < nTiles) {
            FireGrafix::FOWBaseLayerDynamics* pDynamics = static_cast<FireGrafix::FOWBaseLayerDynamics*>(m_pBaseLayerShader->m_pConstantSet);
            FogLayerConstants* pConstants = static_cast<FogLayerConstants*>(pDynamics->LockDynamic(m_pCommandStream));
            int nBatch = nTiles - iTile;
            if (nBatch > kFogTilesPerDraw) {
                nBatch = kFogTilesPerDraw;
            }
            for (int i = 0; i < nBatch; ++i) {
                pConstants->amWorld[i] = amWorld[iTile + i];
                pConstants->amWorldViewProj[i] = amWorldViewProj[iTile + i];
            }
            iTile += nBatch;
            for (int i = nBatch; i < kFogTilesPerDraw; ++i) {
                memset(&pConstants->amWorldViewProj[i], 0, sizeof(D3DXMATRIX));
                memset(&pConstants->amWorld[i], 0, sizeof(D3DXMATRIX));
            }
            pConstants->afScaleScroll[0] = pfBaseScale[0];
            pConstants->afScaleScroll[1] = pfBaseScale[1];
            pConstants->afScaleScroll[2] = pfBaseScale[2] * fScrollScale;
            pConstants->afScaleScroll[3] = pfBaseScale[3] * fScrollScale;
            pConstants->mOverlayProjection = FrameGlobals::g_OverlayProjection[uCamera];
            m_pBaseLayerShader->m_pConstantSet->UnlockDynamic();
            m_pCommandStream->DrawSurface(hSurface, m_pBaseLayerShader->m_hInstance, 0);
        }
    }
}

/// 0x0886c152
UNK_RET FluidFOWSystem::Render(unsigned int hRenderTarget) {
    if (m_bGameStarted == 0 || m_bEnabled == 0) {
        return;
    }
    FGXVector2 vWorldMin;
    vWorldMin.x = WorldGlobals::g_WorldMin.x;
    vWorldMin.y = WorldGlobals::g_WorldMin.y;
    // Both matrices are built and never read.
    D3DXMATRIX mWorldToUnit;
    D3DXMatrixScaling(&mWorldToUnit, 1.0f / WorldGlobals::g_WorldSize.x, -1.0f / WorldGlobals::g_WorldSize.y,
                      255.0f / (WorldGlobals::g_TerrainScale.z * 65535.0f));
    D3DXMATRIX mWorldOffset;
    D3DXMatrixTranslation(&mWorldOffset, -vWorldMin.x, -vWorldMin.y, 0.0f);
    if (m_uFogLevel == 0) {
        return;
    }

    // The binary leaves a null in the third slot; the callee never reads it.
    RenderFOWDepthCullSurface(hRenderTarget, WorldGlobals::g_TerrainScale.z * kCullHeight, vWorldMin);

    const float fLayer0Height = WorldGlobals::g_TerrainScale.z * kFogLayer0Height;
    const FGXVector4 vClear = {0.0f, 0.0f, 0.0f, 0.0f};
    const unsigned int hBaseSet = m_pCommandStream->BeginCommandSet(hRenderTarget, 8, 3, 0, "FOW Base Layer", vClear, 1.0f, 0, 0, 0);
    RenderFogLayer(m_hFogLayer0Surface, fLayer0Height, vWorldMin, 1.0f);
    RenderFogLayer(m_hFogLayer1Surface, WorldGlobals::g_TerrainScale.z * kFogLayer1Height, vWorldMin, m_fFogLayer1ScrollScale);
    RenderFogLayer(m_hFogLayer2Surface, WorldGlobals::g_TerrainScale.z * kFogLayer2Height, vWorldMin, m_fFogLayer2ScrollScale);
    m_pCommandStream->EndCommandSet();
    m_pCommandStream->AddCommandSetToRenderQueue(hBaseSet);

    if (m_uFogLevel < kParticleFogLevel) {
        return;
    }
    const unsigned int hParticleSet = m_pCommandStream->BeginCommandSet(hRenderTarget, 8, 10, 0, "FOW Particles", vClear, 1.0f, 0, 0, 0);
    reinterpret_cast<FluidParticles*>(unk_8)->Render(m_pCommandStream);
    m_pCommandStream->EndCommandSet();
    m_pCommandStream->AddCommandSetToRenderQueue(hParticleSet);
}

/// 0x08869a94
UNK_RET FluidFOWSystem::UpdateSettings() {
    FluidParticles* pParticles = reinterpret_cast<FluidParticles*>(unk_8);
    pParticles->m_fSpeed = m_fSpeed;
    pParticles->m_fDensity = m_fDensity;
    pParticles->m_fDrag = m_fDrag;
    pParticles->m_fMasterSize = m_fMasterSize;
    pParticles->m_fForceStrength = m_fForceStrength;
    pParticles->m_fMaxRotate = m_fMaxRotate;
    pParticles->m_iIterations = m_iIterations;
    pParticles->m_iPadding = m_iPadding;
    if (m_pDebugContext != 0) {
        LuaContext* pContext = m_pDebugContext;
        auto SetDebugLabel = [pContext](char const* pszControl, char const* pszText) {
            ControlBase* pControl = pContext->LookUpControl(pszControl);
            if (pControl != 0) {
                static_cast<TextControl*>(pControl)->SetText(pszText);
            }
        };
        char acText[128];
        sprintf_s(acText, 0x80, "%i", m_iIterations);
        SetDebugLabel("DebugMenu/FluidFOW_Panel/Iterations_Label", acText);
        sprintf_s(acText, 0x80, "%i", m_iPadding);
        SetDebugLabel("DebugMenu/FluidFOW_Panel/Padding_Label", acText);
        sprintf_s(acText, 0x80, "%f", (double)m_fDensity);
        SetDebugLabel("DebugMenu/FluidFOW_Panel/Density_Label", acText);
        sprintf_s(acText, 0x80, "%f", (double)m_fSpeed);
        SetDebugLabel("DebugMenu/FluidFOW_Panel/Speed_Label", acText);
        sprintf_s(acText, 0x80, "%f", (double)m_fDrag);
        SetDebugLabel("DebugMenu/FluidFOW_Panel/Drag_Label", acText);
        sprintf_s(acText, 0x80, "%f", (double)m_fMasterSize);
        SetDebugLabel("DebugMenu/FluidFOW_Panel/MasterSize_Label", acText);
        sprintf_s(acText, 0x80, "%f", (double)m_fForceStrength);
        SetDebugLabel("DebugMenu/FluidFOW_Panel/ForceStrength_Label", acText);
        sprintf_s(acText, 0x80, "%f", (double)m_fMaxRotate);
        SetDebugLabel("DebugMenu/FluidFOW_Panel/MaxRotate_Label", acText);
    }
}

/// 0x08869108
UNK_RET FluidFOWSystem::OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1) {
    // HexFOWStateChangedData is not declared yet: +4 short x, +6 short y, +8 state, +9 "whole map".
    const unsigned char* pEvent = reinterpret_cast<const unsigned char*>(&a1);
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

    FluidFOWGrid& kGrid = m_aGrids[0];
    if (!bWholeMap) {
        const HEXVec_i kHex = {sX, sY, -(sY + sX)};
        const HEXVec_i kOffset = Fun_086616f0(kGrid.m_mxForward, kHex);
        const unsigned int uColumn = static_cast<unsigned int>(kOffset.y);
        kGrid.m_piCells[((uColumn >> 1) + kOffset.x) * kGrid.m_iWidth + uColumn] = iValue;
    } else {
        for (int iRow = 0; iRow < kGrid.m_iHeight; ++iRow) {
            for (int iColumn = 0; iColumn < kGrid.m_iWidth; ++iColumn) {
                kGrid.m_piCells[kGrid.m_iWidth * iRow + iColumn] = iValue;
            }
        }
    }
    m_bFOWDirty = 1;
}

/// 0x08869e24
UNK_RET FluidFOWSystem::StartupGame() {
    const int iWidth = m_iGridWidth;
    const int iHeight = m_iGridHeight;

    for (int i = 0; i < kNumGrids; ++i) {
        FluidFOWGrid& kGrid = m_aGrids[i];
        kGrid.m_kTransform.field_0 = m_aiGridTransform[0];
        kGrid.m_kTransform.field_4 = m_aiGridTransform[1];
        kGrid.m_kTransform.field_8 = m_aiGridTransform[2];
        kGrid.m_mxForward = kGrid.m_kTransform.matrix();
        kGrid.m_mxInverse = kGrid.m_kTransform.inv_matrix();
    }

    const int iZero = 0;
    const int nCells = iWidth * iHeight;
    for (int i = 0; i < kNumGrids; ++i) {
        FluidFOWGrid& kGrid = m_aGrids[i];
        int nGrow = nCells - static_cast<int>(kGrid.m_nCells);
        if (nGrow < 0) {
            nGrow = 0;
        }
        Fun_0886c502(&kGrid.m_piCells, &iZero, nGrow);
        kGrid.m_iWidth = iWidth;
        kGrid.m_iHeight = iHeight;
    }
    for (int iRow = 0; iRow < iHeight; ++iRow) {
        for (int iColumn = 0; iColumn < iWidth; ++iColumn) {
            m_aGrids[0].m_piCells[m_aGrids[0].m_iWidth * iRow + iColumn] = 0;
            m_aGrids[1].m_piCells[m_aGrids[1].m_iWidth * iRow + iColumn] = 0;
            m_aGrids[2].m_piCells[m_aGrids[2].m_iWidth * iRow + iColumn] = 1;
        }
    }
    m_bGameStarted = 1;
    m_bCityFogActive = 0;
    m_bFOWDirty = 1;

    FGXInt2 kMax;
    FGXInt2 kMin;
    kMax.x = (((int)(WorldGlobals::g_WorldMax.x - WorldGlobals::g_WorldMin.x) / kWorldChunk) * kWorldChunk + kWorldChunk) >> 1;
    kMin.x = -kMax.x;
    kMax.y = (((int)(WorldGlobals::g_WorldMax.y - WorldGlobals::g_WorldMin.y) / kWorldChunk) * kWorldChunk + kSpawnAreaPadding) >> 1;
    kMin.y = -kMax.y;
    m_fBaseScale = (float)(int)(kBaseScaleFraction * (WorldGlobals::g_WorldMax.x - WorldGlobals::g_WorldMin.x)) /
                   (WorldGlobals::g_WorldMax.x - WorldGlobals::g_WorldMin.x);

    // Level 3 is never reached: Startup clamps m_uFogLevel to 2.
    if (m_uFogLevel > 2) {
        FXmlDocument kDocument;
        if (kDocument.Load("Assets\\Effects\\FOW\\FluidFOWSettings.xml")) {
            FXmlTraverser kRoot;
            kDocument.GetRootElement(kRoot);

            int iSpawnFrequency = kDefaultSpawnFrequency;
            float fSizeMean = kDefaultSizeMean;
            float fSizeDeviation = kDefaultSizeDeviation;
            float fSizeMin = kDefaultSizeMin;
            float fSizeMax = kDefaultSizeMax;
            float fMassMean = kDefaultMassMean;
            float fMassDeviation = kDefaultMassDeviation;
            float fMassMin = kDefaultMassMin;
            float fMassMax = kDefaultMassMax;
            float fCorrelation = kDefaultCorrelation;
            float fFogHeight = kDefaultFogHeight;
            float fHeightVariation = kDefaultHeightVariation;
            if (Fun_08746c70(kRoot, iSpawnFrequency, "SpawnFrequency", 0xe) &&
                Fun_08725468(kRoot, fSizeMean, "SizeMean", 8) &&
                Fun_08725468(kRoot, fSizeMin, "SizeMin", 7) &&
                Fun_08725468(kRoot, fSizeMax, "SizeMax", 7) &&
                Fun_08725468(kRoot, fSizeDeviation, "SizeDeviation", 0xd) &&
                Fun_08725468(kRoot, fMassMean, "MassMean", 8) &&
                Fun_08725468(kRoot, fMassMin, "MassMin", 7) &&
                Fun_08725468(kRoot, fMassMax, "MassMax", 7) &&
                Fun_08725468(kRoot, fMassDeviation, "MassDeviation", 0xd) &&
                Fun_08725468(kRoot, fCorrelation, "MassAndSizeCorrelation", 0x16) &&
                Fun_08725468(kRoot, fFogHeight, "FogHeight", 9)) {
                Fun_08725468(kRoot, fHeightVariation, "HeightVariation", 0xf);
            }

            m_fDensity = kDefaultDensity;
            m_fSpeed = kDefaultSpeed;
            m_fDrag = kDefaultDrag;
            m_fMasterSize = kDefaultMasterSize;
            m_fForceStrength = kDefaultForceStrength;
            m_fMaxRotate = kDefaultMaxRotate;
            m_iIterations = kDefaultIterations;
            if (Fun_08725468(kRoot, m_fDensity, "FluidDensity", 0xc) &&
                Fun_08725468(kRoot, m_fSpeed, "Speed", 5) &&
                Fun_08725468(kRoot, m_fDrag, "Drag", 4) &&
                Fun_08725468(kRoot, m_fMasterSize, "MasterSize", 10) &&
                Fun_08725468(kRoot, m_fForceStrength, "ForceStrength", 0xd)) {
                Fun_08725468(kRoot, m_fMaxRotate, "MaxRotate", 9);
            }
            UpdateSettings();

            FluidParticles* pParticles = reinterpret_cast<FluidParticles*>(unk_8);
            pParticles->StartupGame(kMin, kMax, kParticleStartupArg);
            const int iParticleRange = pParticles->field_8f8;
            fCorrelation = fCorrelation * kCorrelationScale;
            const float fSizeScale = sqrtf(fSizeDeviation);
            const float fMassScale = sqrtf(fMassDeviation);

            int iParticle = 0;
            for (int iX = kMin.x; iX < kMax.x; iX += iSpawnFrequency) {
                for (int iY = kMin.y; iY < kMax.y; iY += iSpawnFrequency) {
                    float fSize;
                    float fMass;
                    unsigned int uTries = 0;
                    do {
                        FGXVector2 vRand;
                        Get2UnitNormRand(vRand);
                        const float fRadiusSq = vRand.y * vRand.y + vRand.x * vRand.x;
                        float fPolar = 0.0f;
                        if (kMinRadiusSq < fRadiusSq) {
                            fPolar = sqrtf((logf(fRadiusSq) * -2.0f) / fRadiusSq);
                        }
                        fSize = (fCorrelation * vRand.y + vRand.x * (1.0f - fCorrelation)) * fPolar * fSizeScale + fSizeMean;
                        fMass = ((1.0f - fCorrelation) * vRand.y + fCorrelation * vRand.x) * fPolar * fMassScale + fMassMean;
                    } while ((fSize <= fSizeMin || fSizeMax <= fSize || fMass <= fMassMin || fMassMax <= fMass) &&
                             (++uTries < kMaxSampleTries));

                    const int iHeightRand = (int)rand_wincompatible();
                    const unsigned int uIdRand = rand_wincompatible();
                    ++iParticle;
                    FGXVector3 vPos;
                    vPos.x = (float)iX;
                    vPos.y = (float)iY;
                    vPos.z = fFogHeight + WorldGlobals::g_TerrainScale.z * (fHeightVariation * ((float)iHeightRand / kRandMax) + kHeightBias);
                    FGXVector2 vVelocity;
                    vVelocity.x = 0.0f;
                    vVelocity.y = 0.0f;
                    pParticles->AddParticle(vPos, vVelocity, fMass, fSize,
                                            (iParticle * 0x100) | ((uIdRand * iParticleRange) / 0x7fff));
                }
            }
            pParticles->PackAllParticles();
        }
    }
}
