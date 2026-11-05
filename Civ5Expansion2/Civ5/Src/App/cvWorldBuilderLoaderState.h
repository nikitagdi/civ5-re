/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"

class LinearEventDispatcher;
class NullEventClass;
class SerialContainer;
class cvEventSystemSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fd520
class cvWorldBuilderLoaderState : public Civ5GameState {
public:
    /// 0x0884cb98
    cvWorldBuilderLoaderState();
    /// 0x0884cc8c
    /// deleting 0x0884cc64
    virtual ~cvWorldBuilderLoaderState();
    /// 0x0884cd54
    virtual UNK_RET Begin();
    /// 0x0884d038
    virtual UNK_RET End();
    /// 0x0884d03c
    virtual UNK_RET Update(float a1);
    /// 0x0884d03a
    virtual UNK_RET Render(unsigned int a1);
    /// 0x0865f3b4
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x0884cc34
    UNK_RET HandleGameInitFinished(GameCoreEventTemplate<Event_Int2Type<113>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0884cd28
    UNK_RET Initialize(wchar_t const* a1);
};
