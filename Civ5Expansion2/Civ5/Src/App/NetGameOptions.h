/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb400
class NetGameOptions : public FNetMessage {
public:
    /// 0x087165e8
    NetGameOptions();
    /// 0x0871afdc
    /// deleting 0x0871b00c
    virtual ~NetGameOptions();
    /// 0x0871674a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08716b02
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08716790
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08716804
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08716634
    UNK_RET Add(char const* a1, int a2);
};
