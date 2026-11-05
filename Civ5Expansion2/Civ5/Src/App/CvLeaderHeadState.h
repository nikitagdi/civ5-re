/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"
#include <Windows.h>

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SystemUpdateUIData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fd330
class CvLeaderHeadState : public Civ5GameState {
public:
    /// 0x0882b754
    CvLeaderHeadState();
    /// 0x0882b820
    /// deleting 0x0882b7dc
    virtual ~CvLeaderHeadState();
    /// 0x0882bb6c
    virtual UNK_RET Begin();
    /// 0x0882bbd4
    virtual UNK_RET End();
    /// 0x0882bc50
    virtual UNK_RET Update(float a1);
    /// 0x0882bf36
    virtual UNK_RET Render(unsigned int a1);
    /// 0x0882c02c
    virtual UNK_RET ProcessInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x0882b85e
    UNK_RET StartupGame();
    /// 0x0882ba0e
    UNK_RET HandleSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0882bab0
    UNK_RET ShutdownGame();
    /// 0x0882bbc0
    UNK_RET AreParticlesInitialized();
    /// 0x0882bf24
    UNK_RET AreLeaderHeadsInitialized();
};
