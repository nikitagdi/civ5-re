/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFSMConditionVariable.h"
#include "FirePlace/FireWorks/FIConditionEvent.h"

class FClassId;
class FContext;

/// size 0x34
/// vtable 0x0a3060a0
class FFSMConditionEventSetVariable : public FFSMConditionVariable, public FIConditionEvent {
public:
    /// 0x08daf7b2
    /// deleting 0x08daf7f2
    virtual ~FFSMConditionEventSetVariable();
    /// 0x08daf838
    virtual bool evaluate(FContext& a1);
    /// 0x08daf75c
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08daf77c
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dad6d0
    /// thunk 0x08dad712
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08dad756
    /// thunk 0x08dad798
    virtual UNK_RET fire_event(FContext& a1) const;
};
