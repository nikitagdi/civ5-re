/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa900
class NetMPDropInit : public FNetMessage {
public:
    /// 0x08701a44
    NetMPDropInit(PlayerTypes a1);
    /// 0x0871a7d4
    virtual ~NetMPDropInit();
    /// 0x08701a84
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f65a8
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08701ac2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08701afe
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
