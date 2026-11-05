/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faa80
class NetContactCiv : public FNetMessage {
public:
    /// 0x087030c4
    NetContactCiv();
    /// 0x08703110
    NetContactCiv(NetContactTypes a1, PlayerTypes a2, PlayerTypes a3);
    /// 0x0871a8c4
    virtual ~NetContactCiv();
    /// 0x08703160
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f673e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087031b0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870320a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
