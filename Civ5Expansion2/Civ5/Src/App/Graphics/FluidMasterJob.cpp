/// origin guess
#include "Civ5/Src/App/Graphics/FluidMasterJob.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/HEXMatrix.h"
#include "Civ5/Src/App/HEXVec.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/FOWCell.h"
#include "FirePlace/FireGrafix/TextureData.h"

/// 0x08873c6c
UNK_RET FOWSystem::DrawBlackHex(FGXInt2& kCell, unsigned int a2, D3DXMATRIX mViewProj) {
    const int iHexX = kCell.x - (kCell.y >> 1);
    const HEXVec_i kOffsetHex = {iHexX, kCell.y, -(iHexX + kCell.y)};
    const HEXVec_i kHex = Fun_086616f0(m_kGameGridInvMatrix, kOffsetHex);
    const HEXVec vHex = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), static_cast<float>(-(kHex.y + kHex.x)), 1.0f};

    D3DXMATRIX mHexTranslation;
    HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&mHexTranslation), vHex);
    D3DXMATRIX mWorld;
    D3DXMatrixMultiply(&mWorld, &mHexTranslation, &m_mHexToTex);
    D3DXMatrixTranslation(&mWorld, mWorld._41 + m_vWorldMin.x, mWorld._42 * m_vTerrainScale.y + m_vWorldMin.y, mWorld._43 + m_vWorldMin.z);

    FireGrafix::FOWDynamics* pDynamics = static_cast<FireGrafix::FOWDynamics*>(m_pShader->m_pConstantSet);
    D3DXMATRIX* pWorldViewProj = static_cast<D3DXMATRIX*>(pDynamics->LockDynamic(m_pCommandStream));
    D3DXMATRIX mWorldViewProj;
    D3DXMatrixMultiply(&mWorldViewProj, &mWorld, &mViewProj);
    *pWorldViewProj = mWorldViewProj;
    m_pShader->m_pConstantSet->UnlockDynamic();

    m_pCommandStream->DrawSurface(m_hHexSurface, m_pShader->m_hInstance, 0);
}

/// 0x08873e08
void FOWSystem::DrawFogCell(FGXInt2& kCell, unsigned int a2, D3DXMATRIX mViewProj, unsigned int uState) {
    if (kCell.x < 0 || kCell.x >= m_iFOWGridWidth || kCell.y < 0 || kCell.y >= m_iFOWGridHeight) {
        return;
    }
    const int x = kCell.x;
    const int y = kCell.y;
    if (static_cast<unsigned int>(m_piFOWGrid[x * m_iFOWGridHeight + y]) != uState) {
        return;
    }
    const int iWrap = m_iGameGridWidth;
    const int iPrevState = m_piFOWGrid[((iWrap + x - 1) % iWrap) * m_iFOWGridHeight + y];
    const int iNextState = m_piFOWGrid[((iWrap + x + 1) % iWrap) * m_iFOWGridHeight + y];

    const int iHexX = x - (y >> 1);
    const HEXVec_i kOffsetHex = {iHexX, y, -(iHexX + y)};
    const HEXVec_i kHex = Fun_086616f0(m_kGameGridInvMatrix, kOffsetHex);
    const HEXVec vHex = {static_cast<float>(kHex.x), static_cast<float>(kHex.y), static_cast<float>(-(kHex.y + kHex.x)), 1.0f};

    D3DXMATRIX mHexTranslation;
    HEXMatTranslation(reinterpret_cast<HEXMatrix*>(&mHexTranslation), vHex);
    D3DXMATRIX mWorld;
    D3DXMatrixMultiply(&mWorld, &mHexTranslation, &m_mHexToTex);
    D3DXMatrixTranslation(&mWorld, mWorld._41 + m_vWorldMin.x, mWorld._42 * m_vTerrainScale.y + m_vWorldMin.y, mWorld._43 + m_vWorldMin.z);

    FireGrafix::FOWDynamics* pDynamics = static_cast<FireGrafix::FOWDynamics*>(m_pShader->m_pConstantSet);
    D3DXMATRIX* pWorldViewProj = static_cast<D3DXMATRIX*>(pDynamics->LockDynamic(m_pCommandStream));
    D3DXMATRIX mWorldViewProj;
    D3DXMatrixMultiply(&mWorldViewProj, &mWorld, &mViewProj);
    *pWorldViewProj = mWorldViewProj;
    m_pShader->m_pConstantSet->UnlockDynamic();

    if (uState != 1) {
        m_pCommandStream->DrawSurface(m_hHexSurface, m_pShader->m_hInstance, 0);
        return;
    }
    unsigned int hSurface = m_hHexSurface;
    if (iPrevState != 0 && iNextState != 0) {
        hSurface = m_hHexSurfaceOpenBoth;
    }
    if (iPrevState == 0 && iNextState != 0) {
        hSurface = m_hHexSurfaceOpenNext;
    }
    if (iPrevState != 0 && iNextState == 0) {
        hSurface = m_hHexSurfaceOpenPrev;
    }
    m_pCommandStream->DrawSurface(hSurface, m_pShader->m_hInstance, 1);
}

/// 0x088740c8
void FOWSystem::Render(unsigned int hRenderTarget) {
    if (m_bGameStarted == 0 || field_9c0 == 0 || GraphicsSettings::GeneralSettings::g_uFOWLevel != 0) {
        return;
    }
    FGXInt2 kCell = {0, 0};
    const FGXVector4 vClear = {0.0f, 0.0f, 0.0f, 0.0f};
    m_pCommandStream->BeginCommandSet(hRenderTarget, 8, 0, 0, "FOW", vClear, 1.0f, 0, 0, 0);

    for (unsigned int uCamera = 0; uCamera < m_nCameras; ++uCamera) {
        D3DXMATRIX mViewProj;
        D3DXMatrixMultiply(&mViewProj, &m_amView[uCamera], &m_mProj);
        const FGXInt2& kTopLeft = m_akGameGridTopLeftVisible[uCamera];
        const FGXInt2& kBottomRight = m_akGameGridBottomRightVisible[uCamera];
        const FGXInt2& kTopLeftUnclamped = m_akGameGridTopLeftVisibleUnclamped[uCamera];
        const FGXInt2& kBottomRightUnclamped = m_akGameGridBottomRightVisibleUnclamped[uCamera];

        // The binary leaves the a2 slot unwritten; both callees ignore it.
        for (kCell.y = kTopLeft.y - 1; kCell.y <= kBottomRight.y; ++kCell.y) {
            for (kCell.x = kTopLeft.x - 1; kCell.x <= kBottomRight.x; ++kCell.x) {
                DrawFogCell(kCell, 0, mViewProj, 2);
            }
        }
        // Rows past the clamped visible range, i.e. off the map, get the prism without a state test.
        for (kCell.y = kBottomRight.y; kCell.y < kBottomRightUnclamped.y; ++kCell.y) {
            for (kCell.x = kTopLeftUnclamped.x; kCell.x < kBottomRightUnclamped.x; ++kCell.x) {
                DrawBlackHex(kCell, 0, mViewProj);
            }
        }
        for (kCell.y = kTopLeftUnclamped.y; kCell.y < kTopLeft.y; ++kCell.y) {
            for (kCell.x = kTopLeftUnclamped.x; kCell.x < kBottomRightUnclamped.x; ++kCell.x) {
                DrawBlackHex(kCell, 0, mViewProj);
            }
        }
    }

    m_pCommandStream->EndCommandSet();
    m_pCommandStream->AddAllCommandSetsToRenderQueue();
}
