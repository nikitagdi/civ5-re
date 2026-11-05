/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FString.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa320
class NetMapScriptCheck : public FNetMessage {
public:
    /// 0x086f78fa
    NetMapScriptCheck(FStringA const& a1);
    /// 0x0871a2b2
    /// deleting 0x0871a312
    virtual ~NetMapScriptCheck();
    /// 0x086f79c4
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f7a7a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f7a02
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f7a3e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
