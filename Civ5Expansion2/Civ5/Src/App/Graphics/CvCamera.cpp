/// origin guess
#include "Civ5/Src/App/Graphics/CvCamera.h"
#include "Civ5/Src/App/CameraZoomData.h"
#include "Civ5/Src/App/CvInitMgr.h"
#include "Civ5/Src/App/DragCameraData.h"
#include "Civ5/Src/App/EventTemplate.h"
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "Civ5/Src/App/GameViewState.h"
#include "Civ5/Src/App/Graphics/cvStrategicViewVisSystem.h"
#include "Civ5/Src/App/Graphics/cvTerrainSampler.h"
#include "Civ5/Src/App/NetProxySessionCallbacks.h"
#include "Civ5/Src/App/UI/UIManager.h"
#include "Civ5/Src/App/UI/UIString16384.h"
#include "Civ5/Src/App/Graphics/Terrain.h"
#include "FirePlace/FireGrafix/CRenderer9.h"
#include "FirePlace/FireWorks/FCSVFile.h"
#include <math.h>
#include <stdio.h>
#include <string.h>

// Zoom height is MinZoomHeight * e^(kZoomLevelScale * level).
static const float kZoomLevelScale = 0.15f;
static const float kEuler = 2.7182817f;
static const double kPi = 3.141592653589793;
static const double kHalfPi = 1.5707963267948966;
static const float kRotateSpeed = 40.0f;
static const float kReleaseSpeedScale = 0.9f;
// Follow easing: remaining distance is k * max((d / k)^0.25 / k - dt, 0)^4.
static const float kFollowScale = 1.025f;
// Drag picks on the plane z = 60 * heightScale.
static const float kDragPlaneD = -60.0f;
static const float kDragBoundsInset = 0.001f;

static const unsigned int kLocalMachineStride = 0x600;
static const unsigned int kNonSerialStride = 0x180;
// Slots of g_EventSystemLocalMachine / g_EventSystemNonSerial: dispatcher, its context at +4.
static const unsigned int kCameraStartedMovingSlot = 0x1d0;
static const unsigned int kCameraStoppedMovingSlot = 0x1d8;
static const unsigned int kWrapEventSlot = 0x1e0;
static const unsigned int kViewChangedSlot = 0x28;

static const unsigned int kOptionZoomAlongViewAxis = 0xb;
static const unsigned int kOptionDragInertia = 0x14;
static const unsigned int kOptionDragSpeed = 0x44;

static const unsigned int kUIManagerDragCameraBit = 0x40;
static const int kUIManagerModeAlwaysRefresh = 2;

static const unsigned int kApplicationView = 0xb8;

// A serial game-core event carries its payload after a 4-byte header.
static const unsigned int kSerialEventPayload = 4;

typedef void (*PfnDispatchEvent)(const void* pEvent, void* pContext);

/// 0x088af9e8
CvCamera::CvCamera() : field_0(0x40), m_camera(45.0f), m_vPosition(), m_vVelocity(), m_vPrevPosition() {
    for (MoveSample& kSample : m_aMoveSamples) {
        kSample.m_vDelta = FGXVector3();
    }
    m_iFlip = -1;
    if (DONT_FLIP_CAMERA()) {
        m_iFlip = 1;
    }
    Reset();
}

/// 0x088afcaa
void CvCamera::RecalculateCurrentZoomLevel() {
    m_fZoomLevel = logf(m_vPosition.z / m_fMinZoomHeight) / kZoomLevelScale;
}

/// 0x088b03b6
UNK_RET CvCamera::MoveCameraSpaceInstantly(FGXVector3 const& vDelta) {
    FGXVector3 vRight = { 0.0f, 0.0f, 0.0f };
    FGXVector3 vForward = { 0.0f, 0.0f, 0.0f };
    FGXVector3 vUp = { 0.0f, 0.0f, 0.0f };
    FGXVector3 vNew = { 0.0f, 0.0f, 0.0f };
    m_camera.GetRow(&vRight, 0);
    m_camera.GetRow(&vUp, 1);
    m_camera.GetRow(&vForward, 2);
    m_camera.GetRow(&vNew, 3);
    const float fRightLength = sqrtf(vRight.x * vRight.x + vRight.y * vRight.y + vRight.z * vRight.z);
    vRight.x = vRight.x / fRightLength;
    vRight.y = vRight.y / fRightLength;
    vRight.z = vRight.z / fRightLength;
    const float fForwardLength = sqrtf(vForward.x * vForward.x + vForward.y * vForward.y + vForward.z * vForward.z);
    vForward.x = vForward.x / fForwardLength;
    vForward.y = vForward.y / fForwardLength;
    vForward.z = vForward.z / fForwardLength;
    const float fUpLength = sqrtf(vUp.x * vUp.x + vUp.y * vUp.y + vUp.z * vUp.z);
    vUp.x = vUp.x / fUpLength;
    vUp.y = vUp.y / fUpLength;
    vUp.z = vUp.z / fUpLength;
    vNew.x = vUp.x * vDelta.z + ((vRight.x * vDelta.x + vNew.x) - vForward.x * vDelta.y);
    vNew.y = vUp.y * vDelta.z + ((vRight.y * vDelta.x + vNew.y) - vForward.y * vDelta.y);
    vNew.z = vUp.z * vDelta.z + ((vRight.z * vDelta.x + vNew.z) - vForward.z * vDelta.y);
    if (PositionIsWithinBounds(vNew)) {
        m_vPosition = vNew;
        m_camera.m_mWorld._41 = vNew.x;
        m_camera.m_mWorld._42 = vNew.y;
        m_camera.m_mWorld._43 = vNew.z;
    }
    RecalculateCurrentZoomLevel();
}

/// 0x088b0cda
float CvCamera::ClipZoomToBounds(float fZoomDelta) const {
    const float fLevel = m_fZoomSpeed * fZoomDelta + m_fZoomLevel;
    if (fLevel < m_fMinZoomLevel) {
        return m_fMinZoomLevel;
    }
    if (fLevel > m_fMaxZoomLevel) {
        return m_fMaxZoomLevel;
    }
    if (!(fLevel > -HUGE_VALF && fLevel < HUGE_VALF)) {
        return m_fMinZoomLevel;
    }
    return fLevel;
}

/// 0x088b0c04
bool CvCamera::ZoomAlongVector(FGXVector3 const& vDirection, float fZoomDelta) {
    const float fLevel = ClipZoomToBounds(fZoomDelta);
    m_fZoomLevel = fLevel;
    const float fHeight = m_fMinZoomHeight * powf(kEuler, fLevel * kZoomLevelScale);
    FGXVector3 vMove;
    vMove.z = fHeight - m_vPosition.z;
    vMove.y = (vDirection.y / vDirection.z) * vMove.z;
    vMove.x = (vDirection.x / vDirection.z) * vMove.z;
    return MoveAlongVector(vMove, 1.0f);
}

/// 0x088b2c36
void CvCamera::Pitch(float fAngle) {
    m_camera.Tilt(fAngle);
}

/// 0x088b0ab8
void CvCamera::ClipPositionToBounds(FGXVector3& vPos) {
    FGXVector3 vForward = { 0.0f, 0.0f, 0.0f };
    m_camera.GetRow(&vForward, 2);
    const float fOffsetY = (vPos.z - WorldGlobals::g_WorldMin.z) * (-vForward.y / vForward.z);
    const float fMaxY = WorldGlobals::g_WorldMax.y - fOffsetY;
    const float fMinY = WorldGlobals::g_WorldMin.y - fOffsetY;
    float fX = vPos.x;
    const float fY = vPos.y;
    float fClampedY = fY;
    if (fY > fMaxY) {
        vPos.y = fMaxY;
        fClampedY = fMaxY;
    }
    if (fMinY > fClampedY) {
        vPos.y = fMinY;
    }
    if (WorldGlobals::g_eWrapMode == 1) {
        while (fX < WorldGlobals::g_WorldMin.x) {
            fX += WorldGlobals::g_WorldSize.x;
        }
        while (fX >= WorldGlobals::g_WorldMax.x) {
            fX -= WorldGlobals::g_WorldSize.x;
        }
    }
    if (m_fBoundsMinX > fX) {
        vPos.x = m_fBoundsMinX;
    }
    const float fRowY = fY + fOffsetY;
    if (fX > m_fBoundsMaxX) {
        vPos.x = m_fBoundsMaxX;
    }
    if (m_fBoundsMinY > fRowY) {
        vPos.y = m_fBoundsMinY - fOffsetY;
    }
    if (fRowY > m_fBoundsMaxY) {
        vPos.y = m_fBoundsMaxY - fOffsetY;
    }
}

/// 0x088b06e4
void CvCamera::MoveCameraSpaceXY(FGXVector2 const& vDelta) {
    FGXVector3 vRight = { 0.0f, 0.0f, 0.0f };
    FGXVector3 vForward = { 0.0f, 0.0f, 0.0f };
    m_camera.GetRow(&vRight, 0);
    m_camera.GetRow(&vForward, 2);
    FGXVector3 vNew = m_vPosition;
    vRight.z = 0.0f;
    vForward.z = 0.0f;
    const float fRightLength = sqrtf(vRight.y * vRight.y + vRight.x * vRight.x + 0.0f);
    vRight.x = vRight.x / fRightLength;
    vRight.y = vRight.y / fRightLength;
    vRight.z = vRight.z / fRightLength;
    const float fForwardLength = sqrtf(vForward.z * vForward.z + vForward.y * vForward.y + vForward.x * vForward.x);
    vForward.x = vForward.x / fForwardLength;
    vForward.y = vForward.y / fForwardLength;
    vForward.z = vForward.z / fForwardLength;
    vNew.x = (vRight.x * vDelta.x + vNew.x) - vForward.x * vDelta.y;
    vNew.y = (vRight.y * vDelta.x + vNew.y) - vForward.y * vDelta.y;
    vNew.z = (vRight.z * vDelta.x + vNew.z) - vForward.z * vDelta.y;
    if (PositionIsWithinBounds(vNew)) {
        m_vPosition = vNew;
    }
    RecalculateCurrentZoomLevel();
}

/// 0x088b0f78
UNK_RET CvCamera::Update(float fDeltaTime) {
    // An event system is an array of instances; its first int selects the current one.
    const auto dispatchEvent = [](UNK_DATA* pSystem, unsigned int uStride, unsigned int uSlot, const void* pEvent) {
        const unsigned int uOffset = *reinterpret_cast<int*>(pSystem) * uStride;
        PfnDispatchEvent pfnDispatch = *reinterpret_cast<PfnDispatchEvent*>(pSystem + uOffset + uSlot);
        void* pContext = *reinterpret_cast<void**>(pSystem + uOffset + uSlot + 4);
        pfnDispatch(pEvent, pContext);
    };
    const auto uiManager = []() -> const UIManager& {
        return **reinterpret_cast<const UIManager* const*>(g_pUIManager);
    };
    m_fElapsedTime += fDeltaTime;
    if ((uiManager().m_uMouseDragFlags & kUIManagerDragCameraBit) != 0 &&
        reinterpret_cast<const unsigned char*>(cvOptionsManager::g_GameOptionsStore)[kOptionDragInertia] != 0) {
        if ((uiManager().m_uMouseKeyState & 1) != 0) {
            // While the drag button is held, record how far the camera moved this frame.
            m_bInertia = false;
            MoveSample& kSample = m_aMoveSamples[m_ucMoveSampleIndex];
            kSample.m_fDeltaTime = fDeltaTime;
            kSample.m_vDelta.x = m_camera.m_mWorld._41 - m_vPrevPosition.x;
            kSample.m_vDelta.y = m_camera.m_mWorld._42 - m_vPrevPosition.y;
            kSample.m_vDelta.z = m_camera.m_mWorld._43 - m_vPrevPosition.z;
            unsigned char ucNext = m_ucMoveSampleIndex + 1;
            if (ucNext == 2) {
                ucNext = 0;
            }
            m_ucMoveSampleIndex = ucNext;
        } else {
            // Released: the release velocity is the time-weighted mean of the recorded samples.
            m_vVelocity.x = 0.0f;
            m_vVelocity.y = 0.0f;
            m_vVelocity.z = 0.0f;
            float fTotalTime = 0.0f;
            for (const MoveSample& kSample : m_aMoveSamples) {
                if (kSample.m_fDeltaTime != 0.0f) {
                    m_vVelocity.x += kSample.m_vDelta.x;
                    m_vVelocity.y += kSample.m_vDelta.y;
                    m_vVelocity.z += kSample.m_vDelta.z;
                    fTotalTime += kSample.m_fDeltaTime;
                }
            }
            if (fTotalTime > 0.0f) {
                m_vVelocity.x = m_vVelocity.x / fTotalTime * kReleaseSpeedScale;
                m_vVelocity.y = m_vVelocity.y / fTotalTime * kReleaseSpeedScale;
                m_vVelocity.z = m_vVelocity.z / fTotalTime * kReleaseSpeedScale;
                m_bInertia = true;
                m_fInertiaStartTime = m_fElapsedTime;
                const float fSpeed = sqrtf(m_vVelocity.x * m_vVelocity.x + m_vVelocity.y * m_vVelocity.y + m_vVelocity.z * m_vVelocity.z);
                const float fDuration = fSpeed * 0.5f / m_camera.m_mWorld._43 + 0.125f;
                m_fInertiaDuration = fDuration < 2.0f ? fDuration * 0.5f : 1.0f;
            }
            for (MoveSample& kSample : m_aMoveSamples) {
                kSample = MoveSample();
            }
            m_ucMoveSampleIndex = 0;
        }
    }

    bool bMoved = false;
    if (m_bInertia) {
        const float fT = (m_fElapsedTime - m_fInertiaStartTime) / m_fInertiaDuration;
        if (!(fT <= 1.0f)) {
            m_bInertia = false;
        } else {
            // Half-cosine ease-out: the speed factor falls from 1 to 0 over the inertia duration.
            const float fPhase = static_cast<float>(fT * kPi);
            const float fFactor = static_cast<float>(sin(fPhase + kHalfPi) * 0.5) + 0.5f;
            MoveAlongVector_WithSlide(m_vVelocity, fFactor * fDeltaTime);
            m_camera.m_mWorld._41 = m_vPosition.x;
            m_camera.m_mWorld._42 = m_vPosition.y;
            m_camera.m_mWorld._43 = m_vPosition.z;
            bMoved = true;
        }
    }

    const auto scrollStep = [this, fDeltaTime]() {
        FGXVector3 vLocation = { 0.0f, 0.0f, 0.0f };
        m_camera.GetWorldLocation(&vLocation);
        return fabsf(vLocation.z) * m_fScrollSpeed * fDeltaTime;
    };
    if (m_kMoveFlags.m_bLeft) {
        const FGXVector2 vStep = { -scrollStep(), 0.0f };
        MoveCameraSpaceXY(vStep);
        bMoved = true;
    }
    if (m_kMoveFlags.m_bRight) {
        const FGXVector2 vStep = { scrollStep(), 0.0f };
        MoveCameraSpaceXY(vStep);
        bMoved = true;
    }
    if (m_kMoveFlags.m_bForward) {
        const FGXVector2 vStep = { 0.0f, scrollStep() };
        MoveCameraSpaceXY(vStep);
        bMoved = true;
    }
    if (m_kMoveFlags.m_bBack) {
        const FGXVector2 vStep = { 0.0f, -scrollStep() };
        MoveCameraSpaceXY(vStep);
        bMoved = true;
    }
    if (m_kMoveFlags.m_bRotateCW) {
        RotateZ(kRotateSpeed * fDeltaTime);
        bMoved = true;
    }
    if (m_kMoveFlags.m_bRotateCCW) {
        RotateZ(-kRotateSpeed * fDeltaTime);
        bMoved = true;
    }
    bool bChanged = bMoved || m_kMoveFlags.m_bPitchUp || m_kMoveFlags.m_bPitchDown;
    RephaseCamera();

    // The camera eases toward m_vPosition, across the wrap seam by the shorter way.
    FGXVector3 vCurrent = { 0.0f, 0.0f, 0.0f };
    m_camera.GetRow(&vCurrent, 3);
    FGXVector3 vOffset;
    vOffset.x = vCurrent.x - m_vPosition.x;
    vOffset.y = vCurrent.y - m_vPosition.y;
    vOffset.z = vCurrent.z - m_vPosition.z;
    const float fDistance = sqrtf(vOffset.x * vOffset.x + vOffset.y * vOffset.y + vOffset.z * vOffset.z);
    if (fDistance > 1.0f) {
        FGXVector3 vTarget = m_vPosition;
        if (WorldGlobals::g_eWrapMode == 1) {
            const float fSpan = WorldGlobals::g_WorldMax.x - WorldGlobals::g_WorldMin.x;
            const float fRestSq = vOffset.y * vOffset.y + vOffset.z * vOffset.z;
            const float fEastX = vTarget.x + fSpan;
            const float fEastOffsetX = vCurrent.x - fEastX;
            if (fDistance > sqrtf(fEastOffsetX * fEastOffsetX + fRestSq)) {
                vOffset.x = fEastOffsetX;
                vTarget.x = fEastX;
            } else {
                const float fWestX = vTarget.x - fSpan;
                const float fWestOffsetX = vCurrent.x - fWestX;
                if (fDistance > sqrtf(fWestOffsetX * fWestOffsetX + fRestSq)) {
                    vOffset.x = fWestOffsetX;
                    vTarget.x = fWestX;
                }
            }
        }
        const float fLengthSq = vOffset.x * vOffset.x + vOffset.y * vOffset.y + vOffset.z * vOffset.z;
        const float fLength = sqrtf(fLengthSq);
        if (fLength > 1.0f) {
            float fTime = powf(fLength / kFollowScale, 0.25f) / kFollowScale - fDeltaTime;
            if (0.0f > fTime) {
                fTime = 0.0f;
            }
            const float fRemaining = fTime * fTime * fTime * fTime * kFollowScale;
            const float fNorm = sqrtf(fLengthSq);
            FGXVector3 vNew;
            vNew.x = vTarget.x + (vOffset.x / fNorm) * fRemaining;
            vNew.y = vTarget.y + (vOffset.y / fNorm) * fRemaining;
            vNew.z = vTarget.z + (vOffset.z / fNorm) * fRemaining;
            m_camera.SetPosition(&vNew);
            RephaseCamera();
            bChanged = true;
        }
    }

    bool bViewChanged;
    if (m_bViewDirty) {
        m_bViewDirty = false;
        bViewChanged = true;
    } else {
        bViewChanged = uiManager().field_ae8 ==kUIManagerModeAlwaysRefresh || bChanged;
    }
    if (bViewChanged) {
        D3DXMATRIX mView;
        D3DXMATRIX mProj;
        m_camera.GetViewProj(&mView, &mProj);
        dispatchEvent(g_EventSystemNonSerial, kNonSerialStride, kViewChangedSlot, &mView);
    }
    FrameGlobals::fMaxZoom = m_fMaxZoomLevel;
    FrameGlobals::fMinZoom = m_fMinZoomLevel;
    FrameGlobals::fCurrZoom = m_fZoomLevel;
    FrameGlobals::fCurrZoomNorm = (m_fZoomLevel - m_fMinZoomLevel) / (m_fMaxZoomLevel - m_fMinZoomLevel);
    if (bViewChanged != m_bMoving) {
        unsigned char abEvent[8];
        dispatchEvent(g_EventSystemLocalMachine, kLocalMachineStride, bViewChanged ? kCameraStartedMovingSlot : kCameraStoppedMovingSlot, abEvent);
        m_bMoving = bViewChanged;
    }
    m_vPrevPosition.x = m_camera.m_mWorld._41;
    m_vPrevPosition.y = m_camera.m_mWorld._42;
    m_vPrevPosition.z = m_camera.m_mWorld._43;
}

/// 0x088b1aba
void CvCamera::RephaseCamera() {
    if (WorldGlobals::g_eWrapMode != 1) {
        return;
    }
    const auto dispatchWrapped = [](int iSide) {
        UNK_DATA* pSystem = g_EventSystemLocalMachine;
        const unsigned int uOffset = *reinterpret_cast<int*>(pSystem) * kLocalMachineStride;
        PfnDispatchEvent pfnDispatch = *reinterpret_cast<PfnDispatchEvent*>(pSystem + uOffset + kWrapEventSlot);
        void* pContext = *reinterpret_cast<void**>(pSystem + uOffset + kWrapEventSlot + 4);
        pfnDispatch(&iSide, pContext);
    };
    FGXVector3 vPos = { 0.0f, 0.0f, 0.0f };
    m_camera.GetRow(&vPos, 3);
    const float fMinX = WorldGlobals::g_WorldMin.x;
    const float fSpan = WorldGlobals::g_WorldMax.x - WorldGlobals::g_WorldMin.x;
    if (WorldGlobals::g_WorldMax.x < vPos.x) {
        vPos.x = vPos.x - fSpan;
        m_vPosition.x = m_vPosition.x - fSpan;
        m_camera.SetPosition(&vPos);
        FGXVector3 vRow1 = { 0.0f, 0.0f, 0.0f };
        m_camera.GetRow(&vRow1, 1);
        float fFlip = static_cast<float>(m_iFlip);
        m_camera.m_mWorld._21 = vRow1.x * fFlip;
        m_camera.m_mWorld._22 = vRow1.y * fFlip;
        m_camera.m_mWorld._23 = fFlip * vRow1.z;
        FGXVector3 vRow0 = { 0.0f, 0.0f, 0.0f };
        m_camera.GetRow(&vRow0, 0);
        const int iFlip = m_iFlip;
        fFlip = static_cast<float>(iFlip);
        m_camera.m_mWorld._11 = vRow0.x * fFlip;
        m_camera.m_mWorld._12 = vRow0.y * fFlip;
        m_camera.m_mWorld._13 = fFlip * vRow0.z;
        m_iFlip = iFlip > 0 ? iFlip : -iFlip;
        dispatchWrapped(1);
    }
    if (vPos.x < fMinX) {
        vPos.x = vPos.x + fSpan;
        m_vPosition.x = fSpan + m_vPosition.x;
        m_camera.SetPosition(&vPos);
        FGXVector3 vRow1 = { 0.0f, 0.0f, 0.0f };
        m_camera.GetRow(&vRow1, 1);
        float fFlip = static_cast<float>(m_iFlip);
        m_camera.m_mWorld._21 = vRow1.x * fFlip;
        m_camera.m_mWorld._22 = vRow1.y * fFlip;
        m_camera.m_mWorld._23 = fFlip * vRow1.z;
        FGXVector3 vRow0 = { 0.0f, 0.0f, 0.0f };
        m_camera.GetRow(&vRow0, 0);
        const int iFlip = m_iFlip;
        fFlip = static_cast<float>(iFlip);
        m_camera.m_mWorld._11 = vRow0.x * fFlip;
        m_camera.m_mWorld._12 = vRow0.y * fFlip;
        m_camera.m_mWorld._13 = fFlip * vRow0.z;
        m_iFlip = iFlip > 0 ? iFlip : -iFlip;
        dispatchWrapped(0);
    }
}

/// 0x088b1e4a
UNK_RET CvCamera::OnDragCamera(EventTemplate<Event_Int2Type<100>, DragCameraData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const DragCameraData& kDrag = reinterpret_cast<const DragCameraData&>(kEvent);
    const auto isStrategicView = []() {
        const unsigned char* pApplication = *reinterpret_cast<const unsigned char* const*>(g_pkApplication);
        if (pApplication == nullptr) {
            return false;
        }
        const GameViewState* pView = *reinterpret_cast<const GameViewState* const*>(pApplication + kApplicationView);
        if (pView == nullptr) {
            return false;
        }
        return pView->m_eViewType == GameViewState::VIEWTYPE_STRATEGIC;
    };
    FGXVector3 vNew = { 0.0f, 0.0f, 0.0f };
    if (isStrategicView()) {
        const cvStrategicViewVisSystem& kStrategicView = *reinterpret_cast<const cvStrategicViewVisSystem*>(g_cvStrategicViewVisSystem);
        FGXVector3 vFromWorld = { 0.0f, 0.0f, 0.0f };
        FGXVector3 vToWorld = { 0.0f, 0.0f, 0.0f };
        kStrategicView.GetWorldSpace(kDrag.m_vTo, vToWorld);
        kStrategicView.GetWorldSpace(kDrag.m_vFrom, vFromWorld);
        vNew.x = (vFromWorld.x - vToWorld.x) + m_camera.m_mWorld._41;
        vNew.y = (vFromWorld.y - vToWorld.y) + m_camera.m_mWorld._42;
        vNew.z = m_camera.m_mWorld._43;
    } else if (field_108) {
        // Both screen points are picked on a horizontal plane; the camera moves by their difference.
        D3DXVECTOR3 avHits[2] = { { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f } };
        const FGXVector2 avScreen[2] = { kDrag.m_vTo, kDrag.m_vFrom };
        D3DXMATRIX mView;
        D3DXMATRIX mProj;
        m_camera.GetViewProj(&mView, &mProj);
        for (int i = 0; i < 2; ++i) {
            const D3DXVECTOR3 vNearScreen = { avScreen[i].x, avScreen[i].y, 0.0f };
            const D3DXVECTOR3 vFarScreen = { avScreen[i].x, avScreen[i].y, 1.0f };
            D3DXVECTOR3 vFar = { 0.0f, 0.0f, 0.0f };
            D3DXVECTOR3 vNear = { 0.0f, 0.0f, 0.0f };
            D3DXVec3Unproject(&vFar, &vFarScreen, nullptr, &mProj, &mView, nullptr);
            D3DXVec3Unproject(&vNear, &vNearScreen, nullptr, &mProj, &mView, nullptr);
            const D3DXPLANE kPlane = { 0.0f, 0.0f, 1.0f, MapGlobals::g_TerrainSampler.m_fHeightScale * kDragPlaneD };
            D3DXPlaneIntersectLine(&avHits[i], &kPlane, &vFar, &vNear);
        }
        const float fDragSpeed = *reinterpret_cast<const float*>(reinterpret_cast<const unsigned char*>(cvOptionsManager::g_GameOptionsCache) + kOptionDragSpeed);
        vNew.x = (avHits[1].x - avHits[0].x) * fDragSpeed + m_camera.m_mWorld._41;
        vNew.y = (avHits[1].y - avHits[0].y) * fDragSpeed + m_camera.m_mWorld._42;
        vNew.z = m_camera.m_mWorld._43;
    }

    FGXVector3 vForward = { 0.0f, 0.0f, 0.0f };
    m_camera.GetRow(&vForward, 2);
    float fSlope = 0.0f;
    if (vForward.z != 0.0f) {
        fSlope = -vForward.y / vForward.z;
    }
    const float fOffsetY = (vNew.z - WorldGlobals::g_WorldMin.z) * fSlope;
    if (vNew.y > WorldGlobals::g_WorldMax.y - fOffsetY) {
        vNew.y = (WorldGlobals::g_WorldMax.y - fOffsetY) - kDragBoundsInset;
    } else if (WorldGlobals::g_WorldMin.y - fOffsetY > vNew.y) {
        vNew.y = (WorldGlobals::g_WorldMin.y - fOffsetY) + kDragBoundsInset;
    }
    float fWrappedX = vNew.x;
    if (WorldGlobals::g_eWrapMode == 1) {
        while (WorldGlobals::g_WorldMin.x > fWrappedX) {
            fWrappedX += WorldGlobals::g_WorldSize.x;
        }
        while (fWrappedX >= WorldGlobals::g_WorldMax.x) {
            fWrappedX -= WorldGlobals::g_WorldSize.x;
        }
    }
    const float fRowY = fOffsetY + vNew.y;
    if (m_fBoundsMinX > fWrappedX) {
        vNew.x = m_fBoundsMinX + kDragBoundsInset;
    } else if (fWrappedX > m_fBoundsMaxX) {
        vNew.x = m_fBoundsMaxX - kDragBoundsInset;
    }
    if (m_fBoundsMinY > fRowY) {
        vNew.y = (m_fBoundsMinY + kDragBoundsInset) - fOffsetY;
    } else if (fRowY > m_fBoundsMaxY) {
        vNew.y = (m_fBoundsMaxY - kDragBoundsInset) - fOffsetY;
    }
    if (PositionIsWithinBounds(vNew)) {
        m_vPosition = vNew;
        m_camera.m_mWorld._41 = vNew.x;
        m_camera.m_mWorld._42 = vNew.y;
    }
    m_bViewDirty = true;
    m_bInertia = false;
}

/// 0x088b267c
void CvCamera::OnCameraIn(GameCoreEventTemplate<Event_Int2Type<24>, CameraZoomData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const CameraZoomData& kZoom = *reinterpret_cast<const CameraZoomData*>(reinterpret_cast<const unsigned char*>(&kEvent) + kSerialEventPayload);
    const auto pickRay = [this](float fX, float fY) {
        D3DXMATRIX mView;
        D3DXMATRIX mProj;
        m_camera.GetViewProj(&mView, &mProj);
        D3DXVECTOR3 vNearScreen = { fX, fY, 0.0f };
        D3DXVECTOR3 vFarScreen = { fX, fY, 1.0f };
        D3DXVECTOR3 vNear = { 0.0f, 0.0f, 0.0f };
        D3DXVECTOR3 vFar = { 0.0f, 0.0f, 0.0f };
        D3DXVec3Unproject(&vNear, &vNearScreen, nullptr, &mProj, &mView, nullptr);
        D3DXVec3Unproject(&vFar, &vFarScreen, nullptr, &mProj, &mView, nullptr);
        FGXVector3 vRay;
        vRay.x = vFar.x - vNear.x;
        vRay.y = vFar.y - vNear.y;
        vRay.z = vFar.z - vNear.z;
        const float fLength = sqrtf(vRay.z * vRay.z + vRay.y * vRay.y + vRay.x * vRay.x);
        vRay.x = vRay.x / fLength;
        vRay.y = vRay.y / fLength;
        vRay.z = vRay.z / fLength;
        return vRay;
    };
    FGXVector3 vDir = pickRay(kZoom.m_vScreenPos.x, kZoom.m_vScreenPos.y);
    if (!ZoomAlongVector(vDir, -kZoom.m_fAmount)) {
        m_camera.GetRow(&vDir, 2);
        ZoomAlongVector(vDir, -kZoom.m_fAmount);
    }
}

/// 0x088b289a
void CvCamera::OnCameraOut(GameCoreEventTemplate<Event_Int2Type<25>, CameraZoomData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    const CameraZoomData& kZoom = *reinterpret_cast<const CameraZoomData*>(reinterpret_cast<const unsigned char*>(&kEvent) + kSerialEventPayload);
    const auto pickRay = [this](float fX, float fY) {
        D3DXMATRIX mView;
        D3DXMATRIX mProj;
        m_camera.GetViewProj(&mView, &mProj);
        D3DXVECTOR3 vNearScreen = { fX, fY, 0.0f };
        D3DXVECTOR3 vFarScreen = { fX, fY, 1.0f };
        D3DXVECTOR3 vNear = { 0.0f, 0.0f, 0.0f };
        D3DXVECTOR3 vFar = { 0.0f, 0.0f, 0.0f };
        D3DXVec3Unproject(&vNear, &vNearScreen, nullptr, &mProj, &mView, nullptr);
        D3DXVec3Unproject(&vFar, &vFarScreen, nullptr, &mProj, &mView, nullptr);
        FGXVector3 vRay;
        vRay.x = vFar.x - vNear.x;
        vRay.y = vFar.y - vNear.y;
        vRay.z = vFar.z - vNear.z;
        const float fLength = sqrtf(vRay.z * vRay.z + vRay.y * vRay.y + vRay.x * vRay.x);
        vRay.x = vRay.x / fLength;
        vRay.y = vRay.y / fLength;
        vRay.z = vRay.z / fLength;
        return vRay;
    };
    const bool bAlongViewAxis = reinterpret_cast<const unsigned char*>(cvOptionsManager::g_GameOptionsStore)[kOptionZoomAlongViewAxis] != 0;
    FGXVector3 vDir = { 0.0f, 0.0f, 0.0f };
    if (bAlongViewAxis) {
        m_camera.GetRow(&vDir, 2);
    } else {
        vDir = pickRay(kZoom.m_vScreenPos.x, kZoom.m_vScreenPos.y);
    }
    const bool bMoved = ZoomAlongVector(vDir, kZoom.m_fAmount);
    if (!bAlongViewAxis && !bMoved) {
        m_camera.GetRow(&vDir, 2);
        ZoomAlongVector(vDir, kZoom.m_fAmount);
    }
}

/// 0x088b2e8e
UNK_RET CvCamera::OnCameraSetLookatAndZoom(GameCoreEventTemplate<Event_Int2Type<130>, FGXVector3, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent) {
    // x, y: the ground point to look at; z: the camera height, from which the zoom level follows.
    const FGXVector3& vLookAt = *reinterpret_cast<const FGXVector3*>(reinterpret_cast<const unsigned char*>(&kEvent) + kSerialEventPayload);
    m_kMoveFlags = MoveFlags();
    m_bInertia = false;
    FGXVector3 vForward = { 0.0f, 0.0f, 0.0f };
    m_camera.GetRow(&vForward, 2);
    FGXVector3 vCamera;
    vCamera.z = vLookAt.z;
    vCamera.x = (vForward.x / vForward.z) * vLookAt.z + vLookAt.x;
    vCamera.y = (vForward.y / vForward.z) * vLookAt.z + vLookAt.y;
    MoveAbsoluteWorldSpaceXYZ(vCamera);
}

/// 0x088bb128
void cvDistanceFog::Startup(bool bReload) {
    m_fHeightFalloff = 0.05f;
    m_fDensity = 1.0e-4f;
    m_bLoaded = false;
    m_bDensityEnabled = true;

    char szColorFile[260] = "assets/DistanceFog/FogColor.csv";
    FCSVFile csv(true, true, false);
    csv.Load(szColorFile);
    bool bR = csv.GetFloat(0, 0, m_afColor[0]);
    bool bG = csv.GetFloat(0, 1, m_afColor[1]);
    bool bB = csv.GetFloat(0, 2, m_afColor[2]);
    // Channels are stored as 0..255 in the file.
    m_afColor[0] /= 255.0f;
    m_afColor[1] /= 255.0f;
    m_afColor[2] /= 255.0f;
    if (!(bR && bG && bB)) {
        char szMessage[300];
        sprintf_s(szMessage, sizeof(szMessage), "%s did not load properly.", szColorFile);
        MessageBoxA(nullptr, szMessage, "Distance Fog Error", MB_ICONEXCLAMATION);
    }

    char szRampFile[260] = "assets/DistanceFog/FogRamp.dds";
    if (bReload) {
        FireGrafix::g_pRenderer->ReloadTexture(szRampFile, m_uRampHandle);
    } else {
        m_uRampHandle = FireGrafix::g_pRenderer->LoadTexture(szRampFile);
    }
    RenderGlobals::g_TexHandles[4] = m_uRampHandle;
    m_bLoaded = true;
}

/// 0x088bb662
void cvDistanceFog::Shutdown() {
    Fun_08672408(FireGrafix::CRenderer9::m_TextureList, m_uRampHandle);
    m_uRampHandle = 0;
    m_bLoaded = false;
}