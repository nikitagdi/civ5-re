/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa8e0
class NetNeverJoined : public FNetMessage {
public:
    /// 0x0870194e
    NetNeverJoined(PlayerTypes a1);
    /// 0x0871a7c0
    virtual ~NetNeverJoined();
    /// 0x0870198e
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f65a6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087019cc
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08701a08
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
