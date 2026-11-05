/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa4e0
class NetSlotReassignment : public FNetMessage {
public:
    /// 0x086f9a78
    NetSlotReassignment(PlayerTypes a1, PlayerTypes a2);
    /// 0x0871a51c
    virtual ~NetSlotReassignment();
    /// 0x086f9ac2
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f9ba2
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f9b0a
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f9b56
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
