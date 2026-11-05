/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/NexusSupport/Mode.h"
#include <Windows.h>

class LinearEventDispatcher;
class LocalMachineContainer;
class NonSerialContainer;
class NullEventClass;
class StateMachineChangeStateData;
class StateMachineDumpStatesData;
class UnitFormationResultData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemNonSerialAccessor;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a300740
class NexusUnitViewer : public Nexus::Mode {
public:
    /// 0x08b69bc8
    NexusUnitViewer();
    /// 0x08b69dfa
    /// deleting 0x08b69dd2
    virtual ~NexusUnitViewer();
    /// 0x08b6cdd6
    virtual UNK_RET Shutdown();
    /// 0x08b69e9e
    virtual UNK_RET Startup();
    /// 0x08b6cfc8
    virtual UNK_RET Update();
    /// 0x08b6b338
    UNK_RET OnUnitFormationQueryResult(EventTemplate<Event_Int2Type<31>, UnitFormationResultData, LinearEventDispatcher, NonSerialContainer, NullEventClass, BaseEvent<cvEventSystemNonSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b6b414
    UNK_RET OnStateEntered(EventTemplate<Event_Int2Type<60>, StateMachineChangeStateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b6b4a0
    UNK_RET OnStateLeft(EventTemplate<Event_Int2Type<61>, StateMachineChangeStateData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b6b4de
    UNK_RET OnRecievedStateMachineStates(EventTemplate<Event_Int2Type<63>, StateMachineDumpStatesData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b6b58c
    UNK_RET PublishEvents();
    /// 0x08b6b5cc
    UNK_RET SetWindow(HWND__* a1);
    /// 0x08b6b69e
    UNK_RET SendBackgroundColor();
    /// 0x08b6b6ea
    UNK_RET SetBackgroundColor(float a1, float a2, float a3);
    /// 0x08b6b71a
    UNK_RET WindowResized();
    /// 0x08b6b75c
    UNK_RET MouseMove(int a1, int a2);
    /// 0x08b6b86e
    UNK_RET MiddleMouseButton(bool a1);
    /// 0x08b6b87a
    UNK_RET MouseWheel(int a1);
    /// 0x08b6b8a8
    UNK_RET SendUnitTypes();
    /// 0x08b6b97e
    UNK_RET SendAnimCodePresets();
    /// 0x08b6ba84
    UNK_RET SetUnitType(char const* a1);
    /// 0x08b6bfb6
    UNK_RET SetAnimationCode(float a1);
    /// 0x08b6c0aa
    UNK_RET SetPathOption(int a1, float a2);
    /// 0x08b6c1b4
    UNK_RET GetDamageStates();
    /// 0x08b6c274
    UNK_RET SetDamageState(unsigned int a1, unsigned int a2);
    /// 0x08b6c358
    UNK_RET SetTimeScale(float a1);
    /// 0x08b6c36c
    UNK_RET JumpToState(char const* a1, unsigned int a2);
    /// 0x08b6c416
    UNK_RET WindowResizing();
    /// 0x08b6c42a
    UNK_RET ReloadEffects();
    /// 0x08b6c4be
    UNK_RET ResetUnit();
    /// 0x08b6c4f2
    UNK_RET ResetCamera();
    /// 0x08b6c518
    UNK_RET SendFormationTypes();
    /// 0x08b6c5b0
    UNK_RET SendFormationProperties();
    /// 0x08b6c61a
    UNK_RET SetUnitFormation(char const* a1);
    /// 0x08b6c7a6
    UNK_RET SetUnitFormationProperty(unsigned int a1, float a2);
    /// 0x08b6c888
    UNK_RET CreateUnitFormation(char const* a1, char const* a2);
    /// 0x08b6c9e4
    UNK_RET SaveFormationData(char const* a1);
    /// 0x08b6ca10
    UNK_RET SendTeamColor();
    /// 0x08b6cbe0
    UNK_RET SetPrimaryTeamColor(float a1, float a2, float a3);
    /// 0x08b6cc3e
    UNK_RET SetSecondaryTeamColor(float a1, float a2, float a3);
    /// 0x08b6cc9c
    UNK_RET ReloadUnitLibrary();
    /// 0x08b6cd2e
    UNK_RET ReloadUnitAsset();
    /// 0x08b6d378
    UNK_RET UpdateGlobals();
    /// 0x08b6db20
    UNK_RET UpdateCameraAspectRatio();
    /// 0x08b6db8a
    UNK_RET SendTeamColorEntry(char const* a1, PlayerColorTypes a2);
};
