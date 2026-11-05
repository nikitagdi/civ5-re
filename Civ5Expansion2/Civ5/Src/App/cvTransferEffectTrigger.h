/// origin guess
#pragma once
#include "FirePlace/FireWorks/FTimedTriggerTransfer.h"

class FClassId;
class FContext;
class FXmlElement;

/// size ?
/// vtable 0x0a300270
class cvTransferEffectTrigger : public FTimedTriggerTransfer {
public:
    /// 0x08a27928
    cvTransferEffectTrigger();
    /// 0x08a27a4a
    /// deleting 0x08a27972
    /// thunk 0x08a2799a
    /// thunk 0x08a279c6
    /// thunk 0x08a279f2
    /// thunk 0x08a27a1e
    /// thunk 0x08a27a5e
    /// thunk 0x08a27a82
    /// thunk 0x08a27aa6
    /// thunk 0x08a27aca
    virtual ~cvTransferEffectTrigger();
    /// 0x08a27b36
    /// thunk 0x08a27b4a
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08a27aee
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08a27b76
    virtual UNK_RET fired(FContext& a1);
    /// 0x08a2800c
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08a2802c
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08a27c06
    UNK_RET OnFired(unsigned int a1, FContext& a2);
};
