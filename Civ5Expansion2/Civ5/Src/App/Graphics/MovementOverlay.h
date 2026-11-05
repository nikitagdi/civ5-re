/// origin guess
#pragma once
#include <Windows.h>

class LinearEventDispatcher;
class LocalMachineContainer;
class MapGlobalsData;
class NullEventClass;
class UnitHexHighlightData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class CCommandStream;
class DrawSimpleOverlay;
}

// The unit selection ring: one textured quad around the selected hex, loaded from Civ5ArtDefines_Overlay.xml UnitSelectionHighlight.
/// size 0x5d0
class MovementOverlay {
public:
    FireGrafix::DrawSimpleOverlay* m_pShader;  ///< +0x0
    unsigned int m_hVertexBuffer;              ///< +0x4
    unsigned int m_hIndexBuffer;               ///< +0x8
    unsigned int m_hSurface;                   ///< +0xc
    int m_iHexX;                               ///< +0x10
    int m_iHexY;                               ///< +0x14
    int m_iHexZ;                               ///< +0x18
    float m_fOffsetX;                          ///< +0x1c
    float m_fOffsetY;                          ///< +0x20
    float m_fOffsetZ;                          ///< +0x24
    bool m_bVisible;                           ///< +0x28
    bool m_bUseOffset;                         ///< +0x29
    int m_iOwner;                              ///< +0x2c
    float m_fPulse;                            ///< +0x30
    float m_fPulseSpeed;                       ///< +0x34
    bool m_bPulseEnabled;                      ///< +0x38
    bool m_bPulseFalling;                      ///< +0x39
    float m_fAngle;                            ///< +0x3c
    float m_fRotationSpeed;                    ///< +0x40
    bool m_bRotationEnabled;                   ///< +0x44
    bool m_bClockwise;                         ///< +0x45
    bool m_bField46;                           ///< +0x46
    unsigned int m_kFileWatch[0x10d];          ///< +0x48
    float m_fColorR;                           ///< +0x47c
    float m_fColorG;                           ///< +0x480
    float m_fColorB;                           ///< +0x484
    float m_fColorA;                           ///< +0x488
    D3DXMATRIX m_mHexToOverlay;                ///< +0x48c
    D3DXMATRIX m_mRing;                        ///< +0x4cc
    D3DXMATRIX m_mGlow;                        ///< +0x50c
    char m_kMapGlobals[0x84];                  ///< +0x54c

    /// 0x08a58a44
    MovementOverlay();
    /// 0x08a58b00
    ~MovementOverlay();
    /// 0x08a58b24
    UNK_RET Startup();
    /// 0x08a58f5a
    UNK_RET SubscribeToEvents();
    /// 0x08a58fb8
    UNK_RET LoadFromXML();
    /// 0x08a595b0
    UNK_RET Shutdown();
    /// 0x08a59670
    UNK_RET UnSubscribeToEvents();
    /// 0x08a596ce
    UNK_RET StartupGame();
    /// 0x08a596d0
    UNK_RET ShutdownGame();
    /// 0x08a596e4
    UNK_RET Render(FireGrafix::CCommandStream* a1, int a2);
    /// 0x08a598c6
    UNK_RET Update(float a1);
    /// 0x08a59b5a
    UNK_RET OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08a59cba
    UNK_RET OnHexHighlight(EventTemplate<Event_Int2Type<11>, UnitHexHighlightData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
};
