/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb000
class NetDealAccepted : public FNetMessage {
public:
    /// 0x0870e074
    NetDealAccepted();
    /// 0x0870e174
    NetDealAccepted(PlayerTypes a1, PlayerTypes a2, ICvDeal1* a3, int a4, int a5, int a6);
    /// 0x0870e2ec
    /// deleting 0x0870e2c4
    virtual ~NetDealAccepted();
    /// 0x0870e3b6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870e6b6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870e3fe
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870e552
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x0870e6a6
    UNK_RET getFromPlayer() const;
    /// 0x0870e6ae
    UNK_RET getToPlayer() const;
};
