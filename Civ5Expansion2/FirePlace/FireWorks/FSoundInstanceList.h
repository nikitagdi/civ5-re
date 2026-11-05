/// origin guess
#pragma once

class FSharedSoundData;
class FSoundBase;
class SoundFinishedInfo;

/// size ?
class FSoundInstanceList {
public:
    /// 0x08d35438
    FSoundInstanceList();
    /// 0x08d35452
    ~FSoundInstanceList();
    /// 0x08d35466
    UNK_RET DestroyMarkedSounds();
    /// 0x08d354ba
    UNK_RET StopAll(bool a1);
    /// 0x08d3551e
    UNK_RET PauseAll();
    /// 0x08d35552
    UNK_RET ResumeAll();
    /// 0x08d35592
    UNK_RET RemoveSoundWithLowestPriority(FSoundBase* a1);
    /// 0x08d35610
    UNK_RET DestroySounds();
    /// 0x08d3563c
    UNK_RET UpdateFadingSounds();
    /// 0x08d35670
    UNK_RET UpdateSounds(float a1);
    /// 0x08d356b0
    UNK_RET DoProcessSoundEnded(FSoundBase* a1, SoundFinishedInfo const& a2);
    /// 0x08d35708
    UNK_RET ProcessSoundEnded(SoundFinishedInfo& a1);
    /// 0x08d35754
    UNK_RET DestroyAnySoundsUsingThisSharedData(FSharedSoundData* a1);
    /// 0x08d357a6
    UNK_RET OnSoundInstanceCreated(FSoundBase* a1);
    /// 0x08d357f4
    UNK_RET OnUnloadSound(FSoundBase* a1);
    /// 0x08d35884
    UNK_RET SetVolumeForSoundsOfSoundType(char const* a1, float a2);
    /// 0x08d35912
    UNK_RET FindByID(long a1) const;
    /// 0x08d3594a
    UNK_RET GetAt(unsigned int a1) const;
};
