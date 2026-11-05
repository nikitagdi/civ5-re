/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/Database.h"
#include "FirePlace/FireWorks/FFastList.h"
#include <memory>
#include <string>

namespace Modding {
class IAction;
class IModdingAsyncTask;
}

namespace Modding {
/// size ?
/// vtable 0x0a301060
class System {
public:
    /// 0x08c6f40c
    System();
    /// 0x08c6f634
    /// deleting 0x08c6f60c
    virtual ~System();
    /// 0x08c6f6e8
    UNK_RET Initialize(wchar_t const* a1, wchar_t const* a2);
    /// 0x08c6f7c2
    UNK_RET OpenDatabase(wchar_t const* a1);
    /// 0x08c6f95a
    UNK_RET DeactivateMods();
    /// 0x08c6f994
    UNK_RET Shutdown();
    /// 0x08c6f9d2
    UNK_RET ActivateMod(char const* a1, int a2);
    /// 0x08c6faa2
    UNK_RET ActivateEnabledMods();
    /// 0x08c6fb28
    UNK_RET ModsActivated();
    /// 0x08c6fbc0
    UNK_RET OpenUserData(char const* a1, int a2, Database::Connection& a3, bool a4);
    /// 0x08c6fd28
    UNK_RET PerformActionsOnInstalledMod(char const* a1, int a2, char const* a3);
    /// 0x08c6ff58
    UNK_RET DeleteUserData(char const* a1, int a2);
    /// 0x08c700de
    UNK_RET EnableMod(char const* a1, int a2);
    /// 0x08c701ae
    UNK_RET DisableMod(char const* a1, int a2);
    /// 0x08c7027e
    UNK_RET CreateDatabase(wchar_t const* a1);
    /// 0x08c70358
    UNK_RET RegisterAction(char const* a1, Modding::IAction* a2);
    /// 0x08c703dc
    UNK_RET ClearActions();
    /// 0x08c70402
    UNK_RET PerformActionsOnActivatedMods(char const* a1);
    /// 0x08c709d0
    UNK_RET AllActivatedModsContainPropertyValue(char const* a1, char const* a2);
    /// 0x08c70b32
    UNK_RET AnyActivatedModsContainPropertyValue(char const* a1, char const* a2);
    /// 0x08c70c06
    UNK_RET AllEnabledModsContainPropertyValue(char const* a1, char const* a2);
    /// 0x08c70d68
    UNK_RET AnyEnabledModsContainPropertyValue(char const* a1, char const* a2);
    /// 0x08c70e3c
    UNK_RET Update();
    /// 0x08c70ede
    UNK_RET CreateModsDatabaseConnection(Database::Connection& a1);
    /// 0x08c70fb8
    UNK_RET GetChangeCounter();
    /// 0x08c70fc4
    UNK_RET IncrementChangeCounter();
    /// 0x08c70fd6
    UNK_RET PopTask();
    /// 0x08c71018
    UNK_RET QueueTask(Modding::IModdingAsyncTask* a1);
    /// 0x08c7107c
    UNK_RET SetBackgroundWorkerRunning(bool a1);
};
}

/// 0x08c72ca0
UNK_RET StringSplit(std::string const& a1, std::string const& a2);
/// 0x08c72e85
UNK_RET VersionFromString(char const* a1);

namespace Modding { namespace Utilities {
/// 0x08c721fc
UNK_RET GetSafePathName(wchar_t const* a1, unsigned int a2, wchar_t* a3, unsigned int a4);
/// 0x08c722c2
UNK_RET DeleteAllFoldersAndFiles(wchar_t const* a1);
/// 0x08c72528
UNK_RET EnumerateFilesRecursive(FFastList<std::array<wchar_t, 260u>, 4569u, 0u>& a1, wchar_t const* a2, wchar_t const* a3, unsigned int a4);
/// 0x08c7280c
UNK_RET DeleteEmptyFolders(wchar_t const* a1, bool a2);
/// 0x08c72964
UNK_RET DirectoryIsEmpty(wchar_t const* a1);
}}
