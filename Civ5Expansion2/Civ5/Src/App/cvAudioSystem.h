/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/cvGeneralRefHandle.h"
#include "FirePlace/FireWorks/FString.h"
#include <lua.h>

class Audio2DSoundData;
class Audio3DSoundData;
class AudioListenerData;
class AudioSoundTriggerData;
class AudioVolumeData;
class FSoundBase;
class FSoundScape;
class LinearEventDispatcher;
class LocalMachineContainer;
class NullEventClass;
class cvAudioDataDef;
class cvAudioSystem;
class cvAudioSystem_MusicStatus;
class cvAudioSystem_Play2DSound;
class cvAudioSystem_Play3DSound;
class cvAudioSystem_PlayMusic;
class cvAudioSystem_PlaySoundscape;
class cvAudioSystem_PlaySoundscapeBed;
class cvAudioSystem_SoundscapeStatus;
class cvAudioSystem_SpeakerVolumes;
class cvAudioSystem_StopMusic;
class cvAudioSystem_StopSound;
class cvEventSystemLocalMachineAccessor;
class cvSubscriptionValidator;
enum EAudioTag : int;
template <class T0, class T1, class T2, class T3, class T4, class T5, class T6> class EventTemplate;
template <class T0, class T1> class BaseEvent;
template <class> class cvAudioSystem_Accessor;
template <int T0> class Event_Int2Type;

/// size ?
class cvAudioSystem {
public:
    /// 0x0878e694
    cvAudioSystem();
    /// 0x0878e6f2
    ~cvAudioSystem();
    /// 0x0878e722
    UNK_RET Startup();
    /// 0x0878e7e2
    UNK_RET CacheLoopingInfo();
    /// 0x0878e826
    UNK_RET SubscribeToEvents();
    /// 0x0878e8fc
    UNK_RET Shutdown();
    /// 0x0878e924
    UNK_RET ClearLog();
    /// 0x0878e97e
    UNK_RET UnSubscribeFromEvents();
    /// 0x0878ea54
    UNK_RET OnVolumeChanged(EventTemplate<Event_Int2Type<17>, AudioVolumeData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0878eaa8
    UNK_RET OnPlay2DSound(EventTemplate<Event_Int2Type<18>, Audio2DSoundData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0878eb14
    UNK_RET OnPlay3DSound(EventTemplate<Event_Int2Type<19>, Audio3DSoundData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0878ebde
    UNK_RET OnSetListenerInfo(EventTemplate<Event_Int2Type<25>, AudioListenerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0878ec32
    UNK_RET OnTriggerSound(EventTemplate<Event_Int2Type<21>, AudioSoundTriggerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0878f054
    UNK_RET OnTriggerStopSound(EventTemplate<Event_Int2Type<22>, AudioSoundTriggerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0878f08c
    UNK_RET SaveAudioOptions();
    /// 0x0878f1f4
    UNK_RET LoadAudioOptions();
    /// 0x0878f72e
    UNK_RET PauseAudio(bool a1);
    /// 0x0878f76e
    UNK_RET OnPlaySoundscape(EventTemplate<Event_Int2Type<20>, Audio3DSoundData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass>& a1);
    /// 0x0878f7b8
    UNK_RET GetCurrentTierFromUnitHandle(unsigned int a1);
    /// 0x0878f924
    UNK_RET IsOKForTierSoundToPlay(unsigned int a1, char a2, bool& a3);
    /// 0x0878f97a
    UNK_RET HotLoadXMLAudioParams(char const* a1);
    /// 0x0878f9ee
    UNK_RET LogTriggeredSound(EventTemplate<Event_Int2Type<21>, AudioSoundTriggerData, LinearEventDispatcher, LocalMachineContainer, NullEventClass, BaseEvent<cvEventSystemLocalMachineAccessor, cvSubscriptionValidator>, NullEventClass> const& a1, EAudioTag a2, bool a3, bool a4);
    /// 0x08790342
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundBase*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_Play2DSound const& a2);
    /// 0x087903c0
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundBase*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_Play3DSound const& a2);
    /// 0x0879049a
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundBase*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_StopSound const& a2);
    /// 0x087904d6
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundBase*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_PlayMusic const& a2);
    /// 0x0879052a
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundBase*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_StopMusic const& a2);
    /// 0x0879056a
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundBase*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_MusicStatus const& a2);
    /// 0x087905d0
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundBase*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_SpeakerVolumes const& a2);
    /// 0x08790610
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundScape*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_PlaySoundscape const& a2);
    /// 0x08790678
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundScape*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_StopSound const& a2);
    /// 0x087906b6
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundScape*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_SoundscapeStatus const& a2);
    /// 0x087906f4
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundScape*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_SpeakerVolumes const& a2);
    /// 0x08790734
    UNK_RET UpdateFromHandle(cvGeneralRefHandle<cvAudioSystem, FSoundScape*, cvAudioDataDef, cvAudioSystem_Accessor>& a1, cvAudioSystem_PlaySoundscapeBed const& a2);
    /// 0x08790766
    UNK_RET GetHandleID(cvGeneralRefHandle<cvAudioSystem, FSoundBase*, cvAudioDataDef, cvAudioSystem_Accessor>& a1);
    /// 0x08790776
    UNK_RET BeginLogging();
    /// 0x087907b0
    UNK_RET CreateLog();
    /// 0x0879096a
    UNK_RET EndLogging();
    /// 0x08790972
    UNK_RET SaveLog(char const* a1) const;
    /// 0x08790ede
    UNK_RET FormatTimeForLog(FStringA& a1);
    /// 0x08790fde
    UNK_RET GetMainMenuMusicOverrideScriptID(int a1);
    /// 0x0879114a
    UNK_RET GetMainMenuMusicNormalScriptID();
    /// 0x087911ae
    UNK_RET IsMainMenuMusicCurrentlyPlaying(int a1);
    /// 0x08791218
    UNK_RET GetMainMenuMusicScriptID(int a1);
};

/// 0x0878f0bf
UNK_RET SaveAudioOptions();
/// 0x0878f0fd
/// lua ?
int LuaThunk_SaveAudioOptions(lua_State* a1);
/// 0x0878f181
int LuaToString_SaveAudioOptions(lua_State* a1);
/// 0x0878f227
UNK_RET LoadAudioOptions();
/// 0x0878f265
/// lua ?
int LuaThunk_LoadAudioOptions(lua_State* a1);
/// 0x0878f294
int LuaToString_LoadAudioOptions(lua_State* a1);
/// 0x0878f307
UNK_RET GetVolumeKnobValue(int a1);
/// 0x0878f357
/// lua ?
int LuaThunk_GetVolumeKnobValue(lua_State* a1);
/// 0x0878f3d0
int LuaToString_GetVolumeKnobValue(lua_State* a1);
/// 0x0878f451
UNK_RET SetVolumeKnobValue(int a1, float a2);
/// 0x0878f49f
/// lua ?
int LuaThunk_SetVolumeKnobValue(lua_State* a1);
/// 0x0878f540
int LuaToString_SetVolumeKnobValue(lua_State* a1);
/// 0x0878f5cf
UNK_RET GetVolumeKnobIDFromName(char const* a1);
/// 0x0878f626
/// lua ?
int LuaThunk_GetVolumeKnobIDFromName(lua_State* a1);
/// 0x0878f6a7
int LuaToString_GetVolumeKnobIDFromName(lua_State* a1);
/// 0x08791254
UNK_RET GetSoundRolloffFactor();
/// 0x08791281
/// lua ?
int LuaThunk_GetSoundRolloffFactor(lua_State* a1);
/// 0x08791310
int LuaToString_GetSoundRolloffFactor(lua_State* a1);
/// 0x08791383
UNK_RET SetSoundRolloffFactor(float a1);
/// 0x087913b8
/// lua ?
int LuaThunk_SetSoundRolloffFactor(lua_State* a1);
/// 0x0879144a
int LuaToString_SetSoundRolloffFactor(lua_State* a1);
/// 0x087914cb
UNK_RET GetSpeakerFalloffFactor();
/// 0x087914f8
/// lua ?
int LuaThunk_GetSpeakerFalloffFactor(lua_State* a1);
/// 0x08791525
int LuaToString_GetSpeakerFalloffFactor(lua_State* a1);
/// 0x08791598
UNK_RET SetSpeakerFalloffFactor(float a1);
/// 0x087915cd
/// lua ?
int LuaThunk_SetSpeakerFalloffFactor(lua_State* a1);
/// 0x087915fc
int LuaToString_SetSpeakerFalloffFactor(lua_State* a1);
/// 0x0879167d
/// lua ?
int LuaThunk_BeginAudioLogging(lua_State* a1);
/// 0x087916da
UNK_RET BeginAudioLogging();
/// 0x087916fd
int LuaToString_BeginAudioLogging(lua_State* a1);
/// 0x08791770
/// lua ?
int LuaThunk_EndAudioLogging(lua_State* a1);
/// 0x087917c8
UNK_RET EndAudioLogging();
/// 0x087917de
int LuaToString_EndAudioLogging(lua_State* a1);
/// 0x08791851
/// lua ?
int LuaThunk_GetAudioLog(lua_State* a1);
/// 0x0879190d
UNK_RET GetAudioLog();
/// 0x0879193d
int LuaToString_GetAudioLog(lua_State* a1);
/// 0x087919b0
/// lua ?
int LuaThunk_SaveAudioLog(lua_State* a1);
/// 0x08791a3e
UNK_RET SaveAudioLog(char const* a1);
/// 0x08791a6c
int LuaToString_SaveAudioLog(lua_State* a1);
