/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faf40
class NetUpdateCityCitizens : public FNetMessage {
public:
    /// 0x0870c858
    NetUpdateCityCitizens();
    /// 0x0870c89c
    NetUpdateCityCitizens(PlayerTypes a1, int a2);
    /// 0x0871ad1a
    virtual ~NetUpdateCityCitizens();
    /// 0x0870c8e6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870c9c6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870c92e
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870c97a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
