/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faee0
class NetBarbarianRansom : public FNetMessage {
public:
    /// 0x0870c0f8
    NetBarbarianRansom();
    /// 0x0870c144
    NetBarbarianRansom(PlayerTypes a1, int a2, int a3);
    /// 0x0871acde
    virtual ~NetBarbarianRansom();
    /// 0x0870c194
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870c286
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870c1d2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870c22c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
