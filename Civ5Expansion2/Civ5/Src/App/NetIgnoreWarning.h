/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa9c0
class NetIgnoreWarning : public FNetMessage {
public:
    /// 0x087023d2
    NetIgnoreWarning();
    /// 0x08702416
    NetIgnoreWarning(PlayerTypes a1, TeamTypes a2);
    /// 0x0871a84c
    virtual ~NetIgnoreWarning();
    /// 0x08702460
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08702536
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870249e
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087024ea
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
