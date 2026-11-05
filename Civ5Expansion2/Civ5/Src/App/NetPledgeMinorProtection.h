/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa9e0
class NetPledgeMinorProtection : public FNetMessage {
public:
    /// 0x08702650
    NetPledgeMinorProtection();
    /// 0x0870269a
    NetPledgeMinorProtection(PlayerTypes a1, PlayerTypes a2, bool a3, bool a4);
    /// 0x0871a860
    virtual ~NetPledgeMinorProtection();
    /// 0x087026f2
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08702804
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08702730
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870279a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
