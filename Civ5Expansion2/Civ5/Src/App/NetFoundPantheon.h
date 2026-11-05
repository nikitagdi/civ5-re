/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb200
class NetFoundPantheon : public FNetMessage {
public:
    /// 0x08713320
    NetFoundPantheon(PlayerTypes a1, BeliefTypes a2);
    /// 0x0871aeb0
    virtual ~NetFoundPantheon();
    /// 0x0871336a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087133b2
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087134cc
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08713518
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
