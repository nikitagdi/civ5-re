/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa640
class NetPlayerOption : public FImmediateNetMessage {
public:
    /// 0x086fe2fe
    NetPlayerOption(PlayerTypes a1, PlayerOptionTypes a2, bool a3);
    /// 0x0871a5d2
    virtual ~NetPlayerOption();
    /// 0x086fe358
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f528e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fe3b2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fe40c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
