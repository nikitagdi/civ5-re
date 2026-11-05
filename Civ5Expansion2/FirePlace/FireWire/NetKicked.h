/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304460
class NetKicked : public FImmediateNetMessage {
public:
    /// 0x08d55e92
    virtual ~NetKicked();
    /// 0x08d52ef8
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d52f36
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d52e60
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d52eac
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
