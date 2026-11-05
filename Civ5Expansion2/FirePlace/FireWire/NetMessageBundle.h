/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetMessage;
class FNetSessionIFace;

/// size ?
/// vtable 0x0a304630
class NetMessageBundle : public FImmediateNetMessage {
public:
    /// 0x08d5601c
    /// deleting 0x08d5604c
    virtual ~NetMessageBundle();
    /// 0x08d551c8
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d551c6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d550a4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d5512c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08d55212
    UNK_RET operator+=(FNetMessage const& a1);
    /// 0x08d55246
    UNK_RET Clear();
};
