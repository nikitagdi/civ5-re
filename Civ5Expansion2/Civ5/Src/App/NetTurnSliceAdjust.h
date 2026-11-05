/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fafe0
class NetTurnSliceAdjust : public FImmediateNetMessage {
public:
    /// 0x0870def0
    NetTurnSliceAdjust();
    /// 0x0870df32
    NetTurnSliceAdjust(int a1);
    /// 0x0870df8a
    /// deleting 0x0870df76
    virtual ~NetTurnSliceAdjust();
    /// 0x0870e036
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870e01a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870dfa2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870dfde
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x0870df8c
    UNK_RET GetTurnSliceMessageLimit();
};
