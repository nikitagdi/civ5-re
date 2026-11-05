/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2facc0
class NetMinorCivBullyUnit : public FNetMessage {
public:
    /// 0x087065e6
    NetMinorCivBullyUnit();
    /// 0x08706632
    NetMinorCivBullyUnit(PlayerTypes a1, PlayerTypes a2, UnitTypes a3);
    /// 0x0871aa26
    virtual ~NetMinorCivBullyUnit();
    /// 0x08706682
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087066d2
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087067f4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870684e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
