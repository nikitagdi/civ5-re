/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb0e0
class NetChangePlayerID : public FImmediateNetMessage {
public:
    /// 0x087110e4
    NetChangePlayerID();
    /// 0x08711130
    NetChangePlayerID(PlayerTypes a1, PlayerTypes a2, bool a3);
    /// 0x08711198
    /// deleting 0x08711184
    virtual ~NetChangePlayerID();
    /// 0x0871119a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087112b0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087111fc
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08711256
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x087111ec
    UNK_RET oldID() const;
    /// 0x087111f4
    UNK_RET newID() const;
    /// 0x087115c6
    UNK_RET SetWaitingForProcessing();
    /// 0x087115da
    UNK_RET WaitingForProcessing();
};
