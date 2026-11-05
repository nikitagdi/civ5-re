/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fab80
class NetMinorCivQuestNoInterest : public FNetMessage {
public:
    /// 0x08705074
    NetMinorCivQuestNoInterest(PlayerTypes a1, PlayerTypes a2, bool a3);
    /// 0x087050d8
    /// deleting 0x087050c4
    virtual ~NetMinorCivQuestNoInterest();
    /// 0x087050da
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087051c2
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870510e
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08705168
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
