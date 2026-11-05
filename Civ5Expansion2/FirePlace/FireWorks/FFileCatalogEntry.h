/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"

/// size 0x10
class FFileCatalogEntry {
public:
    /// 0x08d9d9b4
    UNK_RET Clear();
    /// 0x08d9d9d4
    UNK_RET GetFullyQualifiedName(FStringW& a1) const;
    /// 0x08d9db28
    UNK_RET GetName() const;
    /// 0x08d9db56
    UNK_RET GetPath() const;
    /// 0x08d9db84
    UNK_RET SetPathIndex(unsigned int a1);
    unsigned int m_uNameHash; ///< +0x0
    unsigned int m_uSize; ///< +0x4
    // Zero for a loose file; the archive offset otherwise. A free entry keeps the next free entry here.
    unsigned int m_uArchiveOffset; ///< +0x8
    unsigned int : 19;
    // Set when the entry lives in the pak's own array, so RemoveEntry does not return it to the free list.
    unsigned int m_bPakOwned : 1; ///< +0xe
    unsigned int m_uFolderIndex : 12; ///< +0xe
};
