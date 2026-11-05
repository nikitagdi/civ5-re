/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/Database.h"

namespace Modding {
class Installer;
class InstallerData;
class InstallerModInfoData;
}

namespace Modding {
/// size ?
class Installer {
public:
    /// 0x08c7a114
    Installer(Modding::InstallerData const& a1);
    /// 0x08c7a14e
    Installer(Modding::Installer const& a1);
    /// 0x08c7a1d2
    ~Installer();
    /// 0x08c7a18c
    UNK_RET operator=(Modding::Installer const& a1);
    /// 0x08c7a22e
    UNK_RET operator()();
    /// 0x08c7a94a
    UNK_RET PostError(int a1, wchar_t const* a2, int a3);
    /// 0x08c7a9fa
    UNK_RET PostError(int a1, void* a2, int a3);
    /// 0x08c7aa3e
    UNK_RET ExtractArchiveContents(wchar_t const* a1, wchar_t const* a2);
    /// 0x08c7af2a
    UNK_RET InstallMod(wchar_t const* a1, wchar_t const* a2, Database::Connection& a3, unsigned long long* a4);
    /// 0x08c7bac2
    UNK_RET PostError(int a1);
    /// 0x08c7bb02
    UNK_RET GrowFolderBufferToSize(unsigned int a1);
    /// 0x08c7bb4e
    UNK_RET CreateAllDirectories(wchar_t* a1, unsigned int a2);
    /// 0x08c7bc0e
    UNK_RET ParseModInfo(char* a1, unsigned int a2, Modding::InstallerModInfoData& a3);
    /// 0x08c7c22c
    UNK_RET ModAlreadyInstalled(Modding::InstallerModInfoData const& a1, Database::Connection& a2);
    /// 0x08c7c384
    UNK_RET CanInstallMod(Modding::InstallerModInfoData const& a1, Database::Connection& a2);
    /// 0x08c7c61c
    UNK_RET CalculateMD5(wchar_t const* a1, char* a2);
};
}
