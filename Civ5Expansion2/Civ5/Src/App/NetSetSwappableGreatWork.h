/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb380
class NetSetSwappableGreatWork : public FNetMessage {
public:
    /// 0x087159e6
    NetSetSwappableGreatWork();
    /// 0x08715a32
    NetSetSwappableGreatWork(PlayerTypes a1, int a2, int a3);
    /// 0x0871afa0
    virtual ~NetSetSwappableGreatWork();
    /// 0x08715a82
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08715b86
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08715ad2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08715b2c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
