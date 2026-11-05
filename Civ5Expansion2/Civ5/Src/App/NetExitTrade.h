/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faac0
class NetExitTrade : public FNetMessage {
public:
    /// 0x08703398
    NetExitTrade();
    /// 0x087033d6
    NetExitTrade(PlayerTypes a1);
    /// 0x0871a95e
    virtual ~NetExitTrade();
    /// 0x08703416
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f67b4
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08703454
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08703490
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
