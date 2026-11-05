/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304230
class NetPeerFailure : public FForgettableNetMessage {
public:
    /// 0x08d55c98
    virtual ~NetPeerFailure();
    /// 0x08d51ca8
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d51cde
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d51c30
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d51c6c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
