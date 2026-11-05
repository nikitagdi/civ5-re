/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIGrannyTimedTrigger.h"
#include "FirePlace/FireWorks/FTimedTrigger.h"

class FClassId;
class FXmlElement;

/// size ?
/// vtable 0x0a302090
class FTimedTriggerEffect : public FTimedTrigger, public FIGrannyTimedTrigger {
public:
    /// 0x08ce09a8
    FTimedTriggerEffect();
    /// 0x08ce0ae6
    /// deleting 0x08ce0a0e
    /// thunk 0x08ce0a36
    /// thunk 0x08ce0a62
    /// thunk 0x08ce0a8e
    /// thunk 0x08ce0aba
    /// thunk 0x08ce0afa
    /// thunk 0x08ce0b1e
    /// thunk 0x08ce0b42
    /// thunk 0x08ce0b66
    virtual ~FTimedTriggerEffect();
    /// 0x08ce0bf4
    /// thunk 0x08ce0d08
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08ce0d34
    /// thunk 0x08ce0e14
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08ce0b8a
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08ce10b0
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ce10d0
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08ce0e8a
    /// thunk 0x08ce0e9a
    virtual UNK_RET SetEventCode(int a1);
    /// 0x08ce0ea6
    /// thunk 0x08ce0eb2
    virtual UNK_RET GetEventCode() const;
    /// 0x08ce0eba
    /// thunk 0x08ce0ec2
    virtual UNK_RET GetGrannyBoneName() const;
    /// 0x087ff3ba
    virtual UNK_RET vf_19(UNK_PARAMS);
    /// 0x08ce0e40
    UNK_RET SetEventName(char const* a1);
    /// 0x08ce0e82
    UNK_RET GetEventName() const;
    /// 0x08ce0eca
    UNK_RET SetBoneName(char const* a1);
    /// 0x08ce0f0c
    UNK_RET GetBoneName() const;
};
