/// origin guess
#pragma once

namespace Modding {
/// size ?
/// vtable 0x0a301268
class Archive {
public:
    /// 0x08c7cf54
    Archive();
    /// 0x08c7d0a4
    /// deleting 0x08c7d07c
    virtual ~Archive();
    /// 0x08c7cf9c
    UNK_RET SzAlloc(void* a1, unsigned int a2);
    /// 0x08c7cfd8
    UNK_RET SzFree(void* a1, void* a2);
    /// 0x08c7d00c
    UNK_RET SzAllocTemp(void* a1, unsigned int a2);
    /// 0x08c7d048
    UNK_RET SzFreeTemp(void* a1, void* a2);
    /// 0x08c7d0d8
    UNK_RET Close();
    /// 0x08c7d12c
    UNK_RET Open(wchar_t const* a1);
    /// 0x08c7d1d4
    UNK_RET GetFileName(unsigned int a1, wchar_t* a2, unsigned int a3);
    /// 0x08c7d254
    UNK_RET GetFileOffsetAndSize(unsigned int a1, unsigned int& a2, unsigned int& a3);
    /// 0x08c7d2d8
    UNK_RET GetFolderIndex(unsigned int a1);
    /// 0x08c7d300
    UNK_RET GetFolderSize(unsigned int a1);
    /// 0x08c7d32e
    UNK_RET DecodeFolder(unsigned int a1, void* a2, unsigned int a3);
    /// 0x08c7d446
    UNK_RET ExtractContents(wchar_t const* a1, unsigned int a2);
};
}
