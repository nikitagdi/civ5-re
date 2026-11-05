/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa940
class NetMPDropUpdate : public FNetMessage {
public:
    /// 0x08701c6c
    NetMPDropUpdate(int a1, int a2);
    /// 0x0871a7fc
    virtual ~NetMPDropUpdate();
    /// 0x08701cb6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f65ac
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08701cfe
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08701d4a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
