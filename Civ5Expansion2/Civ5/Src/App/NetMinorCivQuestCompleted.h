/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faba0
class NetMinorCivQuestCompleted : public FNetMessage {
public:
    /// 0x087051c4
    NetMinorCivQuestCompleted(PlayerTypes a1, PlayerTypes a2, bool a3);
    /// 0x08705228
    /// deleting 0x08705214
    virtual ~NetMinorCivQuestCompleted();
    /// 0x0870522a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08705312
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870525e
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087052b8
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
