/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fafc0
class NetTurnSlice : public FImmediateNetMessage {
public:
    /// 0x0870d24a
    NetTurnSlice();
    /// 0x0870d2a4
    /// deleting 0x0870d290
    virtual ~NetTurnSlice();
    /// 0x0870d4ee
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08704462
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870d2bc
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870d2f8
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x0870d2a6
    UNK_RET getTurnSlice();
    /// 0x0870d658
    UNK_RET update();
};
