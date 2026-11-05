/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa600
class NetAuthResponse : public FNetMessage {
public:
    /// 0x086fdf5a
    NetAuthResponse(char const* a1, int a2, char const* a3);
    /// 0x0871a5aa
    virtual ~NetAuthResponse();
    /// 0x086fe098
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fe1d8
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fe0e8
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fe160
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
