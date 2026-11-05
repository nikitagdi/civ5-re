/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa6e0
class NetTurnUnready : public FImmediateNetMessage {
public:
    /// 0x086fec16
    NetTurnUnready();
    /// 0x086fec58
    NetTurnUnready(PlayerTypes a1);
    /// 0x0871a636
    virtual ~NetTurnUnready();
    /// 0x086fec9c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fed6c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fecf4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fed30
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
