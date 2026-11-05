/// origin binary
#pragma once

class FHashTable;

/// size ?
class FHashTable {
public:
    class ConstIterator;
    class Iterator;
    /// size ?
    class ConstIterator {
    public:
        /// 0x08dd4090
        ConstIterator(FHashTable const* a1);
        /// 0x08dd40f0
        ConstIterator();
        /// 0x08dd410a
        ConstIterator(FHashTable::ConstIterator const& a1);
        /// 0x08dd40bc
        UNK_RET operator++();
        /// 0x08dd4124
        UNK_RET operator=(FHashTable::ConstIterator const& a1);
        /// 0x08dd413e
        UNK_RET operator*() const;
        /// 0x08dd4150
        UNK_RET IsEnd() const;
    };
    class Entry;
    /// size ?
    class Iterator {
    public:
        /// 0x08dd3f84
        Iterator(FHashTable* a1);
        /// 0x08dd3fe4
        Iterator();
        /// 0x08dd3ffe
        Iterator(FHashTable::Iterator const& a1);
        /// 0x08dd3fb0
        UNK_RET operator++();
        /// 0x08dd4018
        UNK_RET operator=(FHashTable::Iterator const& a1);
        /// 0x08dd4032
        UNK_RET operator*();
        /// 0x08dd4044
        UNK_RET IsEnd() const;
    };
    /// 0x08dd39c6
    FHashTable();
    /// 0x08dd3a5a
    ~FHashTable();
    /// 0x08dd3a10
    UNK_RET vResetValues();
    /// 0x08dd3ac8
    UNK_RET vRelease();
    /// 0x08dd3b36
    UNK_RET vInit(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5);
    /// 0x08dd3c12
    UNK_RET vResize(unsigned int a1);
    /// 0x08dd3d50
    UNK_RET eInsert(unsigned int a1, void* a2, unsigned int a3);
    /// 0x08dd3dce
    UNK_RET pkGetFreeEntry();
    /// 0x08dd3de4
    UNK_RET eAddSingleNode(FHashTable::Entry* a1, FHashTable::Entry** a2, unsigned int a3);
    /// 0x08dd3e68
    UNK_RET pvRemove(unsigned int a1);
    /// 0x08dd3e9a
    UNK_RET pvRemove(unsigned int a1, void* a2);
    /// 0x08dd3f4c
    UNK_RET pvFind(unsigned int a1) const;
    /// 0x08dd4058
    UNK_RET Begin();
    /// 0x08dd4164
    UNK_RET Begin() const;
};
