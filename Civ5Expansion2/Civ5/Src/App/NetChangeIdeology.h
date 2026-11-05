/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb3e0
class NetChangeIdeology : public FNetMessage {
public:
    /// 0x0871638a
    NetChangeIdeology();
    /// 0x087163c8
    NetChangeIdeology(PlayerTypes a1);
    /// 0x0871641c
    /// deleting 0x08716408
    virtual ~NetChangeIdeology();
    /// 0x0871641e
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087164d4
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0871645c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08716498
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
