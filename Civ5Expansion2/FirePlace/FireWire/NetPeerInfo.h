/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"
#include "FirePlace/FireWorks/FString.h"

class FNetPlatformDetailsSteam;
class FNetSessionIFace;

/// size ?
/// vtable 0x0a303fe0
class NetPeerInfo : public FForgettableNetMessage {
public:
    /// 0x08d50ec4
    NetPeerInfo(int a1, FStringA a2, FStringA a3, FNetPlatformDetailsSteam const* a4);
    /// 0x08d55bc4
    /// deleting 0x08d55c5c
    virtual ~NetPeerInfo();
    /// 0x08d5111a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d51160
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d51046
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d510b0
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
