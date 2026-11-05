/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb260
class NetMoveSpy : public FNetMessage {
public:
    /// 0x08713f1e
    NetMoveSpy();
    /// 0x08713f74
    NetMoveSpy(PlayerTypes a1, int a2, int a3, int a4, bool a5);
    /// 0x0871aeec
    virtual ~NetMoveSpy();
    /// 0x08713fd2
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0871411c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0871402c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087140a4
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
