/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fac20
class NetSwapUnits : public FNetMessage {
public:
    /// 0x08705806
    NetSwapUnits();
    /// 0x0870586a
    NetSwapUnits(PlayerTypes a1, int a2, MissionTypes a3, int a4, int a5, int a6, bool a7);
    /// 0x0871a9c2
    virtual ~NetSwapUnits();
    /// 0x08705716
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087058d6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087059e2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08705a78
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
