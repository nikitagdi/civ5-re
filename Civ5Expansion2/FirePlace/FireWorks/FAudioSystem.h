/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

class FGXVector3;
class SAudioSystemInit;
enum ESpeakerChannels : int;

/// size ?
/// vtable 0x0a303590
class FAudioSystem {
public:
    class ChannelVolumeInfo;
    enum ESoundType : int;
    /// 0x08d2be50
    FAudioSystem();
    /// 0x08d2be7a
    ~FAudioSystem();
    /// 0x08d2beb6
    virtual UNK_RET InitSystem(SAudioSystemInit* a1);
    /// 0x08d2bf42
    virtual UNK_RET DeinitSystem();
    /// 0x08d2be7c
    virtual UNK_RET Update(float a1);
    /// pure
    virtual UNK_RET GetLowLevelSoundObject(void* a1) = 0;
    /// pure
    virtual UNK_RET GetSpeakerConfigValue(int a1) = 0;
    /// pure
    virtual UNK_RET InitSoundHandle(FAudioSystem::ESoundType a1, void* a2, unsigned int a3, char const* a4) = 0;
    /// pure
    virtual UNK_RET DeinitSoundHandle(FAudioSystem::ESoundType a1, int a2) = 0;
    /// pure
    virtual UNK_RET SetDistanceFactor(float a1) = 0;
    /// pure
    virtual UNK_RET SetRolloffFactor(float a1) = 0;
    /// pure
    virtual UNK_RET SetDopplerFactor(float a1) = 0;
    /// pure
    virtual UNK_RET SetSpeakerFalloffPower(float a1) = 0;
    /// pure
    virtual UNK_RET vf_11(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET SetGlobalReverbAttributes(float a1, float a2, float a3) = 0;
    /// pure
    virtual UNK_RET SetGlobalReverbLevels(float a1, float a2) = 0;
    /// pure
    virtual UNK_RET SetGlobalEffectsType(int a1) = 0;
    /// pure
    virtual UNK_RET InitListener() = 0;
    /// pure
    virtual UNK_RET DeinitListener() = 0;
    /// pure
    virtual UNK_RET SetListenerPosition(FGXVector3 const& a1) = 0;
    /// pure
    virtual UNK_RET SetListenerVelocity(FGXVector3 const& a1) = 0;
    /// pure
    virtual UNK_RET SetListenerOrientation(FGXVector3 const& a1, FGXVector3 const& a2) = 0;
    /// pure
    virtual UNK_RET Calculate3DChannelVolumes(FAudioSystem::ChannelVolumeInfo& a1, float* a2) = 0;
    /// pure
    virtual UNK_RET PlaySoundA(FAudioSystem::ESoundType a1, int a2) = 0;
    /// pure
    virtual UNK_RET StopSound(FAudioSystem::ESoundType a1, int a2) = 0;
    /// pure
    virtual UNK_RET PauseSound(FAudioSystem::ESoundType a1, int a2, bool a3) = 0;
    /// pure
    virtual UNK_RET RestartSound(FAudioSystem::ESoundType a1, int a2) = 0;
    /// pure
    virtual UNK_RET PrimeBuffer(FAudioSystem::ESoundType a1, int a2) = 0;
    /// pure
    virtual UNK_RET GetPlaybackRate(FAudioSystem::ESoundType a1, int a2, unsigned int& a3) = 0;
    /// pure
    virtual UNK_RET SetPlaybackRate(FAudioSystem::ESoundType a1, int a2, unsigned int a3) = 0;
    /// pure
    virtual UNK_RET GetSoundLooping(FAudioSystem::ESoundType a1, int a2, int& a3) = 0;
    /// pure
    virtual UNK_RET SetSoundLooping(FAudioSystem::ESoundType a1, int a2, int a3) = 0;
    /// pure
    virtual UNK_RET GetSoundVolume(FAudioSystem::ESoundType a1, int a2, float& a3) = 0;
    /// pure
    virtual UNK_RET SetSoundVolume(FAudioSystem::ESoundType a1, int a2, float a3) = 0;
    /// pure
    virtual UNK_RET SetLowPassFilterCutoff(FAudioSystem::ESoundType a1, int a2, float a3) = 0;
    /// pure
    virtual UNK_RET GetSoundPan(FAudioSystem::ESoundType a1, int a2, float& a3) = 0;
    /// pure
    virtual UNK_RET SetSoundPan(FAudioSystem::ESoundType a1, int a2, float a3, bool a4) = 0;
    /// pure
    virtual UNK_RET GetSoundPlaybackPosition(FAudioSystem::ESoundType a1, int a2, unsigned int& a3) = 0;
    /// pure
    virtual UNK_RET SetSoundPlaybackPosition(FAudioSystem::ESoundType a1, int a2, unsigned int a3) = 0;
    /// pure
    virtual UNK_RET GetSoundTotalPlaybackLength(FAudioSystem::ESoundType a1, int a2, int& a3) = 0;
    /// pure
    virtual UNK_RET Get3DSoundPosition(FAudioSystem::ESoundType a1, int a2, FGXVector3& a3) = 0;
    /// pure
    virtual UNK_RET Set3DSoundPosition(FAudioSystem::ESoundType a1, int a2, FGXVector3 const& a3) = 0;
    /// pure
    virtual UNK_RET Get3DSoundVelocity(FAudioSystem::ESoundType a1, int a2, FGXVector3& a3) = 0;
    /// pure
    virtual UNK_RET Set3DSoundVelocity(FAudioSystem::ESoundType a1, int a2, FGXVector3 const& a3) = 0;
    /// pure
    virtual UNK_RET Get3DSoundDistanceValues(FAudioSystem::ESoundType a1, int a2, float& a3, float& a4) = 0;
    /// pure
    virtual UNK_RET Set3DSoundDistanceValues(FAudioSystem::ESoundType a1, int a2, float a3, float a4) = 0;
    /// pure
    virtual UNK_RET Get3DSoundConeValues(FAudioSystem::ESoundType a1, int a2, int& a3, int& a4, float& a5) = 0;
    /// pure
    virtual UNK_RET Set3DSoundConeValues(FAudioSystem::ESoundType a1, int a2, int a3, int a4, float a5) = 0;
    /// pure
    virtual UNK_RET Get3DSoundOrientation(FAudioSystem::ESoundType a1, int a2, FGXVector3& a3, FGXVector3& a4) = 0;
    /// pure
    virtual UNK_RET Set3DSoundOrientation(FAudioSystem::ESoundType a1, int a2, FGXVector3 const& a3, FGXVector3 const& a4) = 0;
    /// pure
    virtual UNK_RET GetReverbLevels(FAudioSystem::ESoundType a1, int a2, float& a3, float& a4) = 0;
    /// pure
    virtual UNK_RET SetReverbLevels(FAudioSystem::ESoundType a1, int a2, float a3, float a4) = 0;
    /// pure
    virtual UNK_RET StartCaptureOutputToWAV(char const* a1) = 0;
    /// pure
    virtual UNK_RET StopCaptureOutputToWAV() = 0;
    /// pure
    virtual UNK_RET AttachVolumeRampFilter(FAudioSystem::ESoundType a1, int a2) = 0;
    /// pure
    virtual UNK_RET SetVolumeRamp(FAudioSystem::ESoundType a1, int a2, float a3, float a4, bool a5, float a6) = 0;
    /// pure
    virtual UNK_RET GetVolumeRampCurrentVolume(FAudioSystem::ESoundType a1, int a2, float& a3) = 0;
    /// pure
    virtual UNK_RET SetOutputChannel(FAudioSystem::ESoundType a1, int a2, ESpeakerChannels a3) = 0;
    /// pure
    virtual UNK_RET SetChannelVolumes(FAudioSystem::ESoundType a1, int a2, ESpeakerChannels const* a3, float const* a4, int a5) = 0;
    /// pure
    virtual UNK_RET SetupCustomFalloffFunction(FAudioSystem::ESoundType a1, int a2, int a3) = 0;
    /// pure
    virtual UNK_RET SetDebugFlag(FAudioSystem::ESoundType a1, int a2, bool a3) = 0;
    /// pure
    virtual UNK_RET vf_59(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET CalculateNumSamples(SAudioSystemInit* a1) = 0;
    /// pure
    virtual UNK_RET InitHandles(FAudioSystem::ESoundType a1) = 0;
    /// pure
    virtual UNK_RET DeinitHandles(FAudioSystem::ESoundType a1) = 0;
    /// pure
    virtual UNK_RET AcquireSoundHandleId(FAudioSystem::ESoundType a1) = 0;
    /// pure
    virtual UNK_RET ReleaseSoundHandleId(FAudioSystem::ESoundType a1, int a2) = 0;
    /// pure
    virtual UNK_RET Init2DSamples() = 0;
    /// pure
    virtual UNK_RET Deinit2DSamples() = 0;
    /// pure
    virtual UNK_RET Init3DSamples() = 0;
    /// pure
    virtual UNK_RET Deinit3DSamples() = 0;
    /// pure
    virtual UNK_RET Init2DStreams() = 0;
    /// pure
    virtual UNK_RET Deinit2DStreams() = 0;
    /// pure
    virtual UNK_RET Init3DStreams() = 0;
    /// pure
    virtual UNK_RET Deinit3DStreams() = 0;
    /// 0x08d2be82
    UNK_RET GetAudioMgrEndOfSoundCallback(FAudioSystem::ESoundType a1);
};

namespace FAudioVariation {
/// 0x08d2bfa8
UNK_RET Shutdown();
/// 0x08d2bfe4
UNK_RET pkGetVariationCache();
/// 0x08d2c036
UNK_RET ClearVariationCache();
/// 0x08d2c090
UNK_RET IsVariationInLegalRange(int a1);
/// 0x08d2c0c0
UNK_RET ComputeSoundVariationName(FStringA const& a1, char const* a2, int a3, FStringA& a4);
/// 0x08d2c15a
UNK_RET GetFullFilename(FStringA const& a1, char const* a2, int a3, FStringA& a4);
/// 0x08d2c21e
UNK_RET Do_CalculateNumVariations(FStringA const& a1, char const* a2);
/// 0x08d2c410
UNK_RET GetCachedEntry(int a1);
/// 0x08d2c492
UNK_RET CalculateNumVariations(int a1, FStringA const& a2);
/// 0x08d2c586
UNK_RET GetFileExtensionForSoundDefineIndex(int a1, FStringA const& a2);
}
