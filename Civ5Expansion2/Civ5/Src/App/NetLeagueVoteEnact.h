/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb2c0
class NetLeagueVoteEnact : public FNetMessage {
public:
    /// 0x087147b8
    NetLeagueVoteEnact();
    /// 0x08714812
    NetLeagueVoteEnact(LeagueTypes a1, int a2, PlayerTypes a3, int a4, int a5);
    /// 0x0871af28
    virtual ~NetLeagueVoteEnact();
    /// 0x08714870
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087149c0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087148d0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08714948
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
