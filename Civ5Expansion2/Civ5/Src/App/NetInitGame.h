/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa580
class NetInitGame : public FNetMessage {
public:
    /// 0x086fc9ac
    NetInitGame();
    /// 0x086fcb74
    /// deleting 0x086fcb60
    virtual ~NetInitGame();
    /// 0x086fcb76
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fcd26
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fd01c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fd086
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
