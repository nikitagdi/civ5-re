/// origin binary
#pragma once
#include "FirePlace/FireWorks/FITimedTriggerSystem.h"
#include "FirePlace/FireWorks/FString.h"

class FClassId;
class FITimedTrigger;

/// size ?
/// vtable 0x0a306970
class FTimedTriggerSystem : public FITimedTriggerSystem {
public:
    char unk_4[0x30];    ///< +0x4
    /// 0x08dcb03c
    FTimedTriggerSystem();
    /// 0x08dcb0f8
    /// deleting 0x08dcb0d0
    virtual ~FTimedTriggerSystem();
    /// 0x08dcb4fa
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08dcba02
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dcba22
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dcb1b6
    virtual UNK_RET add_trigger(FITimedTrigger* a1, int a2);
    /// 0x08dcba58
    virtual UNK_RET FinalizeTriggers();
    /// 0x08dcb3ec
    virtual UNK_RET size() const;
    /// 0x08dcb460
    virtual UNK_RET get_triggers() const;
    /// 0x08dcb468
    virtual UNK_RET get_trigger(int a1) const;
    /// 0x08dcb458
    virtual UNK_RET get_sorted() const;
    /// 0x08dcb4b4
    virtual UNK_RET set_resource_name(FStringA const& a1);
    /// 0x08dcb4f2
    virtual UNK_RET get_resource_name() const;
    /// 0x08ce3a16
    virtual UNK_RET vf_13(UNK_PARAMS);
    /// 0x08ce3a22
    virtual UNK_RET vf_14(UNK_PARAMS);
    /// 0x08dcb2f8
    virtual UNK_RET remove_trigger(int a1);
    /// 0x08dcb3f4
    virtual UNK_RET clear();
    /// 0x08dcb176
    UNK_RET get_new_id();
};
