/// origin guess
#pragma once

namespace Modding {
class InstallerData;
}

namespace Modding {
/// size ?
class InstallerData {
public:
    /// 0x08c79dbe
    InstallerData();
    /// 0x08c79e2c
    InstallerData(Modding::InstallerData const& a1);
    /// 0x08c79f0c
    UNK_RET operator=(Modding::InstallerData const& a1);
    /// 0x08c79f5a
    UNK_RET AddArchive(wchar_t const* a1);
    /// 0x08c79fd4
    UNK_RET GetStartCallback() const;
    /// 0x08c79fdc
    UNK_RET SetStartCallback(void (*a1)(void*));
    /// 0x08c79fe8
    UNK_RET GetProgressCallback() const;
    /// 0x08c79ff0
    UNK_RET SetProgressCallback(void (*a1)(void*, int, unsigned int, unsigned int));
    /// 0x08c79ffc
    UNK_RET GetFinishCallback() const;
    /// 0x08c7a004
    UNK_RET SetFinishCallback(void (*a1)(void*, bool));
    /// 0x08c7a010
    UNK_RET GetErrorCallback() const;
    /// 0x08c7a018
    UNK_RET SetErrorCallback(void (*a1)(void*, int, void*, int));
    /// 0x08c7a024
    UNK_RET GetLogCallback() const;
    /// 0x08c7a02c
    UNK_RET SetLogCallback(void (*a1)(void*, char const*));
    /// 0x08c7a038
    UNK_RET GetContext() const;
    /// 0x08c7a040
    UNK_RET SetContext(void* a1);
    /// 0x08c7a04c
    UNK_RET GetArchivePath(unsigned int a1) const;
    /// 0x08c7a070
    UNK_RET GetArchivePathCount() const;
    /// 0x08c7a078
    UNK_RET GetInstallPath() const;
    /// 0x08c7a08a
    UNK_RET SetInstallPath(wchar_t const* a1);
    /// 0x08c7a0c6
    UNK_RET GetModsDBPath() const;
    /// 0x08c7a0d8
    UNK_RET SetModsDBPath(wchar_t const* a1);
};
}
