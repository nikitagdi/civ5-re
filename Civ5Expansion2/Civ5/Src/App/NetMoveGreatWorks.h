/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb3c0
class NetMoveGreatWorks : public FNetMessage {
public:
    /// 0x08715fac
    NetMoveGreatWorks();
    /// 0x08716014
    NetMoveGreatWorks(PlayerTypes a1, int a2, int a3, int a4, int a5, int a6, int a7);
    /// 0x0871afc8
    virtual ~NetMoveGreatWorks();
    /// 0x08716080
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08716222
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087160f6
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0871618c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
