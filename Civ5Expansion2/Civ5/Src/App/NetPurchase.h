/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa7c0
class NetPurchase : public FNetMessage {
public:
    /// 0x087003d2
    NetPurchase();
    /// 0x08700432
    NetPurchase(PlayerTypes a1, int a2, UnitTypes a3, BuildingTypes a4, ProjectTypes a5, int a6);
    /// 0x0871a6c2
    virtual ~NetPurchase();
    /// 0x08700496
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f604e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087005ce
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08700676
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
