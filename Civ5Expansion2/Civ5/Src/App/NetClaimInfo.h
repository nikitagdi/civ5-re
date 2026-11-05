/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa420
class NetClaimInfo : public FNetMessage {
public:
    /// 0x086f92f6
    NetClaimInfo(PlayerTypes a1, char const* a2);
    /// 0x0871a184
    /// deleting 0x0871a1ba
    virtual ~NetClaimInfo();
    /// 0x086f9362
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f944e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f93b6
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f9402
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
