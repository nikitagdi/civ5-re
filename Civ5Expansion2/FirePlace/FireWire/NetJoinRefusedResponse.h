/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3044c0
class NetJoinRefusedResponse : public FImmediateNetMessage {
public:
    /// 0x08d55eba
    virtual ~NetJoinRefusedResponse();
    /// 0x08d530ec
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d53120
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d530c4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d530d8
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
