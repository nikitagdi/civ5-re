/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/DynamicHexGrid.h"
#include "Civ5/Src/App/HEXVec_i.h"
#include "FirePlace/FireGrafix/FGXVector2.h"
#include "FirePlace/FireGrafix/FGXVector3.h"

class ActivePlayerData;
class CityCaptureData;
class CityCreatedData;
class FGameIniParser;
class FMemTracker;
class FrameGlobalsData;
class HexCultureChangedData;
class HexFOWStateChangedData;
class LinearEventDispatcher;
class LocalMachineContainer;
class MinimapAddIndicatorData;
class MinimapClickData;
class MinimapRemoveIndicatorData;
class NullEventClass;
class SerialContainer;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvMinimapSystem;
class cvSubscriptionValidator;
class cvTerrainSampler;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

// One hex of a player's minimap grid. Bytes 1 to 3 each hold a two-bit value and, in bits 2..7, a mask of the six sides
// whose neighbour differs in that value (side i is bit i + 2).
/// size 0x4
class MinimapCell {
public:
    unsigned char m_ucCultureOwner;   ///< +0x0
    unsigned char m_ucTerrain;        ///< +0x1
    unsigned char m_ucFog;            ///< +0x2
    unsigned char m_ucCultureEdges;   ///< +0x3
};

// A blinking marker added by the Lua panel (AddIndicator); Update toggles the alpha byte of the colour each half period.
/// size 0x20
class MinimapIndicator {
public:
    unsigned char unk_0[0x4];     ///< +0x0
    unsigned char m_aucColor[4];  ///< +0x4
    FGXVector2 m_vPosition;       ///< +0x8
    float m_fSize;                ///< +0x10
    float m_fBlinkPeriod;         ///< +0x14
    float m_fElapsed;             ///< +0x18
    float m_fLifetime;            ///< +0x1c
};

// A city dot: world position and the RGBA colour of its sprite.
/// size 0x10
class MinimapCity {
public:
    FGXVector2 m_vPosition;       ///< +0x0
    unsigned char unk_8[0x4];     ///< +0x8
    unsigned int m_uiColor;       ///< +0xc
};

// Bits of cvMinimapSystem::m_uiDirtyFlags. RenderMiniMap rebuilds the hex meshes for any of the low three, the
// sprites for 0x1c (0x8 is set by handlers not recovered yet) and the camera rects for 0x20.
enum MinimapDirtyFlags : unsigned int {
    MINIMAP_DIRTY_FOG = 0x1,
    MINIMAP_DIRTY_CULTURE = 0x2,
    MINIMAP_DIRTY_EXPLORED_REGION = 0x4,
    MINIMAP_DIRTY_INDICATORS = 0x10,
    MINIMAP_DIRTY_CAMERA = 0x20
};

// Payload of event 114: one hex gaining a fog state, or the whole map when m_bWholeMap is set.
// State 0 is unexplored, anything else is explored.
/// size 0x10
class HexFOWStateChangedData {
public:
    unsigned int field_0;         ///< +0x0
    short m_sHexX;                ///< +0x4
    short m_sHexY;                ///< +0x6
    unsigned char m_ucState;      ///< +0x8
    bool m_bWholeMap;             ///< +0x9
    unsigned char unk_a[0x6];     ///< +0xa
};

// Payload of event 29: a hex changing its culture owner; m_iOwner is -1 for none.
/// size 0x10
class HexCultureChangedData {
public:
    unsigned int field_0;         ///< +0x0
    short m_sHexX;                ///< +0x4
    short m_sHexY;                ///< +0x6
    int m_iOwner;                 ///< +0x8
    unsigned char unk_c[0x4];     ///< +0xc
};

// Handler record the event system stores with a queued event: the object and the member function pointer
// (code address, then this adjustment) to call on it.
/// size 0xc
class MinimapEventCallback {
public:
    cvMinimapSystem* m_pObject;   ///< +0x0
    void* m_pfnHandler;           ///< +0x4
    int m_iThisAdjust;            ///< +0x8
};

// Appends one queued call (callback, tag and a copy of the 16-byte payload) to the byte queue of a system.
/// 0x087508be
void Fun_087508be(void* pQueue, void const* pPayload, MinimapEventCallback const* pCallback, unsigned int uiTag);
// As Fun_087508be, the copy used by the culture event.
/// 0x088a9446
void Fun_088a9446(void* pQueue, void const* pPayload, MinimapEventCallback const* pCallback, unsigned int uiTag);

/// size ?
class cvMinimapData {
public:
    // The per-player map: a 256 x 256 cell array on top of a square hex grid policy (size, forward and inverse matrices).
    // Cells outside the grid read as m_kOutsideCell; m_iMin*/m_iMax* bound the explored offset coordinates.
    /// size 0x400cc
    class PlayerData : public SquareGridPolicy {
    public:
        bool m_bWrapX;                      ///< +0x9c
        MinimapCell m_aCells[0x100 * 0x100]; ///< +0x9d
        MinimapCell m_kOutsideCell;         ///< +0x4009d
        MinimapIndicator* m_pIndicators;    ///< +0x400a4
        unsigned int m_uiNumIndicators;     ///< +0x400a8
        unsigned int m_uiIndicatorCapacity; ///< +0x400ac
        int m_iAnchorX;                     ///< +0x400b0
        int m_iAnchorY;                     ///< +0x400b4
        int m_iMinX;                        ///< +0x400b8
        int m_iMinY;                        ///< +0x400bc
        int m_iMaxX;                        ///< +0x400c0
        int m_iMaxY;                        ///< +0x400c4
        bool m_bNothingExplored;            ///< +0x400c8
        unsigned char unk_400c9[0x3];       ///< +0x400c9
    };

    // Node of the player list (std::list of pair<PlayerTypes, PlayerData>).
    /// size 0x400d8
    class PlayerNode {
    public:
        PlayerNode* m_pPrev;                ///< +0x0
        PlayerNode* m_pNext;                ///< +0x4
        PlayerTypes m_ePlayer;              ///< +0x8
        PlayerData m_kData;                 ///< +0xc
    };

    /// 0x088a679c
    PlayerData* GetPlayerData(PlayerTypes a1);
    /// 0x088a6a8a
    UNK_RET ClearPlayerData();

    PlayerTypes m_eActivePlayer;            ///< +0x0
    PlayerNode* m_pPlayerListEndPrev;       ///< +0x4
    PlayerNode* m_pPlayerListFirst;         ///< +0x8
    unsigned int m_uiNumPlayers;            ///< +0xc
    // Primary then secondary player colour, RGBA bytes, filled by StartupGame; swapped for players whose flag is set.
    unsigned char m_aaucPlayerColors[64][8]; ///< +0x10
    FGXVector2* m_pMountains;               ///< +0x210
    unsigned int m_uiNumMountains;          ///< +0x214
    unsigned int m_uiMountainCapacity;      ///< +0x218
    MinimapCity* m_pCities;                 ///< +0x21c
    unsigned int m_uiNumCities;             ///< +0x220
    unsigned int m_uiCityCapacity;          ///< +0x224
    cvTerrainSampler* m_pTerrainSampler;    ///< +0x228
    float m_fCitySpriteSize;                ///< +0x22c
    bool m_bWrapX;                          ///< +0x230
    unsigned char unk_231[0x3];             ///< +0x231
    unsigned int m_uiNumViews;              ///< +0x234
    FGXVector2 m_akViewMin[4];              ///< +0x238
    FGXVector2 m_akViewMax[4];              ///< +0x258
    FGXVector3 m_aakViewCorners[4][4];      ///< +0x278
};

// Cell at offset coordinates (x, y); outside the grid, or beyond the width without wrapping, it is m_kOutsideCell.
/// 0x088a9b1c
MinimapCell* Fun_088a9b1c(cvMinimapData::PlayerData* a1, int a2, int a3);
// Cell under a world hex: the hex goes through m_mxForward, then Fun_088a9b1c.
/// 0x088a9aa6
MinimapCell* Fun_088a9aa6(cvMinimapData::PlayerData* a1, HEXVec_i const* a2);

// The minimap sim system; the one instance is g_MinimapSystem (0x0c03d288), passed as this by every caller.
// The INI settings, the cvMinimapRenderer and the cvMinimapData are separate globals at +0x1c4, +0x1dc and +0x4d8 from it.
/// size ?
class cvMinimapSystem {
public:
    /// 0x088a6aae
    static UNK_RET ParseINIFile(FGameIniParser* a1);
    /// 0x088a6c1a
    static UNK_RET SetupMemTracker(FMemTracker* a1);
    /// 0x088a6c1c
    UNK_RET Startup();
    /// 0x088a6d90
    UNK_RET OnMinimapClick(EventTemplate<Event_Int2Type<94>, MinimapClickData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088a6e76
    UNK_RET OnCityCreated(GameCoreEventTemplate<Event_Int2Type<34>, CityCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088a6fd6
    UNK_RET OnCityDestroyed(GameCoreEventTemplate<Event_Int2Type<40>, CityCaptureData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088a70b4
    UNK_RET OnCityCaptured(GameCoreEventTemplate<Event_Int2Type<42>, CityCaptureData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088a71a6
    void OnFOWChanged(GameCoreEventTemplate<Event_Int2Type<114>, HexFOWStateChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x088a759a
    void OnHexCultureChanged(GameCoreEventTemplate<Event_Int2Type<29>, HexCultureChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x088a776c
    UNK_RET OnFrameGlobalsUpdated(EventTemplate<Event_Int2Type<34>, FrameGlobalsData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088a7a54
    UNK_RET AddIndicator(EventTemplate<Event_Int2Type<96>, MinimapAddIndicatorData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088a7c66
    UNK_RET RemoveIndicator(EventTemplate<Event_Int2Type<97>, MinimapRemoveIndicatorData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088a7dc8
    UNK_RET OnGameplaySetActivePlayer(GameCoreEventTemplate<Event_Int2Type<131>, ActivePlayerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088a7e3a
    UNK_RET Shutdown();
    /// 0x088a7f96
    UNK_RET StartupGame();
    /// 0x088a8ce8
    UNK_RET ShutdownGame();
    /// 0x088a8d22
    UNK_RET Update(float a1);
    /// 0x088a8e84
    UNK_RET RenderMiniMap();
    /// 0x088a8ec2
    UNK_RET RequestBroadcast();

    unsigned int m_uiRenderCount;       ///< +0x0
    unsigned int m_uiFrameValue;        ///< +0x4
    // Head of the byte queue the event callbacks append to while bit 0 of m_ucDeferEvents is set.
    unsigned char m_aucEventQueue[0xc]; ///< +0x8
    unsigned char m_ucDeferEvents;      ///< +0x14
    unsigned char unk_15[0x3];          ///< +0x15
    unsigned int m_uiDirtyFlags;        ///< +0x18
    unsigned char field_1c;             ///< +0x1c
    unsigned char unk_1d[0x3];          ///< +0x1d
};
