/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa6c0
class NetTurnAllComplete : public FImmediateNetMessage {
public:
    /// 0x086fe98a
    NetTurnAllComplete();
    /// 0x086fe9cc
    NetTurnAllComplete(PlayerTypes a1, bool a2);
    /// 0x0871a622
    virtual ~NetTurnAllComplete();
    /// 0x086feae6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086febb6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086feb3e
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086feb7a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
