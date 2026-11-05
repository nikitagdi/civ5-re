/// origin guess
#pragma once
#include "FirePlace/FireWorks/FNetCallbackIFaceENet.h"

/// size ?
/// vtable 0x0a305590
class FNetCallbacksENet : public FNetCallbackIFaceENet {
public:
    /// 0x08d7a3ac
    FNetCallbacksENet(int a1);
    /// 0x08d7acb8
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08d7acba
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08d7a3d0
    virtual UNK_RET ConnectionAttempt(long a1, unsigned char const* a2, unsigned int a3);
    /// 0x08d7a9fc
    virtual UNK_RET ConnectionAccepted(long a1, unsigned char const* a2, unsigned int a3);
    /// 0x08d7a468
    virtual UNK_RET ConnectionComplete(long a1, unsigned char const* a2, unsigned int a3);
    /// 0x08d7aa68
    virtual UNK_RET ConnectionRejected(long a1, unsigned char const* a2, unsigned int a3);
    /// 0x08d7aaea
    virtual UNK_RET MessageReceived(long a1, unsigned char const* a2, unsigned int a3, bool a4);
    /// 0x08d7ac1e
    virtual UNK_RET ConnectionClosed(long a1);
    /// 0x08d7a438
    UNK_RET GetSession() const;
};
