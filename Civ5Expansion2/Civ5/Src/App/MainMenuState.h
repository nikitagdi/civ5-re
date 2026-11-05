/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"
#include <Windows.h>

class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class SteamLobbyInviteData;
class SteamServerInviteData;
class SystemUpdateUIData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvPath;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
/// vtable 0x0a2fd420
class MainMenuState : public Civ5GameState {
public:
    /// 0x088482e4
    MainMenuState();
    /// 0x08848a30
    /// deleting 0x08848a08
    virtual ~MainMenuState();
    /// 0x08848bb8
    virtual UNK_RET Begin();
    /// 0x08848de0
    virtual UNK_RET End();
    /// 0x08848ed0
    virtual UNK_RET Update(float a1);
    /// 0x08848e2a
    virtual UNK_RET Render(unsigned int a1);
    /// 0x08848e26
    virtual UNK_RET ProcessInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x08848472
    UNK_RET ResetValues();
    /// 0x08848474
    UNK_RET HandleStartGame(GameCoreEventTemplate<Event_Int2Type<3>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088486ae
    UNK_RET HandleGameInitFinished(GameCoreEventTemplate<Event_Int2Type<113>, NullEventClass, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088486de
    UNK_RET HandleSystemUpdateUI(EventTemplate<Event_Int2Type<101>, SystemUpdateUIData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08848796
    UNK_RET HandleJoinRoomAttempt(EventTemplate<Event_Int2Type<134>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088487e2
    UNK_RET HandleGameLobbyInvite(EventTemplate<Event_Int2Type<137>, SteamLobbyInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08848886
    UNK_RET HandleGameServerInvite(EventTemplate<Event_Int2Type<138>, SteamServerInviteData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x088488f2
    UNK_RET HandleAssetContentChanging(EventTemplate<Event_Int2Type<92>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08848e58
    UNK_RET StartInviteJoinGameScreen();
    /// 0x08848f50
    UNK_RET SetStartupLoad(cvPath const& a1, bool a2);
};
