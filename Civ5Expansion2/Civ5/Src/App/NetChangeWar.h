/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa9a0
class NetChangeWar : public FNetMessage {
public:
    /// 0x0870217e
    NetChangeWar();
    /// 0x087021c6
    NetChangeWar(PlayerTypes a1, TeamTypes a2, bool a3);
    /// 0x0871a838
    virtual ~NetChangeWar();
    /// 0x08702216
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08702308
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08702254
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087022ae
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
