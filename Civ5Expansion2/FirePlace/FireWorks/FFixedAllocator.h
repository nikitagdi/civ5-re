/// origin guess
#pragma once

class FSmallChunkHash;
class FSmallObjectAllocatorInitParams;

/// size ?
class FFixedAllocator {
public:
    class Chunk;
    /// size ?
    class Chunk {
    public:
        /// 0x08dbe466
        UNK_RET Init(FSmallChunkHash* a1, unsigned int a2, unsigned short a3);
        /// 0x08dbe4c8
        UNK_RET Reset(unsigned int a1, unsigned short a2);
        /// 0x08dbe4f6
        UNK_RET Release(FSmallChunkHash* a1);
        /// 0x08dbe522
        UNK_RET Allocate(unsigned int a1, unsigned int a2);
        /// 0x08dbe554
        UNK_RET Deallocate(void* a1, unsigned int a2, unsigned int a3);
        /// 0x08dbe574
        UNK_RET HasAvailable(unsigned short a1) const;
        /// 0x08dbe588
        UNK_RET HasBlock(unsigned char* a1, unsigned int a2) const;
        /// 0x08dbe5a8
        UNK_RET IsFilled() const;
        /// 0x08dbe5b8
        UNK_RET Validate(unsigned int a1, unsigned int a2, unsigned int a3);
    };
    /// 0x08dbe5f0
    FFixedAllocator(FSmallObjectAllocatorInitParams const& a1);
    /// 0x08dbe6aa
    ~FFixedAllocator();
    /// 0x08dbe656
    UNK_RET Init(FSmallChunkHash* a1, unsigned int a2);
    /// 0x08dbe716
    UNK_RET Push_Back(FFixedAllocator::Chunk& a1);
    /// 0x08dbe75a
    UNK_RET Reserve(unsigned int a1);
    /// 0x08dbe7ae
    UNK_RET Pop_Back();
    /// 0x08dbe7b6
    UNK_RET Allocate();
    /// 0x08dbe8f2
    UNK_RET Deallocate(void* a1);
    /// 0x08dbe944
    UNK_RET VicinityFind(void* a1);
    /// 0x08dbe9d6
    UNK_RET DoDeallocate(void* a1);
    /// 0x08dbead4
    UNK_RET Validate(unsigned int a1);
};
