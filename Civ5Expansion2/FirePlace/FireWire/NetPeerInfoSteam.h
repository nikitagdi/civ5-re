/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"
#include <steam.h>

class FNetPlatformDetailsSteam;
class FNetSessionIFace;

/// size ?
/// vtable 0x0a304000
class NetPeerInfoSteam : public FForgettableNetMessage {
public:
    /// 0x08d51912
    NetPeerInfoSteam(int a1, CSteamID a2, FNetPlatformDetailsSteam const* a3);
    /// 0x08d55c84
    virtual ~NetPeerInfoSteam();
    /// 0x08d51a8c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d51aca
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d519b0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d51a16
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
