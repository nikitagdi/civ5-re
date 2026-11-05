/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fac80
class NetMinorCivGiftTileImprovement : public FNetMessage {
public:
    /// 0x08706020
    NetMinorCivGiftTileImprovement();
    /// 0x08706072
    NetMinorCivGiftTileImprovement(PlayerTypes a1, PlayerTypes a2, int a3, int a4);
    /// 0x0871a9fe
    virtual ~NetMinorCivGiftTileImprovement();
    /// 0x087060c8
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08706122
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08706250
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087062ba
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
