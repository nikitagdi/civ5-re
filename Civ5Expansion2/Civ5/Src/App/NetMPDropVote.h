/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa920
class NetMPDropVote : public FNetMessage {
public:
    /// 0x08701b3a
    NetMPDropVote(PlayerTypes a1, int a2);
    /// 0x0871a7e8
    virtual ~NetMPDropVote();
    /// 0x08701b84
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f65aa
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08701bd4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08701c20
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
