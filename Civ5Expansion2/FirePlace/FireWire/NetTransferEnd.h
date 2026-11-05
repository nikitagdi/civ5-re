/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304570
class NetTransferEnd : public FForgettableNetMessage {
public:
    /// 0x08d55fcc
    virtual ~NetTransferEnd();
    /// 0x08d54c26
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d54c64
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d54b8e
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d54bda
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
