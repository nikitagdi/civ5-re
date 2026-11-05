/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa660
class NetExtendedGame : public FNetMessage {
public:
    /// 0x086fe466
    NetExtendedGame(PlayerTypes a1);
    /// 0x0871a5e6
    virtual ~NetExtendedGame();
    /// 0x086fe4a6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f5362
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fe4e4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fe520
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
