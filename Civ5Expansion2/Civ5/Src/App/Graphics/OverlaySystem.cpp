/// origin guess
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/CultureBorders.h"
#include "Civ5/Src/App/Graphics/HighlightSystem.h"
#include "Civ5/Src/App/Graphics/MovementOverlay.h"
#include "Civ5/Src/App/Graphics/OverlaySystem.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "Civ5/Src/App/Graphics/TradeVisSystem.h"
#include "Civ5/Src/App/NetProxyConnectionResponseData.h"
#include "Civ5/Src/App/UI/UIRenderBuffer.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/core/FImageCoder.h"

// Color clear flag of CCommandStream::BeginCommandSet.
static const unsigned int kClearColor = 4;
// Depth clear flag, and both clears together.
static const unsigned int kClearDepth = 2;
static const unsigned int kClearColorDepth = 6;
// Offset of the overlay projection inside the UI shader global constants.
static const unsigned int kUIProjectionOffset = 0x40;
// The overlay projection is snapped to this many world units and padded by one snap cell.
static const int kProjectionSnapMask = ~63;
static const float kProjectionPadding = 64.0f;
// Height of the upper clip plane that bounds the overlay volume.
static const float kOverlayTopHeight = 127.5f;

/// 0x088c5dec
void OverlaySystem::Startup(int iFlag) {
    m_pCommandStream = FireGrafix::g_pRenderer->CreateCommandStream(0x2000, 0x80, 0x100000, 0);
    if (GraphicsSettings::GeneralSettings::g_uOverlayLevel == 2) {
        RenderGlobals::g_TexHandles[0] = FireGrafix::g_pRenderer->CreateRenderTexture(0x800, 0x800, 0, 0x15, "Terrain Overlay");
        RenderGlobals::g_TexHandles[1] = FireGrafix::g_pRenderer->CreateRenderTexture(FireGrafix::CRenderer9::m_nScreenWidth, FireGrafix::CRenderer9::m_nScreenHeight, 0, 0x15, "Screen Overlay");
    } else {
        const unsigned int uSize = GraphicsSettings::GeneralSettings::g_uOverlayLevel == 1 ? 0x800 : 0x400;
        RenderGlobals::g_TexHandles[0] = FireGrafix::g_pRenderer->CreateRenderTexture(uSize, uSize, 0, 0x15, "Terrain Overlay");
        RenderGlobals::g_TexHandles[1] = 0;
    }
    reinterpret_cast<MovementOverlay*>(m_kMovementOverlay)->Startup();
    reinterpret_cast<CultureBorders*>(m_kCultureBorders)->Startup();
    m_kHexOverlay.Startup();
    reinterpret_cast<HighlightSystem*>(m_kHighlightSystem)->Startup();
    m_kTradeVisSystem.Startup();
    m_hZState = FireGrafix::g_pRenderer->CreateZState(true, true, static_cast<_D3DCMPFUNC>(2), static_cast<_D3DCULL>(1), 0.0f);
    FGXVector4 vBlendFactor = {0.0f, 0.0f, 0.0f, 0.0f};
    m_hAlphaState = FireGrafix::g_pRenderer->CreateAlphaState(true, static_cast<_D3DBLEND>(2), static_cast<_D3DBLEND>(6), static_cast<_D3DBLENDOP>(1),
                                                              static_cast<_D3DBLEND>(2), static_cast<_D3DBLEND>(6), static_cast<_D3DBLENDOP>(1), 0xf, vBlendFactor);
    m_iEnabled = 1;
    m_bFlag = iFlag != 0;
}

/// 0x088c6046
void OverlaySystem::Shutdown() {
    m_kTradeVisSystem.Shutdown();
    reinterpret_cast<MovementOverlay*>(m_kMovementOverlay)->Shutdown();
    reinterpret_cast<CultureBorders*>(m_kCultureBorders)->Shutdown();
    m_kHexOverlay.Shutdown();
    reinterpret_cast<HighlightSystem*>(m_kHighlightSystem)->Shutdown();
    FireGrafix::g_pRenderer->ReleaseCommandStream(m_pCommandStream);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, RenderGlobals::g_TexHandles[0]);
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, RenderGlobals::g_TexHandles[1]);
}

/// 0x088c60e6
void OverlaySystem::StartupGame() {
    m_kTradeVisSystem.StartupGame();
    reinterpret_cast<MovementOverlay*>(m_kMovementOverlay)->StartupGame();
    m_kHexOverlay.StartupGame();
    reinterpret_cast<CultureBorders*>(m_kCultureBorders)->StartupGame();
    reinterpret_cast<HighlightSystem*>(m_kHighlightSystem)->StartupGame();
    // Bit 0 of application byte 0xaa0 gates the initial clear of the terrain overlay target.
    const unsigned char* pApplication = *reinterpret_cast<const unsigned char* const*>(g_pkApplication);
    if ((pApplication[0xaa0] & 1) != 0 && m_iEnabled != 0) {
        const FGXVector4 vClear = {1.0f, 1.0f, 1.0f, 0.0f};
        m_pCommandStream->BeginCommandSet(RenderGlobals::g_TexHandles[0], 0, 0, kClearColor, "Terrain Overlay: Initial Clear", vClear, 1.0f, 0, 0, 0);
        m_pCommandStream->EndCommandSet();
        m_pCommandStream->AddAllCommandSetsToRenderQueue();
    }
}

/// 0x088c61fa
void OverlaySystem::ShutdownGame() {
    m_kTradeVisSystem.ShutdownGame();
    reinterpret_cast<MovementOverlay*>(m_kMovementOverlay)->ShutdownGame();
    reinterpret_cast<CultureBorders*>(m_kCultureBorders)->ShutdownGame();
    reinterpret_cast<HighlightSystem*>(m_kHighlightSystem)->ShutdownGame();
}

/// 0x088c624c
void OverlaySystem::SetupFrameGlobals() {
    if (m_iEnabled == 0) {
        return;
    }
    // Camera 0's side clip planes (2, 3) and top/bottom clip planes (4, 5) cut the ground plane and the upper plane into 8 corners.
    FGXVector4 vGroundPlane = {0.0f, 0.0f, 1.0f, 0.0f};
    FGXVector4 vTopPlane = {0.0f, 0.0f, 1.0f, -kOverlayTopHeight};
    FGXVector3 avCorners[8] = {};
    FGXVector4* pClipPlanes = FrameGlobals::g_ViewClipPlanes[0];
    FGX3PlaneIntersect(&avCorners[0], &vGroundPlane, &pClipPlanes[2], &pClipPlanes[4]);
    FGX3PlaneIntersect(&avCorners[1], &vGroundPlane, &pClipPlanes[2], &pClipPlanes[5]);
    FGX3PlaneIntersect(&avCorners[2], &vGroundPlane, &pClipPlanes[3], &pClipPlanes[4]);
    FGX3PlaneIntersect(&avCorners[3], &vGroundPlane, &pClipPlanes[3], &pClipPlanes[5]);
    FGX3PlaneIntersect(&avCorners[4], &vTopPlane, &pClipPlanes[2], &pClipPlanes[4]);
    FGX3PlaneIntersect(&avCorners[5], &vTopPlane, &pClipPlanes[2], &pClipPlanes[5]);
    FGX3PlaneIntersect(&avCorners[6], &vTopPlane, &pClipPlanes[3], &pClipPlanes[4]);
    FGX3PlaneIntersect(&avCorners[7], &vTopPlane, &pClipPlanes[3], &pClipPlanes[5]);

    float fMaxX = avCorners[0].x;
    float fMaxY = avCorners[0].y;
    float fMinX = fMaxX;
    float fMinY = fMaxY;
    for (int i = 1; i < 8; ++i) {
        const float fX = avCorners[i].x;
        const float fY = avCorners[i].y;
        fMaxX = fMaxX < fX ? fX : fMaxX;
        fMinX = fX < fMinX ? fX : fMinX;
        fMaxY = fMaxY < fY ? fY : fMaxY;
        fMinY = fY < fMinY ? fY : fMinY;
    }
    const auto snapBound = [](float fBound) {
        return static_cast<float>(static_cast<int>(fBound) & kProjectionSnapMask);
    };
    D3DXMatrixOrthoOffCenterRH(&FrameGlobals::g_OverlayProjection[0],
                               snapBound(fMinX - kProjectionPadding), snapBound(fMaxX + kProjectionPadding),
                               snapBound(fMinY - kProjectionPadding), snapBound(fMaxY + kProjectionPadding),
                               -1.0f, 1.0f);
    // Other cameras reach the overlay target through camera 0's projection.
    for (unsigned int uCamera = 1; uCamera < FrameGlobals::g_nCameras; ++uCamera) {
        D3DXMATRIX mViewToCamera0;
        D3DXMatrixMultiply(&mViewToCamera0, &FrameGlobals::g_View[uCamera], &FrameGlobals::g_InvView[0]);
        D3DXMatrixMultiply(&FrameGlobals::g_OverlayProjection[uCamera], &mViewToCamera0, &FrameGlobals::g_OverlayProjection[0]);
    }
}

/// 0x088c659e
void OverlaySystem::Render() {
    if (m_iEnabled == 0) {
        return;
    }
    const FGXVector4 vZero = {0.0f, 0.0f, 0.0f, 0.0f};
    m_pCommandStream->BeginCommandSet(RenderGlobals::g_TexHandles[0], 0, 0, kClearColorDepth, "Terrain Overlay: Primer Pass Clear", vZero, 1.0f, 0, 0, 0);
    m_pCommandStream->EndCommandSet();

    // Both global sets receive the camera's overlay projection; the aliases live for this command set only.
    FireGrafix::OverlayGlobals* pOverlayGlobals = nullptr;
    FireGrafix::UIShaderGlobalData* pUIShaderGlobals = nullptr;
    const auto setProjection = [this, &pOverlayGlobals, &pUIShaderGlobals](const D3DXMATRIX& mProjection) {
        pOverlayGlobals = static_cast<FireGrafix::OverlayGlobals*>(FireGrafix::g_pOverlayGlobals->AliasGlobal(m_pCommandStream));
        memcpy(pOverlayGlobals->LockGlobal(0), &mProjection, sizeof(D3DXMATRIX));
        pOverlayGlobals->Unlock(0);
        pUIShaderGlobals = static_cast<FireGrafix::UIShaderGlobalData*>(FireGrafix::g_pUIShaderGlobalData->AliasGlobal(m_pCommandStream));
        memcpy(static_cast<char*>(pUIShaderGlobals->LockGlobal(0)) + kUIProjectionOffset, &mProjection, sizeof(D3DXMATRIX));
        pUIShaderGlobals->Unlock(0);
    };
    const auto releaseAliases = [&pOverlayGlobals, &pUIShaderGlobals]() {
        FireGrafix::FGXConstantSetManager::ReleaseConstantSet(pOverlayGlobals->field_34);
        FireGrafix::FGXConstantSetManager::ReleaseConstantSet(pUIShaderGlobals->field_34);
    };

    for (unsigned int uCamera = 0; uCamera < FrameGlobals::g_nCameras; ++uCamera) {
        // Camera 0 draws the culture borders before the other overlays.
        if (uCamera == 0) {
            m_pCommandStream->BeginCommandSet(RenderGlobals::g_TexHandles[0], m_hAlphaState, m_hZState, 0, "Terrain Overlay", vZero, 1.0f, 0, 0, 0);
            setProjection(FrameGlobals::g_OverlayProjection[0]);
            reinterpret_cast<CultureBorders*>(m_kCultureBorders)->Render(m_pCommandStream, 0);
            m_pCommandStream->EndCommandSet();
            releaseAliases();
        }
        const FGXVector4 vWhite = {1.0f, 1.0f, 1.0f, 0.0f};
        m_pCommandStream->BeginCommandSet(RenderGlobals::g_TexHandles[0], 0, 0, kClearDepth, "Terrain Overlay: Post Culture Borders Z Clear", vWhite, 1.0f, 0, 0, 0);
        m_pCommandStream->EndCommandSet();

        m_pCommandStream->BeginCommandSet(RenderGlobals::g_TexHandles[0], m_hAlphaState, 9, 0, "Terrain Overlay", vZero, 1.0f, 0, 0, 0);
        setProjection(FrameGlobals::g_OverlayProjection[uCamera]);
        reinterpret_cast<MovementOverlay*>(m_kMovementOverlay)->Render(m_pCommandStream, uCamera);
        reinterpret_cast<HighlightSystem*>(m_kHighlightSystem)->Render(m_pCommandStream, uCamera);
        m_kHexOverlay.Render(m_pCommandStream, uCamera);
        m_kTradeVisSystem.Render(m_pCommandStream, uCamera);
        m_pCommandStream->EndCommandSet();
        releaseAliases();
    }
    m_pCommandStream->AddAllCommandSetsToRenderQueue();
}

/// 0x088c6a82
void OverlaySystem::Update(float fDeltaTime, bool bFlag) {
    reinterpret_cast<MovementOverlay*>(m_kMovementOverlay)->Update(fDeltaTime);
    m_kHexOverlay.Update(fDeltaTime);
    reinterpret_cast<CultureBorders*>(m_kCultureBorders)->Update(fDeltaTime);
    reinterpret_cast<HighlightSystem*>(m_kHighlightSystem)->Update(fDeltaTime);
    m_kTradeVisSystem.Update(fDeltaTime, bFlag);
}
