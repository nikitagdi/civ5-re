/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"
#include <steam.h>

class FNetPlatformDetailsSteam;
class FNetSessionIFace;

/// size ?
/// vtable 0x0a304040
class NetIntroductionSteam : public FImmediateNetMessage {
public:
    /// 0x08d523a2
    NetIntroductionSteam(int a1, CSteamID a2, FNetPlatformDetailsSteam const* a3);
    /// 0x08d55d96
    virtual ~NetIntroductionSteam();
    /// 0x08d52518
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d52556
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d5243c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d524a2
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
