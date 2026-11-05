/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa500
class NetPlayerInfo : public FImmediateNetMessage {
public:
    /// 0x086f9ba4
    NetPlayerInfo();
    /// 0x086f9d60
    NetPlayerInfo(PlayerTypes a1);
    /// 0x086b4562
    /// deleting 0x0871a530
    virtual ~NetPlayerInfo();
    /// 0x086fad88
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fb322
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086faf52
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fb13a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
