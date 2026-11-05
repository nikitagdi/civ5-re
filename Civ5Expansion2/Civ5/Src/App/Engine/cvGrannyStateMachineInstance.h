/// origin guess
#pragma once
#include "Civ5/Src/App/Engine/FGrannyStateMachineInstance.h"

class FGrannyModelInstanceData;
class FGrannyState;
class FGrannyStateMachine;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class StateMachineRequestStatesData;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0> class FFiniteStateMachineReferenceState;
template <int T0> class Event_Int2Type;

/// size 0xb8
/// vtable 0x0a2fcd50
class cvGrannyStateMachineInstance : public FGrannyStateMachineInstance {
public:
    /// 0x087fb658
    cvGrannyStateMachineInstance();
    /// 0x087fb6c0
    /// deleting 0x087fb698
    virtual ~cvGrannyStateMachineInstance();
    /// 0x087fb734
    virtual void Init(FGrannyModelInstanceData* a1, FGrannyStateMachine& a2, unsigned int a3);
    /// 0x087fb85a
    virtual void on_enter(int a1, FFiniteStateMachineReferenceState<FGrannyState>& a2, int a3);
    /// 0x087fb92c
    virtual void on_leave(int a1, FFiniteStateMachineReferenceState<FGrannyState>& a2, int a3);
    /// 0x087fb7e2
    virtual void Release();
    /// 0x087fb7a6
    UNK_RET SubscribeToEvents();
    /// 0x087fb81e
    UNK_RET UnSubscribeFromEvents();
    /// 0x087fb9f4
    void OnStateMachineRequestStates(EventTemplate<Event_Int2Type<62>, StateMachineRequestStatesData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x087fbaa6
    static bool EnableEvents(bool a1);
    /// 0x0a30fa0c
    static UNK_DATA ms_bEnableEvents[0x1];

    unsigned int m_uiModelInstanceId;   ///< +0xb0
    unsigned int m_uiHandle;            ///< +0xb4
};
