/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb220
class NetFoundReligion : public FNetMessage {
public:
    /// 0x08713564
    NetFoundReligion();
    /// 0x087135f6
    NetFoundReligion(PlayerTypes a1, ReligionTypes a2, char const* a3, BeliefTypes a4, BeliefTypes a5, BeliefTypes a6, BeliefTypes a7, int a8, int a9);
    /// 0x0871aec4
    virtual ~NetFoundReligion();
    /// 0x087136aa
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08713742
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087138de
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087139bc
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
