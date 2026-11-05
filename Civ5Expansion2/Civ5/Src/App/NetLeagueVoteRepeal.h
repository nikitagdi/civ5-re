/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb2e0
class NetLeagueVoteRepeal : public FNetMessage {
public:
    /// 0x08714b00
    NetLeagueVoteRepeal();
    /// 0x08714b5a
    NetLeagueVoteRepeal(LeagueTypes a1, int a2, PlayerTypes a3, int a4, int a5);
    /// 0x0871af3c
    virtual ~NetLeagueVoteRepeal();
    /// 0x08714bb8
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08714d08
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08714c18
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08714c90
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
