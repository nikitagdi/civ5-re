/// origin guess
#pragma once
#include "FirePlace/FireWorks/FTimedTriggerSound.h"

class FClassId;
class FContext;
class FXmlElement;

/// size ?
/// vtable 0x0a300030
class cvSoundEffectTrigger : public FTimedTriggerSound {
public:
    /// 0x08a26a78
    cvSoundEffectTrigger();
    /// 0x08a26be2
    /// deleting 0x08a26ade
    /// thunk 0x08a26b06
    /// thunk 0x08a26b32
    /// thunk 0x08a26b5e
    /// thunk 0x08a26b8a
    /// thunk 0x08a26bb6
    /// thunk 0x08a26bf6
    /// thunk 0x08a26c1a
    /// thunk 0x08a26c3e
    /// thunk 0x08a26c62
    /// thunk 0x08a26c86
    virtual ~cvSoundEffectTrigger();
    /// 0x08a26cf2
    /// thunk 0x08a26d7c
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08a26caa
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08a26da8
    virtual UNK_RET fired(FContext& a1);
    /// 0x08a2702a
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08a2704a
    virtual UNK_RET vf_01(UNK_PARAMS);
};
