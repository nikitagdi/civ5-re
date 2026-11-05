/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa960
class NetMPDropResult : public FNetMessage {
public:
    /// 0x08701d96
    NetMPDropResult(int a1);
    /// 0x0871a810
    virtual ~NetMPDropResult();
    /// 0x08701dd6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f65ae
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08701e14
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08701e50
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
