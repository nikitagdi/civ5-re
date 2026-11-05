/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304850
class NetAuthorizationResponseSteam : public FImmediateNetMessage {
public:
    /// 0x08d5e7ce
    NetAuthorizationResponseSteam(unsigned int a1);
    /// 0x08d6034e
    virtual ~NetAuthorizationResponseSteam();
    /// 0x08d5e88a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d5e8c0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d5e812
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d5e84e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
