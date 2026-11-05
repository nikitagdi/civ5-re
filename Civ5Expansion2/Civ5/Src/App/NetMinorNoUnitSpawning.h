/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faa00
class NetMinorNoUnitSpawning : public FNetMessage {
public:
    /// 0x08702934
    NetMinorNoUnitSpawning();
    /// 0x0870297c
    NetMinorNoUnitSpawning(PlayerTypes a1, PlayerTypes a2, bool a3);
    /// 0x0871a874
    virtual ~NetMinorNoUnitSpawning();
    /// 0x087029cc
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08702abe
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08702a0a
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08702a64
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
