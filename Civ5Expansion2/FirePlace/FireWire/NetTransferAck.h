/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3045d0
class NetTransferAck : public FForgettableNetMessage {
public:
    /// 0x08d55ff4
    virtual ~NetTransferAck();
    /// 0x08d54ef8
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d54f48
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d54e60
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d54eac
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
