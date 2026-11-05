/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa680
class NetAutoMoves : public FNetMessage {
public:
    /// 0x086fe55c
    NetAutoMoves(PlayerTypes a1);
    /// 0x0871a5fa
    virtual ~NetAutoMoves();
    /// 0x086fe59c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f541e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fe5da
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fe616
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
