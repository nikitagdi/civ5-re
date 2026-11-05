/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3041e0
class NetWelcome : public FImmediateNetMessage {
public:
    /// 0x08d55bb0
    virtual ~NetWelcome();
    /// 0x08d50e00
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d50e50
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d50d2c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d50d96
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
