/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FString.h"
#include <lua.h>

class AILeaderMessageData;
class ActivePlayerData;
class AudioDebugChangeMusicData;
class EndGameShowData;
class EraChangedData;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class SerialContainer;
class TeamMetData;
class cvEventSystemLocalMachineAccessor;
class cvEventSystemSerialAccessor;
class cvMusicPlaylist;
class cvSubscriptionValidator;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class GameCoreEventTemplate;
template <class T0, class T1> class BaseEvent;
template <class T0, class T1> class cvSerialBaseEvent;
template <int T0> class Event_Int2Type;

/// size ?
class cvAudioSimMainGameMusic {
public:
    class PlayerMusicEntry;
    /// size ?
    class PlayerMusicEntry {
    public:
        /// 0x08b740f0
        PlayerMusicEntry();
    };
    enum TRACK_LIST_TYPE : int;
    /// 0x08b741e4
    cvAudioSimMainGameMusic();
    /// 0x08b74262
    UNK_RET Startup();
    /// 0x08b744ba
    UNK_RET SubscribeToEvents();
    /// 0x08b74608
    UNK_RET InitializePlayers();
    /// 0x08b7464e
    UNK_RET GetActivePlayerEntry();
    /// 0x08b74656
    UNK_RET SetActivePlayerEntry(PlayerTypes a1);
    /// 0x08b746aa
    UNK_RET GetPlayerEntry(PlayerTypes a1);
    /// 0x08b746d4
    UNK_RET Shutdown();
    /// 0x08b74746
    UNK_RET StopMusic();
    /// 0x08b7479a
    UNK_RET UnSubscribeFromEvents();
    /// 0x08b748e8
    UNK_RET UpdatePlayerTracks(PlayerTypes a1, cvAudioSimMainGameMusic::TRACK_LIST_TYPE a2);
    /// 0x08b74e84
    UNK_RET OnAudioDebugChangeMusic(EventTemplate<Event_Int2Type<24>, AudioDebugChangeMusicData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b74e96
    UNK_RET OnLeaderViewMessage(GameCoreEventTemplate<Event_Int2Type<118>, AILeaderMessageData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b74eb6
    UNK_RET OnLeaveLeaderView(EventTemplate<Event_Int2Type<39>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b74eca
    UNK_RET OnLoadingScreenClose(EventTemplate<Event_Int2Type<170>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b74edc
    UNK_RET OnGameOverScreenShow(GameCoreEventTemplate<Event_Int2Type<136>, EndGameShowData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b74eea
    UNK_RET OnAudioAdvanceCurrentPlaylistTrack(EventTemplate<Event_Int2Type<26>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b74f28
    UNK_RET OnAudioRewindCurrentPlaylistTrack(EventTemplate<Event_Int2Type<27>, NullEventClass, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b74f56
    UNK_RET OnEraChanged(GameCoreEventTemplate<Event_Int2Type<48>, EraChangedData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b74f8a
    UNK_RET OnTeamMet(GameCoreEventTemplate<Event_Int2Type<119>, TeamMetData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b7502e
    UNK_RET OnActivePlayerChanged(GameCoreEventTemplate<Event_Int2Type<131>, ActivePlayerData, LinearEventDispatcher, SerialContainer, NullEventClass, cvSerialBaseEvent<cvEventSystemSerialAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x08b7505a
    UNK_RET OnEnterLeaderView(PlayerTypes a1);
    /// 0x08b75070
    UNK_RET Update(float a1);
    /// 0x08b750e2
    UNK_RET UpdateMusic(float a1);
    /// 0x08b75188
    UNK_RET ShouldUpdateMusic();
    /// 0x08b751b8
    UNK_RET ShouldPickNewSong();
    /// 0x08b75210
    UNK_RET IsCurrentTrackStillPlaying();
    /// 0x08b75258
    UNK_RET ActivatePlaylist(cvMusicPlaylist* a1);
    /// 0x08b7528a
    UNK_RET IsMyTeamAtWarWith(PlayerTypes a1);
    /// 0x08b753ee
    UNK_RET UpdateWarStatus();
    /// 0x08b75550
    UNK_RET IsLeaderViewerActive();
    /// 0x08b75560
    UNK_RET PickNonPlaylistTrack(unsigned int& a1, bool& a2);
    /// 0x08b759ca
    UNK_RET BuildNonPlaylistCivSpecificSongName(PlayerTypes a1, bool a2, FStringA& a3);
    /// 0x08b75bf4
    UNK_RET PlayNewSong(float a1);
    /// 0x08b75e08
    UNK_RET GetLeaderSongHash(PlayerTypes a1, bool a2);
    /// 0x08b75f64
    UNK_RET PlayMusic(unsigned int a1, int a2);
    /// 0x08b76022
    UNK_RET DebugPanelPrint(char const* a1);
};

/// 0x08b762c0
UNK_RET GetCurCivSong();
/// 0x08b762e5
/// lua ?
int LuaThunk_GetCurCivSong(lua_State* a1);
/// 0x08b7635d
int LuaToString_GetCurCivSong(lua_State* a1);
