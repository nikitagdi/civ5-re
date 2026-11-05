/// origin guess
#pragma once

class FGXVector3;
class FSound2D;
class FSound3D;
class SSoundScapeElement;
class SoundScriptData;
enum ESpeakerChannels : int;

/// size ?
class FSoundScape {
public:
    /// 0x08d3656c
    FSoundScape();
    /// 0x08d36630
    ~FSoundScape();
    /// 0x08d365ae
    UNK_RET Clear();
    /// 0x08d3665c
    UNK_RET Init(int a1, float a2, FGXVector3 const& a3);
    /// 0x08d369de
    UNK_RET CalculateDelayTime(SoundScriptData const& a1);
    /// 0x08d36a7a
    UNK_RET SetVolume(float a1, bool a2);
    /// 0x08d36ae4
    UNK_RET Deinit();
    /// 0x08d36b5e
    UNK_RET Update(float a1);
    /// 0x08d36c12
    UNK_RET Update2DSounds(float a1);
    /// 0x08d36f4e
    UNK_RET Update3DSounds(float a1);
    /// 0x08d37334
    UNK_RET Pause();
    /// 0x08d373a6
    UNK_RET IsPaused() const;
    /// 0x08d373b6
    UNK_RET Unpause();
    /// 0x08d37428
    UNK_RET SetFadeIn(float a1);
    /// 0x08d3748a
    UNK_RET SetFadeOut(float a1);
    /// 0x08d374d2
    UNK_RET IsPlaying() const;
    /// 0x08d374de
    UNK_RET IsInitialized() const;
    /// 0x08d374ee
    UNK_RET HasSoundsPlaying() const;
    /// 0x08d37522
    UNK_RET PlayAudioBed(bool a1);
    /// 0x08d375fe
    UNK_RET GetVolume() const;
    /// 0x08d37606
    UNK_RET SetTaperVolume(float a1);
    /// 0x08d3766c
    UNK_RET SetChannelVolumes(ESpeakerChannels const* a1, float const* a2, int a3);
    /// 0x08d3776e
    UNK_RET Destroyed2DSoundNotification(FSound2D* a1);
    /// 0x08d37796
    UNK_RET Destroyed3DSoundNotification(FSound3D* a1);
    /// 0x08d377be
    UNK_RET GetScriptId();
};

namespace FSoundScapeMixer {
/// 0x08d37d10
UNK_RET Init();
/// 0x08d37d4a
UNK_RET Deinit();
/// 0x08d37dc0
UNK_RET SoundScapeExists(SSoundScapeElement* a1);
/// 0x08d37e06
UNK_RET Update();
/// 0x08d37ed0
UNK_RET SetSoundTaperVolumes(float a1);
/// 0x08d37f34
UNK_RET Start(int a1, float a2, float a3, FGXVector3 const& a4);
/// 0x08d381b2
UNK_RET SetVolume(float a1);
/// 0x08d381fa
UNK_RET Stop(float a1);
/// 0x08d3829c
UNK_RET IsPlaying();
/// 0x08d3831a
UNK_RET GetVolume();
/// 0x08d38322
UNK_RET SetTaperVolume(float a1);
/// 0x08d38332
UNK_RET SetTaperTimer(float a1);
}
