/// origin binary
#pragma once
#include "FirePlace/FireWorks/FHashTable.h"
#include "FirePlace/FireWorks/FString.h"

class FFileCatalogEntry;
class FFileManager;

/// size ?
class FFileCatalog {
public:
    // Bit 0 lets a loose file replace a loose entry; bits 1-3 pick the rule for an existing entry (RemoveEntry).
    enum AddMode : int {
        ADD_KEEP_EXISTING = 0x0,
        ADD_REPLACE_LOOSE = 0x1,
        ADD_REPLACE = 0x2,
        ADD_REPLACE_ARCHIVED = 0x4,
        ADD_REPLACE_IF_NEWER = 0x6,
        ADD_RULE_MASK = 0xe,
        ADD_EXISTING_ONLY = 0x10,
    };
    /// 0x08d9e07e
    FFileCatalog();
    /// 0x08d9e0d6
    ~FFileCatalog();
    /// 0x08d9db9e
    UNK_RET GetFolderByIndex(unsigned int a1);
    /// 0x08d9dbc6
    UNK_RET GetNameByHash(unsigned int a1);
    /// 0x08d9e0bc
    UNK_RET ResetValues();
    /// 0x08d9e114
    UNK_RET Release();
    /// 0x08d9e196
    UNK_RET Init(FFileManager* a1);
    /// 0x08d9e2c2
    UNK_RET InitializeCommon(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08d9e394
    UNK_RET FreeEntry(FFileCatalogEntry* a1);
    /// 0x08d9e3c8
    UNK_RET FindFolder(char const* a1) const;
    /// 0x08d9e496
    UNK_RET FindFolder(wchar_t const* a1) const;
    /// 0x08d9e4f4
    UNK_RET AddFolder(char const* a1);
    /// 0x08d9e5c8
    UNK_RET AddFolder(wchar_t const* a1);
    /// 0x08d9e63e
    UNK_RET Scan(char const* a1, FFileCatalog::AddMode a2, char const* a3, char const* a4);
    /// 0x08d9e662
    UNK_RET ScanFolder(char const* a1, FFileCatalog::AddMode a2, char const* a3, char const* a4);
    /// 0x08d9ec7e
    UNK_RET AddFileLoose(unsigned int a1, char const* a2, unsigned int a3, FFileCatalog::AddMode a4, long a5);
    /// 0x08d9ed78
    UNK_RET AddFileLoose(char const* a1, FFileCatalog::AddMode a2);
    /// 0x08d9f1b2
    UNK_RET AddFileLoose(wchar_t const* a1, FFileCatalog::AddMode a2);
    /// 0x08d9f52a
    UNK_RET Find(char const* a1, unsigned int a2) const;
    /// 0x08d9f748
    UNK_RET CalculateHashValue(FStringA const& a1);
    /// 0x08d9f76c
    UNK_RET Find(wchar_t const* a1, unsigned int a2) const;
    /// 0x08d9f9fa
    UNK_RET RemoveEntry(FFileCatalogEntry* a1, FFileCatalog::AddMode a2, long a3);
    /// 0x08d9fae2
    UNK_RET CalculateHashValue(char const* a1);
    /// 0x08d9fc10
    UNK_RET AddName(char const* a1);
    /// 0x08d9fcb6
    UNK_RET GetFreeEntry();
    /// 0x08d9fd10
    UNK_RET AddFileArchived(unsigned int a1, char const* a2, unsigned int a3, unsigned int a4, FFileCatalog::AddMode a5);
    /// 0x08d9fdce
    UNK_RET AddFileArchived(unsigned int a1, char const* a2, FFileCatalogEntry* a3, FFileCatalog::AddMode a4);
    /// 0x08d9fe80
    UNK_RET RemoveFile(FFileCatalogEntry* a1);
    /// 0x08d9fefe
    UNK_RET GetMemoryUsage() const;
    /// 0x08d9ff66
    UNK_RET ShutdownCommon();
    /// 0x08d9ffba
    UNK_RET GetFirstEntry();
    /// 0x08d9ffe6
    UNK_RET GetNextEntry(FHashTable::Iterator& a1);
    /// 0x08da0018
    UNK_RET IsEndEntry(FHashTable::Iterator const& a1);
    /// 0x08da003c
    UNK_RET GetFirstEntry() const;
    /// 0x08da0068
    UNK_RET GetNextEntry(FHashTable::ConstIterator& a1) const;
    /// 0x08da009a
    UNK_RET IsEndEntry(FHashTable::ConstIterator& a1) const;
    /// 0x0ff8bb8c
    static UNK_DATA ms_awszFolders[0x4000];
    /// 0x0ff8fb90
    static UNK_DATA ms_kFolderStorage[0x14];
    /// 0x0ff8bb78
    static UNK_DATA ms_kNameStorage[0x14];
    /// 0x0ff8bb50
    static UNK_DATA ms_kNames[0x28];
    /// 0x0ff8fb8c
    static UNK_DATA ms_nUsedFolders[0x4];
    /// 0x0a312edc
    static UNK_DATA ms_uiDefaultFolderStorageSize[0x4];
    /// 0x0a312ee0
    static UNK_DATA ms_uiDefaultMaxCatalogEntries[0x4];
    /// 0x0a312ed8
    static UNK_DATA ms_uiDefaultNameStorageSize[0x4];
};

extern "C" {
    /// 0x0a312f54
    extern UNK_DATA bytesFromFUTF8[0x100];
    /// 0x0a313054
    extern UNK_DATA firstByteMark[0x7];
    /// 0x0a312f3c
    extern UNK_DATA offsetsFromFUTF8[0x18];
}

namespace FStringTable {
/// 0x0a31305c
extern UNK_DATA ms_aszLanguageCodes[0xa4];
}
