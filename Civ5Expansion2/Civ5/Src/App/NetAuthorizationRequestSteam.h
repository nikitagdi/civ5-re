/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"
#include <steam.h>

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304830
class NetAuthorizationRequestSteam : public FImmediateNetMessage {
public:
    /// 0x08d5e390
    NetAuthorizationRequestSteam(CSteamID a1, void const* a2, unsigned int a3);
    /// 0x08d6033a
    virtual ~NetAuthorizationRequestSteam();
    /// 0x08d5e546
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d5e57c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d5e42c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d5e4a4
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08d5e3d6
    UNK_RET SetAuthData(void const* a1, unsigned int a2);
};
