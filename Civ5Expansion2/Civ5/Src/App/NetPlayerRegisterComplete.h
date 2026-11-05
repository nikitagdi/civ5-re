/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb440
class NetPlayerRegisterComplete : public FImmediateNetMessage {
public:
    /// 0x08716d14
    NetPlayerRegisterComplete();
    /// 0x08716d56
    NetPlayerRegisterComplete(int a1, bool a2);
    /// 0x0871b056
    virtual ~NetPlayerRegisterComplete();
    /// 0x08716efe
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08716e38
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08716da0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08716dec
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
