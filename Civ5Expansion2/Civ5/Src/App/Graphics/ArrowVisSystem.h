/// origin guess
#pragma once
#include "Civ5/Src/App/ArrowVis.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "Civ5/Src/App/cvFileChangeNotification.h"
#include "FirePlace/FireGrafix/FGXVector3.h"
#include "FirePlace/FireGrafix/VStruct.h"
#include "FirePlace/FireWorks/FFastVector.h"

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class RemoveArrowEventData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
struct TerrainEventDelegate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;
namespace FireGrafix {
class ArrowShader;
class CCommandStream;
}

// Payload of event 165: the arrow runs from the centre of m_kFrom to the centre of m_kTo.
/// size ?
class SpawnArrowEventData {
public:
    HEXVec_i m_kFrom;  ///< +0x0
    HEXVec_i m_kTo;    ///< +0xc
};

// The global g_cvArrowVisSystem (0x0bff988c); the fields +0x4..+0x30 come from assets/UI/Overlays/ArrowSettings.xml.
/// size 0x4c8
class ArrowVisSystem {
public:
    /// 0x08851232
    ArrowVisSystem();
    /// 0x088512d4
    ~ArrowVisSystem();
    /// 0x08851320
    void Startup();
    /// 0x0885145a
    void SubscribeToEvents();
    /// 0x088514d6
    void LoadSettingsFromXML();
    /// 0x088519f2
    void Shutdown();
    /// 0x08851a66
    void UnSubscribeToEvents();
    /// 0x08851ae2
    void StartupGame();
    /// 0x08851ae4
    void ShutdownGame();
    /// 0x08851ae6
    void Update(float fDeltaTime);
    /// 0x08851c24
    void Render(unsigned int hRenderTarget);
    // Always reuses arrow slot 0 and returns its index.
    /// 0x08851ed2
    unsigned int SpawnArrow(FGXVector3 const& vStart, FGXVector3 const& vEnd);
    /// 0x08851f5a
    void MoveArrow(unsigned int uArrow, FGXVector3 const& vStart, FGXVector3 const& vEnd);
    /// 0x08851fa8
    void RemoveArrow(unsigned int uArrow);
    /// 0x08851fc2
    void OnSpawnArrow(EventTemplate<Event_Int2Type<165>, SpawnArrowEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x08852218
    void OnRemoveArrow(EventTemplate<Event_Int2Type<167>, RemoveArrowEventData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x0885221a
    void OnRemoveAllArrows(EventTemplate<Event_Int2Type<166>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    // Parses "r,g,b" (0..255 each, clamped) into vColor; empty fields between commas are skipped.
    /// 0x08852226
    void ExtractColor(char const* pszText, FGXVector3& vColor) const;

    int field_0;                                                    ///< +0x0
    float m_fMinWidth;                                              ///< +0x4
    float m_fMaxWidth;                                              ///< +0x8
    float m_fWidth2;                                                ///< +0xc
    float m_fOutlineWidth;                                          ///< +0x10
    float m_fOutlineWidth2;                                         ///< +0x14
    float m_fArcHeight;                                             ///< +0x18
    float m_fTargetHeight;                                          ///< +0x1c
    float m_fAnimSpeed;                                             ///< +0x20
    float m_fAlpha;                                                 ///< +0x24
    float m_fAmbientLightLevel;                                     ///< +0x28
    // Packed 0xAABBGGRR with alpha 0xff.
    unsigned int m_uBodyColor;                                      ///< +0x2c
    unsigned int m_uOutlineColor;                                   ///< +0x30
    FStaticVector<ArrowVis, 1> m_aArrows;                           ///< +0x34
    FireGrafix::CCommandStream* m_pCommandStream;                   ///< +0x74
    FireGrafix::ArrowShader* m_pShader;                             ///< +0x78
    cvFileChangeNotification m_kSettingsWatch;                      ///< +0x7c
    FFastVector<FireGrafix::VStruct<5>, true, 62u, 0u> m_aVertices; ///< +0x4b0
    FFastVector<unsigned int, true, 62u, 0u> m_aIndices;            ///< +0x4bc
};

// Event (un)subscribe helpers for the arrow events; the delegate holds the system and its handler.
// Subscribes to event 165 (SpawnArrow).
/// 0x088535c6
UNK_RET Fun_088535c6(TerrainEventDelegate* a1);
// Subscribes to event 166 (RemoveAllArrows).
/// 0x08853234
UNK_RET Fun_08853234(TerrainEventDelegate* a1);
// Subscribes to event 167 (RemoveArrow).
/// 0x08852ea2
UNK_RET Fun_08852ea2(TerrainEventDelegate* a1);
// Unsubscribes from event 165.
/// 0x08852c76
UNK_RET Fun_08852c76(TerrainEventDelegate* a1);
// Unsubscribes from event 166.
/// 0x08852a4a
UNK_RET Fun_08852a4a(TerrainEventDelegate* a1);
// Unsubscribes from event 167.
/// 0x0885281e
UNK_RET Fun_0885281e(TerrainEventDelegate* a1);
