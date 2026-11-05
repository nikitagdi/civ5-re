/// origin guess
#pragma once
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb1e0
class NetHotJoinCompleted : public FImmediateNetMessage {
public:
    /// 0x08713242
    NetHotJoinCompleted();
    /// 0x08713290
    /// deleting 0x0871327c
    virtual ~NetHotJoinCompleted();
    /// 0x08713292
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087132c6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
};
