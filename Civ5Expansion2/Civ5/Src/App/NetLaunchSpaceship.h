/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fab40
class NetLaunchSpaceship : public FNetMessage {
public:
    /// 0x08704bd6
    NetLaunchSpaceship(PlayerTypes a1, VictoryTypes a2);
    /// 0x08704c34
    /// deleting 0x08704c20
    virtual ~NetLaunchSpaceship();
    /// 0x08704c36
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08704d02
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08704c6a
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08704cb6
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
