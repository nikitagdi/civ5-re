/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fade0
class NetSetCityAvoidGrowth : public FNetMessage {
public:
    /// 0x08707d46
    NetSetCityAvoidGrowth();
    /// 0x08707d8e
    NetSetCityAvoidGrowth(PlayerTypes a1, int a2, bool a3);
    /// 0x0871aada
    virtual ~NetSetCityAvoidGrowth();
    /// 0x08707dde
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08707e30
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08707efe
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08707f58
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
