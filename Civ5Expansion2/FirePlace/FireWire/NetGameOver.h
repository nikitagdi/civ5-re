/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304370
class NetGameOver : public FImmediateNetMessage {
public:
    /// 0x08d55e2e
    virtual ~NetGameOver();
    /// 0x08d543b0
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d543d0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d54388
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d5439c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
