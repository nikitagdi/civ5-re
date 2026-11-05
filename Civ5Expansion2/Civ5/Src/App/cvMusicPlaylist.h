/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FString.h"

class cvMusicPlaylist;

/// size ?
class cvMusicPlaylist {
public:
    /// 0x08b765f4
    UNK_RET Startup(bool a1, PlayerTypes a2);
    /// 0x08b76670
    UNK_RET RescanForMusicScripts();
    /// 0x08b7686a
    UNK_RET RandomizeSongList();
    /// 0x08b76972
    UNK_RET Shutdown();
    /// 0x08b76992
    UNK_RET AdvancePlaylistToNextTrack();
    /// 0x08b769ba
    UNK_RET RewindPlaylistToPreviousTrack();
    /// 0x08b769e2
    UNK_RET IsInPlaylist(unsigned int a1) const;
    /// 0x08b76a0a
    UNK_RET GetCurrentPlaylistSongID(unsigned int& a1) const;
    /// 0x08b76a46
    UNK_RET BuildPlaylistTrackName(FStringA& a1, char const* a2, int a3);
    /// 0x08b76daa
    UNK_RET AdvanceToHighestPrioritySongInPlaylist();
    /// 0x08b76dce
    UNK_RET FindHighPrioritySong();
    /// 0x08b76f3c
    UNK_RET GetSongTagCount(char const* a1);
    /// 0x08b7711a
    UNK_RET OnActivated();
    /// 0x08b7713e
    UNK_RET SetCurrentIndexToSongID(unsigned int a1);
    /// 0x08b7716c
    UNK_RET RemoveTracks(PlayerTypes a1);
    /// 0x08b77240
    UNK_RET RemoveNonOwnerTracks();
    /// 0x08b77312
    UNK_RET AddTracks(cvMusicPlaylist const& a1, unsigned int a2);
    /// 0x08b7741a
    UNK_RET GetTrackIndex(unsigned int a1) const;
};
