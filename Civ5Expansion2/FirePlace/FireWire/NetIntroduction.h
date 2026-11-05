/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"
#include "FirePlace/FireWorks/FString.h"

class FNetPlatformDetailsSteam;
class FNetSessionIFace;

/// size ?
/// vtable 0x0a304020
class NetIntroduction : public FImmediateNetMessage {
public:
    /// 0x08d52002
    NetIntroduction(int a1, FStringA a2, FNetPlatformDetailsSteam const* a3);
    /// 0x08d55cd4
    /// deleting 0x08d55d34
    virtual ~NetIntroduction();
    /// 0x08d521b2
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d521f0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d520fe
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d52158
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
