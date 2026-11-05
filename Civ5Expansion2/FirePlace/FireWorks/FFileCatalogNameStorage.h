/// origin guess
#pragma once

/// size ?
class FFileCatalogNameStorage {
public:
    /// 0x08d9dc90
    ~FFileCatalogNameStorage();
    /// 0x08d9dcc0
    UNK_RET Deallocate();
    /// 0x08d9dce8
    UNK_RET Initialize(unsigned int a1, unsigned int a2);
    /// 0x08d9dd70
    UNK_RET Allocate(unsigned int a1);
    /// 0x08d9de1c
    UNK_RET GetSize() const;
    /// 0x08d9de32
    UNK_RET GetSizeBytes() const;
    /// 0x08d9de48
    UNK_RET GetUsed() const;
    /// 0x08d9de62
    UNK_RET GetUsedBytes() const;
};
