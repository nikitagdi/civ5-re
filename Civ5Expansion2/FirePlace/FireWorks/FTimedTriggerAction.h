/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIGrannyTimedTrigger.h"
#include "FirePlace/FireWorks/FTimedTrigger.h"

class FClassId;
class FXmlElement;

/// size ?
/// vtable 0x0a301f70
class FTimedTriggerAction : public FTimedTrigger, public FIGrannyTimedTrigger {
public:
    /// 0x08ce0348
    FTimedTriggerAction();
    /// 0x08ce0486
    /// deleting 0x08ce03ae
    /// thunk 0x08ce03d6
    /// thunk 0x08ce0402
    /// thunk 0x08ce042e
    /// thunk 0x08ce045a
    /// thunk 0x08ce049a
    /// thunk 0x08ce04be
    /// thunk 0x08ce04e2
    /// thunk 0x08ce0506
    virtual ~FTimedTriggerAction();
    /// 0x08ce0594
    /// thunk 0x08ce0672
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08ce069e
    /// thunk 0x08ce075a
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08ce052a
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08ce0844
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ce0864
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08ce079a
    /// thunk 0x08ce07a6
    virtual UNK_RET SetEventCode(int a1);
    /// 0x08ce07b2
    /// thunk 0x08ce07ba
    virtual UNK_RET GetEventCode() const;
    /// 0x08ce07c2
    /// thunk 0x08ce07ca
    virtual UNK_RET GetGrannyBoneName() const;
    /// 0x08ce081c
    /// thunk 0x08ce0830
    virtual UNK_RET GetDisplayValue() const;
    /// 0x08ce0786
    UNK_RET SetActionCode(int a1);
    /// 0x08ce0792
    UNK_RET GetActionCode() const;
    /// 0x08ce07d2
    UNK_RET SetBoneName(char const* a1);
    /// 0x08ce0814
    UNK_RET GetBoneName() const;
};
