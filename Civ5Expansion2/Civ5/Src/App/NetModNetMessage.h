/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fac00
class NetModNetMessage : public FNetMessage {
public:
    /// 0x0870557c
    NetModNetMessage(int a1, int a2, int a3, int a4, int a5);
    /// 0x087055ee
    /// deleting 0x087055da
    virtual ~NetModNetMessage();
    /// 0x087055f0
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08705714
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08705624
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870569c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
