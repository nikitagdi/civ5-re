/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include <lua.h>

class LeaderboardFindResult_t;
class LeaderboardScoresDownloaded_t;

/// size ?
class CvLeaderboardManager {
public:
    /// 0x08775302
    CvLeaderboardManager();
    /// 0x087753e8
    ~CvLeaderboardManager();
    /// 0x0877543e
    UNK_RET RequestLeaderboardScores();
    /// 0x087754b0
    UNK_RET OnLeaderboardFindResult(LeaderboardFindResult_t* a1, bool a2);
    /// 0x0877557e
    UNK_RET RecordLeaderboardScore(int a1);
    /// 0x087757a6
    UNK_RET BuildLeaderboardString(CvString& a1, CvString& a2);
    /// 0x08776182
    UNK_RET OnLeaderboardFindResultRecording(LeaderboardFindResult_t* a1, bool a2);
    /// 0x087767e4
    static int GetLeaderboardScores(lua_State* a1);
    /// 0x08776d9a
    static int GetLeaderboardTitle(lua_State* a1);
    /// 0x08776dd2
    static int GetAvailableLeaderboards(lua_State* a1);
    /// 0x0877735a
    UNK_RET AddLeaderboardTableRow(lua_State* a1, char const* a2, char const* a3, int a4, int& a5);
    /// 0x08777406
    UNK_RET SetLeaderboard(char const* a1, char const* a2);
    /// 0x0877744a
    UNK_RET SetLeaderboardCategory(int a1);
    /// 0x08777496
    UNK_RET ScrollLeaderboardUp();
    /// 0x087774a4
    UNK_RET ScrollLeaderboardDown();
    /// 0x087774b2
    UNK_RET OnLeaderboardScoresDownloaded(LeaderboardScoresDownloaded_t* a1, bool a2);
};
