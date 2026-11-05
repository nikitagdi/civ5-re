/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb300
class NetLeagueVoteAbstain : public FNetMessage {
public:
    /// 0x08714e48
    NetLeagueVoteAbstain();
    /// 0x08714e94
    NetLeagueVoteAbstain(LeagueTypes a1, PlayerTypes a2, int a3);
    /// 0x0871af50
    virtual ~NetLeagueVoteAbstain();
    /// 0x08714ee4
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08714fe8
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08714f34
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08714f8e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
