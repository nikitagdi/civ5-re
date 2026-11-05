/// origin guess
#pragma once
#include "FirePlace/FireWorks/FAudioSystem.h"
#include "FirePlace/FireWorks/FString.h"
#include <miles.h>

class FGXVector3;
class SAudioSystemInit;
enum ESpeakerChannels : int;

/// size ?
/// vtable 0x0a3038e0
class FAudioSystemMiles : public FAudioSystem {
public:
    class MilesStream;
    /// 0x08d399fe
    FAudioSystemMiles();
    /// 0x08d3c5aa
    virtual UNK_RET AcquireSoundHandleId(FAudioSystem::ESoundType a1);
    /// 0x08d3aabe
    virtual UNK_RET AttachVolumeRampFilter(FAudioSystem::ESoundType a1, int a2);
    /// 0x08d3aa8a
    virtual UNK_RET Calculate3DChannelVolumes(FAudioSystem::ChannelVolumeInfo& a1, float* a2);
    /// 0x08d3ba8c
    virtual UNK_RET CalculateNumSamples(SAudioSystemInit* a1);
    /// 0x08d3bbe8
    virtual UNK_RET Deinit2DSamples();
    /// 0x08d3bef4
    virtual UNK_RET Deinit2DStreams();
    /// 0x08d3bd08
    virtual UNK_RET Deinit3DSamples();
    /// 0x08d3c0e4
    virtual UNK_RET Deinit3DStreams();
    /// 0x08d3c1b2
    virtual UNK_RET DeinitHandles(FAudioSystem::ESoundType a1);
    /// 0x08d3a9e6
    virtual UNK_RET DeinitListener();
    /// 0x08d3a2de
    virtual UNK_RET DeinitSoundHandle(FAudioSystem::ESoundType a1, int a2);
    /// 0x08d39e0a
    virtual UNK_RET DeinitSystem();
    /// 0x08d3b7ee
    virtual UNK_RET Get3DSoundConeValues(FAudioSystem::ESoundType a1, int a2, int& a3, int& a4, float& a5);
    /// 0x08d3b74c
    virtual UNK_RET Get3DSoundDistanceValues(FAudioSystem::ESoundType a1, int a2, float& a3, float& a4);
    /// 0x08d3b89a
    virtual UNK_RET Get3DSoundOrientation(FAudioSystem::ESoundType a1, int a2, FGXVector3& a3, FGXVector3& a4);
    /// 0x08d3b634
    virtual UNK_RET Get3DSoundPosition(FAudioSystem::ESoundType a1, int a2, FGXVector3& a3);
    /// 0x08d3b6c0
    virtual UNK_RET Get3DSoundVelocity(FAudioSystem::ESoundType a1, int a2, FGXVector3& a3);
    /// 0x08d39e64
    virtual UNK_RET GetLowLevelSoundObject(void* a1);
    /// 0x08d3af34
    virtual UNK_RET GetPlaybackRate(FAudioSystem::ESoundType a1, int a2, unsigned int& a3);
    /// 0x08d3b934
    virtual UNK_RET GetReverbLevels(FAudioSystem::ESoundType a1, int a2, float& a3, float& a4);
    /// 0x08d3b01c
    virtual UNK_RET GetSoundLooping(FAudioSystem::ESoundType a1, int a2, int& a3);
    /// 0x08d3b390
    virtual UNK_RET GetSoundPan(FAudioSystem::ESoundType a1, int a2, float& a3);
    /// 0x08d3b50c
    virtual UNK_RET GetSoundPlaybackPosition(FAudioSystem::ESoundType a1, int a2, unsigned int& a3);
    /// 0x08d3c772
    virtual UNK_RET GetSoundTotalPlaybackLength(FAudioSystem::ESoundType a1, int a2, int& a3);
    /// 0x08d3b144
    virtual UNK_RET GetSoundVolume(FAudioSystem::ESoundType a1, int a2, float& a3);
    /// 0x08d39e88
    virtual UNK_RET GetSpeakerConfigValue(int a1);
    /// 0x08d3ac06
    virtual UNK_RET GetVolumeRampCurrentVolume(FAudioSystem::ESoundType a1, int a2, float& a3);
    /// 0x08d3bb2c
    virtual UNK_RET Init2DSamples();
    /// 0x08d3bd6c
    virtual UNK_RET Init2DStreams();
    /// 0x08d3bc4c
    virtual UNK_RET Init3DSamples();
    /// 0x08d3bf5c
    virtual UNK_RET Init3DStreams();
    /// 0x08d3c14c
    virtual UNK_RET InitHandles(FAudioSystem::ESoundType a1);
    /// 0x08d3a9e0
    virtual UNK_RET InitListener();
    /// 0x08d3a286
    virtual UNK_RET InitSoundHandle(FAudioSystem::ESoundType a1, void* a2, unsigned int a3, char const* a4);
    /// 0x08d39ac2
    virtual UNK_RET InitSystem(SAudioSystemInit* a1);
    /// 0x08d3ad8e
    virtual UNK_RET PauseSound(FAudioSystem::ESoundType a1, int a2, bool a3);
    /// 0x08d3ac5c
    virtual UNK_RET PlaySoundA(FAudioSystem::ESoundType a1, int a2);
    /// 0x08d3aed8
    virtual UNK_RET PrimeBuffer(FAudioSystem::ESoundType a1, int a2);
    /// 0x08d3c626
    virtual UNK_RET ReleaseSoundHandleId(FAudioSystem::ESoundType a1, int a2);
    /// 0x08d3ae4e
    virtual UNK_RET RestartSound(FAudioSystem::ESoundType a1, int a2);
    /// 0x08d3b7f4
    virtual UNK_RET Set3DSoundConeValues(FAudioSystem::ESoundType a1, int a2, int a3, int a4, float a5);
    /// 0x08d3b752
    virtual UNK_RET Set3DSoundDistanceValues(FAudioSystem::ESoundType a1, int a2, float a3, float a4);
    /// 0x08d3b8a0
    virtual UNK_RET Set3DSoundOrientation(FAudioSystem::ESoundType a1, int a2, FGXVector3 const& a3, FGXVector3 const& a4);
    /// 0x08d3b63a
    virtual UNK_RET Set3DSoundPosition(FAudioSystem::ESoundType a1, int a2, FGXVector3 const& a3);
    /// 0x08d3b6c6
    virtual UNK_RET Set3DSoundVelocity(FAudioSystem::ESoundType a1, int a2, FGXVector3 const& a3);
    /// 0x08d3a46e
    virtual UNK_RET SetChannelVolumes(FAudioSystem::ESoundType a1, int a2, ESpeakerChannels const* a3, float const* a4, int a5);
    /// 0x08d3c730
    virtual UNK_RET SetDebugFlag(FAudioSystem::ESoundType a1, int a2, bool a3);
    /// 0x08d3a878
    virtual UNK_RET SetDistanceFactor(float a1);
    /// 0x08d3a8e8
    virtual UNK_RET SetDopplerFactor(float a1);
    /// 0x08d3a9ae
    virtual UNK_RET SetGlobalEffectsType(int a1);
    /// 0x08d3a91e
    virtual UNK_RET SetGlobalReverbAttributes(float a1, float a2, float a3);
    /// 0x08d3a96c
    virtual UNK_RET SetGlobalReverbLevels(float a1, float a2);
    /// 0x08d3aa50
    virtual UNK_RET SetListenerOrientation(FGXVector3 const& a1, FGXVector3 const& a2);
    /// 0x08d3a9ec
    virtual UNK_RET SetListenerPosition(FGXVector3 const& a1);
    /// 0x08d3aa1e
    virtual UNK_RET SetListenerVelocity(FGXVector3 const& a1);
    /// 0x08d3a81e
    virtual UNK_RET SetLowPassFilterCutoff(FAudioSystem::ESoundType a1, int a2, float a3);
    /// 0x08d3a5c8
    virtual UNK_RET SetOutputChannel(FAudioSystem::ESoundType a1, int a2, ESpeakerChannels a3);
    /// 0x08d3af84
    virtual UNK_RET SetPlaybackRate(FAudioSystem::ESoundType a1, int a2, unsigned int a3);
    /// 0x08d3b9da
    virtual UNK_RET SetReverbLevels(FAudioSystem::ESoundType a1, int a2, float a3, float a4);
    /// 0x08d3a8ae
    virtual UNK_RET SetRolloffFactor(float a1);
    /// 0x08d3b0ac
    virtual UNK_RET SetSoundLooping(FAudioSystem::ESoundType a1, int a2, int a3);
    /// 0x08d3b426
    virtual UNK_RET SetSoundPan(FAudioSystem::ESoundType a1, int a2, float a3, bool a4);
    /// 0x08d3b59c
    virtual UNK_RET SetSoundPlaybackPosition(FAudioSystem::ESoundType a1, int a2, unsigned int a3);
    /// 0x08d3b21a
    virtual UNK_RET SetSoundVolume(FAudioSystem::ESoundType a1, int a2, float a3);
    /// 0x08d3c694
    virtual UNK_RET SetSpeakerFalloffPower(float a1);
    /// 0x08d3ab0c
    virtual UNK_RET SetVolumeRamp(FAudioSystem::ESoundType a1, int a2, float a3, float a4, bool a5, float a6);
    /// 0x08d3c6c8
    virtual UNK_RET SetupCustomFalloffFunction(FAudioSystem::ESoundType a1, int a2, int a3);
    /// 0x08d39a4e
    virtual UNK_RET StartCaptureOutputToWAV(char const* a1);
    /// 0x08d39a92
    virtual UNK_RET StopCaptureOutputToWAV();
    /// 0x08d3ad04
    virtual UNK_RET StopSound(FAudioSystem::ESoundType a1, int a2);
    /// 0x08d3c882
    virtual UNK_RET vf_11(UNK_PARAMS);
    /// 0x08d3c88a
    virtual UNK_RET vf_59(UNK_PARAMS);
    /// 0x08d39a38
    ~FAudioSystemMiles();
    /// 0x08d399bc
    virtual UNK_RET Print51Info(int a1, FStringA& a2);
    /// 0x08d39a4c
    UNK_RET LogHardwareData();
    /// 0x08d39eac
    UNK_RET InitSoundHandle_Sample(FAudioSystem::ESoundType a1, void* a2, unsigned int a3, char const* a4);
    /// 0x08d39fd4
    UNK_RET ComputeFileHandleNameForStream(FStringA& a1, FAudioSystemMiles::MilesStream* a2, char const* a3) const;
    /// 0x08d3a03c
    UNK_RET InitSoundHandle_Streamed(FAudioSystem::ESoundType a1, void* a2, unsigned int a3, char const* a4);
    /// 0x08d3a38a
    UNK_RET SetupSampleDoneCallback(FAudioSystem::ESoundType a1, int a2, bool a3);
    /// 0x08d3a558
    UNK_RET GetSampleHandle(FAudioSystem::ESoundType a1, int a2);
    /// 0x08d3c218
    UNK_RET Acquire2DSampleId();
    /// 0x08d3c27a
    UNK_RET Release2DSampleId(int a1);
    /// 0x08d3c28e
    UNK_RET Acquire3DSampleId();
    /// 0x08d3c2f0
    UNK_RET Release3DSampleId(int a1);
    /// 0x08d3c304
    UNK_RET Acquire2DStreamId();
    /// 0x08d3c330
    UNK_RET Release2DStreamId(int a1);
    /// 0x08d3c392
    UNK_RET Acquire3DStreamId();
    /// 0x08d3c3be
    UNK_RET Release3DStreamId(int a1);
    /// 0x08d3c420
    UNK_RET Get2DSampleIndex(_SAMPLE* a1);
    /// 0x08d3c452
    UNK_RET Get3DSampleIndex(_SAMPLE* a1);
    /// 0x08d3c484
    UNK_RET Get2DStreamIndex(_STREAM* a1);
    /// 0x08d3c4b8
    UNK_RET Get3DStreamIndex(_STREAM* a1);
    /// 0x08d3c4ec
    UNK_RET HandleIsAvailable(FAudioSystem::ESoundType a1);
};

/// 0x08d3c89c
UNK_RET FMilesDebugTraceCallback(char* a1, int a2);
/// 0x08d3c8f0
UNK_RET FMilesFileOpenCallback(char const* a1, unsigned int* a2);
/// 0x08d3c970
UNK_RET FMilesFileCloseCallback(unsigned int a1);
/// 0x08d3c9a1
UNK_RET FMilesFileSeekCallback(unsigned int a1, int a2, unsigned int a3);
/// 0x08d3c9fb
UNK_RET FMilesFileReadCallback(unsigned int a1, void* a2, unsigned int a3);

namespace FMiles {
/// 0x08d3ca46
UNK_RET Startup(int a1, int a2, int a3);
/// 0x08d3cabe
UNK_RET WinPCPreInit();
/// 0x08d3cac4
UNK_RET WinPCPostInit();
/// 0x08d3caca
UNK_RET SetPreference(unsigned int a1, int a2);
/// 0x08d3cade
UNK_RET OpenDriver(unsigned int a1, int a2, int a3, unsigned int a4);
/// 0x08d3cb36
UNK_RET XenonPreInit();
/// 0x08d3cb3c
UNK_RET PS3PreInit();
/// 0x08d3cb40
UNK_RET XenonPostInit();
/// 0x08d3cb46
UNK_RET PS3PostInit();
/// 0x08d3cb4c
UNK_RET Shutdown();
/// 0x08d3cb60
UNK_RET GetDirectSoundObject(void* a1);
/// 0x08d3cb62
UNK_RET Print51Info(_SAMPLE* a1, FStringA& a2);
/// 0x08d3cce4
UNK_RET CloseDriver(_DIG_DRIVER* a1);
/// 0x08d3ccf8
UNK_RET SetRedistDirectory(FStringA* a1);
/// 0x08d3cd1c
UNK_RET FindActualConfiguration(int a1, int a2);
/// 0x08d3cd5a
UNK_RET SetSpeakerFalloffPower(_DIG_DRIVER* a1, float a2);
/// 0x08d3cd9c
UNK_RET SetCustomFalloffFunction(_SAMPLE* a1, float (*a2)(_SAMPLE*, float, float, float, float));
/// 0x08d3cdb0
UNK_RET StartCaptureOutputToWavFile(_DIG_DRIVER* a1, char const* a2);
/// 0x08d3ce6a
UNK_RET StopCaptureOutputToWavFile(int a1);
/// 0x08d3ce7e
UNK_RET SetMallocCallback(void* (*a1)(unsigned int));
/// 0x08d3ce92
UNK_RET SetFreeCallback(void (*a1)(void*));
/// 0x08d3cea6
UNK_RET SetListenerPosition(_DIG_DRIVER* a1, FGXVector3 const& a2);
/// 0x08d3ceec
UNK_RET SetListenerVelocity(_DIG_DRIVER* a1, FGXVector3 const& a2);
/// 0x08d3ceee
UNK_RET SetListenerOrientation(_DIG_DRIVER* a1, FGXVector3 const& a2, FGXVector3 const& a3);
/// 0x08d3cf58
UNK_RET Calculate3DChannelVolumes(_DIG_DRIVER* a1, FAudioSystem::ChannelVolumeInfo& a2, float* a3);
/// 0x08d3d0d6
UNK_RET SetDistanceFactor(_DIG_DRIVER* a1, float a2);
/// 0x08d3d0ea
UNK_RET SetRolloffFactor(_DIG_DRIVER* a1, float a2);
/// 0x08d3d0fe
UNK_RET SetDopplerFactor(_DIG_DRIVER* a1, float a2);
/// 0x08d3d112
UNK_RET SetRoomType(_DIG_DRIVER* a1, int a2);
/// 0x08d3d114
UNK_RET SetReverbAttributes(_DIG_DRIVER* a1, float a2, float a3, float a4);
/// 0x08d3d116
UNK_RET SetGlobalReverbLevels(_DIG_DRIVER* a1, float a2, float a3);
/// 0x08d3d118
UNK_RET AllocateSampleHandle(_DIG_DRIVER* a1);
/// 0x08d3d12c
UNK_RET DeinitSample(_SAMPLE* a1);
/// 0x08d3d140
UNK_RET GetSampleStatus(_SAMPLE* a1);
/// 0x08d3d154
UNK_RET EndSample(_SAMPLE* a1);
/// 0x08d3d168
UNK_RET EndStream(_STREAM* a1);
/// 0x08d3d192
UNK_RET Stop_Stream(_STREAM* a1);
/// 0x08d3d1bc
UNK_RET OpenStream(_DIG_DRIVER* a1, FStringA const& a2, int a3);
/// 0x08d3d1f0
UNK_RET GetLastError();
/// 0x08d3d204
UNK_RET CloseStream(_STREAM* a1);
/// 0x08d3d218
UNK_RET ServiceStream(_STREAM* a1, int a2);
/// 0x08d3d22c
UNK_RET Play_Sample(_SAMPLE* a1);
/// 0x08d3d240
UNK_RET Play_Stream(_STREAM* a1);
/// 0x08d3d254
UNK_RET Stop_Sample(_SAMPLE* a1);
/// 0x08d3d268
UNK_RET Pause_Sample(_SAMPLE* a1, bool a2);
/// 0x08d3d28c
UNK_RET Pause_Stream(_STREAM* a1, bool a2);
/// 0x08d3d2b6
UNK_RET Restart_Sample(_SAMPLE* a1);
/// 0x08d3d2ca
UNK_RET Restart_Stream(_STREAM* a1);
/// 0x08d3d2de
UNK_RET AttachFilter(_SAMPLE* a1, SAMPLESTAGE a2, unsigned int a3);
/// 0x08d3d31c
UNK_RET SetSampleFile(_SAMPLE* a1, void const* a2, int a3, char const* a4);
/// 0x08d3d360
UNK_RET RegisterCallback_SampleDone(_SAMPLE* a1, void (*a2)(_SAMPLE*));
/// 0x08d3d374
UNK_RET RegisterCallback_StreamDone(_STREAM* a1, void (*a2)(_STREAM*));
/// 0x08d3d388
UNK_RET GetPlaybackRate(_SAMPLE* a1);
/// 0x08d3d39c
UNK_RET SetPlaybackRate_Sample(_SAMPLE* a1, unsigned int a2);
/// 0x08d3d3b0
UNK_RET SetPlaybackRate_Stream(_STREAM* a1, unsigned int a2);
/// 0x08d3d3e4
UNK_RET GetLooping_Sample(_SAMPLE* a1);
/// 0x08d3d3f8
UNK_RET GetLooping_Stream(_STREAM* a1);
/// 0x08d3d40c
UNK_RET SetLooping_Sample(_SAMPLE* a1, int a2);
/// 0x08d3d420
UNK_RET SetLooping_Stream(_STREAM* a1, int a2);
/// 0x08d3d434
UNK_RET GetVolumeAndPan_Sample2D(_SAMPLE* a1, float& a2, float& a3);
/// 0x08d3d448
UNK_RET GetVolume_Sample3D(_SAMPLE* a1, float& a2);
/// 0x08d3d47a
UNK_RET GetVolumeAndPan_Stream2D(_STREAM* a1, float& a2, float& a3);
/// 0x08d3d4b8
UNK_RET GetVolume_Stream3D(_STREAM* a1, float& a2);
/// 0x08d3d4f4
UNK_RET SetVolumeAndPan_Sample2D(_SAMPLE* a1, float a2, float a3);
/// 0x08d3d508
UNK_RET SetVolume_Sample3D(_SAMPLE* a1, float a2);
/// 0x08d3d53e
UNK_RET SetVolumeAndPan_Stream2D(_STREAM* a1, float a2, float a3);
/// 0x08d3d580
UNK_RET SetVolume_Stream3D(_STREAM* a1, float a2);
/// 0x08d3d5be
UNK_RET GetTotalLength_Sample(_SAMPLE* a1);
/// 0x08d3d5f4
UNK_RET GetTotalLength_Stream(_STREAM* a1);
/// 0x08d3d62a
UNK_RET GetPlaybackPosition_Sample(_SAMPLE* a1);
/// 0x08d3d660
UNK_RET GetPlaybackPosition_Stream(_STREAM* a1);
/// 0x08d3d696
UNK_RET SetPlaybackPosition_Sample(_SAMPLE* a1, unsigned int a2);
/// 0x08d3d6aa
UNK_RET SetPlaybackPosition_Stream(_STREAM* a1, unsigned int a2);
/// 0x08d3d6be
UNK_RET SetPosition_Sample(_SAMPLE* a1, FGXVector3 const& a2);
/// 0x08d3d704
UNK_RET SetPosition_Stream(_STREAM* a1, FGXVector3 const& a2);
/// 0x08d3d754
UNK_RET SetVelocity_Sample(_SAMPLE* a1, FGXVector3 const& a2);
/// 0x08d3d79a
UNK_RET SetVelocity_Stream(_STREAM* a1, FGXVector3 const& a2);
/// 0x08d3d7ea
UNK_RET SetDistances_Sample(_SAMPLE* a1, float a2, float a3);
/// 0x08d3d82c
UNK_RET SetDistances_Stream(_STREAM* a1, float a2, float a3);
/// 0x08d3d876
UNK_RET SetCone_Sample(_SAMPLE* a1, int a2, int a3, float a4);
/// 0x08d3d8bc
UNK_RET SetCone_Stream(_STREAM* a1, int a2, int a3, float a4);
/// 0x08d3d90a
UNK_RET SetOrientation_Sample(_SAMPLE* a1, FGXVector3 const& a2, FGXVector3 const& a3);
/// 0x08d3d974
UNK_RET SetOrientation_Stream(_STREAM* a1, FGXVector3 const& a2, FGXVector3 const& a3);
/// 0x08d3d9ea
UNK_RET GetReverbLevels_Sample(_SAMPLE* a1, float& a2, float& a3);
/// 0x08d3d9fe
UNK_RET GetReverbLevels_Stream(_STREAM* a1, float& a2, float& a3);
/// 0x08d3da3c
UNK_RET SetReverbLevels_Sample(_SAMPLE* a1, float a2, float a3);
/// 0x08d3da50
UNK_RET SetReverbLevels_Stream(_STREAM* a1, float a2, float a3);
/// 0x08d3da92
UNK_RET GetCalculated3DChannelVolumes(_SAMPLE* a1, float* a2, int a3);
/// 0x08d3db32
UNK_RET SetVolumeRampFilterParam(_SAMPLE* a1, char const* a2, float a3);
/// 0x08d3db98
UNK_RET GetVolumeRampFilterParam(_SAMPLE* a1, char const* a2, float& a3);
}
