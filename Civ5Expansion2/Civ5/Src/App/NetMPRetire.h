/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa8c0
class NetMPRetire : public FNetMessage {
public:
    /// 0x08701858
    NetMPRetire(PlayerTypes a1);
    /// 0x0871a7ac
    virtual ~NetMPRetire();
    /// 0x08701898
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f65a4
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087018d6
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08701912
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
