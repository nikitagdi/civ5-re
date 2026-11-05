/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304310
class NetInvalidLaunch : public FForgettableNetMessage {
public:
    /// 0x08d55e06
    virtual ~NetInvalidLaunch();
    /// 0x08d542ae
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d542e4
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d54236
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d54272
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
