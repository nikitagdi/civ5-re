/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304100
class NetLaunchValidate : public FForgettableNetMessage {
public:
    /// 0x08d53daa
    NetLaunchValidate(unsigned int a1, unsigned char const* a2);
    /// 0x08d53e62
    /// deleting 0x08d53e28
    virtual ~NetLaunchValidate();
    /// 0x08d53f5c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d53f92
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d53e9c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d53ef4
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08d55de6
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x08d55dee
    virtual UNK_RET vf_07(UNK_PARAMS);
};
