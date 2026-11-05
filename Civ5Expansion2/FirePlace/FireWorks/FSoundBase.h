/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FSharedSoundData;
class FSoundBase;
class FSoundInit;
enum EFadeState : int;
enum EScriptDisabledFlags : int;
enum ESpeakerChannels : int;

/// size ?
/// vtable 0x0a303800
class FSoundBase {
public:
    /// 0x08d332b6
    virtual UNK_RET Init(FSoundInit const& a1);
    /// 0x08d331f8
    virtual UNK_RET Clear();
    /// 0x08d35168
    virtual UNK_RET Update(float a1);
    /// 0x08d33ce4
    virtual UNK_RET Play();
    /// pure
    virtual UNK_RET Destroy() = 0;
    /// pure
    virtual UNK_RET Load(FStringA* a1) = 0;
    /// 0x08d35236
    virtual UNK_RET FinishLoading();
    /// pure
    virtual UNK_RET InsertSoundIntoAudioManager() = 0;
    /// pure
    virtual UNK_RET RemoveSoundWithLowestPriority() = 0;
    /// 0x08d34d0a
    virtual UNK_RET ApplyScript();
    /// pure
    virtual UNK_RET NotifyAudioManagerSoundUnloaded() = 0;
    /// pure
    virtual UNK_RET InitSoundType() = 0;
    /// 0x08d34e08
    virtual UNK_RET ComputePriorityScoreVersus(FSoundBase const* a1) const;
    /// 0x08d33340
    UNK_RET BaseLoad(FStringA* a1, bool a2);
    /// 0x08d33606
    UNK_RET ComputeWhichVariationToUse(FStringA const& a1);
    /// 0x08d336a6
    UNK_RET InitHandle(char const* a1);
    /// 0x08d33752
    UNK_RET SetSharedData(FSharedSoundData* a1);
    /// 0x08d3376c
    UNK_RET GetLastPlayTime() const;
    /// 0x08d337a6
    UNK_RET IsPlaying() const;
    /// 0x08d337ee
    UNK_RET PrimeBuffer();
    /// 0x08d33842
    UNK_RET Pause();
    /// 0x08d338e2
    UNK_RET Restart();
    /// 0x08d3396e
    UNK_RET OnPlaybackActivated();
    /// 0x08d339ee
    UNK_RET SetToBeDestroyed(bool a1);
    /// 0x08d33a42
    UNK_RET OnAudioSystemPlaybackEnded();
    /// 0x08d33ae2
    UNK_RET OnPlaybackDeactivated();
    /// 0x08d33b5e
    UNK_RET ApplyScriptRandomPosition();
    /// 0x08d33c36
    UNK_RET GetSoundTotalPlaybackLength() const;
    /// 0x08d33c94
    UNK_RET SetPlaybackPosition(unsigned int a1);
    /// 0x08d33ece
    UNK_RET SetLooping(bool a1, int a2);
    /// 0x08d33f38
    UNK_RET Stop();
    /// 0x08d33fbe
    UNK_RET Unload();
    /// 0x08d340a2
    UNK_RET IsLooping() const;
    /// 0x08d340b2
    UNK_RET GetFileSize() const;
    /// 0x08d340ce
    UNK_RET GetReverbLevels(float& a1, float& a2) const;
    /// 0x08d340ee
    UNK_RET SetReverbLevels(float a1, float a2);
    /// 0x08d34196
    UNK_RET SetChannelVolumes(ESpeakerChannels const* a1, float const* a2, int a3);
    /// 0x08d3423c
    UNK_RET GetCurrentVolume() const;
    /// 0x08d342b8
    UNK_RET GetPlaybackPosition() const;
    /// 0x08d34316
    UNK_RET SetPitchChange(int a1);
    /// 0x08d343c4
    UNK_RET GetDisabledFlag(EScriptDisabledFlags a1);
    /// 0x08d343da
    UNK_RET SetDisabledFlag(EScriptDisabledFlags a1);
    /// 0x08d343ea
    UNK_RET GetFadeState();
    /// 0x08d3440e
    UNK_RET SetFadeState(EFadeState a1);
    /// 0x08d34446
    UNK_RET UpdateFade();
    /// 0x08d34508
    UNK_RET Alternate_UpdateFade();
    /// 0x08d34612
    UNK_RET SetVolume(float a1, bool a2);
    /// 0x08d34670
    UNK_RET SetFadeOut(float a1, float a2, float a3, float a4, bool a5);
    /// 0x08d34756
    UNK_RET SetFadeIn(float a1, float a2, float a3, float a4);
    /// 0x08d3483c
    UNK_RET DontTriggerDuplicatesOnAssets() const;
    /// 0x08d3485a
    UNK_RET UpdateVolume();
    /// 0x08d348be
    UNK_RET GetCurrentLowPassCutoffLevel() const;
    /// 0x08d34906
    UNK_RET ApplyScriptVolume();
    /// 0x08d349c2
    UNK_RET ApplyScriptPitchChange();
    /// 0x08d34a70
    UNK_RET ApplyScriptReverb();
    /// 0x08d34aaa
    UNK_RET ApplyScriptTimeEnded();
    /// 0x08d34d5c
    UNK_RET InitAndLoad(FSoundInit const& a1, bool a2, bool a3);
    /// 0x08d34ee8
    UNK_RET operator<(FSoundBase const& a1) const;
    /// 0x08d34f14
    UNK_RET Alternate_FadeIn(float a1, bool a2);
    /// 0x08d34fea
    UNK_RET Alternate_FadeOut(float a1, float a2, bool a3);
    /// 0x08d350e0
    UNK_RET Alternate_GetFadeCurrentVolume();
    /// 0x08d35154
    UNK_RET Alternate_IsDoneFading();
    /// 0x08d351d2
    UNK_RET UpdateLoadingSound(float a1);
    /// 0x08d353dc
    UNK_RET OnDataFinishedLoading();
    /// 0x08d35426
    UNK_RET SetLoopMaxLengthLimit(float a1);
};
