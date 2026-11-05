/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa820
class NetChat : public FImmediateNetMessage {
public:
    /// 0x087010a6
    NetChat();
    /// 0x08701112
    NetChat(PlayerTypes a1, CvString a2, ChatTargetTypes a3, PlayerTypes a4);
    /// 0x0871a6fe
    /// deleting 0x0871a734
    virtual ~NetChat();
    /// 0x087011b0
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f6480
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870120c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08701276
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x086f6588
    UNK_RET getChatString() const;
    /// 0x08701204
    UNK_RET getWhoFrom() const;
    /// 0x087012e0
    UNK_RET getTargetType() const;
};
