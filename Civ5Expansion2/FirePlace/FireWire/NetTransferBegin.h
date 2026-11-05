/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3041a0
class NetTransferBegin : public FForgettableNetMessage {
public:
    /// 0x08d5473e
    NetTransferBegin(unsigned int a1, unsigned int a2);
    /// 0x08d55fb8
    virtual ~NetTransferBegin();
    /// 0x08d54828
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d54866
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d54790
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d547dc
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
