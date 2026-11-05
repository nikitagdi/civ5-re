/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faae0
class NetKillDeal : public FNetMessage {
public:
    /// 0x087034cc
    NetKillDeal();
    /// 0x0870350e
    NetKillDeal(int a1, bool a2);
    /// 0x0871a972
    virtual ~NetKillDeal();
    /// 0x08703554
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f67b6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08703592
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087035de
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
