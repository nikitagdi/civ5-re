/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb2a0
class NetFaithPurchase : public FNetMessage {
public:
    /// 0x087144e6
    NetFaithPurchase();
    /// 0x08714532
    NetFaithPurchase(PlayerTypes a1, FaithPurchaseTypes a2, int a3);
    /// 0x0871af14
    virtual ~NetFaithPurchase();
    /// 0x08714582
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08714696
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087145d2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08714634
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
