/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb100
class NetSetPlayerDesiredSlot : public FImmediateNetMessage {
public:
    /// 0x087115ee
    NetSetPlayerDesiredSlot();
    /// 0x08711636
    NetSetPlayerDesiredSlot(PlayerTypes a1, PlayerTypes a2);
    /// 0x08711698
    /// deleting 0x08711684
    virtual ~NetSetPlayerDesiredSlot();
    /// 0x0871169a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0871178a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087116f2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0871173e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x087116e2
    UNK_RET curID() const;
    /// 0x087116ea
    UNK_RET desiredID() const;
};
