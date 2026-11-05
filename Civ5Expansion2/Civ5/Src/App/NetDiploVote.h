/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faa60
class NetDiploVote : public FNetMessage {
public:
    /// 0x08702f6a
    NetDiploVote();
    /// 0x08702fae
    NetDiploVote(PlayerTypes a1, PlayerTypes a2);
    /// 0x0871a8b0
    virtual ~NetDiploVote();
    /// 0x08702ff8
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f667c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870302c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08703078
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
