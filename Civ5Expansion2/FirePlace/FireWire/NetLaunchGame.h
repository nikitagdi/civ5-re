/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304340
class NetLaunchGame : public FImmediateNetMessage {
public:
    /// 0x08d55e1a
    virtual ~NetLaunchGame();
    /// 0x08d5432c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08d54360
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08d54318
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08d5434c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
