/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIGrannyTimedTrigger.h"
#include "FirePlace/FireWorks/FTimedTrigger.h"

class FClassId;
class FXmlElement;

/// size ?
/// vtable 0x0a302400
class FTimedTriggerTransfer : public FTimedTrigger, public FIGrannyTimedTrigger {
public:
    /// 0x08ce1fc4
    FTimedTriggerTransfer();
    /// 0x08ce211a
    /// deleting 0x08ce2042
    /// thunk 0x08ce206a
    /// thunk 0x08ce2096
    /// thunk 0x08ce20c2
    /// thunk 0x08ce20ee
    /// thunk 0x08ce212e
    /// thunk 0x08ce2152
    /// thunk 0x08ce2176
    /// thunk 0x08ce219a
    virtual ~FTimedTriggerTransfer();
    /// 0x08ce2250
    /// thunk 0x08ce23b2
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08ce23de
    /// thunk 0x08ce252e
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08ce21be
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08ce2582
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08ce25a2
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08ce255a
    /// thunk 0x08ce2566
    virtual UNK_RET SetEventCode(int a1);
    /// 0x08ce2572
    /// thunk 0x08ce257a
    virtual UNK_RET GetEventCode() const;
    /// 0x08a28062
    virtual UNK_RET vf_18(UNK_PARAMS);
    /// 0x08ce2228
    /// thunk 0x08ce223c
    virtual UNK_RET GetDisplayValue() const;
};
