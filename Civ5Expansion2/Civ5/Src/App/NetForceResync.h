/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"
#include <memory>
#include <vector>

class FNetSessionIFace;
class cvPath;

/// size ?
/// vtable 0x0a2fb080
class NetForceResync : public FImmediateNetMessage {
public:
    /// 0x08710182
    NetForceResync();
    /// 0x087101e6
    NetForceResync(cvPath const& a1, std::vector<int, std::allocator<int> >* a2);
    /// 0x08719dac
    /// deleting 0x0871ae3a
    virtual ~NetForceResync();
    /// 0x087103b4
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0871051a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08710482
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087104ce
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08710388
    UNK_RET queue();
    /// 0x0871039e
    UNK_RET IsResyncing();
};
