/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb160
class NetAICivsProcessed : public FImmediateNetMessage {
public:
    /// 0x08711fb6
    NetAICivsProcessed();
    /// 0x08711ff0
    NetAICivsProcessed(PlayerTypes a1, int a2);
    /// 0x08712052
    /// deleting 0x0871203e
    virtual ~NetAICivsProcessed();
    /// 0x08712054
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870433e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087120b4
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08712100
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x0871214c
    UNK_RET CheckNetAICivsProcessedCompleted();
    /// 0x08712314
    UNK_RET processedThisTurn();
};
