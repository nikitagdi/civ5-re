/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa720
class NetAutoMission : public FNetMessage {
public:
    /// 0x086ff2c4
    NetAutoMission();
    /// 0x086ff308
    NetAutoMission(PlayerTypes a1, int a2);
    /// 0x0871a65e
    virtual ~NetAutoMission();
    /// 0x086ff352
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f5bfc
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086ff390
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086ff3dc
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
