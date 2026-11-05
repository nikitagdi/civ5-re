/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"
#include "FirePlace/FireWorks/FString.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304160
class NetTransferNotify : public FForgettableNetMessage {
public:
    /// 0x08d543f8
    NetTransferNotify(unsigned int a1, unsigned int a2, FStringA a3);
    /// 0x08d55ee2
    /// deleting 0x08d55f42
    virtual ~NetTransferNotify();
    /// 0x08d54570
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d545b6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d544bc
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d54516
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
