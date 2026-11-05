/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faf20
class NetReturnCivilian : public FNetMessage {
public:
    /// 0x0870c5ba
    NetReturnCivilian();
    /// 0x0870c60a
    NetReturnCivilian(bool a1, PlayerTypes a2, PlayerTypes a3, int a4);
    /// 0x0871ad06
    virtual ~NetReturnCivilian();
    /// 0x0870c660
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870c784
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870c6b0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870c71a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
