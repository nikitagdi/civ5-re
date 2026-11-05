/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa3e0
class NetCivChoice : public FNetMessage {
public:
    /// 0x086f90c4
    NetCivChoice(PlayerTypes a1);
    /// 0x0871a15c
    virtual ~NetCivChoice();
    /// 0x086f9104
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f91ba
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f9142
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f917e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
