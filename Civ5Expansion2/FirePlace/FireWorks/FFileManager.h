/// origin binary
#pragma once
#include "FirePlace/FireWorks/FArray.h"
#include "FirePlace/FireWorks/FFileCatalog.h"
#include "FirePlace/FireWorks/FMemHooks.h"

class FFileCatalogEntry;
class FPakFile;
class FStringArray;

/// size ?
class FFileManager {
public:
    /// 0x08da3bd8
    FFileManager(char const* a1, char const* a2, void* a3);
    /// 0x08da3df2
    ~FFileManager();
    /// 0x08da3d9c
    UNK_RET Init(char const* a1, char const* a2, void* a3);
    /// 0x08da3fb0
    UNK_RET Uninit();
    /// 0x08da400e
    UNK_RET ScanFolder(char const* a1, char const* a2, FFileCatalog::AddMode a3, char const* a4, char const* a5);
    /// 0x08da4582
    UNK_RET AddArchive(char const* a1, FFileCatalog::AddMode a2);
    /// 0x08da4808
    UNK_RET GetPakFile(char const* a1) const;
    /// 0x08da4960
    UNK_RET AddFile(char const* a1, FFileCatalog::AddMode a2);
    /// 0x08da4e0c
    UNK_RET DoAddFile(char const* a1, FFileCatalog::AddMode a2);
    /// 0x08da5272
    UNK_RET IsInScannedFolderPath(char const* a1, bool a2);
    /// 0x08da52ec
    UNK_RET Find(char const* a1, FPakFile** a2, bool a3) const;
    /// 0x08da534a
    UNK_RET GetPakFile(FFileCatalogEntry* a1) const;
    /// 0x08da538c
    UNK_RET Find(wchar_t const* a1, FPakFile** a2, bool a3) const;
    /// 0x08da53ea
    UNK_RET GetPakFile(wchar_t const* a1) const;
    /// 0x08da563c
    UNK_RET Enumerate(FArray<FFileCatalogEntry const*, (eMPoolType)35>& a1, char const* a2) const;
    /// 0x08da587c
    UNK_RET Enumerate(FStringArray& a1, char const* a2) const;
    /// 0x08da5b30
    UNK_RET SetEnabled(bool a1);
    /// 0x08da5b3c
    UNK_RET SetName(char const* a1);
    /// 0x08da5b96
    UNK_RET SetCategory(unsigned int a1);
    /// 0x08da5ba2
    UNK_RET SetPriority(unsigned int a1);
};

extern "C" {
    /// 0x08da65a8
    UNK_RET GetFileSystemMgr(UNK_PARAMS);
}

namespace FFolder {
/// 0x08da6f64
UNK_RET Create(char const* a1, char a2);
/// 0x08da72b2
UNK_RET Create(wchar_t const* a1, wchar_t a2);
/// 0x08da75c6
UNK_RET Delete(char const* a1, char a2);
/// 0x08da77f2
UNK_RET Delete(wchar_t const* a1, wchar_t a2);
}

namespace FFolderContents {
/// 0x08da7a10
UNK_RET GetRandomFile(bool a1);
/// 0x08da7c0e
UNK_RET Fill(char const* a1, char const* a2, unsigned int a3);
}

namespace FFolderContentsW {
/// 0x08da7dd8
UNK_RET GetRandomFile(bool a1);
/// 0x08da7fd6
UNK_RET Fill(wchar_t const* a1, wchar_t const* a2, unsigned int a3);
}
