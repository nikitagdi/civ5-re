/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fab20
class NetHotDropNotice : public FImmediateNetMessage {
public:
    /// 0x0870399a
    NetHotDropNotice(PlayerTypes a1);
    /// 0x087039de
    NetHotDropNotice();
    /// 0x0871a99a
    virtual ~NetHotDropNotice();
    /// 0x08703a18
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08703b0a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08704b56
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08704b92
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08704bce
    UNK_RET getDroppedPlayer() const;
};
