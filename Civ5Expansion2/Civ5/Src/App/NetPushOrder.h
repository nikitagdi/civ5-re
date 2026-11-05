/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa760
class NetPushOrder : public FNetMessage {
public:
    /// 0x086ffa1c
    NetPushOrder();
    /// 0x086ffa78
    NetPushOrder(PlayerTypes a1, int a2, OrderTypes a3, int a4, bool a5, bool a6, bool a7);
    /// 0x0871a686
    virtual ~NetPushOrder();
    /// 0x086ffae4
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f5dae
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fff8a
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08700020
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
