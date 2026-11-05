/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2face0
class NetMinorCivBuyout : public FNetMessage {
public:
    /// 0x087068a8
    NetMinorCivBuyout();
    /// 0x087068ec
    NetMinorCivBuyout(PlayerTypes a1, PlayerTypes a2);
    /// 0x0871aa3a
    virtual ~NetMinorCivBuyout();
    /// 0x08706936
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870697e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08706a98
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08706ae4
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
