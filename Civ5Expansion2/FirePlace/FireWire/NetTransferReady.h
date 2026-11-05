/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304180
class NetTransferReady : public FForgettableNetMessage {
public:
    /// 0x08d54604
    NetTransferReady(unsigned int a1);
    /// 0x08d55fa4
    virtual ~NetTransferReady();
    /// 0x08d546c4
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d546fa
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d5464c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d54688
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
