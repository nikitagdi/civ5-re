/// origin binary
#pragma once
#include "FirePlace/FireWorks/FIFile.h"

class FCompressionMgr;
class FFile;
class FFileCatalog;
class FFileCatalogEntry;
class FPakBatchLoadInfo;

/// size ?
class FPakFile {
public:
    /// 0x08da9114
    FPakFile();
    /// 0x08da924a
    ~FPakFile();
    /// 0x08da90e4
    UNK_RET GetCompressionMgr();
    /// 0x08da90fa
    UNK_RET SetCompressionMgr(FCompressionMgr* a1);
    /// 0x08da91e8
    UNK_RET ResetValues();
    /// 0x08da92a6
    UNK_RET Release();
    /// 0x08da92dc
    UNK_RET CloseArchive();
    /// 0x08da9300
    UNK_RET GetFileViewCaret(FFile* a1) const;
    /// 0x08da9304
    UNK_RET GetFileViewEnd(FFile* a1) const;
    /// 0x08da9308
    UNK_RET GetFileViewPtr(FFile* a1) const;
    /// 0x08da930c
    UNK_RET OpenFile(char const* a1);
    /// 0x08da9310
    UNK_RET GetFileLength(FFile* a1);
    /// 0x08da9324
    UNK_RET GetFileModifiedTime() const;
    /// 0x08da93ce
    UNK_RET SeekFile(FFile* a1, long a2, FIFile::SeekMode a3);
    /// 0x08da9468
    UNK_RET OpenArchive(char const* a1, FFileCatalog* a2, unsigned int a3);
    /// 0x08da9604
    UNK_RET OpenArchive(FFileCatalog* a1, unsigned int a2, char const* a3);
    /// 0x08da9618
    UNK_RET ScanArchive(FFileCatalog* a1, unsigned int a2, char const* a3);
    /// 0x08da9dee
    UNK_RET ReadArchiveHeader();
    /// 0x08daa1c0
    UNK_RET BatchLoad(FPakBatchLoadInfo const& a1, unsigned int a2, unsigned int a3);
    /// 0x08daa3ba
    UNK_RET OpenFile(FFileCatalogEntry* a1);
    /// 0x08daa3c0
    UNK_RET CloseFile(FFileCatalogEntry* a1);
    /// 0x08daa3c2
    UNK_RET ReadFile(FFile* a1, void* a2, unsigned int a3);
    /// 0x08daa4a8
    UNK_RET GetFileName() const;
    /// 0x10090164
    static UNK_DATA ms_pkCompressionMgr[0x4];
};

extern "C" {
    /// 0x10090524
    extern UNK_DATA g_bPreventSBACreation[0x1];
}

namespace FMemHooks {
/// 0x1009051c
extern UNK_DATA ms_uiValidateFrequency[0x4];
/// 0x10090520
extern UNK_DATA ms_uiValidateIndex[0x4];
/// 0x10090518
extern UNK_DATA ms_uiValidateOptions[0x4];
}
