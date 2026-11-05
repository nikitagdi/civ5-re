/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304400
class NetForcePlayerCommTurn : public FImmediateNetMessage {
public:
    /// 0x08d55e6a
    virtual ~NetForcePlayerCommTurn();
    /// 0x08d52db0
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d52dee
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d52d18
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d52d64
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
