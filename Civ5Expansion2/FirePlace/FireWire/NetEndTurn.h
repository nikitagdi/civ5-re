/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3043a0
class NetEndTurn : public FImmediateNetMessage {
public:
    /// 0x08d55e42
    virtual ~NetEndTurn();
    /// 0x08d52baa
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d52be0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d52b32
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d52b6e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
