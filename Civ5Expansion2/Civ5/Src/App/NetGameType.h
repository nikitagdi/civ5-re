/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa280
class NetGameType : public FNetMessage {
public:
    /// 0x086f6ac0
    NetGameType();
    /// 0x086f6af6
    NetGameType(GameTypes a1, bool a2, bool a3);
    /// 0x0871a0a6
    virtual ~NetGameType();
    /// 0x086f6b44
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f6c4a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f6b96
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f6bf0
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
