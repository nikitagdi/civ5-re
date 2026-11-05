/// origin guess
#pragma once
#include "Civ5/Src/App/HEXMatrix_i.h"
#include "Civ5/Src/App/HEXTransform_i.h"

class CityFogData;
class FXmlElement;
class FGXVector2;
class FrameGlobalsData;
class HexFOWStateChangedData;
class LinearEventDispatcher;
class LocalMachineContainer;
class LuaContext;
class NullEventClass;
class SerialContainer;
class WorldGlobalsData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class CCommandStream;
class FOWBaseLayer;
class FOWDrawMask;
class TextureData;
}

// One fog grid over the hex map: transform, its matrices and a flat cell array (row-major, m_iWidth per row).
/// size 0xa8
class FluidFOWGrid {
public:
    HEXMatrix_i m_mxForward;         ///< +0x0
    HEXMatrix_i m_mxInverse;         ///< +0x40
    HEXTransform_i m_kTransform;     ///< +0x80
    int m_iHeight;                   ///< +0x94
    int m_iWidth;                    ///< +0x98
    int* m_piCells;                  ///< +0x9c
    unsigned int m_nCells;           ///< +0xa0
    unsigned int m_nCellsCapacity;   ///< +0xa4
};

/// size 0x1548
class FluidFOWSystem {
public:
    /// 0x08867e4c
    FluidFOWSystem();
    /// 0x08868154
    ~FluidFOWSystem();
    /// 0x088681a2
    UNK_RET Startup();
    /// 0x0886909c
    UNK_RET OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088690d2
    UNK_RET OnWorldGlobalsChanged(EventTemplate<Event_Int2Type<36>, WorldGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08869108
    UNK_RET OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088691f4
    UNK_RET OnCityFogEvent(EventTemplate<Event_Int2Type<105>, CityFogData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0886929c
    UNK_RET Shutdown();
    /// 0x088694d4
    UNK_RET RegisterDebugMenu(LuaContext* a1);
    /// 0x0886984e
    UNK_RET IterationsButton(unsigned int a1, unsigned int a2);
    /// 0x0886988c
    UNK_RET PaddingButton(unsigned int a1, unsigned int a2);
    /// 0x088698ca
    UNK_RET DensityButton(unsigned int a1, unsigned int a2);
    /// 0x08869906
    UNK_RET SpeedButton(unsigned int a1, unsigned int a2);
    /// 0x08869942
    UNK_RET DragButton(unsigned int a1, unsigned int a2);
    /// 0x0886997e
    UNK_RET MasterSizeButton(unsigned int a1, unsigned int a2);
    /// 0x088699ba
    UNK_RET ForceStrengthButton(unsigned int a1, unsigned int a2);
    /// 0x088699f6
    UNK_RET MaxRotateButton(unsigned int a1, unsigned int a2);
    /// 0x08869a32
    UNK_RET ReloadFogTexture(unsigned int a1, unsigned int a2);
    /// 0x08869a94
    UNK_RET UpdateSettings();
    /// 0x08869e24
    UNK_RET StartupGame();
    /// 0x0886a9d6
    UNK_RET ShutdownGame();
    /// 0x0886aa0e
    UNK_RET LaunchJobs();
    /// 0x0886aa78
    UNK_RET RenderFOWMask(bool bClear);
    /// 0x0886b1c0
    UNK_RET RenderFOWDepthCullSurface(unsigned int hRenderTarget, float fHeight, FGXVector2& a3);
    /// 0x0886b784
    UNK_RET Update(float fDeltaSeconds);
    /// 0x0886ba88
    UNK_RET RenderFogLayer(unsigned int hSurface, float fHeight, FGXVector2& vWorldMin, float fScrollScale);
    /// 0x0886c152
    UNK_RET Render(unsigned int hRenderTarget);

    int field_0;                                   ///< +0x0
    FireGrafix::CCommandStream* m_pCommandStream;  ///< +0x4
    // FluidParticles (constructor 0x08867e4c); its layout is not recovered, it ends with an FJobSet at +0x918.
    unsigned char unk_8[0x91c];                    ///< +0x8
    FireGrafix::FOWDrawMask* m_pMaskShader;        ///< +0x924
    FireGrafix::FOWBaseLayer* m_pBaseLayerShader;  ///< +0x928
    unsigned int m_hParticleTexture;               ///< +0x92c
    unsigned int m_hCloudTexture03;                ///< +0x930
    unsigned int m_hCloudTexture01;                ///< +0x934
    unsigned int m_hHexVB;                         ///< +0x938
    unsigned int m_hQuadVB;                        ///< +0x93c
    unsigned int m_hHexIB;                         ///< +0x940
    unsigned int m_hQuadIB;                        ///< +0x944
    // Mask hexes with a soft rim out to 1.33 hex radius: visible and revealed plots.
    unsigned int m_hMaskHexSurface;                ///< +0x948
    unsigned int m_hFogLayer0Surface;              ///< +0x94c
    // Mask hexes with a soft rim out to 1.4 hex radius: unrevealed plots.
    unsigned int m_hMaskWideHexSurface;            ///< +0x950
    unsigned int m_hFogLayer1Surface;              ///< +0x954
    unsigned int m_hFogLayer2Surface;              ///< +0x958
    unsigned int m_hCullSurface;                   ///< +0x95c
    unsigned char unk_960[0x97c];                  ///< +0x960
    int m_aiGridTransform[3];                      ///< +0x12dc
    unsigned char unk_12e8[0x8];                   ///< +0x12e8
    int m_iGridHeight;                             ///< +0x12f0
    int m_iGridWidth;                              ///< +0x12f4
    int m_bGameStarted;                            ///< +0x12f8
    int m_bEnabled;                                ///< +0x12fc
    int m_bParticlesActive;                        ///< +0x1300
    // Set by OnCityFogEvent: type 0 clears m_aGrids[1] and enables, type 2 disables.
    int m_bCityFogActive;                          ///< +0x1304
    float m_fDensity;                              ///< +0x1308
    float m_fSpeed;                                ///< +0x130c
    float m_fDrag;                                 ///< +0x1310
    float m_fMasterSize;                           ///< +0x1314
    float m_fForceStrength;                        ///< +0x1318
    float m_fMaxRotate;                            ///< +0x131c
    float m_fElapsedTime;                          ///< +0x1320
    int m_iIterations;                             ///< +0x1324
    int m_iPadding;                                ///< +0x1328
    float m_fFogLayer1ScrollScale;                 ///< +0x132c
    float m_fFogLayer2ScrollScale;                 ///< +0x1330
    unsigned int m_hAlphaState;                    ///< +0x1334
    float m_fBaseScale;                            ///< +0x1338
    unsigned char unk_133c[0x4];                   ///< +0x133c
    FireGrafix::TextureData* m_pFlowField;         ///< +0x1340
    int m_bFOWDirty;                               ///< +0x1344
    LuaContext* m_pDebugContext;                   ///< +0x1348
    // min(GeneralSettings::g_uFOWLevel, 2): 0 draws nothing here, 1 uses a 512 mask, 2 a 1024 mask.
    unsigned int m_uFogLevel;                      ///< +0x134c
    FluidFOWGrid m_aGrids[3];                      ///< +0x1350
};

/// 0x08869d93
UNK_RET Get2UnitNormRand(FGXVector2& a1);

/// 0x08746c70
bool Fun_08746c70(FXmlElement const& kParent, int& rVal, char const* pszName, unsigned int uNameLen);
/// 0x08725468
bool Fun_08725468(FXmlElement const& kParent, float& rVal, char const* pszName, unsigned int uNameLen);
