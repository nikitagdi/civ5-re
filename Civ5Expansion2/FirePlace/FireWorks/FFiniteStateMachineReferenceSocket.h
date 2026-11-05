/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFSMSocket.h"

class FContext;
class FIFSMCondition;
class FIFSMState;

/// size ?
/// vtable 0x0a306530
class FFiniteStateMachineReferenceSocket : public FIFSMSocket {
public:
    /// 0x08db1f54
    FFiniteStateMachineReferenceSocket(FIFSMState& a1);
    /// 0x08db1fe2
    /// deleting 0x08db1fba
    virtual ~FFiniteStateMachineReferenceSocket();
    /// 0x08db26c6
    virtual UNK_RET GetUniqueId() const;
    /// 0x08db26ce
    virtual UNK_RET SetUniqueId(int a1);
    /// 0x08db201a
    virtual UNK_RET evaluate(FContext& a1) const;
    /// 0x08db2186
    virtual UNK_RET get_owner_id() const;
    /// 0x08db21aa
    virtual UNK_RET get_state_id() const;
    /// 0x08db21b2
    virtual UNK_RET connect(int a1);
    /// 0x08db21be
    virtual UNK_RET disconnect();
    /// 0x08db21ea
    virtual UNK_RET is_bidirectional() const;
    /// 0x08db2208
    virtual UNK_RET set_bidirectional(bool a1);
    /// 0x08db21f4
    virtual UNK_RET set_order(int a1);
    /// 0x08db2200
    virtual UNK_RET get_order() const;
    /// 0x08db2244
    virtual UNK_RET make_condition(char const* a1);
    /// 0x08db23e0
    virtual UNK_RET add_condition(FIFSMCondition* a1);
    /// 0x08db2470
    virtual UNK_RET add_condition(char const* a1);
    /// 0x08db24a8
    virtual UNK_RET add_condition(char const* a1, float a2);
    /// 0x08db24f8
    virtual UNK_RET remove_condition(int a1);
    /// 0x08db25ea
    virtual UNK_RET find_condition(int a1) const;
    /// 0x08db25a6
    virtual UNK_RET sort_conditions();
    /// 0x08db2648
    virtual UNK_RET get_num_conditions() const;
    /// 0x08db2656
    virtual UNK_RET get_condition_by_index(int a1);
    /// 0x08db23b4
    virtual UNK_RET ReserveConditions(unsigned int a1);
    /// 0x08db2666
    virtual UNK_RET clear();
    /// 0x08db26be
    virtual UNK_RET get_state();
};

/// 0x08db3945
UNK_RET FireMallocFromSystem(unsigned int a1, char const* a2, int a3);
/// 0x08db396d
UNK_RET FireMallocAligned(unsigned int a1, unsigned int a2, char const* a3, int a4, int a5, int a6);
/// 0x08db39be
UNK_RET FireCalloc(unsigned int a1, unsigned int a2, char const* a3, int a4, int a5, int a6);
/// 0x08db39d2
UNK_RET FireCallocNoTracking(unsigned int a1, unsigned int a2, char const* a3, int a4);
/// 0x08db39e6
UNK_RET FireReallocNoTracking(void* a1, unsigned int a2, char const* a3, int a4);
/// 0x08db39fa
UNK_RET FireReallocFromSystem(void* a1, unsigned int a2, char const* a3, int a4);
/// 0x08db3a0e
UNK_RET FireReallocAligned(void* a1, unsigned int a2, unsigned int a3, char const* a4, int a5, int a6, int a7);
/// 0x08db3a22
UNK_RET FireReallocAlignedNoTracking(void* a1, unsigned int a2, unsigned int a3);
/// 0x08db3a36
UNK_RET FireReallocAlignedNoTracking(void* a1, unsigned int a2, unsigned int a3, char const* a4, int a5);
/// 0x08db3a58
UNK_RET FireFree(void* a1);
/// 0x08db3a80
UNK_RET FireFreeAligned(void* a1);
/// 0x08db3aa8
UNK_RET FireMemSize(void* a1);
/// 0x08db3abc
UNK_RET TrackAllocate(void const* a1, unsigned int a2, char const* a3, int a4, int a5, int a6);
/// 0x08db3ac1
UNK_RET TrackReallocate(void const* a1, void* a2, unsigned int a3, char const* a4, int a5, int a6, int a7);
/// 0x08db3ac6
UNK_RET TrackFree(void const* a1);
/// 0x08db3ad1
UNK_RET FMemoryManagerAutoValidateAllocation();
/// 0x08db3ad7
UNK_RET FMemoryManagerAutoValidateDeallocation();
/// 0x08db3b32
UNK_RET FMemoryManagerDisableSBA(bool a1);
/// 0x08db3b54
UNK_RET Validate();
