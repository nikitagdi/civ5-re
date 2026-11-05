/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fad20
class NetMayaBonusChoice : public FNetMessage {
public:
    /// 0x08706d56
    NetMayaBonusChoice();
    /// 0x08706d9a
    NetMayaBonusChoice(PlayerTypes a1, UnitTypes a2);
    /// 0x0871aa62
    virtual ~NetMayaBonusChoice();
    /// 0x08706de4
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08706e22
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08706f3c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08706f88
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
