/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa800
class NetResearch : public FNetMessage {
public:
    /// 0x08700e28
    NetResearch();
    /// 0x08700e78
    NetResearch(PlayerTypes a1, TechTypes a2, int a3, PlayerTypes a4, bool a5);
    /// 0x0871a6ea
    virtual ~NetResearch();
    /// 0x08700ed6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f6312
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08700fb6
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870102e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
