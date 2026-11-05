/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa2c0
class NetIDAssignment : public FNetMessage {
public:
    /// 0x086f7636
    NetIDAssignment();
    /// 0x086f7674
    NetIDAssignment(PlayerTypes a1);
    /// 0x0871a120
    virtual ~NetIDAssignment();
    /// 0x086f76b4
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f776a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f76f2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f772e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
