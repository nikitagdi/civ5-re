/// origin guess
#pragma once

/// size ?
class FFileCatalogNameStorageW {
public:
    /// 0x08d9de7c
    ~FFileCatalogNameStorageW();
    /// 0x08d9deac
    UNK_RET Deallocate();
    /// 0x08d9ded4
    UNK_RET Initialize(unsigned int a1, unsigned int a2);
    /// 0x08d9df5c
    UNK_RET Allocate(unsigned int a1);
    /// 0x08d9e010
    UNK_RET GetSize() const;
    /// 0x08d9e02e
    UNK_RET GetSizeBytes() const;
    /// 0x08d9e044
    UNK_RET GetUsed() const;
    /// 0x08d9e064
    UNK_RET GetUsedBytes() const;
};
