/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb1a0
class NetDestroyUnit : public FImmediateNetMessage {
public:
    /// 0x08712776
    NetDestroyUnit();
    /// 0x087127be
    NetDestroyUnit(PlayerTypes a1, int a2);
    /// 0x08712820
    /// deleting 0x0871280c
    virtual ~NetDestroyUnit();
    /// 0x08712822
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08712a40
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08712b02
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08712b4e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08712a30
    UNK_RET getPlayerID() const;
    /// 0x08712a38
    UNK_RET getUnitID() const;
};
