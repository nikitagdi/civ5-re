/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;
class cvPath;

/// size ?
/// vtable 0x0a2fa360
class NetLoadGame : public FNetMessage {
public:
    /// 0x086f7c52
    NetLoadGame(cvPath const& a1, bool a2);
    /// 0x086f7e96
    NetLoadGame();
    /// 0x086f7f8e
    /// deleting 0x086f7f66
    virtual ~NetLoadGame();
    /// 0x086f804e
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f8246
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f8082
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f8164
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
