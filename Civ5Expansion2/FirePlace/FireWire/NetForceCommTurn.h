/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3043d0
class NetForceCommTurn : public FImmediateNetMessage {
public:
    /// 0x08d55e56
    virtual ~NetForceCommTurn();
    /// 0x08d52c68
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d52cda
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d52c2c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d52c9e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
