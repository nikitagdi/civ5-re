/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa460
class NetCivChoiceNack : public FNetMessage {
public:
    /// 0x086f9540
    NetCivChoiceNack(PlayerTypes a1, CivLoginStates a2);
    /// 0x0871a20a
    virtual ~NetCivChoiceNack();
    /// 0x086f958a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f9660
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f95c8
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f9614
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
