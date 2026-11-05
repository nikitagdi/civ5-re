/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb420
class NetInitialInfoComplete : public FImmediateNetMessage {
public:
    /// 0x08716b9c
    NetInitialInfoComplete();
    /// 0x08716bde
    NetInitialInfoComplete(int a1);
    /// 0x0871b042
    virtual ~NetInitialInfoComplete();
    /// 0x08716cd6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08716c9a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08716c22
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08716c5e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
