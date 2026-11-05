/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FForgettableNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3041c0
class NetTransferAppend : public FForgettableNetMessage {
public:
    /// 0x08d54868
    NetTransferAppend(unsigned int a1, unsigned char* a2, unsigned int a3, unsigned int a4, unsigned int a5);
    /// 0x08d5493a
    /// deleting 0x08d54900
    virtual ~NetTransferAppend();
    /// 0x08d54a90
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d54ae0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d54974
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d549fa
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
