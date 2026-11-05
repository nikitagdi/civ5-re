/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb340
class NetLeagueProposeRepeal : public FNetMessage {
public:
    /// 0x0871540e
    NetLeagueProposeRepeal();
    /// 0x0871545a
    NetLeagueProposeRepeal(LeagueTypes a1, int a2, PlayerTypes a3);
    /// 0x0871af78
    virtual ~NetLeagueProposeRepeal();
    /// 0x087154aa
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087155ae
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087154fa
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08715554
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
