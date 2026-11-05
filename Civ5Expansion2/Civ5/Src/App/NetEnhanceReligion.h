/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb240
class NetEnhanceReligion : public FNetMessage {
public:
    /// 0x08713a9a
    NetEnhanceReligion();
    /// 0x08713b22
    NetEnhanceReligion(PlayerTypes a1, ReligionTypes a2, char const* a3, BeliefTypes a4, BeliefTypes a5, int a6, int a7);
    /// 0x0871aed8
    virtual ~NetEnhanceReligion();
    /// 0x08713bc2
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08713c36
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08713daa
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08713e64
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
