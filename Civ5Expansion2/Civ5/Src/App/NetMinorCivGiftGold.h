/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fac60
class NetMinorCivGiftGold : public FNetMessage {
public:
    /// 0x08705db6
    NetMinorCivGiftGold();
    /// 0x08705e02
    NetMinorCivGiftGold(PlayerTypes a1, PlayerTypes a2, int a3);
    /// 0x0871a9ea
    virtual ~NetMinorCivGiftGold();
    /// 0x08705e52
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08705ea2
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08705f6c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08705fc6
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
