/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/CvGameDatabase/Database.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include <lua.h>

class UserAchievementStored_t;
class UserStatsReceived_t;
class UserStatsStored_t;
class cvPath;
namespace CvLeaderHeadBufferManager {
class BufferDesc;
}
namespace FireGrafix {
enum _FGX_INDEXFORMATS : int;
}

/// size ?
class CvAchievementsManager {
public:
    /// 0x089dd748
    CvAchievementsManager();
    /// 0x089e5542
    ~CvAchievementsManager();
    /// 0x089e545a
    UNK_RET OnUserStatsReceived(UserStatsReceived_t* a1);
    /// 0x089e54b8
    UNK_RET OnUserStatsStored(UserStatsStored_t* a1);
    /// 0x089e550a
    UNK_RET OnUserAchievementStored(UserAchievementStored_t* a1);
    /// 0x089e558c
    UNK_RET Update();
    /// 0x089e55f4
    UNK_RET UnlockAchievement(EAchievement a1);
    /// 0x089e570a
    UNK_RET IsAchievementUnlocked(EAchievement a1) const;
    /// 0x089e575a
    UNK_RET GetStat(ESteamStat a1, int* a2) const;
    /// 0x089e57c4
    UNK_RET SetStat(ESteamStat a1, int a2);
    /// 0x089e58b4
    UNK_RET IncrementStat(ESteamStat a1);
    /// 0x089e59da
    UNK_RET IncrementStatAndUnlock(ESteamStat a1, int a2, EAchievement a3);
    /// 0x089e5b2a
    UNK_RET ResetAllAchievements();
    /// 0x089e5b2c
    UNK_RET LoadAchievementsFromCache();
    /// 0x089e5e7e
    UNK_RET IsOurGameID(unsigned long long a1) const;
};

/// 0x089e86ec
UNK_RET LHGrannyAlloc(char const* a1, int a2, int a3, int a4);
/// 0x089e8717
UNK_RET LHGrannyFree(char const* a1, int a2, void* a3);

namespace CvHallofFame {
/// 0x089e68a0
UNK_RET RecordVictoryInformation(int a1);
/// 0x089e79f0
UNK_RET GetDefaultHallofFameDatabasePath(cvPath& a1);
/// 0x089e7afc
UNK_RET OpenHallofFameDatabase(Database::Connection& a1, cvPath const& a2);
/// 0x089e7c7c
UNK_RET CreateHallofFameDatabase(Database::Connection& a1, cvPath const& a2);
/// 0x089e7d76
UNK_RET UpdateHallofFameDatabase(Database::Connection& a1, cvPath const& a2);
}

namespace CvLeaderHeadBufferManager {
/// 0x089e7e4c
UNK_RET Startup();
/// 0x089e7ec8
UNK_RET Shutdown();
/// 0x089e7fc0
UNK_RET CreateStaticVB(void* a1, unsigned int a2, unsigned int a3, unsigned int a4);
/// 0x089e80a6
UNK_RET AllocateBufferFromPool(void* a1, FFastVector<CvLeaderHeadBufferManager::BufferDesc, true, 46u, 0u, BaseVector<CvLeaderHeadBufferManager::BufferDesc, true>::FDefaultFastVectorAllocator>& a2, unsigned int a3, unsigned int a4);
/// 0x089e8108
UNK_RET CreateStaticIB(void* a1, FireGrafix::_FGX_INDEXFORMATS a2, unsigned int a3, unsigned int a4);
/// 0x089e81e0
UNK_RET CreateAndFillStaticVB(void* a1, unsigned int a2, unsigned int a3, void* a4, unsigned int a5);
/// 0x089e824c
UNK_RET CreateAndFillStaticIB(void* a1, FireGrafix::_FGX_INDEXFORMATS a2, unsigned int a3, void* a4, unsigned int a5);
/// 0x089e82b8
UNK_RET CreateDynamicVB(void* a1, unsigned int a2, unsigned int a3, unsigned int a4);
/// 0x089e838e
UNK_RET AllocateCPU(void* a1, unsigned int a2);
/// 0x089e8440
UNK_RET ReclaimBuffers(void* a1);
}

namespace CvLeaderHeadGrannyAllocator {
/// 0x089e8742
UNK_RET Startup();
/// 0x089e8765
UNK_RET Push();
/// 0x089e879a
UNK_RET Pop();
}

namespace cvLuaGameEvent {
/// 0x089eae98
/// lua ?
int lCreate(lua_State* a1);
/// 0x089eaf66
UNK_RET RegisterMethods(lua_State* a1, int a2, int a3, int a4);
/// 0x089eb0d6
int RegisterMetatable(lua_State* a1);
/// 0x089eb17e
/// lua ?
int lAdd(lua_State* a1);
/// 0x089eb1d4
/// lua ?
int lCount(lua_State* a1);
/// 0x089eb210
/// lua ?
int lRemove(lua_State* a1);
/// 0x089eb306
/// lua ?
int lRemoveAll(lua_State* a1);
/// 0x089eb360
/// lua ?
int lCall(lua_State* a1);
/// 0x089eb534
/// lua ?
int lAccumulate(lua_State* a1);
/// 0x089eb708
/// lua ?
int lAccumulateINT(lua_State* a1);
/// 0x089eb8c6
/// lua ?
int lTestAll(lua_State* a1);
/// 0x089ebaba
/// lua ?
int lTestAny(lua_State* a1);
/// 0x089ebc8a
UNK_RET RegisterMethod(lua_State* a1, int (*a2)(lua_State*), char const* a3, int a4, int a5);
}
