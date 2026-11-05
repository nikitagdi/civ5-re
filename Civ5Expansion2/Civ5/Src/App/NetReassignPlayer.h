/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa520
class NetReassignPlayer : public FNetMessage {
public:
    /// 0x086fc6a4
    NetReassignPlayer();
    /// 0x086fc6e8
    NetReassignPlayer(PlayerTypes a1, PlayerTypes a2);
    /// 0x0871a558
    virtual ~NetReassignPlayer();
    /// 0x086fc732
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fc812
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fc77a
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fc7c6
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
