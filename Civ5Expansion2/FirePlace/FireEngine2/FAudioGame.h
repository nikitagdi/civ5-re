/// origin binary
#pragma once
#include "FirePlace/FireEngine2/FGameListener.h"

class FBaseAudioComponent;
class FFileBackgroundLoader;
class FGXVector3;
class FGameIniParser;
class FGrannyAssetInstance;
class FSound3D;
class FSoundBase;
class FSoundScape;
class FStringArray;
class FTransformNode;
enum EAudioScriptType : int;
enum EAudioTag : int;
template <class T0> class FTPointer;

/// size ?
/// vtable 0x0a3018e8
class FAudioGame {
public:
    /// 0x08cbb258
    FAudioGame();
    /// 0x08cbb352
    ~FAudioGame();
    /// 0x08cbc84c
    virtual UNK_RET AdjustSoundDelta(float* a1, float* a2, FTPointer<FTransformNode> a3, FGXVector3 const& a4);
    /// 0x08cbb42c
    UNK_RET Shutdown();
    /// 0x08cbb50a
    UNK_RET HotLoadAudioFile(char const* a1);
    /// 0x08cbb510
    UNK_RET InitFiresoundAudioManager(bool a1);
    /// 0x08cbb638
    UNK_RET Do2DSound(int a1, float a2, float a3, int a4, float a5, int a6, int a7, int a8);
    /// 0x08cbb6ea
    UNK_RET Init(FGameIniParser* a1);
    /// 0x08cbba82
    UNK_RET ClearTrackedSound(int a1);
    /// 0x08cbbab2
    UNK_RET CacheLoopingInfo();
    /// 0x08cbbaf6
    UNK_RET RequestNewTrackedLoopStart(unsigned int a1, int a2, bool& a3, FGXVector3 const& a4);
    /// 0x08cbbbde
    UNK_RET StopTrackedSound(unsigned int a1, int a2);
    /// 0x08cbbc74
    UNK_RET OnTriggeredTrackedSound(FSound3D* a1, unsigned int a2, bool a3);
    /// 0x08cbbcf4
    UNK_RET EnableHotLoading() const;
    /// 0x08cbbdec
    UNK_RET StartCaptureOutputToWAV(char const* a1);
    /// 0x08cbbe36
    UNK_RET StopCaptureOutputToWAV();
    /// 0x08cbbe70
    UNK_RET SetVolumeKnobVolume(int a1, float a2);
    /// 0x08cbbeca
    UNK_RET GetVolumeKnob(int a1) const;
    /// 0x08cbbf10
    UNK_RET FadeVolumeKnobVolume(char const* a1, float a2, float a3);
    /// 0x08cbbfa6
    UNK_RET FadeVolumeKnobVolume(int a1, float a2, float a3);
    /// 0x08cbc00c
    UNK_RET GetAudioTagIndex(EAudioTag a1, char const* a2, EAudioScriptType a3, bool a4);
    /// 0x08cbc064
    UNK_RET SetKnobLowPassCutoffLevel(int a1, float a2);
    /// 0x08cbc0ca
    UNK_RET LoadContext(int a1, int a2, int a3);
    /// 0x08cbc116
    UNK_RET UnloadContext(int a1);
    /// 0x08cbc148
    UNK_RET SetListenerType(FGameListener::EListenerType a1);
    /// 0x08cbc178
    UNK_RET Update(float a1, bool a2);
    /// 0x08cbc1e4
    UNK_RET UpdateFiresoundAudioManager();
    /// 0x08cbc21e
    UNK_RET UpdateTrackedSounds(float a1);
    /// 0x08cbc2f0
    UNK_RET UpdateDebug();
    /// 0x08cbc318
    UNK_RET UpdateDebugSounds();
    /// 0x08cbc3ca
    UNK_RET UpdateDebugSurroundSoundTest();
    /// 0x08cbc57a
    UNK_RET StopSound(FSoundBase*& a1, float a2);
    /// 0x08cbc5f4
    UNK_RET StopAllMusic(float a1);
    /// 0x08cbc632
    UNK_RET Do3DSound(int a1, FTPointer<FTransformNode> a2, FGXVector3 const& a3, float a4, int a5, float a6, float a7, float a8, int a9, int a10, int a11);
    /// 0x08cbc806
    UNK_RET StopAllSounds(bool a1);
    /// 0x08cbc862
    UNK_RET SetScriptIDError(int a1, bool a2);
    /// 0x08cbc89c
    UNK_RET IsMusicPlaying();
    /// 0x08cbc8d6
    UNK_RET PlayMusic(int a1, float a2, float a3, int a4);
    /// 0x08cbc936
    UNK_RET RestartLastSongPlayed(float a1, float a2);
    /// 0x08cbc986
    UNK_RET StopMusicIfPlaying(int a1, float a2);
    /// 0x08cbc9ce
    UNK_RET GetScriptIDError(int a1) const;
    /// 0x08cbca24
    UNK_RET GetAudioTagIndex(EAudioTag a1, unsigned int a2, EAudioScriptType a3);
    /// 0x08cbca6e
    UNK_RET DoSoundScape(int a1, float a2, float a3, FGXVector3 const& a4);
    /// 0x08cbcad4
    UNK_RET EndSoundScape(FSoundScape* a1, float a2);
    /// 0x08cbcb22
    UNK_RET DoSoundScapeWithMixer(int a1, float a2, float a3);
    /// 0x08cbcb9c
    UNK_RET EndSoundScapeMixer(float a1);
    /// 0x08cbcbe2
    UNK_RET ProcessAnimationSoundMarker(char const* a1, FTPointer<FTransformNode> a2, FBaseAudioComponent* a3, float a4, int a5, int a6);
    /// 0x08cbd002
    UNK_RET GetScriptTypeFromName(char const* a1);
    /// 0x08cbd088
    UNK_RET SetVolumeForSoundsOfSoundType(char const* a1, float a2);
    /// 0x08cbd0c6
    UNK_RET GetScriptTypeFromScriptNameHash(unsigned int a1);
    /// 0x08cbd0f4
    UNK_RET StartMechLoop(FGrannyAssetInstance const* a1, char const* a2, char const* a3, float a4);
    /// 0x08cbd0f6
    UNK_RET StopMechLoop(FGrannyAssetInstance const* a1, char const* a2);
    /// 0x08cbd0f8
    UNK_RET SetMarkersEnabledOnAsset(FGrannyAssetInstance const* a1, bool a2);
    /// 0x08cbd0fa
    UNK_RET StopAllMarkers(FGrannyAssetInstance const* a1);
    /// 0x08cbd0fc
    UNK_RET GetSoundFileNames(char const* a1, FStringArray& a2);
    /// 0x08cbd572
    UNK_RET DoSound(char const* a1, float a2, float a3, int a4);
    /// 0x08cbd576
    UNK_RET Add3DDebugSound(FSound3D* a1);
    /// 0x08cbd578
    UNK_RET SetDistanceCheckOptimizationEnabled(bool a1);
    /// 0x08cbd5ac
    UNK_RET GetBackgroundLoader() const;
    /// 0x08cbd5ce
    UNK_RET SetBackgroundLoader(FFileBackgroundLoader* a1);
    /// 0x08cbd610
    UNK_RET FlushCache(bool a1);
    /// 0x08cbd69c
    UNK_RET PreloadSound(int a1, EAudioTag a2);
    /// 0x08cbd6ee
    UNK_RET Preload2DSound(char const* a1, bool a2, bool a3);
    /// 0x08cbd82a
    UNK_RET Preload3DSound(char const* a1, bool a2, bool a3);
    /// 0x08cbd966
    UNK_RET IsSpecificMusicPlaying(int a1);
    /// 0x08cbd9b0
    UNK_RET PlayMusicIfNotAlreadyPlaying(int a1, float a2, float a3);
    /// 0x08cbda36
    UNK_RET PreloadContextOfDynamicResSounds(int a1);
    /// 0x08cbda80
    UNK_RET SetDuplicateFilePrefix(char const* a1);
    /// 0x08cbdac2
    UNK_RET PlayMenuScrollSound();
    /// 0x08cbdbb2
    UNK_RET PlayMenuAcceptSound();
    /// 0x08cbdca2
    UNK_RET PlayMenuCancelSound();
    /// 0x08cbdd92
    UNK_RET IsContextLoaded(int a1);
    /// 0x08cbdddc
    UNK_RET SetMusicShouldRestartOnStoppedUserMediaPlayer(bool a1);
    /// 0x08cbde0e
    UNK_RET MuteAllCurrentlyPlayingSpeech();
    /// 0x0d267db0
    static UNK_DATA m_pInst[0x4];
};

namespace FAxisGlobals {
/// 0x0d267e78
extern UNK_DATA ms_bIsInTool[0x1];
/// 0x0d267e79
extern UNK_DATA ms_bOverrideMiddleWareAllocators[0x1];
}
