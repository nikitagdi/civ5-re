/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa880
class NetDiploChat : public FNetMessage {
public:
    /// 0x0870150c
    NetDiploChat();
    /// 0x0870156a
    NetDiploChat(char* a1, PlayerTypes a2, PlayerTypes a3);
    /// 0x0871a8d8
    /// deleting 0x0871a90e
    virtual ~NetDiploChat();
    /// 0x08701652
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f65a0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087016ae
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08701708
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
