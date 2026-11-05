/// origin guess
#pragma once
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb120
class NetStartLaunch : public FImmediateNetMessage {
public:
    /// 0x087117e8
    NetStartLaunch();
    /// 0x0871ae9c
    virtual ~NetStartLaunch();
    /// 0x08711822
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08711856
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
};
