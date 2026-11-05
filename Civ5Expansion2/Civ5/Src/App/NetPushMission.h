/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa700
class NetPushMission : public FNetMessage {
public:
    /// 0x086fede0
    NetPushMission();
    /// 0x086fee44
    NetPushMission(PlayerTypes a1, int a2, MissionTypes a3, int a4, int a5, int a6, bool a7);
    /// 0x0871a64a
    virtual ~NetPushMission();
    /// 0x086fefdc
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f5420
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086feeb0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fef46
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
