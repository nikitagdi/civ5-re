/// origin guess
#pragma once

class FSmallObjectAllocatorInitParams;

/// size ?
class FSmallChunkHash {
public:
    /// 0x08dbdec8
    FSmallChunkHash(FSmallObjectAllocatorInitParams const& a1);
    /// 0x08dbe06a
    ~FSmallChunkHash();
    /// 0x08dbe05a
    UNK_RET GetPhysPageAddress(unsigned int a1);
    /// 0x08dbe0c2
    UNK_RET AllocatePhysPage(void* a1);
    /// 0x08dbe184
    UNK_RET GetPhysPageIndex(void const* a1);
    /// 0x08dbe194
    UNK_RET GetPhysPageLocked(unsigned int a1);
    /// 0x08dbe1ae
    UNK_RET DeallocatePhysPage(void* a1);
    /// 0x08dbe25c
    UNK_RET AllocatePage(unsigned int a1);
    /// 0x08dbe360
    UNK_RET GetVirtPageAddress(unsigned int a1);
    /// 0x08dbe370
    UNK_RET DeallocatePage(void* a1);
    /// 0x08dbe3d2
    UNK_RET GetVirtPageIndex(void const* a1);
    /// 0x08dbe3e2
    UNK_RET GetObjectSize(void const* a1, unsigned int& a2);
    /// 0x08dbe442
    UNK_RET GetUsedVirtPages() const;
    /// 0x08dbe44a
    UNK_RET GetUsedPhysPages() const;
    /// 0x08dbe452
    UNK_RET GetReservedPhysMemory() const;
    /// 0x08dbe45a
    UNK_RET GetCommittedMemorySize() const;
};
