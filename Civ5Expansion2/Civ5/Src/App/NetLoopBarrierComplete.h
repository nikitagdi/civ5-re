/// origin guess
#pragma once
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb140
class NetLoopBarrierComplete : public FImmediateNetMessage {
public:
    /// 0x08711ef8
    NetLoopBarrierComplete();
    /// 0x08711f46
    /// deleting 0x08711f32
    virtual ~NetLoopBarrierComplete();
    /// 0x08711f48
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08703f0c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
};
