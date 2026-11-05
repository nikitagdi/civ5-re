/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa6a0
class NetTurnComplete : public FImmediateNetMessage {
public:
    /// 0x086fe652
    NetTurnComplete();
    /// 0x086fe694
    NetTurnComplete(PlayerTypes a1);
    /// 0x0871a60e
    virtual ~NetTurnComplete();
    /// 0x086fe85a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fe92a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fe8b2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fe8ee
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
