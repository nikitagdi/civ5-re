/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a3040a0
class NetMigrationInfo : public FImmediateNetMessage {
public:
    /// 0x08d533fc
    NetMigrationInfo(unsigned char* a1, unsigned short a2);
    /// 0x08d534b6
    /// deleting 0x08d5347c
    virtual ~NetMigrationInfo();
    /// 0x08d535b0
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d535e4
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d534f0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d5354a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
