/// origin guess
#pragma once
#include "Civ5/Src/App/GameCoreEventTemplate.h"
#include "FirePlace/FireWorks/FFastList.h"
#include "FirePlace/FireWorks/FFastVector.h"

class LinearEventDispatcher;
class MarshCreatedData;
class MarshRemovedData;
class NullEventClass;
class SerialContainer;
class Terrain;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
class cvTerrainSampler;
struct TerrainEventDelegate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

// Event 53 as delivered: the serial header word, then the payload; kept until the game starts.
/// size 0x18
struct MarshCreatedEventRecord {
    unsigned int m_uHeader;      ///< +0x0
    MarshCreatedData m_kData;    ///< +0x4
};

// L8 mask of marsh_stamp.dds (128x128 texels); the texels start one byte into the record.
/// size 0x4008
class MarshStamp {
public:
    unsigned char m_aucMask[0x4008]; ///< +0x0
};

// A mask placed on the terrain ID map: texel (column, row) is m_pucMask[column * m_iHeight + row].
/// size 0x14
class BlitMaskedIDSurface {
public:
    unsigned char const* m_pucMask;  ///< +0x0
    int m_iWidth;                    ///< +0x4
    int m_iHeight;                   ///< +0x8
    int m_iX;                        ///< +0xc
    int m_iY;                        ///< +0x10
};

// What Fun_087e735c writes under the mask: a terrain index of g_TerrainSystem.field_27720, 0xff for none.
/// size 0x8
struct MaskedIDPaint {
    cvTerrainSampler* m_pSampler;      ///< +0x0
    unsigned char m_ucTerrainIndex;    ///< +0x4
};

// Paints the marsh terrain into the terrain ID map around each marsh hex; owned by TerrainSystem at +0x276d8.
/// size 0x40
class cvMarshSimSystem {
public:
    /// 0x087e6a30
    cvMarshSimSystem();
    /// 0x087e6aa2
    ~cvMarshSimSystem();
    /// 0x087e6af2
    UNK_RET Startup();
    /// 0x087e6e16
    UNK_RET SubscribeToEvents();
    /// 0x087e6e74
    UNK_RET Shutdown();
    /// 0x087e6ed2
    UNK_RET UnsubscribeToEvents();
    /// 0x087e6f30
    UNK_RET StartupGame();
    /// 0x087e6f7e
    UNK_RET OnMarshAdded(GameCoreEventTemplate<Event_Int2Type<53>, MarshCreatedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);
    /// 0x087e709a
    UNK_RET ShutdownGame();
    /// 0x087e70a4
    UNK_RET Update(float fDeltaTime);
    // uiHexCode selects the terrain whose TerrainData::m_aHexCode matches.
    /// 0x087e70a6
    UNK_RET BlitMaskedIDs(BlitMaskedIDSurface const& kSurface, unsigned int uiHexCode);
    /// 0x087e7122
    UNK_RET OnMarshRemoved(GameCoreEventTemplate<Event_Int2Type<54>, MarshRemovedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& kEvent);

    int field_0;                                                  ///< +0x0
    // Marshes arriving before StartupGame are queued and replayed there.
    bool m_bGameStarted;                                          ///< +0x4
    FFastVector<MarshCreatedEventRecord, true> m_kPendingMarshes; ///< +0x8
    FFastVector<MarshStamp, true> m_kStamps;                      ///< +0x14
    FFastList<MarshCreatedData> m_kMarshes;                       ///< +0x20
};

// Writes a3 into the terrain ID map under every non-zero mask texel whose heightmap value is 13001..17999, then invalidates the region.
// a1 is &TerrainSystem::field_27718; the heightmap size is read at a1 + 0xc and a1 + 0x10.
/// 0x087e735c
UNK_RET Fun_087e735c(Terrain** a1, BlitMaskedIDSurface const& a2, MaskedIDPaint const& a3);
// Subscribes the delegate to event 53 (MarshCreatedData).
/// 0x087e7eb2
UNK_RET Fun_087e7eb2(TerrainEventDelegate* a1);
// Subscribes the delegate to event 54 (MarshRemovedData).
/// 0x087e7b20
UNK_RET Fun_087e7b20(TerrainEventDelegate* a1);
// Unsubscribes the delegate from event 53.
/// 0x087e78f4
UNK_RET Fun_087e78f4(TerrainEventDelegate* a1);
// Unsubscribes the delegate from event 54.
/// 0x087e76c8
UNK_RET Fun_087e76c8(TerrainEventDelegate* a1);
