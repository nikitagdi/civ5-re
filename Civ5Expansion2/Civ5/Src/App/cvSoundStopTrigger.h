/// origin guess
#pragma once
#include "FirePlace/FireWorks/FTimedTriggerStopSound.h"

class FClassId;
class FContext;
class FXmlElement;

/// size ?
/// vtable 0x0a300150
class cvSoundStopTrigger : public FTimedTriggerStopSound {
public:
    /// 0x08a271f8
    cvSoundStopTrigger();
    /// 0x08a27362
    /// deleting 0x08a2725e
    /// thunk 0x08a27286
    /// thunk 0x08a272b2
    /// thunk 0x08a272de
    /// thunk 0x08a2730a
    /// thunk 0x08a27336
    /// thunk 0x08a27376
    /// thunk 0x08a2739a
    /// thunk 0x08a273be
    /// thunk 0x08a273e2
    /// thunk 0x08a27406
    virtual ~cvSoundStopTrigger();
    /// 0x08a27472
    /// thunk 0x08a274fc
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08a2742a
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08a27528
    virtual UNK_RET fired(FContext& a1);
    /// 0x08a27678
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08a27698
    virtual UNK_RET vf_01(UNK_PARAMS);
};
