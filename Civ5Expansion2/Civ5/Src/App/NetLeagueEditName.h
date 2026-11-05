/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb360
class NetLeagueEditName : public FNetMessage {
public:
    /// 0x087156d0
    NetLeagueEditName();
    /// 0x08715730
    NetLeagueEditName(LeagueTypes a1, PlayerTypes a2, char const* a3);
    /// 0x0871af8c
    virtual ~NetLeagueEditName();
    /// 0x087157a8
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087158c4
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087157f0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0871585a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
