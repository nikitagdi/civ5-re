/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb280
class NetStageCoup : public FNetMessage {
public:
    /// 0x0871425e
    NetStageCoup();
    /// 0x087142a2
    NetStageCoup(PlayerTypes a1, int a2);
    /// 0x0871af00
    virtual ~NetStageCoup();
    /// 0x087142ec
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087143cc
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08714334
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08714380
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
