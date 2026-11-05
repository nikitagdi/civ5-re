/// origin guess
#pragma once
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb0c0
class NetFlushMessagesReply : public FImmediateNetMessage {
public:
    /// 0x08711042
    NetFlushMessagesReply();
    /// 0x0871ae88
    virtual ~NetFlushMessagesReply();
    /// 0x0871107c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087110d0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
};
