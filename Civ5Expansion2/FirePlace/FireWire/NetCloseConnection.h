/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304080
class NetCloseConnection : public FImmediateNetMessage {
public:
    /// 0x08d53150
    NetCloseConnection(int a1);
    /// 0x08d55ece
    virtual ~NetCloseConnection();
    /// 0x08d5320c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d53242
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d53194
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d531d0
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
