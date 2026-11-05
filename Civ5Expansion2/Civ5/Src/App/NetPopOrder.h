/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa780
class NetPopOrder : public FNetMessage {
public:
    /// 0x087000b6
    NetPopOrder();
    /// 0x08700102
    NetPopOrder(PlayerTypes a1, int a2, int a3);
    /// 0x0871a69a
    virtual ~NetPopOrder();
    /// 0x08700152
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f5eba
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08700190
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087001ea
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
