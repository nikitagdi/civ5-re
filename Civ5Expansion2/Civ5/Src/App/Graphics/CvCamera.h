/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FCamera.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include <d3d9.h>

class ActivePlayerData;
class CameraCenterData;
class CameraZoomData;
class DragCameraData;
class FGXVector4;
class HexFOWStateChangedData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size 0x164
class CvCamera {
public:
    class StateData;
    /// size 0x10
    class MoveSample {
    public:
        FGXVector3 m_vDelta;  ///< +0x0
        float m_fDeltaTime;   ///< +0xc
    };
    /// size 0x1
    class MoveFlags {
    public:
        bool m_bLeft : 1;        ///< +0x0
        bool m_bRight : 1;       ///< +0x0
        bool m_bForward : 1;     ///< +0x0
        bool m_bBack : 1;        ///< +0x0
        bool m_bRotateCW : 1;    ///< +0x0
        bool m_bRotateCCW : 1;   ///< +0x0
        bool m_bPitchUp : 1;     ///< +0x0
        bool m_bPitchDown : 1;   ///< +0x0
    };
    /// 0x088af9e8
    CvCamera();
    /// 0x088afb62
    UNK_RET Reset();
    /// 0x088afc16
    UNK_RET SetScrollSpeed(float a1);
    /// 0x088afc2a
    UNK_RET SetZoomSpeed(float a1);
    /// 0x088afc3e
    UNK_RET SetMinZoomHeight(float a1);
    /// 0x088afcaa
    void RecalculateCurrentZoomLevel();
    /// 0x088afcfa
    UNK_RET SetMinZoomLevel(float a1);
    /// 0x088afd16
    UNK_RET SetMaxZoomLevel(float a1);
    /// 0x088afd32
    UNK_RET PitchUp();
    /// 0x088afd5e
    UNK_RET PitchDown();
    /// 0x088afd8a
    UNK_RET SetWorldLocation(FGXVector3 const& a1);
    /// 0x088afe22
    int PositionIsWithinBounds(FGXVector3 const& a1);
    /// 0x088aff46
    UNK_RET SetWorldLocation(FGXVector2 const& a1);
    /// 0x088affd2
    UNK_RET MoveWorldSpace(FGXVector3 const& a1);
    /// 0x088b0088
    UNK_RET MoveAbsoluteWorldSpaceXY(FGXVector2 const& a1);
    /// 0x088b0142
    UNK_RET MoveAbsoluteWorldSpaceXYZ(FGXVector3 const& a1);
    /// 0x088b01bc
    UNK_RET MoveWorldSpaceX(float a1);
    /// 0x088b0264
    UNK_RET MoveWorldSpaceY(float a1);
    /// 0x088b030c
    UNK_RET MoveWorldSpaceZ(float a1);
    /// 0x088b03b6
    UNK_RET MoveCameraSpaceInstantly(FGXVector3 const& vDelta);
    /// 0x088b06e4
    void MoveCameraSpaceXY(FGXVector2 const& vDelta);
    /// 0x088b0936
    bool MoveAlongVector(FGXVector3 const& a1, float a2);
    /// 0x088b0a06
    UNK_RET MoveAlongVector_WithSlide(FGXVector3 const& a1, float a2);
    /// 0x088b0ab8
    void ClipPositionToBounds(FGXVector3& vPos);
    /// 0x088b0c04
    bool ZoomAlongVector(FGXVector3 const& vDirection, float fZoomDelta);
    /// 0x088b0cda
    float ClipZoomToBounds(float fZoomDelta) const;
    /// 0x088b0d64
    UNK_RET RotateZ(float a1);
    /// 0x088b0f76
    UNK_RET RotateX(float a1);
    /// 0x088b0f78
    UNK_RET Update(float fDeltaTime);
    /// 0x088b1aba
    void RephaseCamera();
    /// 0x088b1e16
    UNK_RET GetViewProj(D3DXMATRIX* a1, D3DXMATRIX* a2);
    /// 0x088b1e4a
    UNK_RET OnDragCamera(EventTemplate<Event_Int2Type<100>, DragCameraData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x088b23b4
    UNK_RET OnCameraForward(GameCoreEventTemplate<Event_Int2Type<4>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2436
    UNK_RET OnCameraBack(GameCoreEventTemplate<Event_Int2Type<7>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b24b8
    UNK_RET OnCameraLeft(GameCoreEventTemplate<Event_Int2Type<10>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b253a
    UNK_RET OnCameraRight(GameCoreEventTemplate<Event_Int2Type<13>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b25bc
    UNK_RET OnCameraStartMovingForward(GameCoreEventTemplate<Event_Int2Type<5>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b25c8
    UNK_RET OnCameraStopMovingForward(GameCoreEventTemplate<Event_Int2Type<6>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b25d4
    UNK_RET OnCameraStartMovingBack(GameCoreEventTemplate<Event_Int2Type<8>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b25e0
    UNK_RET OnCameraStopMovingBack(GameCoreEventTemplate<Event_Int2Type<9>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b25ec
    UNK_RET OnCameraStartMovingLeft(GameCoreEventTemplate<Event_Int2Type<11>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b25f8
    UNK_RET OnCameraStopMovingLeft(GameCoreEventTemplate<Event_Int2Type<12>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2604
    UNK_RET OnCameraStartMovingRight(GameCoreEventTemplate<Event_Int2Type<14>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2610
    UNK_RET OnCameraStopMovingRight(GameCoreEventTemplate<Event_Int2Type<15>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b261c
    UNK_RET OnCameraStartRotatingCW(GameCoreEventTemplate<Event_Int2Type<16>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2628
    UNK_RET OnCameraStopRotatingCW(GameCoreEventTemplate<Event_Int2Type<17>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2634
    UNK_RET OnCameraStartRotatingCCW(GameCoreEventTemplate<Event_Int2Type<18>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2640
    UNK_RET OnCameraStopRotatingCCW(GameCoreEventTemplate<Event_Int2Type<19>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b264c
    UNK_RET OnCameraStartPitchingUp(GameCoreEventTemplate<Event_Int2Type<20>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2658
    UNK_RET OnCameraStopPitchingUp(GameCoreEventTemplate<Event_Int2Type<21>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2664
    UNK_RET OnCameraStartPitchingDown(GameCoreEventTemplate<Event_Int2Type<22>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2670
    UNK_RET OnCameraStopPitchingDown(GameCoreEventTemplate<Event_Int2Type<23>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b267c
    void OnCameraIn(GameCoreEventTemplate<Event_Int2Type<24>, CameraZoomData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x088b289a
    void OnCameraOut(GameCoreEventTemplate<Event_Int2Type<25>, CameraZoomData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x088b2b1c
    UNK_RET OnCameraCenter(GameCoreEventTemplate<Event_Int2Type<26>, CameraCenterData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b2c36
    void Pitch(float fAngle);
    /// 0x088b2c66
    UNK_RET GetWorldLocation(FGXVector3* a1);
    /// 0x088b2c92
    UNK_RET GetLookAt(FGXVector3* a1) const;
    /// 0x088b2cc6
    UNK_RET GetClipPlanesInWorldSpace(FGXVector4* a1);
    /// 0x088b2cf2
    UNK_RET SetViewAngle(float a1, float a2);
    /// 0x088b2d2e
    UNK_RET SetViewRange(float a1, float a2);
    /// 0x088b2d6a
    UNK_RET GetFocusPoint(FGXVector3* a1) const;
    /// 0x088b2e3a
    UNK_RET GetNormalizedZoom() const;
    /// 0x088b2e8e
    UNK_RET OnCameraSetLookatAndZoom(GameCoreEventTemplate<Event_Int2Type<130>, FGXVector3, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x088b2f44
    UNK_RET OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b3138
    UNK_RET SaveState(CvCamera::StateData* a1);
    /// 0x088b3176
    UNK_RET RestoreState(CvCamera::StateData const& a1);
    /// 0x088b31ae
    UNK_RET BeginActivePlayerChange(GameCoreEventTemplate<Event_Int2Type<131>, ActivePlayerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b3226
    UNK_RET EndActivePlayerChange(GameCoreEventTemplate<Event_Int2Type<131>, ActivePlayerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088b3280
    UNK_RET SubscribeToEvents();
    /// 0x088b3586
    UNK_RET UnSubscribeToEvents();
    unsigned int field_0;             ///< +0x0
    FCamera m_camera;                 ///< +0x4
    FGXVector3 m_vPosition;           ///< +0xdc
    float m_fScrollSpeed;             ///< +0xe8
    float m_fZoomSpeed;               ///< +0xec
    float m_fMinZoomHeight;           ///< +0xf0
    float m_fMinZoomLevel;            ///< +0xf4
    float m_fMaxZoomLevel;            ///< +0xf8
    float m_fZoomLevel;               ///< +0xfc
    float m_fElapsedTime;             ///< +0x100
    MoveFlags m_kMoveFlags;           ///< +0x104
    bool m_bInertia : 1;              ///< +0x105
    bool m_bMoving;                   ///< +0x106
    bool m_bViewDirty;                ///< +0x107
    bool field_108;                   ///< +0x108
    int m_iFlip;                      ///< +0x10c
    float m_fBoundsMinX;              ///< +0x110
    float m_fBoundsMaxX;              ///< +0x114
    float m_fBoundsMinY;              ///< +0x118
    float m_fBoundsMaxY;              ///< +0x11c
    MoveSample m_aMoveSamples[2];     ///< +0x120
    FGXVector3 m_vVelocity;           ///< +0x140
    FGXVector3 m_vPrevPosition;       ///< +0x14c
    float m_fInertiaStartTime;        ///< +0x158
    float m_fInertiaDuration;         ///< +0x15c
    unsigned char m_ucMoveSampleIndex;  ///< +0x160
};

// g_v4FogControl = (m_fHeightFalloff, m_bDensityEnabled ? m_fDensity : 0, 0, fCurrZoomNorm), written at 0x08840db1.
/// size 0x1c
class cvDistanceFog {
public:
    // bReload recreates the ramp texture in place under the existing handle.
    /// 0x088bb128
    void Startup(bool bReload);
    /// 0x088bb662
    void Shutdown();

    unsigned int m_uRampHandle;  ///< +0x0
    bool m_bLoaded;              ///< +0x4
    bool m_bDensityEnabled;               ///< +0x5
    char unk_06[0x2];            ///< +0x6
    float m_fHeightFalloff;              ///< +0x8
    float m_fDensity;              ///< +0xc
    float m_afColor[3];          ///< +0x10
};
