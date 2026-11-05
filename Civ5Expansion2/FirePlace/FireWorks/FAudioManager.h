/// origin guess
#pragma once
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include "FirePlace/FireWorks/FString.h"
#include <memory>
#include <string>
#include <vector>

class F2DSoundScriptData;
class F3DSoundScriptData;
class FAudioManager;
class FAudioMgrInit;
class FBaseSoundScriptData;
class FFileBackgroundLoader;
class FGXVector3;
class FSharedSoundData;
class FSound2D;
class FSound3D;
class FSoundBase;
class FSoundInit;
class FSoundScape;
class FSoundScapeScriptData;
class FTransformNode;
class FXmlTraverser;
class SoundDefine;
class SoundFinishedInfo;
enum EAudioScriptType : int;
enum EAudioTag : int;
enum ESoundLoadType : int;
template <class T0> class FTPointer;

/// size ?
/// vtable 0x0a303540
class FAudioManager {
public:
    class SoundLibrary;
    /// size ?
    class SoundLibrary {
    public:
        /// 0x08d16798
        UNK_RET FreeSoundDefines();
        /// 0x08d168c0
        UNK_RET Free2DScripts();
        /// 0x08d16928
        UNK_RET Free3DScripts();
        /// 0x08d16990
        UNK_RET FreeSoundScapeScripts();
    };
    /// 0x08d14fbc
    FAudioManager();
    /// 0x08d16afe
    virtual UNK_RET Init(FAudioMgrInit const& a1);
    /// 0x08d15ece
    /// deleting 0x08d15ea6
    virtual ~FAudioManager();
    /// 0x08d1bd10
    virtual UNK_RET LoadAllXmlScripts();
    /// 0x08d1be6e
    virtual UNK_RET LoadAudioDefines(char const* a1, unsigned int a2);
    /// 0x08d1be46
    virtual UNK_RET LoadAudioParams(char const* a1);
    /// 0x08d1be5a
    virtual UNK_RET LoadAdditionalAudioContexts(char const* a1);
    /// 0x08d1be82
    virtual UNK_RET Load2DScripts(char const* a1, unsigned int a2);
    /// 0x08d1be96
    virtual UNK_RET Load3DScripts(char const* a1, unsigned int a2);
    /// 0x08d1beaa
    virtual UNK_RET LoadSoundScapeScripts(char const* a1, unsigned int a2);
    /// 0x08d1c024
    virtual UNK_RET DoHotLoadXMLAudioDefines(char const* a1);
    /// 0x08d1c100
    virtual UNK_RET DoHotLoad2DScripts(char const* a1);
    /// 0x08d1c186
    virtual UNK_RET DoHotLoad3DScripts(char const* a1);
    /// 0x08d1c214
    virtual UNK_RET DoHotLoadSoundscapeScripts(char const* a1);
    /// 0x08d1c882
    virtual UNK_RET DoHotLoadXMLAudioParams(char const* a1);
    /// 0x08d158a4
    UNK_RET Clear();
    /// 0x08d1666a
    UNK_RET Term();
    /// 0x08d169f8
    UNK_RET FreeSoundDefines();
    /// 0x08d16a30
    UNK_RET Free2DScripts();
    /// 0x08d16a68
    UNK_RET Free3DScripts();
    /// 0x08d16aa0
    UNK_RET FreeSoundScapeScripts();
    /// 0x08d16ad8
    UNK_RET GetSupportedExtensionCount() const;
    /// 0x08d16ade
    UNK_RET GetSupportedExtensionType(unsigned int a1) const;
    /// 0x08d16e6c
    UNK_RET IsInitialized() const;
    /// 0x08d16e76
    UNK_RET EndOf2DSampleCallback(int a1);
    /// 0x08d16eb0
    UNK_RET EndOf2DStreamCallback(int a1);
    /// 0x08d16eea
    UNK_RET EndOf3DSampleCallback(int a1);
    /// 0x08d16f24
    UNK_RET EndOf3DStreamCallback(int a1);
    /// 0x08d16f5e
    UNK_RET AllocateMemoryCallback(unsigned int a1);
    /// 0x08d16f92
    UNK_RET DeallocateMemoryCallback(void* a1);
    /// 0x08d16fc6
    UNK_RET GetSpeakerConfigValue(FStringA* a1);
    /// 0x08d1703c
    UNK_RET InitListener();
    /// 0x08d1711e
    UNK_RET InitSoundScapeMixer();
    /// 0x08d17144
    UNK_RET GetListener();
    /// 0x08d17172
    UNK_RET DeinitSoundScapeMixer();
    /// 0x08d1719e
    UNK_RET DestroySharedSoundData();
    /// 0x08d171d2
    UNK_RET TermListener();
    /// 0x08d17216
    UNK_RET UpdateVolumeKnobs(float a1);
    /// 0x08d1727c
    UNK_RET Update();
    /// 0x08d1748e
    UNK_RET GetVolumeKnob(int a1);
    /// 0x08d174c2
    UNK_RET GetAudioTagIndex(EAudioTag a1, char const* a2, EAudioScriptType a3, bool a4);
    /// 0x08d17722
    UNK_RET ReleaseDynamicSharedSoundData(unsigned int a1);
    /// 0x08d1779e
    UNK_RET ProcessCallbackUpdates();
    /// 0x08d1783e
    UNK_RET UpdateSoundScapes(float a1);
    /// 0x08d17890
    UNK_RET DestroyMarkedSoundData();
    /// 0x08d178de
    UNK_RET GetTime() const;
    /// 0x08d178e6
    UNK_RET GetSoundRolloffFactor();
    /// 0x08d17904
    UNK_RET SetSoundRolloffFactor(float a1);
    /// 0x08d17940
    UNK_RET GetSpeakerFalloffPower();
    /// 0x08d1795e
    UNK_RET SetSpeakerFalloffPower(float a1);
    /// 0x08d1799a
    UNK_RET PreloadContextOfDynamicResSounds(int a1);
    /// 0x08d17ace
    UNK_RET BuildContextSoundList(int a1, std::vector<int, std::allocator<int> >& a2, bool a3) const;
    /// 0x08d17c94
    UNK_RET PreloadSoundDefine(int a1);
    /// 0x08d17d20
    UNK_RET PreloadSound(int a1, EAudioTag a2);
    /// 0x08d17d8c
    UNK_RET Get3DScript(unsigned int a1) const;
    /// 0x08d17dd4
    UNK_RET Get2DScript(unsigned int a1) const;
    /// 0x08d17e1c
    UNK_RET GetSoundDefine(unsigned int a1) const;
    /// 0x08d17e78
    UNK_RET AcquireLoadedBuffer(unsigned int a1, int& a2, bool a3, bool a4);
    /// 0x08d18002
    UNK_RET ReleaseLoadedBuffer(unsigned int a1, FSharedSoundData* a2, bool a3);
    /// 0x08d180a4
    UNK_RET IsSoundStreamed(unsigned int a1) const;
    /// 0x08d180e8
    UNK_RET DoAcquireLoadedBuffer(ESoundLoadType a1, int a2, int a3, bool a4, bool a5);
    /// 0x08d18190
    UNK_RET DoAcquireLoadedBuffer_SharedString();
    /// 0x08d181cc
    UNK_RET IsSoundDynamicRes(unsigned int a1) const;
    /// 0x08d18210
    UNK_RET MoveDynamicSoundToFrontOfDynamicList(FSharedSoundData* a1);
    /// 0x08d18362
    UNK_RET GetSoundFilename(unsigned int a1, bool a2);
    /// 0x08d183b4
    UNK_RET AllocNewSharedSoundData();
    /// 0x08d183e6
    UNK_RET ShouldSoundBeCached(unsigned int a1) const;
    /// 0x08d18432
    UNK_RET IsSoundDefineInAnyLoadedContext(int a1) const;
    /// 0x08d184fc
    UNK_RET GetRandomSongFromMusicPath();
    /// 0x08d18548
    UNK_RET BuildFullFilenamePath(FStringA& a1) const;
    /// 0x08d186d6
    UNK_RET IsSoundResident(unsigned int a1) const;
    /// 0x08d1871a
    UNK_RET IsSoundBackgroundLoading(unsigned int a1) const;
    /// 0x08d18772
    UNK_RET GetLoadedSoundIndex(FSharedSoundData* a1) const;
    /// 0x08d187a8
    UNK_RET GetLoadedDynamicSoundIndex(FSharedSoundData* a1) const;
    /// 0x08d187de
    UNK_RET DoRemoveSoundFromDynamicList(unsigned int a1);
    /// 0x08d188ea
    UNK_RET UnCacheAllSounds();
    /// 0x08d1894e
    UNK_RET GetLibrary(unsigned int a1) const;
    /// 0x08d1897e
    UNK_RET GetSoundScapeScript(unsigned int a1) const;
    /// 0x08d189c6
    UNK_RET SetGlobalEffects(int a1);
    /// 0x08d18a86
    UNK_RET SetGlobalEffects(FStringA* a1);
    /// 0x08d18ae4
    UNK_RET GetMasterVolumeKnob();
    /// 0x08d18afe
    UNK_RET LoadContext(int a1, int a2, int a3);
    /// 0x08d18d0c
    UNK_RET QueueSoundsInContextListInOrder(std::vector<int, std::allocator<int> >& a1, int a2);
    /// 0x08d18e36
    UNK_RET QueueSoundsInContextListByVariation(std::vector<int, std::allocator<int> >& a1, int a2, int a3, int a4);
    /// 0x08d191d0
    UNK_RET OnContextFinishedLoading(int a1);
    /// 0x08d19216
    UNK_RET UnloadContext(int a1);
    /// 0x08d1944e
    UNK_RET InitBaseScriptData(FSoundInit& a1, FBaseSoundScriptData const* a2, bool a3, float a4, FSoundBase* a5, int a6, int a7, int a8, int a9);
    /// 0x08d1955a
    UNK_RET DestroyObject(FSound2D* a1);
    /// 0x08d19594
    UNK_RET DestroyObject(FSound3D* a1);
    /// 0x08d195ce
    UNK_RET GetSoundScriptInstanceInfo(unsigned int a1, bool a2);
    /// 0x08d19632
    UNK_RET GetLibrary(unsigned int a1);
    /// 0x08d19662
    UNK_RET ShouldNewSoundPlay(FBaseSoundScriptData const* a1, bool a2);
    /// 0x08d198dc
    UNK_RET GetNextSoundID();
    /// 0x08d1990e
    UNK_RET FindSoundByID(long a1);
    /// 0x08d19960
    UNK_RET DoSound(int a1, bool a2, float a3, float a4, int a5, int a6, bool a7, bool a8, bool a9, bool a10, int a11, int a12);
    /// 0x08d19ba0
    UNK_RET IsPaused();
    /// 0x08d19baa
    UNK_RET AllocNew2DSoundObject();
    /// 0x08d19bdc
    UNK_RET DoSound(FStringA* a1, bool a2, float a3, float a4, int a5, int a6, bool a7);
    /// 0x08d19e28
    UNK_RET LoadSharedSoundData_FakedMethod(char const* a1);
    /// 0x08d1a0a6
    UNK_RET IsSoundCloseEnoughToBeHeard(float a1, float a2, FGXVector3 const& a3) const;
    /// 0x08d1a274
    UNK_RET Do3DSound(int a1, bool a2, FTPointer<FTransformNode> a3, FGXVector3 const& a4, float a5, float a6, int a7, float a8, float a9, int a10, bool a11, int a12, int a13);
    /// 0x08d1a72a
    UNK_RET AllocNew3DSoundObject();
    /// 0x08d1a75c
    UNK_RET DeallocateMemory(void* a1);
    /// 0x08d1a78a
    UNK_RET Do3DSound(FStringA* a1, FGXVector3 const& a2, float a3, int a4, float a5, float a6, FTPointer<FTransformNode> a7, int a8, bool a9);
    /// 0x08d1aa3e
    UNK_RET DoSoundscape(int a1, float a2, float a3, FGXVector3 const& a4);
    /// 0x08d1aadc
    UNK_RET EndSoundscape(FSoundScape* a1, float a2);
    /// 0x08d1ab2e
    UNK_RET DoSoundScapeMixer(int a1, float a2, float a3, FGXVector3 const& a4);
    /// 0x08d1ab8a
    UNK_RET SoundScapeMixerIsPlaying();
    /// 0x08d1abb4
    UNK_RET DoSoundScapeMixerIfNotPlaying(int a1, float a2, float a3);
    /// 0x08d1ac36
    UNK_RET EndSoundScapeMixer(float a1);
    /// 0x08d1ac6e
    UNK_RET SetSoundScapeMixerTaperVolume(float a1);
    /// 0x08d1aca0
    UNK_RET SetSoundScapeMixerTaperTimer(float a1);
    /// 0x08d1acd2
    UNK_RET SetSoundScapeMixerVolume(float a1);
    /// 0x08d1ad04
    UNK_RET IsUsingUserMusicPath();
    /// 0x08d1ad1a
    UNK_RET SetUserMusicPath(FStringA* a1);
    /// 0x08d1adce
    UNK_RET OnSoundInstanceCreated(FSound2D* a1);
    /// 0x08d1ae16
    UNK_RET OnUnloadSound(FSound2D* a1);
    /// 0x08d1aeb6
    UNK_RET OnStartedLoadingSharedSoundData(FSharedSoundData* a1);
    /// 0x08d1af70
    UNK_RET OnStartedLoadingDynamicSharedSoundData(FSharedSoundData* a1);
    /// 0x08d1afc0
    UNK_RET OnFinishedLoadingSharedSoundData(FSharedSoundData* a1);
    /// 0x08d1b0c2
    UNK_RET OnFinishedLoadingDynamicSharedSoundData(FSharedSoundData* a1);
    /// 0x08d1b116
    UNK_RET OnUnloadSharedSoundData(FSharedSoundData* a1);
    /// 0x08d1b21c
    UNK_RET OnUnloadDynamicSharedSoundData(FSharedSoundData* a1);
    /// 0x08d1b2c6
    UNK_RET OnSoundInstanceCreated(FSound3D* a1);
    /// 0x08d1b30e
    UNK_RET OnUnloadSound(FSound3D* a1);
    /// 0x08d1b396
    UNK_RET SetListenerObject(FTPointer<FTransformNode> a1);
    /// 0x08d1b42c
    UNK_RET GetListenerObject();
    /// 0x08d1b480
    UNK_RET StopAll(bool a1);
    /// 0x08d1b4d6
    UNK_RET PauseAll(bool a1);
    /// 0x08d1b51e
    UNK_RET ResumeAll();
    /// 0x08d1b566
    UNK_RET GetDopplerFactor();
    /// 0x08d1b56e
    UNK_RET SetDopplerFactor(float a1);
    /// 0x08d1b5a4
    UNK_RET GetDistanceFactor();
    /// 0x08d1b5ac
    UNK_RET SetDistanceFactor(float a1);
    /// 0x08d1b5e2
    UNK_RET AllocateMemory(unsigned int a1);
    /// 0x08d1b610
    UNK_RET GetSpeakerConfigString(int a1);
    /// 0x08d1b632
    UNK_RET UnloadTagStrings(bool a1);
    /// 0x08d1b8fa
    UNK_RET UnloadScriptTagStrings();
    /// 0x08d1b9c0
    UNK_RET GetAudioTagIndex(EAudioTag a1, unsigned int a2, EAudioScriptType a3);
    /// 0x08d1ba9c
    UNK_RET GetScriptTypeFromScriptNameHash(unsigned int a1);
    /// 0x08d1bb52
    UNK_RET CanAddSound() const;
    /// 0x08d1bb7a
    UNK_RET CanAdd3DSound() const;
    /// 0x08d1bba2
    UNK_RET RemoveSoundWithLowestPriority(FSound2D* a1);
    /// 0x08d1bbd4
    UNK_RET RemoveSoundWithLowestPriority(FSound3D* a1);
    /// 0x08d1bc06
    UNK_RET Reset2DSoundLimit();
    /// 0x08d1bc1e
    UNK_RET Reset3DSoundLimit();
    /// 0x08d1bc36
    UNK_RET NotifySoundFinished(SoundFinishedInfo& a1);
    /// 0x08d1bcb6
    UNK_RET StartCaptureOutputToWAV(char const* a1);
    /// 0x08d1bce8
    UNK_RET StopCaptureOutputToWAV();
    /// 0x08d1bebe
    UNK_RET HandleAudioDataFileHotLoad(char const* a1);
    /// 0x08d1bf34
    UNK_RET ResetForHotLoading(char const* a1);
    /// 0x08d1bfc2
    UNK_RET PostHotLoad(bool a1);
    /// 0x08d1c2ac
    UNK_RET HotLoadXMLAudioDefines(char const* a1);
    /// 0x08d1c2dc
    UNK_RET HotLoad2DScripts(char const* a1);
    /// 0x08d1c30c
    UNK_RET HotLoad3DScripts(char const* a1);
    /// 0x08d1c33c
    UNK_RET HotLoadSoundscapeScripts(char const* a1);
    /// 0x08d1c36c
    UNK_RET OnUserMediaPlayerIdle();
    /// 0x08d1c39e
    UNK_RET OnUserMediaPlayerPlaying();
    /// 0x08d1c3f8
    UNK_RET OnUserMediaPlayerPaused();
    /// 0x08d1c42a
    UNK_RET SetVariationsForSoundDefine(unsigned int a1, int a2);
    /// 0x08d1c4a2
    UNK_RET GetNextVariationForSoundDefine(unsigned int a1, FStringA const& a2);
    /// 0x08d1c554
    UNK_RET LogOnFileLoaded(char const* a1, bool a2);
    /// 0x08d1c626
    UNK_RET GetNumContextIds() const;
    /// 0x08d1c632
    UNK_RET DecNumInstancesOfScriptIDInUse(FBaseSoundScriptData const* a1, bool a2);
    /// 0x08d1c70a
    UNK_RET IncNumInstancesOfScriptIDInUse(FBaseSoundScriptData const* a1, bool a2);
    /// 0x08d1c74c
    UNK_RET SetSpecialMusicThreadModeActive(bool a1);
    /// 0x08d1c77c
    UNK_RET SetBackgroundLoader(FFileBackgroundLoader* a1);
    /// 0x08d1c7b4
    UNK_RET IsContextActive(int a1) const;
    /// 0x08d1c7ec
    UNK_RET IsContextLoaded(int a1) const;
    /// 0x08d1c814
    UNK_RET IsContextLoading(int a1) const;
    /// 0x08d1c836
    UNK_RET MakeSoundTagID(char const* a1);
    /// 0x08d1ca12
    UNK_RET HotLoadXMLAudioParams(char const* a1);
    /// 0x08d1ca42
    UNK_RET IsInSurroundSound();
    /// 0x08d1ca76
    UNK_RET SetVolumeForSoundsOfSoundType(char const* a1, float a2);
    /// 0x08d1cada
    UNK_RET CreateLibrary(unsigned int a1);
    /// 0x08d1cc2a
    UNK_RET CreateLibrary();
    /// 0x08d1cd2a
    UNK_RET RebuildLibraryReferences();
    /// 0x0ff857b8
    static UNK_DATA m_pInst[0x4];
};

/// 0x08d27520
UNK_RET PrintMemoryInfo(char const* a1, unsigned long long a2);

namespace FAudioManagerLoader {
/// 0x08d20350
UNK_RET Load2DScripts(FAudioManager* a1, char const* a2, unsigned int a3);
/// 0x08d20402
UNK_RET Load2DScriptXMLFile(FAudioManager* a1, char const* a2, std::vector<F2DSoundScriptData*, FSTL_Tagged_Allocator<F2DSoundScriptData*, 1u, 12u, 4u> >& a3, int a4);
/// 0x08d2075a
UNK_RET UpdateFiresound2DScripts(FAudioManager* a1, std::vector<F2DSoundScriptData*, FSTL_Tagged_Allocator<F2DSoundScriptData*, 1u, 12u, 4u> >& a2, unsigned int a3);
/// 0x08d208ee
UNK_RET Load2DScripts(FAudioManager* a1, std::vector<std::string, std::allocator<std::string> > const& a2, unsigned int a3);
/// 0x08d20a70
UNK_RET Load2DScriptXMLData(FAudioManager* a1, FXmlTraverser& a2, std::vector<F2DSoundScriptData*, FSTL_Tagged_Allocator<F2DSoundScriptData*, 1u, 12u, 4u> >& a3);
/// 0x08d20c80
UNK_RET Load3DScripts(FAudioManager* a1, char const* a2, unsigned int a3);
/// 0x08d20d34
UNK_RET Load3DScriptXMLFile(FAudioManager* a1, char const* a2, std::vector<F3DSoundScriptData*, FSTL_Tagged_Allocator<F3DSoundScriptData*, 1u, 12u, 4u> >& a3, F3DSoundScriptData* a4, int a5);
/// 0x08d211b8
UNK_RET UpdateFiresound3DScripts(FAudioManager* a1, std::vector<F3DSoundScriptData*, FSTL_Tagged_Allocator<F3DSoundScriptData*, 1u, 12u, 4u> >& a2, unsigned int a3);
/// 0x08d21344
UNK_RET Load3DScripts(FAudioManager* a1, std::vector<std::string, std::allocator<std::string> > const& a2, unsigned int a3);
/// 0x08d2141e
UNK_RET Load3DScriptXMLData(FAudioManager* a1, FXmlTraverser& a2, std::vector<F3DSoundScriptData*, FSTL_Tagged_Allocator<F3DSoundScriptData*, 1u, 12u, 4u> >& a3, F3DSoundScriptData* a4);
/// 0x08d21692
UNK_RET LoadSoundScapeScripts(FAudioManager* a1, char const* a2, unsigned int a3);
/// 0x08d2173e
UNK_RET LoadSoundScapeXMLFile(FAudioManager* a1, char const* a2, std::vector<FSoundScapeScriptData*, FSTL_Tagged_Allocator<FSoundScapeScriptData*, 1u, 12u, 4u> >& a3, int a4);
/// 0x08d21aa4
UNK_RET UpdateFiresoundSoundScapeScripts(FAudioManager* a1, std::vector<FSoundScapeScriptData*, FSTL_Tagged_Allocator<FSoundScapeScriptData*, 1u, 12u, 4u> >& a2, unsigned int a3);
/// 0x08d21be2
UNK_RET LoadSoundScapeScripts(FAudioManager* a1, std::vector<std::string, std::allocator<std::string> > const& a2, unsigned int a3);
/// 0x08d21cb4
UNK_RET LoadSoundScapeXMLData(FAudioManager* a1, FXmlTraverser& a2, std::vector<FSoundScapeScriptData*, FSTL_Tagged_Allocator<FSoundScapeScriptData*, 1u, 12u, 4u> >& a3);
/// 0x08d2244c
UNK_RET SetDefaultSoundScapeScriptData(FSoundScapeScriptData* a1);
/// 0x08d22514
UNK_RET LoadAudioDefines(FAudioManager* a1, char const* a2, unsigned int a3);
/// 0x08d225ae
UNK_RET LoadAudioDefinesXMLFile(FAudioManager* a1, char const* a2, std::vector<SoundDefine, FSTL_Tagged_Allocator<SoundDefine, 1u, 12u, 4u> >& a3, int a4);
/// 0x08d22914
UNK_RET UpdateFiresoundAudioDefines(FAudioManager* a1, std::vector<SoundDefine, FSTL_Tagged_Allocator<SoundDefine, 1u, 12u, 4u> >& a2, unsigned int a3);
/// 0x08d22bfc
UNK_RET LoadAudioDefines(FAudioManager* a1, std::vector<std::string, std::allocator<std::string> > const& a2, unsigned int a3);
/// 0x08d22cc0
UNK_RET LoadAudioDefinesXMLData(FAudioManager* a1, FXmlTraverser& a2, std::vector<SoundDefine, FSTL_Tagged_Allocator<SoundDefine, 1u, 12u, 4u> >& a3);
/// 0x08d23140
UNK_RET LoadAudioDefineContexts(FAudioManager* a1, SoundDefine& a2, FXmlTraverser& a3);
/// 0x08d23426
UNK_RET DoAddSoundIDToContext(FAudioManager* a1, char const* a2, SoundDefine* a3);
/// 0x08d236e8
UNK_RET SetDefaultSoundDefineData(SoundDefine* a1);
/// 0x08d2373c
UNK_RET LoadAudioParams(FAudioManager* a1, char const* a2);
/// 0x08d250fe
UNK_RET LoadSoundTypes(FAudioManager* a1, FXmlTraverser& a2);
/// 0x08d252fe
UNK_RET LoadAdditionalAudioContexts(FAudioManager* a1, char const* a2);
/// 0x08d255c8
UNK_RET ProcessSoundIdInContext(FAudioManager* a1, char const* a2, FXmlTraverser& a3);
/// 0x08d257e2
UNK_RET LoadAllChildSoundTypes(FAudioManager* a1, FXmlTraverser& a2, std::vector<int, std::allocator<int> >& a3, int a4);
/// 0x08d25a48
UNK_RET Shutdown();
/// 0x0ff858a8
extern UNK_DATA bForceResidentSoundsToLoadAsDynamicResident[0x1];
/// 0x0ff858ac
extern UNK_DATA ms_pkDefault3DSound[0x4];
}

namespace FAudioVariation {
/// 0x0ff85a64
extern UNK_DATA m_iVariationCap[0x4];
/// 0x0ff85a60
extern UNK_DATA m_pkVariationCache[0x4];
}
