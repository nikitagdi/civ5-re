/// origin binary
#include "Civ5/Src/App/GameViewState.h"
#include "Civ5/Src/App/CvStrategicViewCamera.h"
#include "Civ5/Src/App/Engine/cvDynamicGeometryVisSystem.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/FrameGlobalsData.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/Graphics/ArrowVisSystem.h"
#include "Civ5/Src/App/Graphics/FluidMasterJob.h"
#include "Civ5/Src/App/Graphics/TerrainSystem.h"
#include "Civ5/Src/App/Graphics/cvLandmarkVisSystem.h"
#include "Civ5/Src/App/Graphics/cvMinimapData.h"
#include "Civ5/Src/App/Graphics/cvMovementPathSPIViewCreator.h"
#include "Civ5/Src/App/Graphics/cvMovementPathVisSystem.h"
#include "Civ5/Src/App/Graphics/cvParticleVisSystem.h"
#include "Civ5/Src/App/Graphics/cvStrategicViewVisSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainIceSystem.h"
#include "Civ5/Src/App/Graphics/cvUnitVisSystem.h"
#include "Civ5/Src/App/Graphics/cvWaveSystem.h"
#include "Civ5/Src/App/NexusSupport/Listener.h"
#include "Civ5/Src/App/cvCityVisSystem.h"
#include "FirePlace/FireGrafix/CCommandStream.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireGrafix/FGXVector4.h"
#include "FirePlace/FireGrafix/TextureData.h"
#include "FirePlace/FireGrafix/core/FImageCoder.h"
#include "FirePlace/FireWorks/FluidFOWSystem.h"
#include "FirePlace/FireWorks/ForestVisSystem.h"
#include <math.h>
#include <string.h>

static const unsigned int kMouseMessageFirst = 0x200;
static const unsigned int kMouseMessageCount = 0xf;
static const unsigned int kKeyMessageFirst = 0x100;
static const unsigned int kKeyMessageCount = 0xa;
static const unsigned int kWonderPlacementModes = 3;

// Bits of m_uInitializedSystemsFlags, one per render system.
static const unsigned int kSystemTerrain = 0x1;
static const unsigned int kSystemLandmarks = 0x2;
static const unsigned int kSystemOverlay = 0x4;
static const unsigned int kSystemCities = 0x20;
static const unsigned int kSystemUnits = 0x40;
static const unsigned int kSystemForests = 0x80;
static const unsigned int kSystemParticles = 0x1000;
static const unsigned int kSystemFOW = 0x4000;
static const unsigned int kSystemFluidFOW = 0x8000;
static const unsigned int kSystemStrategicView = 0x40000;
static const unsigned int kSystemMinimap = 0x80000;
static const unsigned int kSystemWaves = 0x200000;
static const unsigned int kSystemArrows = 0x400000;
static const unsigned int kSystemMovementPath = 0x800000;

// Camera zoom height is MinZoomHeight * e^(kZoomLevelScale * level).
static const float kZoomLevelScale = 0.15f;
// Shadows are full below MaxZoom - 3, half below MaxZoom - 2 and off above.
static const float kShadowFullZoomMargin = 3.0f;
static const float kShadowHalfZoomMargin = 2.0f;
static const float kShadowHalfBand = 0.5f;
static const float kShadowFadeSnap = 0.01f;
static const float kShadowFadeRate = 0.1f;
static const float kShadowFadeOff = 0.001f;
static const float kFramesPerFpsSample = 10.0f;
static const unsigned int kFramesPerFpsSampleCount = 10;
static const float kClearGrey = 0.33f;

// g_EventSystemLocalMachine starts with the index of the active machine; machines are 0x600 bytes apart.
static const unsigned int kLocalMachineStride = 0x600;
// Slot of an event is 8 * id + 8: dispatcher, its context at +4.
static const unsigned int kFrameGlobalsEventSlot = 0x118;
static const unsigned int kVisibilityEventSlot = 0x120;
static const unsigned int kGameGridWidthOffset = 0x94;
static const unsigned int kGameGridHeightOffset = 0x98;
static const unsigned int kUnitVisJobSetOffset = 0x7aa0;
static const unsigned int kApplicationDebugFlagsOffset = 0xf4d;
static const float kClearDepth = 1.0f;
static const float kStrategicPlaneDistance = 255.0f;

typedef void (*PfnDispatchEvent)(const void* pEvent, void* pContext);

/// 0x088333c0
UNK_RET GameViewState::ResetValues() {
    m_uInitializedSystemsFlags = 0xffffffff;
    strcpy(m_szWonderType, "STONEHENGE");
}

/// 0x08836e4e
UNK_RET GameViewState::ChangeMode(unsigned int uMode, unsigned int a2) {
    m_uMode = uMode;
}

/// 0x08836e5e
UNK_RET GameViewState::OnUnitTypeChange(EventTemplate<Event_Int2Type<12>, AssetTypeChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    // AssetTypeChangedData is not declared yet: the art define name starts the event.
    const char* szType = reinterpret_cast<const char*>(&kEvent);
    m_strUnitType.Copy(szType != NULL ? (int)strlen(szType) : 0, szType, 0);
}

/// 0x08836eaa
UNK_RET GameViewState::OnWonderTypeChange(EventTemplate<Event_Int2Type<23>, AssetTypeChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    memcpy(m_szWonderType, &kEvent, sizeof(m_szWonderType));
}

/// 0x08836ee0
UNK_RET GameViewState::OnWonderStateChange(EventTemplate<Event_Int2Type<24>, WonderStateChangedData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    // WonderStateChangedData is not declared yet: its first int is the state.
    m_iWonderState = *reinterpret_cast<const int*>(&kEvent);
}

/// 0x08836ef2
UNK_RET GameViewState::OnWonderTogglePlacement(EventTemplate<Event_Int2Type<26>, NullEventClass, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    m_uWonderPlacement = (m_uWonderPlacement + 1) % kWonderPlacementModes;
}

/// 0x08837cc4
bool GameViewState::ProcessInput(HWND__* hWnd, unsigned int uMsg, unsigned int wParam, long lParam) {
    if (uMsg - kMouseMessageFirst < kMouseMessageCount) {
        return ProcessMouse(uMsg, wParam, lParam);
    }
    if (uMsg - kKeyMessageFirst < kKeyMessageCount) {
        ProcessKeyboard(uMsg, wParam, lParam);
        return true;
    }
    return false;
}

/// 0x0883b7b6
UNK_RET GameViewState::SetupFrameGlobals() {
    const auto setPlane = [](FGXVector4& kPlane, float fX, float fY, float fZ, float fW) {
        kPlane.x = fX;
        kPlane.y = fY;
        kPlane.z = fZ;
        kPlane.w = fW;
    };
    // Low edge of a visible hex span, widened by one hex and limited to the grid.
    const auto clampSpanMin = [](int iMin, int iLimit) {
        if (iMin <= 0) {
            return 0;
        }
        return iMin - 1 > iLimit ? iLimit : iMin - 1;
    };
    // High edge of a visible hex span, widened by one hex and limited to the grid.
    const auto clampSpanMax = [](int iMax, int iLimit) {
        if (iMax + 1 < 0) {
            return 0;
        }
        return iMax >= iLimit ? iLimit : iMax + 1;
    };
    const auto planeDistance = [](const FGXVector4& kPlane, float fX, float fY) {
        return kPlane.z * 0.0f + (kPlane.y * fY + kPlane.x * fX) + kPlane.w;
    };
    // Adds the view shifted by one world width as an extra camera so a wrapping map is drawn on both sides.
    const auto appendWrappedView = [](float fOffsetX) {
        D3DXMATRIX mTranslation;
        D3DXMATRIX mWrapped;
        D3DXMatrixTranslation(&mTranslation, fOffsetX, 0.0f, 0.0f);
        D3DXMatrixMultiply(&mWrapped, &mTranslation, &FrameGlobals::g_View[0]);
        const unsigned int uCamera = FrameGlobals::g_nCameras;
        FrameGlobals::g_View[uCamera] = mWrapped;
        FGXGetClipPlanesFromViewMatrix(&FrameGlobals::g_View[uCamera], &FrameGlobals::g_Proj, FrameGlobals::g_ViewClipPlanes[uCamera]);
        FrameGlobals::g_nCameras = uCamera + 1;
    };
    if (m_eViewType == VIEWTYPE_STRATEGIC) {
        CvStrategicViewCamera akCameras[3];
        cvStrategicViewVisSystem& kStrategicView = *reinterpret_cast<cvStrategicViewVisSystem*>(g_cvStrategicViewVisSystem);
        FrameGlobals::g_nCameras = kStrategicView.GetActiveCameras(akCameras);
        FGXVector3* pRelativeCameraPosition = reinterpret_cast<FGXVector3*>(FrameGlobals::g_RelativeCameraPosition);
        FGXInt2* pTopLeft = reinterpret_cast<FGXInt2*>(FrameGlobals::g_GameGridTopLeftVisible);
        FGXInt2* pBottomRight = reinterpret_cast<FGXInt2*>(FrameGlobals::g_GameGridBottomRightVisible);
        FGXInt2* pTopLeftUnclamped = reinterpret_cast<FGXInt2*>(FrameGlobals::g_GameGridTopLeftVisibleUnclamped);
        FGXInt2* pBottomRightUnclamped = reinterpret_cast<FGXInt2*>(FrameGlobals::g_GameGridBottomRightVisibleUnclamped);
        FGXVector3* pWorldBottomRight = reinterpret_cast<FGXVector3*>(FrameGlobals::g_WorldBottomRightVisible);
        const int iGridWidth = *reinterpret_cast<const int*>(WorldGlobals::g_GameGrid + kGameGridWidthOffset);
        const int iGridHeight = *reinterpret_cast<const int*>(WorldGlobals::g_GameGrid + kGameGridHeightOffset);
        for (unsigned int uCamera = 0; uCamera < FrameGlobals::g_nCameras; ++uCamera) {
            const CvStrategicViewCamera& kCamera = akCameras[uCamera];
            CvStrategicViewPlotRange kRange;
            kStrategicView.GetVisibleRegion(kCamera, kRange);
            pRelativeCameraPosition[uCamera].x = kCamera.m_fX;
            pRelativeCameraPosition[uCamera].y = kCamera.m_fY;
            pTopLeftUnclamped[uCamera].x = kRange.m_iMinX;
            pTopLeftUnclamped[uCamera].y = kRange.m_iMinY;
            pBottomRightUnclamped[uCamera].x = kRange.m_iMaxX;
            pBottomRightUnclamped[uCamera].y = kRange.m_iMaxY;
            pTopLeft[uCamera].x = clampSpanMin(kRange.m_iMinX, iGridWidth);
            pTopLeft[uCamera].y = clampSpanMin(kRange.m_iMinY, iGridHeight);
            pBottomRight[uCamera].x = clampSpanMax(kRange.m_iMaxX, iGridWidth);
            pBottomRight[uCamera].y = clampSpanMax(kRange.m_iMaxY, iGridHeight);
            FGXVector2 vTopLeft;
            FGXVector2 vBottomRight;
            kStrategicView.GetVisibleRegion(kCamera, vTopLeft, vBottomRight);
            FGXVector4* pPlanes = FrameGlobals::g_ViewClipPlanes[uCamera];
            setPlane(pPlanes[1], 0.0f, 0.0f, 1.0f, -kStrategicPlaneDistance);
            setPlane(pPlanes[0], 0.0f, 0.0f, 1.0f, kStrategicPlaneDistance);
            setPlane(pPlanes[3], 1.0f, 0.0f, 0.0f, -vTopLeft.x);
            setPlane(pPlanes[2], 1.0f, 0.0f, 0.0f, -vBottomRight.x);
            setPlane(pPlanes[5], 0.0f, 1.0f, 0.0f, -vTopLeft.y);
            setPlane(pPlanes[4], 0.0f, 1.0f, 0.0f, -vBottomRight.y);
            FrameGlobals::g_WorldTopLeftVisibleUnclamped[uCamera].x = vTopLeft.x;
            FrameGlobals::g_WorldTopLeftVisibleUnclamped[uCamera].y = vTopLeft.y;
            FrameGlobals::g_WorldTopLeftVisibleUnclamped[uCamera].z = 0.0f;
            FrameGlobals::g_WorldBottomRightVisibleUnclamped[uCamera].x = vBottomRight.x;
            FrameGlobals::g_WorldBottomRightVisibleUnclamped[uCamera].y = vBottomRight.y;
            FrameGlobals::g_WorldBottomRightVisibleUnclamped[uCamera].z = 0.0f;
            FrameGlobals::g_WorldTopLeftVisible[uCamera].x = WorldGlobals::g_WorldMin.x > vTopLeft.x ? WorldGlobals::g_WorldMin.x : vTopLeft.x;
            FrameGlobals::g_WorldTopLeftVisible[uCamera].y = WorldGlobals::g_WorldMin.y > vTopLeft.y ? WorldGlobals::g_WorldMin.y : vTopLeft.y;
            pWorldBottomRight[uCamera].x = WorldGlobals::g_WorldMax.x < vBottomRight.x ? WorldGlobals::g_WorldMax.x : vBottomRight.x;
            pWorldBottomRight[uCamera].y = WorldGlobals::g_WorldMax.y < vBottomRight.y ? WorldGlobals::g_WorldMax.y : vBottomRight.y;
        }
    } else if (m_eViewType == VIEWTYPE_GAME) {
        m_kCamera.GetViewProj(&FrameGlobals::g_View[0], &FrameGlobals::g_Proj);
        m_kCamera.GetClipPlanesInWorldSpace(FrameGlobals::g_ViewClipPlanes[0]);
        FrameGlobals::g_nCameras = 1;
        if (WorldGlobals::g_eWrapMode == 1) {
            const FGXVector4* pPlanes = FrameGlobals::g_ViewClipPlanes[0];
            if (planeDistance(pPlanes[3], WorldGlobals::g_WorldMin.x, WorldGlobals::g_WorldMax.y) > 0.0f) {
                appendWrappedView(-WorldGlobals::g_WorldSize.x);
            }
            if (planeDistance(pPlanes[2], WorldGlobals::g_WorldMax.x, WorldGlobals::g_WorldMax.y) > 0.0f) {
                appendWrappedView(WorldGlobals::g_WorldSize.x);
            }
        }
        FGXVector3* pRelativeCameraPosition = reinterpret_cast<FGXVector3*>(FrameGlobals::g_RelativeCameraPosition);
        for (unsigned int uCamera = 0; uCamera < FrameGlobals::g_nCameras; ++uCamera) {
            D3DXMatrixInverse(&FrameGlobals::g_InvView[uCamera], NULL, &FrameGlobals::g_View[uCamera]);
            pRelativeCameraPosition[uCamera].x = FrameGlobals::g_View[uCamera]._41 - FrameGlobals::g_View[0]._41;
            pRelativeCameraPosition[uCamera].y = FrameGlobals::g_View[uCamera]._42 - FrameGlobals::g_View[0]._42;
            pRelativeCameraPosition[uCamera].z = FrameGlobals::g_View[uCamera]._43 - FrameGlobals::g_View[0]._43;
        }
        CalcVisibleRegion();
        g_OverlaySystem.SetupFrameGlobals();
        Fun_08840dac(g_DistanceFog);
        g_TerrainSystem.SetupShadowFrameGlobals();
    }
    PublishFrameGlobals();
}

/// 0x0883beac
GameViewState::ViewType GameViewState::GetViewType() const {
    return m_eViewType;
}

/// 0x0883c71c
UNK_RET GameViewState::PublishFrameGlobals() {
    const auto dispatchLocalMachineEvent = [](unsigned int uSlot, const void* pEvent) {
        UNK_DATA* pSystem = g_EventSystemLocalMachine;
        const unsigned int uOffset = *reinterpret_cast<int*>(pSystem) * kLocalMachineStride;
        PfnDispatchEvent pfnDispatch = *reinterpret_cast<PfnDispatchEvent*>(pSystem + uOffset + uSlot);
        void* pContext = *reinterpret_cast<void**>(pSystem + uOffset + uSlot + 4);
        pfnDispatch(pEvent, pContext);
    };
    FrameGlobalsData kData;
    memset(&kData, 0, sizeof(kData));
    kData.m_fMaxZoom = FrameGlobals::fMaxZoom;
    kData.m_fMinZoom = FrameGlobals::fMinZoom;
    kData.m_fCurrZoom = FrameGlobals::fCurrZoom;
    kData.m_fCurrZoomNorm = FrameGlobals::fCurrZoomNorm;
    kData.m_mProj = FrameGlobals::g_Proj;
    memcpy(kData.m_amView, FrameGlobals::g_View, sizeof(kData.m_amView));
    memcpy(kData.m_amInvView, FrameGlobals::g_InvView, sizeof(kData.m_amInvView));
    memcpy(kData.m_aavViewClipPlanes, FrameGlobals::g_ViewClipPlanes, sizeof(kData.m_aavViewClipPlanes));
    memcpy(kData.m_amOverlayProjection, FrameGlobals::g_OverlayProjection, sizeof(kData.m_amOverlayProjection));
    memcpy(kData.m_avWorldTopLeftVisible, FrameGlobals::g_WorldTopLeftVisible, sizeof(kData.m_avWorldTopLeftVisible));
    memcpy(kData.m_avWorldBottomRightVisible, FrameGlobals::g_WorldBottomRightVisible, sizeof(kData.m_avWorldBottomRightVisible));
    memcpy(kData.m_amShadowViewProj, FrameGlobals::g_ShadowViewProj, sizeof(kData.m_amShadowViewProj));
    memcpy(kData.m_amReflectionViewProj, FrameGlobals::g_ReflectionViewProj, sizeof(kData.m_amReflectionViewProj));
    memcpy(kData.m_avWorldTopLeftVisibleUnclamped, FrameGlobals::g_WorldTopLeftVisibleUnclamped, sizeof(kData.m_avWorldTopLeftVisibleUnclamped));
    memcpy(kData.m_avWorldBottomRightVisibleUnclamped, FrameGlobals::g_WorldBottomRightVisibleUnclamped, sizeof(kData.m_avWorldBottomRightVisibleUnclamped));
    const FGXInt2* pTopLeft = reinterpret_cast<const FGXInt2*>(FrameGlobals::g_GameGridTopLeftVisible);
    const FGXInt2* pBottomRight = reinterpret_cast<const FGXInt2*>(FrameGlobals::g_GameGridBottomRightVisible);
    const bool bVisibilityChanged = memcmp(m_akPublishedGridTopLeftVisible, pTopLeft, sizeof(m_akPublishedGridTopLeftVisible)) != 0
        || memcmp(m_akPublishedGridBottomRightVisible, pBottomRight, sizeof(m_akPublishedGridBottomRightVisible)) != 0;
    if (bVisibilityChanged) {
        memcpy(m_akPublishedGridTopLeftVisible, pTopLeft, sizeof(m_akPublishedGridTopLeftVisible));
        memcpy(m_akPublishedGridBottomRightVisible, pBottomRight, sizeof(m_akPublishedGridBottomRightVisible));
    }
    memcpy(kData.m_akGameGridTopLeftVisible, pTopLeft, sizeof(kData.m_akGameGridTopLeftVisible));
    memcpy(kData.m_akGameGridBottomRightVisible, pBottomRight, sizeof(kData.m_akGameGridBottomRightVisible));
    memcpy(kData.m_akGameGridTopLeftVisibleUnclamped, FrameGlobals::g_GameGridTopLeftVisibleUnclamped, sizeof(kData.m_akGameGridTopLeftVisibleUnclamped));
    memcpy(kData.m_akGameGridBottomRightVisibleUnclamped, FrameGlobals::g_GameGridBottomRightVisibleUnclamped, sizeof(kData.m_akGameGridBottomRightVisibleUnclamped));
    kData.m_uCameras = FrameGlobals::g_nCameras;
    const unsigned int uDeviceId = *reinterpret_cast<const unsigned int*>(FireGrafix::DeviceSettings::g_DeviceId);
    kData.m_uDeviceIdHash = (uDeviceId & 0xffff) | (uDeviceId >> 16);
    memcpy(kData.m_afFOWBaseScale, &FrameGlobals::g_FOWBaseScale, sizeof(kData.m_afFOWBaseScale));
    memcpy(kData.m_afFogColor, &FrameGlobals::g_v4FogColor, sizeof(kData.m_afFogColor));
    memcpy(kData.m_afFogControl, &FrameGlobals::g_v4FogControl, sizeof(kData.m_afFogControl));
    kData.m_uFrame = FireGrafix::g_pRenderer->m_uFrame;
    dispatchLocalMachineEvent(kFrameGlobalsEventSlot, &kData);
    if (bVisibilityChanged) {
        VisibilityUpdatedData kVisibility;
        memset(&kVisibility, 0, sizeof(kVisibility));
        memcpy(kVisibility.m_akGameGridTopLeftVisible, pTopLeft, sizeof(kVisibility.m_akGameGridTopLeftVisible));
        memcpy(kVisibility.m_akGameGridBottomRightVisible, pBottomRight, sizeof(kVisibility.m_akGameGridBottomRightVisible));
        dispatchLocalMachineEvent(kVisibilityEventSlot, &kVisibility);
    }
}

/// 0x0883e89a
UNK_RET GameViewState::OnEndTurn(GameCoreEventTemplate<Event_Int2Type<125>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    m_kTurnTimer.Start();
}

/// 0x0883e8c6
UNK_RET GameViewState::OnBeginTurn(GameCoreEventTemplate<Event_Int2Type<124>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    m_fTurnProcessingTime = m_kTurnTimer.Peek();
    m_kTurnTimer.Stop();
}

/// 0x0883ccd6
void GameViewState::PreRender() {
    g_cvUnitVisSystem.StateSort();
    reinterpret_cast<cvCityVisSystem*>(g_cvCityVisSystem)->StateSort();
    reinterpret_cast<cvLandmarkVisSystem*>(g_cvLandmarkVisSystem)->StateSort();
}

/// 0x0883cd16
void GameViewState::RenderGame(unsigned int hRenderTarget) {
    if (!m_bRenderEnabled) {
        return;
    }
    cvCityVisSystem& kCityVis = *reinterpret_cast<cvCityVisSystem*>(g_cvCityVisSystem);
    cvLandmarkVisSystem& kLandmarkVis = *reinterpret_cast<cvLandmarkVisSystem*>(g_cvLandmarkVisSystem);
    cvDynamicGeometryVisSystem& kDynamicGeometryVis = *reinterpret_cast<cvDynamicGeometryVisSystem*>(g_cvDynamicGeometryVisSystem);
    cvParticleVisSystem& kParticleVis = *reinterpret_cast<cvParticleVisSystem*>(g_cvParticleVisSystem);
    cvMovementPathVisSystem& kMovementPathVis = *reinterpret_cast<cvMovementPathVisSystem*>(g_cvMovementPathVisSystem);
    FluidFOWSystem& kFluidFOW = *reinterpret_cast<FluidFOWSystem*>(g_FluidFOWSystem);
    FOWSystem& kFogOfWar = *reinterpret_cast<FOWSystem*>(g_FOWSystem);
    cvStrategicViewVisSystem& kStrategicViewVis = *reinterpret_cast<cvStrategicViewVisSystem*>(g_cvStrategicViewVisSystem);
    FJobSet& kUnitVisJobSet = *reinterpret_cast<FJobSet*>(reinterpret_cast<char*>(&g_cvUnitVisSystem) + kUnitVisJobSetOffset);
    // Every Clear command set passes 6 as its fourth argument.
    const auto submitClear = [this](unsigned int hTarget, unsigned int uFlags, unsigned int uMode, const char* szName, const FGXVector4& vColor) {
        const unsigned int uCommandSet = m_pCommandStream->BeginCommandSet(hTarget, uFlags, uMode, 6, szName, vColor, kClearDepth, 0, 0, 0);
        m_pCommandStream->EndCommandSet();
        m_pCommandStream->AddCommandSetToRenderQueue(uCommandSet);
    };
    if (*reinterpret_cast<const int*>(FireGrafix::DeviceSettings::g_StereoEnabled) != 0) {
        FGXVector3 vCameraPosition = { 0.0f, 0.0f, 0.0f };
        m_kCamera.m_camera.GetRow(&vCameraPosition, 3);
        const float fZoomLevel = logf(vCameraPosition.z / m_kCamera.m_fMinZoomHeight);
        const float fZoomRange = m_kCamera.m_fMaxZoomLevel - m_kCamera.m_fMinZoomLevel;
        float fNormalizedZoom = 0.0f;
        if (fZoomRange != 0.0f) {
            fNormalizedZoom = (fZoomLevel / kZoomLevelScale - m_kCamera.m_fMinZoomLevel) / fZoomRange;
        }
        const float fBlend = fNormalizedZoom * fNormalizedZoom;
        const float fDepthMin = *reinterpret_cast<const float*>(GraphicsSettings::GeneralSettings::g_fIngame3DConvergenceDepthMin);
        const float fDepthMax = *reinterpret_cast<const float*>(GraphicsSettings::GeneralSettings::g_fIngame3DConvergenceDepthMax);
        FireGrafix::SetStereoConvergence(fBlend * fDepthMax + (1.0f - fBlend) * fDepthMin);
    }
    const FGXVector4 vClearColor = { kClearGrey, kClearGrey, kClearGrey, 0.0f };
    if (m_eViewType == 0) {
        if ((m_uInitializedSystemsFlags & kSystemUnits) != 0) {
            g_cvUnitVisSystem.RenderInternal();
        }
        if ((m_uInitializedSystemsFlags & kSystemMinimap) != 0) {
            reinterpret_cast<cvMinimapSystem*>(g_MinimapSystem)->RenderMiniMap();
        }
        submitClear(hRenderTarget, 8, 0, "Clear", vClearColor);
        if ((m_uInitializedSystemsFlags & kSystemUnits) != 0) {
            kUnitVisJobSet.Wait();
        }
    } else if (m_eViewType == VIEWTYPE_GAME) {
        if ((m_uInitializedSystemsFlags & kSystemUnits) != 0) {
            g_cvUnitVisSystem.RenderInternal();
        }
        if ((m_uInitializedSystemsFlags & kSystemCities) != 0) {
            kCityVis.RenderInternal();
        }
        if ((m_uInitializedSystemsFlags & kSystemLandmarks) != 0) {
            kLandmarkVis.RenderInternal();
            kDynamicGeometryVis.RenderInternal();
        }
        if ((m_uInitializedSystemsFlags & kSystemParticles) != 0) {
            kParticleVis.RenderInternal();
        }
        if ((m_uInitializedSystemsFlags & kSystemMovementPath) != 0) {
            kMovementPathVis.RenderInternal();
        }
        if ((m_uInitializedSystemsFlags & kSystemMinimap) != 0) {
            reinterpret_cast<cvMinimapSystem*>(g_MinimapSystem)->RenderMiniMap();
        }
        if ((m_uInitializedSystemsFlags & kSystemTerrain) != 0) {
            g_TerrainSystem.LaunchJobs();
        }
        if ((m_uInitializedSystemsFlags & kSystemFluidFOW) != 0) {
            kFluidFOW.LaunchJobs();
        }
        g_cvUnitVisSystem.StateSort();
        kCityVis.StateSort();
        kLandmarkVis.StateSort();
        RenderShadows();
        RenderReflections();
        submitClear(hRenderTarget, 8, 0, "Clear", vClearColor);
        if ((m_uInitializedSystemsFlags & kSystemOverlay) != 0) {
            g_OverlaySystem.Render();
        }
        if ((m_uInitializedSystemsFlags & kSystemFluidFOW) != 0) {
            kFluidFOW.RenderFOWMask(false);
        }
        if ((m_uInitializedSystemsFlags & kSystemTerrain) != 0) {
            g_TerrainSystem.Render(hRenderTarget);
            g_TerrainSystem.RenderOverlay();
        }
        if ((m_uInitializedSystemsFlags & kSystemWaves) != 0) {
            reinterpret_cast<cvWaveSystem*>(g_WaveSystem)->Render(hRenderTarget);
        }
        if ((m_uInitializedSystemsFlags & kSystemCities) != 0) {
            kCityVis.OnRenderView(hRenderTarget);
        }
        if ((m_uInitializedSystemsFlags & kSystemArrows) != 0) {
            g_cvArrowVisSystem.Render(hRenderTarget);
        }
        if ((m_uInitializedSystemsFlags & kSystemForests) != 0) {
            reinterpret_cast<ForestVisSystem*>(g_ForestVisSystem)->OnRenderView(hRenderTarget);
        }
        if ((m_uInitializedSystemsFlags & kSystemLandmarks) != 0) {
            kLandmarkVis.OnRenderView(hRenderTarget);
            kDynamicGeometryVis.OnRenderView(hRenderTarget);
        }
        if ((m_uInitializedSystemsFlags & kSystemUnits) != 0) {
            g_cvUnitVisSystem.OnRenderViewZFail(hRenderTarget);
            g_cvUnitVisSystem.OnRenderView(hRenderTarget);
        }
        if ((m_uInitializedSystemsFlags & kSystemParticles) != 0) {
            kParticleVis.OnRenderView(hRenderTarget);
        }
        if ((m_uInitializedSystemsFlags & kSystemFOW) != 0) {
            kFogOfWar.Render(hRenderTarget);
        }
        if ((m_uInitializedSystemsFlags & kSystemFluidFOW) != 0) {
            kFluidFOW.Render(hRenderTarget);
        }
        if ((m_uInitializedSystemsFlags & kSystemMovementPath) != 0) {
            kMovementPathVis.OnRenderView(hRenderTarget);
        }
    } else if (m_eViewType == VIEWTYPE_STRATEGIC && (m_uInitializedSystemsFlags & kSystemStrategicView) != 0) {
        if ((m_uInitializedSystemsFlags & kSystemUnits) != 0) {
            g_cvUnitVisSystem.RenderInternal();
        }
        kStrategicViewVis.Render(hRenderTarget);
        if ((m_uInitializedSystemsFlags & kSystemMinimap) != 0) {
            reinterpret_cast<cvMinimapSystem*>(g_MinimapSystem)->RenderMiniMap();
        }
        if ((m_uInitializedSystemsFlags & kSystemUnits) != 0) {
            kUnitVisJobSet.Wait();
        }
    }
    const unsigned char* pApplication = *reinterpret_cast<const unsigned char* const*>(g_pkApplication);
    if ((pApplication[kApplicationDebugFlagsOffset] & 1) != 0) {
        PrintDebugInfo_noAlloc(hRenderTarget);
    }
    if (FireGrafix::g_pRenderer->m_uFrame % kFramesPerFpsSampleCount == 0) {
        m_fFramesPerSecond = kFramesPerFpsSample / m_kFpsTimer.Peek();
        m_kFpsTimer.Stop();
        m_kFpsTimer.Start();
    }
}

/// 0x0883d34e
void GameViewState::RenderShadows() {
    const unsigned int hShadowTarget = RenderGlobals::g_TexHandles[5];
    const unsigned int uShadowLevel = GraphicsSettings::GeneralSettings::g_uShadowLevel;
    if (hShadowTarget == 0 || uShadowLevel == 0) {
        return;
    }
    cvCityVisSystem& kCityVis = *reinterpret_cast<cvCityVisSystem*>(g_cvCityVisSystem);
    cvLandmarkVisSystem& kLandmarkVis = *reinterpret_cast<cvLandmarkVisSystem*>(g_cvLandmarkVisSystem);
    cvDynamicGeometryVisSystem& kDynamicGeometryVis = *reinterpret_cast<cvDynamicGeometryVisSystem*>(g_cvDynamicGeometryVisSystem);
    cvParticleVisSystem& kParticleVis = *reinterpret_cast<cvParticleVisSystem*>(g_cvParticleVisSystem);
    // Every Clear command set passes 6 as its fourth argument.
    const auto submitClear = [this](unsigned int hTarget, unsigned int uFlags, unsigned int uMode, const char* szName, const FGXVector4& vColor) {
        const unsigned int uCommandSet = m_pCommandStream->BeginCommandSet(hTarget, uFlags, uMode, 6, szName, vColor, kClearDepth, 0, 0, 0);
        m_pCommandStream->EndCommandSet();
        m_pCommandStream->AddCommandSetToRenderQueue(uCommandSet);
    };
    const FGXVector4 vClearZero = { 0.0f, 0.0f, 0.0f, 0.0f };
    if (uShadowLevel <= 2 && GraphicsSettings::GeneralSettings::g_bFadeShadows[0] != 0) {
        float fTargetFade;
        if (FrameGlobals::fCurrZoom > FrameGlobals::fMaxZoom - kShadowHalfZoomMargin) {
            g_fShadowZoomBand = 0.0f;
            fTargetFade = 0.0f;
        } else if (FrameGlobals::fCurrZoom < FrameGlobals::fMaxZoom - kShadowFullZoomMargin) {
            g_fShadowZoomBand = 1.0f;
            fTargetFade = 1.0f;
        } else {
            g_fShadowZoomBand = kShadowHalfBand;
            fTargetFade = kShadowHalfBand;
        }
        const float fDelta = fTargetFade - m_fShadowFade;
        float fFade = fTargetFade;
        if (!(fabsf(fDelta) < kShadowFadeSnap)) {
            fFade = m_fShadowFade + fDelta * kShadowFadeRate;
        }
        m_fShadowFade = fFade;
        FrameGlobals::fShadowFade = fFade;
        if (fFade < kShadowFadeOff) {
            // Shadows are invisible: only the blurred target is cleared.
            submitClear(RenderGlobals::g_TexHandles[6], 0, 9, "Clear", vClearZero);
            return;
        }
    } else {
        m_fShadowFade = 1.0f;
        FrameGlobals::fShadowFade = 1.0f;
    }
    submitClear(hShadowTarget, 8, 9, "Clear", vClearZero);
    if ((m_uInitializedSystemsFlags & kSystemUnits) != 0) {
        g_cvUnitVisSystem.OnRenderShadow(hShadowTarget);
    }
    if ((m_uInitializedSystemsFlags & kSystemCities) != 0) {
        kCityVis.OnRenderShadow(hShadowTarget);
    }
    if ((m_uInitializedSystemsFlags & kSystemLandmarks) != 0) {
        kLandmarkVis.OnRenderShadow(hShadowTarget);
        kDynamicGeometryVis.OnRenderShadow(hShadowTarget);
    }
    if ((m_uInitializedSystemsFlags & kSystemParticles) != 0) {
        kParticleVis.OnRenderShadow(hShadowTarget);
    }
    g_TerrainSystem.PerformShadowBlur();
}

/// 0x0883d62c
void GameViewState::RenderReflections() {
    const unsigned int hReflectionTarget = RenderGlobals::g_TexHandles[7];
    const unsigned int uReflectionLevel = GraphicsSettings::GeneralSettings::g_uReflectionLevel;
    if (hReflectionTarget == 0 || uReflectionLevel == 0) {
        return;
    }
    cvCityVisSystem& kCityVis = *reinterpret_cast<cvCityVisSystem*>(g_cvCityVisSystem);
    cvLandmarkVisSystem& kLandmarkVis = *reinterpret_cast<cvLandmarkVisSystem*>(g_cvLandmarkVisSystem);
    const FGXVector4 vClearZero = { 0.0f, 0.0f, 0.0f, 0.0f };
    const unsigned int uCommandSet = m_pCommandStream->BeginCommandSet(hReflectionTarget, 8, 0, 6, "Clear Reflections", vClearZero, kClearDepth, 0, 0, 0);
    m_pCommandStream->EndCommandSet();
    m_pCommandStream->AddCommandSetToRenderQueue(uCommandSet);
    if ((m_uInitializedSystemsFlags & kSystemUnits) != 0) {
        g_cvUnitVisSystem.OnRenderReflection(hReflectionTarget);
    }
    if ((m_uInitializedSystemsFlags & kSystemCities) != 0) {
        kCityVis.OnRenderReflection(hReflectionTarget);
    }
    if ((m_uInitializedSystemsFlags & kSystemLandmarks) != 0) {
        kLandmarkVis.OnRenderReflection(hReflectionTarget);
    }
    if ((m_uInitializedSystemsFlags & kSystemTerrain) != 0) {
        reinterpret_cast<cvTerrainIceSystem*>(g_TerrainSystem.unk_529bc)->RenderReflections(hReflectionTarget);
    }
}

/// 0x0883e48a
void GameViewState::Render(unsigned int a1) {
    if ((field_1b0 & 0xf) != 0) {
        RenderBenchmark(a1);
    } else {
        RenderGame(a1);
    }
}
