/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/Database.h"
#include "Civ5/Src/CvGameDatabase/DatabaseResults.h"
#include "FirePlace/FireWorks/FFastList.h"
#include <memory>

namespace Modding {
class System;
}

namespace Modding {
/// size ?
/// vtable 0x0a301048
class PackageManager {
public:
    class SearchFolder;
    /// size ?
    class SearchFolder {
    public:
        /// 0x08c69fec
        SearchFolder(wchar_t const* a1, bool a2);
        /// 0x08c6a0ca
        SearchFolder(Modding::PackageManager::SearchFolder const& a1);
        /// 0x08c6a108
        UNK_RET operator=(Modding::PackageManager::SearchFolder const& a1);
        /// 0x08c6a15c
        UNK_RET GetPath() const;
        /// 0x08c6a164
        UNK_RET CanDeleteMods() const;
        /// 0x08c6a16e
        UNK_RET Contains(wchar_t const* a1) const;
    };
    /// 0x08c6a1b2
    PackageManager(Modding::System& a1);
    /// 0x08c6a25a
    /// deleting 0x08c6a232
    virtual ~PackageManager();
    /// 0x08c6a2ae
    UNK_RET Shutdown();
    /// 0x08c6a2c2
    UNK_RET Initialize(wchar_t const* a1);
    /// 0x08c6a304
    UNK_RET ClearSearchFolders();
    /// 0x08c6a34a
    UNK_RET AddSearchFolder(wchar_t const* a1, bool a2);
    /// 0x08c6a434
    UNK_RET RemoveSearchFolder(wchar_t const* a1);
    /// 0x08c6a530
    UNK_RET DiscoverMods();
    /// 0x08c6a9bc
    UNK_RET RemoveMismatchedScannedFiles(FFastList<std::array<wchar_t, 260u>, 4569u, 0u> const& a1, FFastList<std::array<wchar_t, 260u>, 4569u, 0u> const& a2);
    /// 0x08c6aeb8
    UNK_RET FileWasAlreadyScanned(Database::Results& a1, wchar_t const* a2, unsigned int a3);
    /// 0x08c6af6c
    UNK_RET LoadFileIntoBuffer(wchar_t const* a1, char*& a2, unsigned int& a3);
    /// 0x08c6b09c
    UNK_RET LoadModInfo(char* a1, unsigned int a2, wchar_t const* a3, unsigned int a4);
    /// 0x08c6db2c
    UNK_RET AddFileWithDateTime(Database::Results& a1, wchar_t const* a2, unsigned int a3);
    /// 0x08c6dbce
    UNK_RET HasPendingInstalls();
    /// 0x08c6dbd8
    UNK_RET InstallMods();
    /// 0x08c6e00a
    UNK_RET OnModInstallerStart(void* a1);
    /// 0x08c6e018
    UNK_RET OnModInstallerFinish(void* a1, bool a2);
    /// 0x08c6e044
    UNK_RET OnModInstallerError(void* a1, int a2, void* a3, int a4);
    /// 0x08c6e254
    UNK_RET OnModInstallerProgress(void* a1, int a2, unsigned int a3, unsigned int a4);
    /// 0x08c6e274
    UNK_RET GetCurrentModInstallProgress(int& a1, unsigned int& a2, unsigned int& a3) const;
    /// 0x08c6e2aa
    UNK_RET AcquireFileData(char const* a1, int a2, wchar_t const* a3, char*& a4, unsigned int& a5);
    /// 0x08c6e46a
    UNK_RET ReleaseFileData(int a1);
    /// 0x08c6e4aa
    UNK_RET CanDeleteMod(char const* a1, int a2);
    /// 0x08c6e614
    UNK_RET DeleteMod(Database::Connection& a1, char const* a2, int a3, bool a4);
    /// 0x08c6e822
    UNK_RET DeleteMod(char const* a1, int a2, bool a3);
    /// 0x08c6e86c
    UNK_RET GetLatestInstalledModVersion(char const* a1) const;
    /// 0x08c6e92c
    UNK_RET IsModInstalled(char const* a1) const;
    /// 0x08c6e9f4
    UNK_RET IsModInstalled(char const* a1, int a2) const;
};
}
