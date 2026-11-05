/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304600
class NetMuted : public FImmediateNetMessage {
public:
    /// 0x08d56008
    virtual ~NetMuted();
    /// 0x08d5500a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d55050
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d54f92
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d54fce
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
