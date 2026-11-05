/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include <d3d9.h>
#include <Windows.h>

class FGXVector3;
class FrameGlobalsData;
class HEXVec_i;
class LinearEventDispatcher;
class LocalMachineContainer;
class MapGlobalsData;
class NullEventClass;
class UnitHexHighlightData;
class cvEventSystemLocalMachineAccessor;
class cvMovementPathVisSystem;
class cvMovementPathVisSystemDataDef;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <class> class cvMovementPathVisSystem_Accessor;
template <int T0> class Event_Int2Type;

/// size 0x6
struct cvMovementPathVisSystem_PathNode {
    short m_sX;      ///< +0x0
    short m_sY;      ///< +0x2
    short m_sTurns;  ///< +0x4
};

/// size 0x182
class cvMovementPathVisSystem_UpdatePath {
public:
    short m_sNodeCount;                                  ///< +0x0
    cvMovementPathVisSystem_PathNode m_akNodes[64];      ///< +0x2
};

/// size 0x1
class cvMovementPathVisSystem_DisplayPath {
public:
    bool m_bShow;  ///< +0x0
};

/// size 0x44
struct cvMovementPathVisSystem_RenderEntry {
    short m_sModel;       ///< +0x0
    D3DXMATRIX m_kWorld;  ///< +0x4
};

/// size ?
class cvMovementPathVisSystem {
public:
    enum MODEL_INDEX : int {
        MODEL_CENTER_ONE_TURN = 0,
        MODEL_CENTER_TWO_TURNS = 1,
        MODEL_CENTER_THREE_TURNS = 2,
        MODEL_CENTER_INACCESSIBLE = 3,
        MODEL_CENTER_EMBARK = 4,
        MODEL_CENTER_ATTACK = 5,
        MODEL_SELECTED_ONE_TURN = 6,
        MODEL_SELECTED_TWO_TURNS = 7,
        MODEL_SELECTED_THREE_TURNS = 8,
        MODEL_SELECTED_INACCESSIBLE = 9,
        MODEL_SELECTED_EMBARK = 10,
        MODEL_SELECTED_ATTACK = 11,
        MODEL_CONNECTOR_ONE_TURN = 12,
        MODEL_CONNECTOR_TWO_TURNS = 13,
        MODEL_CONNECTOR_THREE_TURNS = 14,
        MODEL_CONNECTOR_INACCESSIBLE = 15,
        MODEL_CONNECTOR_EMBARK = 16,
        MODEL_CONNECTOR_ATTACK = 17,
        MODEL_COUNT = 18
    };
    class RenderPass;

    const char* m_apszModelFiles[18];                   ///< +0x0
    void* m_pCommandStream;                             ///< +0x48
    void* m_apSurfaceSets[2];                           ///< +0x4c
    void* m_apPassConstants[2];                         ///< +0x54
    void* m_pGodRaySurfaceSet;                          ///< +0x5c
    void* m_pGodRayConstants;                           ///< +0x60
    char unk_64[0x10];                                  ///< +0x64
    char unk_74[0x40];                                  ///< +0x74
    char unk_b4[0x680];                                 ///< +0xb4
    char unk_734[0x140];                                ///< +0x734
    unsigned int m_uPassCount;                          ///< +0x874
    char unk_878[0x38];                                 ///< +0x878
    void* m_pTerrainSampler;                            ///< +0x8b0
    char unk_8b4[0x8c];                                 ///< +0x8b4
    char m_akInstances[0x6c0];                          ///< +0x940
    char m_akTemplates[0x7e0];                          ///< +0x1000
    cvMovementPathVisSystem_RenderEntry m_akRenderList[256];  ///< +0x17e0
    cvMovementPathVisSystem_UpdatePath m_kPath;         ///< +0x5be0
    char unk_5d62[0x2];                                 ///< +0x5d62
    unsigned int m_uRenderCount;                        ///< +0x5d64
    float m_fAnimTime;                                  ///< +0x5d68
    float m_fHighlightTime;                             ///< +0x5d6c
    float m_fMinHeight;                                 ///< +0x5d70
    bool m_bStartedUp;                                  ///< +0x5d74
    char unk_5d75[0xb];                                 ///< +0x5d75
    char m_kGodRayTemplate[0x70];                       ///< +0x5d80
    bool m_bHighlightActive;                            ///< +0x5df0
    float m_fHighlightPeriod;                           ///< +0x5df4
    unsigned int m_uGodRayPass;                         ///< +0x5df8
    unsigned int m_uAlphaState;                         ///< +0x5dfc
    void* m_pShaderPackage;                             ///< +0x5e00
    char unk_5e04[0x8];                                 ///< +0x5e04

    /// 0x088bf208
    cvMovementPathVisSystem();
    /// 0x088bf644
    ~cvMovementPathVisSystem();
    /// 0x088bf592
    UNK_RET BindModelFilenames();
    /// 0x088bf6bc
    UNK_RET Startup();
    /// 0x088bf8ae
    UNK_RET SubscribeToEvents();
    /// 0x088bf92a
    UNK_RET CreateTemplatesAndInstances();
    /// 0x088bfe94
    UNK_RET Shutdown();
    /// 0x088c0004
    UNK_RET UnSubscribeToEvents();
    /// 0x088c0080
    UNK_RET StartupGame();
    /// 0x088c0082
    UNK_RET ShutdownGame();
    /// 0x088c010e
    UNK_RET OnMapGlobalsChanged(EventTemplate<Event_Int2Type<37>, MapGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088c0144
    UNK_RET OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088c0178
    UNK_RET OnHexHighlight(EventTemplate<Event_Int2Type<11>, UnitHexHighlightData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088c026e
    UNK_RET OnRenderView(unsigned int a1);
    /// 0x088c0434
    UNK_RET UpdateGlobals(unsigned int a1, cvMovementPathVisSystem::RenderPass a2);
    /// 0x088c059c
    UNK_RET RenderInternal();
    /// 0x088c0896
    void Update(float a1);
    /// 0x088c08c6
    void UpdateFromHandle(cvGeneralRefHandle<cvMovementPathVisSystem, unsigned int, cvMovementPathVisSystemDataDef, cvMovementPathVisSystem_Accessor>& a1, cvMovementPathVisSystem_DisplayPath a2);
    /// 0x088c18fa
    bool IsUnexplored(HEXVec_i a1);
    /// 0x088c1a10
    bool WillTriggerSwap(HEXVec_i const& a1);
    /// 0x088c1c1e
    void LoadModelIntoRenderList(cvMovementPathVisSystem::MODEL_INDEX a1, D3DXMATRIX const& a2);
    /// 0x088c1c72
    bool WillTriggerAttack(HEXVec_i const& a1);
    /// 0x088c1e8e
    void LoadConnectorModelsIntoRenderList(cvMovementPathVisSystem::MODEL_INDEX a1, FGXVector3 const& a2, FGXVector3 const& a3, unsigned int a4, bool a5);
    /// 0x088c219c
    bool WillTriggerEmbark(HEXVec_i const& a1, HEXVec_i const& a2);
    /// 0x088c24a0
    bool WillTriggerDisembark(HEXVec_i const& a1, HEXVec_i const& a2);
    /// 0x088c279e
    void UpdateFromHandle(cvGeneralRefHandle<cvMovementPathVisSystem, unsigned int, cvMovementPathVisSystemDataDef, cvMovementPathVisSystem_Accessor>& a1, cvMovementPathVisSystem_UpdatePath a2);
};
