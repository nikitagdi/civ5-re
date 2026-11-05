/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fad40
class NetFaithGreatPersonChoice : public FNetMessage {
public:
    /// 0x08706fd4
    NetFaithGreatPersonChoice();
    /// 0x08707018
    NetFaithGreatPersonChoice(PlayerTypes a1, UnitTypes a2);
    /// 0x0871aa76
    virtual ~NetFaithGreatPersonChoice();
    /// 0x08707062
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087070a0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087071ba
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08707206
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
