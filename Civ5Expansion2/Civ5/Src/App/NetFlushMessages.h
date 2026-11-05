/// origin guess
#pragma once
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb0a0
class NetFlushMessages : public FImmediateNetMessage {
public:
    /// 0x08710eac
    NetFlushMessages(bool a1);
    /// 0x08710ef0
    NetFlushMessages();
    /// 0x08710f3e
    /// deleting 0x08710f2a
    virtual ~NetFlushMessages();
    /// 0x08710f40
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08710fa0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08711004
    UNK_RET waitingForResponses();
};
