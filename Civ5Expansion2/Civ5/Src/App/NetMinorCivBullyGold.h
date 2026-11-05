/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faca0
class NetMinorCivBullyGold : public FNetMessage {
public:
    /// 0x08706324
    NetMinorCivBullyGold();
    /// 0x08706370
    NetMinorCivBullyGold(PlayerTypes a1, PlayerTypes a2, int a3);
    /// 0x0871aa12
    virtual ~NetMinorCivBullyGold();
    /// 0x087063c0
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08706410
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08706532
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870658c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
