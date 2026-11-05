/// origin binary
#pragma once

class FSound2D;

/// size ?
class FMusicManager {
public:
    enum MUSIC_OPTION : int;
    /// 0x08d2f058
    FMusicManager();
    /// 0x08d2f130
    ~FMusicManager();
    /// 0x08d2f0c4
    UNK_RET Shutdown();
    /// 0x08d2f176
    UNK_RET PlayMusic(int a1, float a2, float a3, FMusicManager::MUSIC_OPTION a4);
    /// 0x08d2f316
    UNK_RET DoCrossFade(FSound2D* a1, float a2, bool a3);
    /// 0x08d2f3fa
    UNK_RET Destroyed2DSoundNotification(FSound2D* a1);
    /// 0x08d2f446
    UNK_RET SpecialDuckMusic(float a1, float a2);
    /// 0x08d2f4c4
    UNK_RET StopMusicIfPlaying(int a1, float a2);
    /// 0x08d2f58a
    UNK_RET StopAllMusic(float a1, bool a2);
    /// 0x08d2f684
    UNK_RET IsMusicPlaying();
    /// 0x08d2f702
    UNK_RET Init();
    /// 0x08d2f752
    UNK_RET RestartLastSongPlayed(float a1, float a2);
    /// 0x08d2f7c8
    UNK_RET DuckSoundtrackVolume(float a1, float a2);
    /// 0x08d2f888
    UNK_RET UnDuckSoundtrackVolume(float a1);
    /// 0x08d2f92a
    UNK_RET SetMusicKnobVolumeAccordingToMediaPlayerStatus();
    /// 0x08d2fa3e
    UNK_RET SetMusicDisabledByExternalUserMediaPlayer(bool a1);
    /// 0x08d2fb0a
    UNK_RET GetMusicDisabledByExternalUserMediaPlayer() const;
    /// 0x08d2fb14
    UNK_RET IsSpecificMusicPlaying(int a1);
    /// 0x08d2fb54
    UNK_RET GetCurrentlyPlayingMusic();
    /// 0x0ff85cb0
    static UNK_DATA m_pInst[0x4];
};
