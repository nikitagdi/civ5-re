/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa5a0
class NetInitPlayers : public FNetMessage {
public:
    /// 0x086fd0f0
    NetInitPlayers();
    /// 0x086fd29e
    /// deleting 0x086fd276
    virtual ~NetInitPlayers();
    /// 0x086fd31a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fd34e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fd452
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fd530
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
