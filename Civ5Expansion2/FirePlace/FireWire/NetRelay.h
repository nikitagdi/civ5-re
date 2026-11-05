/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304290
class NetRelay : public FForgettableNetMessage {
public:
    /// 0x08d55cc0
    virtual ~NetRelay();
    /// 0x08d51f80
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d51fbe
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d51ee8
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d51f34
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
