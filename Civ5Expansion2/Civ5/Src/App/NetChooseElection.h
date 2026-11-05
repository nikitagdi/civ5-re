/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faa40
class NetChooseElection : public FNetMessage {
public:
    /// 0x08702df2
    NetChooseElection();
    /// 0x08702e28
    NetChooseElection(PlayerTypes a1, int a2, int a3);
    /// 0x0871a89c
    virtual ~NetChooseElection();
    /// 0x08702e78
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f65b2
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08702eb6
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08702f10
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
