/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireGrafix/FGXVector4.h"

class FGXVector2;
class HEXVec_i;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
struct TerrainEventDelegate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class CCommandStream;
class TradeArrowShader;
}

// Payload of event 117, filled by CvDLLUtility::TradeVisuals_NewRoute; at most 0x100 plots are kept.
/// size 0x810
class NewTradeRoute_EventData {
public:
    int m_iRoute;                 ///< +0x0
    int m_iPlayer;                ///< +0x4
    TradeConnectionType m_eType;  ///< +0x8
    int m_nPlots;                 ///< +0xc
    // Plot coordinates as (x, y) pairs.
    int m_aiPlots[0x100][2];      ///< +0x10
};

// Payload of event 118 (CvDLLUtility::TradeVisuals_UpdateRouteDirection).
/// size 0x8
class UpdateTradeRouteDirection_EventData {
public:
    int m_iRoute;                 ///< +0x0
    bool m_bForwards;             ///< +0x4
    unsigned char unk_5[0x3];     ///< +0x5
};

// Payload of event 119 (CvDLLUtility::TradeVisuals_DestroyRoute); the player is not read by the visualiser.
/// size 0x8
class DestroyTradeRoute_EventData {
public:
    int m_iRoute;                 ///< +0x0
    int m_iPlayer;                ///< +0x4
};

// Payload of event 120, sent by the Lua UI.
/// size ?
class ToggleTradeRouteDisplay_EventData {
public:
    bool m_bShow;                 ///< +0x0
};

// Payload of event 121: TradeVisuals_ActivatePopupRoute sends (true, route), DeactivatePopupRoute (false, 0).
/// size 0x8
class ToggleTradeRoutePopup_EventData {
public:
    bool m_bActive;               ///< +0x0
    unsigned char unk_1[0x3];     ///< +0x1
    int m_iRoute;                 ///< +0x4
};

// Payload of event 122, posted by Update in strategic view: the route draw batches for cvStrategicViewTracker.
/// size 0x3c
class TradeRoutesPublishBuffers_EventData {
public:
    unsigned int m_hVB;                    ///< +0x0
    unsigned int m_hIB;                    ///< +0x4
    unsigned int m_nBatches;               ///< +0x8
    unsigned int m_ahTextures[4];          ///< +0xc
    unsigned int m_auStartIndex[4];        ///< +0x1c
    unsigned int m_auTriangleCount[4];     ///< +0x2c
};

// Vertex of format 9: z is 0 (the route is drawn into the terrain overlay) or 1 in strategic view.
/// size 0x18
struct TradeRouteVertex {
    float m_fX;                   ///< +0x0
    float m_fY;                   ///< +0x4
    float m_fZ;                   ///< +0x8
    // Bytes R, G, B, A from the low byte up.
    unsigned int m_uColor;        ///< +0xc
    float m_fU;                   ///< +0x10
    float m_fV;                   ///< +0x14
};

// One route slot of the visualiser; m_nPlots == 0 marks a free slot.
/// size 0x18
struct TradeRouteVis {
    int m_iRoute;                 ///< +0x0
    int m_iPlayer;                ///< +0x4
    int m_nPlots;                 ///< +0x8
    TradeConnectionType m_eType;  ///< +0xc
    bool m_bForwards;             ///< +0x10
    unsigned char unk_11[0x3];    ///< +0x11
    // malloc'ed buffer of 0x100 (x, y) pairs.
    int (*m_paiPlots)[2];         ///< +0x14
};

// OverlaySystem member at +0x11d24; all state lives in the file-level globals below.
/// size 0x4
class TradeVisSystem {
public:
    /// 0x089f1ae8
    TradeVisSystem();
    /// 0x089f1af4
    ~TradeVisSystem();
    /// 0x089f1af6
    void Startup();
    /// 0x089f1cb0
    void NewTradeRoute_Handler(EventTemplate<Event_Int2Type<117>, NewTradeRoute_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x089f1db6
    void UpdateTradeRouteDirection_Handler(EventTemplate<Event_Int2Type<118>, UpdateTradeRouteDirection_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x089f1df4
    void DestroyTradeRoute_Handler(EventTemplate<Event_Int2Type<119>, DestroyTradeRoute_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x089f1e5c
    void ToggleDisplay_Handler(EventTemplate<Event_Int2Type<120>, ToggleTradeRouteDisplay_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x089f1e86
    void TogglePopup_Handler(EventTemplate<Event_Int2Type<121>, ToggleTradeRoutePopup_EventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x089f1eca
    void Shutdown();
    // Fills g_aTradeRoutePlayerColors with each live player's primary colour; white for others, opaque black when the colour info is missing.
    /// 0x089f2070
    void StartupGame();
    /// 0x089f2328
    void ShutdownGame();
    /// 0x089f2428
    void Update(float fDeltaTime, bool bStrategicView);
    /// 0x089f2ec8
    void Render(FireGrafix::CCommandStream* pCommandStream, int iCamera);

    int field_0;  ///< +0x0
};

// The copy of a2 (a hex of the wrapped map) that lies next to a1.
/// 0x089f235c
HEXVec_i get_unwrapped_pos(HEXVec_i const& a1, HEXVec_i const& a2);

// Event (un)subscribe helpers for events 117..121; the delegate holds the system and its handler.
// Subscribes to event 117 (NewTradeRoute).
/// 0x089f5e0c
UNK_RET Fun_089f5e0c(TerrainEventDelegate* a1);
// Subscribes to event 118 (UpdateTradeRouteDirection).
/// 0x089f5a7a
UNK_RET Fun_089f5a7a(TerrainEventDelegate* a1);
// Subscribes to event 119 (DestroyTradeRoute).
/// 0x089f56e8
UNK_RET Fun_089f56e8(TerrainEventDelegate* a1);
// Subscribes to event 120 (ToggleTradeRouteDisplay).
/// 0x089f5356
UNK_RET Fun_089f5356(TerrainEventDelegate* a1);
// Subscribes to event 121 (ToggleTradeRoutePopup).
/// 0x089f4fc4
UNK_RET Fun_089f4fc4(TerrainEventDelegate* a1);
// Unsubscribes from event 120.
/// 0x089f4d98
UNK_RET Fun_089f4d98(TerrainEventDelegate* a1);
// Unsubscribes from event 121.
/// 0x089f4b6c
UNK_RET Fun_089f4b6c(TerrainEventDelegate* a1);
// Unsubscribes from event 117.
/// 0x089f4940
UNK_RET Fun_089f4940(TerrainEventDelegate* a1);
// Unsubscribes from event 118.
/// 0x089f4714
UNK_RET Fun_089f4714(TerrainEventDelegate* a1);
// Unsubscribes from event 119.
/// 0x089f44e8
UNK_RET Fun_089f44e8(TerrainEventDelegate* a1);
// Releases the four batch surfaces.
/// 0x089f2026
static void ReleaseTradeRouteSurfaces();
// Turns the plot list of a route into strip pieces and an arrow head. bOwned: active player's route (alpha 0.4), else the popup route (alpha 0.95).
/// 0x089f2983
static void BuildRouteGeometry(TradeRouteVis* pRoute, bool bOwned);
// Makes a draw batch over the shared buffers from uStartIndex; the surface handle goes to phSurface.
/// 0x089f2d95 fastcc
static void CreateTradeRouteSurface(unsigned int* phSurface, unsigned int uStartIndex, int nIndices, unsigned short uTexture);
// Unlocks the shared vertex and index buffers.
/// 0x089f2e4a
static void UnlockTradeRouteBuffers();
// Last piece of a route: a stub (curve parameter 0 to 0.15) from pCur towards pEnd, then the arrow head. pfV is the texture phase.
/// 0x089f2faa fastcc
static void BuildRouteEndCap(float* pfV, HEXVec_i* pPrev, HEXVec_i* pCur, HEXVec_i* pEnd, FGXVector4 const* pColor);
// One interior piece from pCur to pNext (Catmull-Rom through pPrev..pNext2). Returns true when the buffers are full.
/// 0x089f3ac8 fastcc
static bool BuildRouteSegment(float* pfV, HEXVec_i* pPrev, HEXVec_i* pCur, HEXVec_i* pNext, HEXVec_i* pNext2, FGXVector4 const* pColor);
// Samples a Hermite curve at 9 parameters spaced by fStep and writes 4 vertices per sample (feather, edge, edge, feather) with 8 rows of 18 indices.
// pvEnd gets the last curve point, pvNormal its unit normal, *pfV the negated parameter after the last step. Returns true when the buffers are full.
/// 0x089f3eba fastcc
static bool BuildRouteStrip(float* pfV, FGXVector2* pvEnd, FGXVector2* pvNormal, FGXVector2 const* pvP0, FGXVector2 const* pvP1, FGXVector2 const* pvT0, FGXVector2 const* pvT1, FGXVector4 const* pColor, float fStep);
// Reserves vertices and indices in the shared buffers; both pointers are NULL when they do not fit.
/// 0x089f439e
static void AllocateRouteGeometry(TradeRouteVertex** ppVertices, unsigned int** ppIndices, unsigned int* puFirstVertex, int nVertices, int nIndices);
// Cubic Hermite point at parameter fT.
/// 0x089f4420
static void EvaluateHermite(FGXVector2* pvOut, FGXVector2 const* pvP0, FGXVector2 const* pvP1, FGXVector2 const* pvT0, FGXVector2 const* pvT1, float fT);

// File-level state of the trade route visualiser.
// RGBA per player slot: white by default, opaque black when the player colour has no info.
/// 0x0c085be4
extern FGXVector4 g_aTradeRoutePlayerColors[64];
// Route display toggle; Startup copies it from game option byte g_GameOptionsStore+0x11, event 120 sets it.
/// 0x0c085fe4
extern bool g_bShowTradeRoutes;
/// 0x0c085fe5
extern bool g_bTradePopupActive;
/// 0x0c085fe8
extern int g_iTradePopupRoute;
/// 0x0c085fec
extern FireGrafix::TradeArrowShader* g_pTradeArrowShader;
// Texture handles in TEXTURE_PATHS order: arrow, arrow_food, arrow_prod, then the three popup variants.
/// 0x0c085ff0
extern unsigned int g_ahTradeArrowTextures[6];
// Slots in use, free ones included; at most 0x100.
/// 0x0c086008
extern int g_nTradeRouteSlots;
/// 0x0c08600c
extern bool g_bTradeRoutesDirty;
// Texture scroll phase in [0, 1): advances 0.75 per second, read by the shader at constant +0x40.
/// 0x0c086010
extern float g_fTradeArrowScroll;
/// 0x0c086014
extern bool g_bTradeStrategicView;
/// 0x0c086018
extern TradeRouteVis g_aTradeRoutes[0x100];
// Draw batches: [0] the popup route, [1..3] the active player's routes of types 0..2.
/// 0x0c087818
extern unsigned int g_ahTradeRouteSurfaces[4];
/// 0x0c087828
extern unsigned int g_hTradeRouteVB;
/// 0x0c08782c
extern unsigned int g_uTradeRouteVertexCount;
/// 0x0c087830
extern unsigned int g_hTradeRouteIB;
/// 0x0c087834
extern unsigned int g_uTradeRouteIndexCount;
/// 0x0c087838
extern TradeRouteVertex* g_pTradeRouteVertices;
/// 0x0c08783c
extern void* g_pTradeRouteIndices;
