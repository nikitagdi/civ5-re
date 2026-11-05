/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304130
class NetLaunchValidationResponse : public FForgettableNetMessage {
public:
    /// 0x08d53fe8
    NetLaunchValidationResponse(unsigned char const* a1, unsigned int a2);
    /// 0x08d540a0
    /// deleting 0x08d54066
    virtual ~NetLaunchValidationResponse();
    /// 0x08d5419a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d541d0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d540da
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d54132
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08d55df6
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x08d55dfe
    virtual UNK_RET vf_07(UNK_PARAMS);
};
