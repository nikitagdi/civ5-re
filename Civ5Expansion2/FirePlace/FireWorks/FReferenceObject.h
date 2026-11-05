/// origin binary
#pragma once

class LinearEventDispatcher;
class NullEventClass;
class SerialContainer;
class TerrainOverlayModData;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fce8c
class FReferenceObject {
public:
    char unk_4[0x4];    ///< +0x4
    /// 0x087fcab4
    /// deleting 0x087fcad0
    virtual ~FReferenceObject();
};

namespace cvDecalSim {
/// 0x087fcba8
UNK_RET Startup();
/// 0x087fcbe4
UNK_RET OnTerrainOverlayModified(GameCoreEventTemplate<Event_Int2Type<31>, TerrainOverlayModData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
/// 0x087fcbe6
UNK_RET Shutdown();
}
