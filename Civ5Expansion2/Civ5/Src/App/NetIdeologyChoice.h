/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fada0
class NetIdeologyChoice : public FNetMessage {
public:
    /// 0x08707854
    NetIdeologyChoice();
    /// 0x08707898
    NetIdeologyChoice(PlayerTypes a1, PolicyBranchTypes a2);
    /// 0x0871aab2
    virtual ~NetIdeologyChoice();
    /// 0x087078e2
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08707920
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08707a3a
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08707a8e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
