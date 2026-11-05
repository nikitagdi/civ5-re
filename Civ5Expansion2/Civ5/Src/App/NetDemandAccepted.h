/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb020
class NetDemandAccepted : public FNetMessage {
public:
    /// 0x0870e960
    NetDemandAccepted();
    /// 0x0870ea4c
    NetDemandAccepted(PlayerTypes a1, PlayerTypes a2, ICvDeal1* a3);
    /// 0x0870ebb4
    /// deleting 0x0870eb8c
    virtual ~NetDemandAccepted();
    /// 0x0870ec7e
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870ef26
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870ecc6
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870edee
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x0870ef16
    UNK_RET getFromPlayer() const;
    /// 0x0870ef1e
    UNK_RET getToPlayer() const;
};
