/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa380
class NetVerifyVersion : public FNetMessage {
public:
    /// 0x086f84c2
    NetVerifyVersion(char const* a1, char const* a2, char const* a3, char const* a4, char const* a5, bool a6);
    /// 0x0871a388
    /// deleting 0x0871a402
    virtual ~NetVerifyVersion();
    /// 0x086f8660
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f880c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f86fc
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f8784
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
