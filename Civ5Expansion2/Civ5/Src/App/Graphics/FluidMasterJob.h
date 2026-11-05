/// origin guess
#pragma once
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireGrafix/FGXInt2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "Civ5/Src/App/HEXTransform_i.h"
#include <Windows.h>
#include <d3d9.h>

class FrameGlobalsData;
class HexFOWStateChangedData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class WorldGlobalsData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
namespace FireGrafix {
class CCommandStream;
class FOWCell;
}
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fd82c
class FluidMasterJob : public FJob {
public:
    /// 0x088731d8
    virtual void Execute(unsigned int a1);
};

// The low-detail fog (GeneralSettings::g_uFOWLevel == 0): opaque grey hex prisms over the hexes whose grid state is 2.
/// size 0xa6c
class FOWSystem {
public:
    /// 0x08873204
    UNK_RET Startup();
    /// 0x08873892
    UNK_RET OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088738c6
    UNK_RET OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088738fc
    UNK_RET OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088739de
    UNK_RET Shutdown();
    /// 0x08873b18
    UNK_RET StartupGame();
    /// 0x08873c5c
    UNK_RET ShutdownGame();
    /// 0x08873c6c
    UNK_RET DrawBlackHex(FGXInt2& kCell, unsigned int a2, D3DXMATRIX mViewProj);
    /// 0x08873e08
    void DrawFogCell(FGXInt2& kCell, unsigned int a2, D3DXMATRIX mViewProj, unsigned int uState);
    /// 0x088740c8
    void Render(unsigned int hRenderTarget);

    int field_0;                                           ///< +0x0
    FireGrafix::CCommandStream* m_pCommandStream;          ///< +0x4
    FireGrafix::FOWCell* m_pShader;                        ///< +0x8
    unsigned int m_hVB;                                    ///< +0xc
    unsigned int m_hIB;                                    ///< +0x10
    // Hex prism surfaces over m_hIB (top and six walls); each Open* one omits a wall and is used when that x neighbour is set.
    unsigned int m_hHexSurface;                            ///< +0x14
    unsigned int m_hHexSurfaceOpenNext;                    ///< +0x18
    unsigned int m_hHexSurfaceOpenPrev;                    ///< +0x1c
    unsigned int m_hHexSurfaceOpenBoth;                    ///< +0x20
    // +0x24..+0x870 is the FrameGlobalsData copy taken by OnFrameGlobalsUpdated.
    char unk_24[0x10];                                     ///< +0x24
    D3DXMATRIX m_mProj;                                    ///< +0x34
    D3DXMATRIX m_amView[4];                                ///< +0x74
    char unk_174[0x640];                                   ///< +0x174
    FGXInt2 m_akGameGridTopLeftVisible[4];                 ///< +0x7b4
    FGXInt2 m_akGameGridBottomRightVisible[4];             ///< +0x7d4
    FGXInt2 m_akGameGridTopLeftVisibleUnclamped[4];        ///< +0x7f4
    FGXInt2 m_akGameGridBottomRightVisibleUnclamped[4];    ///< +0x814
    unsigned int m_nCameras;                               ///< +0x834
    char unk_838[0x38];                                    ///< +0x838
    // +0x870..+0x9bc is the WorldGlobalsData copy taken by OnWorldGlobalsChanged.
    char unk_870[0x40];                                    ///< +0x870
    D3DXMATRIX m_mHexToTex;                                ///< +0x8b0
    FGXVector3 m_vWorldMin;                                ///< +0x8f0
    char unk_8fc[0x18];                                    ///< +0x8fc
    FGXVector3 m_vTerrainScale;                            ///< +0x914
    HEXMatrix_i m_kGameGridMatrix;                         ///< +0x920
    HEXMatrix_i m_kGameGridInvMatrix;                      ///< +0x960
    HEXTransform_i m_kGameGridTransform;                   ///< +0x9a0
    int m_iGameGridWidth;                                  ///< +0x9b4
    int m_iGameGridHeight;                                 ///< +0x9b8
    int m_bGameStarted;                                    ///< +0x9bc
    int field_9c0;                                         ///< +0x9c0
    HEXMatrix_i m_kFOWGridMatrix;                          ///< +0x9c4
    HEXMatrix_i m_kFOWGridInvMatrix;                       ///< +0xa04
    HEXTransform_i m_kFOWGridTransform;                    ///< +0xa44
    int m_iFOWGridWidth;                                   ///< +0xa58
    int m_iFOWGridHeight;                                  ///< +0xa5c
    // Column-major m_piFOWGrid[x * m_iFOWGridHeight + y]: 2, 1 or 0 from event states 0, 1 and other (OnFOWChanged).
    int* m_piFOWGrid;                                      ///< +0xa60
    unsigned int m_nFOWGrid;                               ///< +0xa64
    unsigned int field_a68;                                ///< +0xa68
};
