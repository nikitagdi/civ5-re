/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faa20
class NetLiberatePlayer : public FNetMessage {
public:
    /// 0x08702b88
    NetLiberatePlayer();
    /// 0x08702bd4
    NetLiberatePlayer(PlayerTypes a1, PlayerTypes a2, int a3);
    /// 0x0871a888
    virtual ~NetLiberatePlayer();
    /// 0x08702c24
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08702d28
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08702c74
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08702cce
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
