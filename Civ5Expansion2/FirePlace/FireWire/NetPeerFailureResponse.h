/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304260
class NetPeerFailureResponse : public FForgettableNetMessage {
public:
    /// 0x08d55cac
    virtual ~NetPeerFailureResponse();
    /// 0x08d51e52
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d51e98
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d51dda
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d51e16
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
