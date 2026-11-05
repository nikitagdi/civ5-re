/// origin guess
#pragma once
#include "FirePlace/FireWorks/FTimedTriggerAction.h"

class FClassId;
class FContext;

/// size ?
/// vtable 0x0a2fff30
class cvActionTrigger : public FTimedTriggerAction {
public:
    /// 0x08a26520
    cvActionTrigger();
    /// 0x08a26642
    /// deleting 0x08a2656a
    /// thunk 0x08a26592
    /// thunk 0x08a265be
    /// thunk 0x08a265ea
    /// thunk 0x08a26616
    /// thunk 0x08a26656
    /// thunk 0x08a2667a
    /// thunk 0x08a2669e
    /// thunk 0x08a266c2
    virtual ~cvActionTrigger();
    /// 0x08a266e6
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08a2672e
    virtual UNK_RET fired(FContext& a1);
    /// 0x08a26892
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08a268b2
    virtual UNK_RET vf_01(UNK_PARAMS);
};
