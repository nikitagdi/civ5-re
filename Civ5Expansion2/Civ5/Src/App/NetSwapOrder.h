/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa7a0
class NetSwapOrder : public FNetMessage {
public:
    /// 0x08700244
    NetSwapOrder();
    /// 0x08700290
    NetSwapOrder(PlayerTypes a1, int a2, int a3);
    /// 0x0871a6ae
    virtual ~NetSwapOrder();
    /// 0x087002e0
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f5f84
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870031e
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08700378
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
