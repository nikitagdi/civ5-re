/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fad60
class NetGoodyChoice : public FNetMessage {
public:
    /// 0x08707252
    NetGoodyChoice();
    /// 0x087072ac
    NetGoodyChoice(PlayerTypes a1, int a2, int a3, GoodyTypes a4, int a5);
    /// 0x0871aa8a
    virtual ~NetGoodyChoice();
    /// 0x0870730a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08707364
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087074a4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08707524
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
