/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"
#include <memory>
#include <vector>

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb1c0
class NetHotJoinStart : public FImmediateNetMessage {
public:
    /// 0x08712b9a
    NetHotJoinStart();
    /// 0x08712bea
    NetHotJoinStart(std::vector<int, std::allocator<int> > const& a1);
    /// 0x08712ca6
    /// deleting 0x08712c70
    virtual ~NetHotJoinStart();
    /// 0x08712cd6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08712e1c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08712da4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08712de0
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x0871322c
    UNK_RET hotJoinSyncStarted();
};
