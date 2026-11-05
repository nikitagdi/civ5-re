/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FImmediateNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb180
class NetInitialAIProcessed : public FImmediateNetMessage {
public:
    /// 0x0871239c
    NetInitialAIProcessed();
    /// 0x087123d6
    NetInitialAIProcessed(PlayerTypes a1, int a2);
    /// 0x08712438
    /// deleting 0x08712424
    virtual ~NetInitialAIProcessed();
    /// 0x0871243a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087043d0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0871249a
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087124e6
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x08712532
    UNK_RET CheckNetGamePlayerInitialAIProcessedCompleted();
    /// 0x087126ee
    UNK_RET processedThisTurn();
};
