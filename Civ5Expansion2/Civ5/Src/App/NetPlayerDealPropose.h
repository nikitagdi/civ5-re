/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb040
class NetPlayerDealPropose : public FNetMessage {
public:
    /// 0x0870f19c
    NetPlayerDealPropose();
    /// 0x0870f288
    NetPlayerDealPropose(ICvDeal1* a1, PlayerTypes a2, PlayerTypes a3);
    /// 0x0870f3f0
    /// deleting 0x0870f3c8
    virtual ~NetPlayerDealPropose();
    /// 0x0870f4ba
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870f752
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870f502
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870f62a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
