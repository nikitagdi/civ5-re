/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3045a0
class NetTransferError : public FForgettableNetMessage {
public:
    /// 0x08d55fe0
    virtual ~NetTransferError();
    /// 0x08d54d8c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d54dc2
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d54d14
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d54d50
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
