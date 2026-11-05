/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SequencerContainer;
class SystemUpdateUIData;
class TerrainCacheData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSequencerAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2f8de0
class LoadGameState : public Civ5GameState {
public:
    /// 0x0865bb54
    LoadGameState();
    /// 0x0865bca8
    /// deleting 0x0865bc80
    virtual ~LoadGameState();
    /// 0x0865bd2e
    virtual UNK_RET Begin();
    /// 0x0865c0f8
    virtual UNK_RET End();
    /// 0x0865efc6
    virtual UNK_RET Update(float a1);
    /// 0x0865f004
    virtual UNK_RET Render(unsigned int a1);
    /// 0x0865f3b4
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x0865e3a2
    virtual UNK_RET StartInitStuffOnMap();
    /// 0x0865cdd8
    virtual UNK_RET StartInitTerrainFromGameplayMap();
    /// 0x0865c6d6
    virtual UNK_RET LaunchNextStage();
    /// 0x0865bbf8
    UNK_RET HandleSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0865c08e
    UNK_RET TerrainDone(EventTemplate<Event_Int2Type<2>, NullEventClass, LinearEventDispatcher, SequencerContainer, NullEventClass, BaseEvent<cvEventSystemSequencerAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0865c09a
    UNK_RET OnTerrainCached(EventTemplate<Event_Int2Type<28>, TerrainCacheData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0865f0d2
    UNK_RET SetLoadCanceled(bool a1);
    /// 0x0a340650
    static UNK_DATA ms_uiGamesStarted[0x4];
};
