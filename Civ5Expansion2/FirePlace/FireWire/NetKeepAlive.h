/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304430
class NetKeepAlive : public FForgettableNetMessage {
public:
    /// 0x08d55e7e
    virtual ~NetKeepAlive();
    /// 0x08d52e5c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d52e5e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d52e34
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d52e48
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
