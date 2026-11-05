/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa5c0
class NetInitInfo : public FImmediateNetMessage {
public:
    /// 0x086fd60e
    NetInitInfo();
    /// 0x086fd6dc
    /// deleting 0x086fd6b4
    virtual ~NetInitInfo();
    /// 0x086fd76c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fd842
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fdc78
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fdd4c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
