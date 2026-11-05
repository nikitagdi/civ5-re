/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa5e0
class NetAuthRequest : public FNetMessage {
public:
    /// 0x086fde20
    NetAuthRequest(char const* a1);
    /// 0x0871a596
    virtual ~NetAuthRequest();
    /// 0x086fde86
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fdf58
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fdec4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fdf0e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
