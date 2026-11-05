/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa400
class NetConfirmCivClaim : public FNetMessage {
public:
    /// 0x086f91bc
    NetConfirmCivClaim(PlayerTypes a1, bool a2);
    /// 0x0871a170
    virtual ~NetConfirmCivClaim();
    /// 0x086f9202
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f92f4
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f925c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f92a8
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
