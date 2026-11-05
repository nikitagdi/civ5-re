/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa7e0
class NetDoTask : public FNetMessage {
public:
    /// 0x08700714
    NetDoTask();
    /// 0x08700774
    NetDoTask(PlayerTypes a1, int a2, TaskTypes a3, int a4, int a5, bool a6, bool a7, bool a8, bool a9);
    /// 0x0871a6d6
    virtual ~NetDoTask();
    /// 0x087007ee
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f61e0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08700cc0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08700d74
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
