/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa560
class NetLaunchingInfo : public FNetMessage {
public:
    /// 0x086fc880
    NetLaunchingInfo(int a1, int a2);
    /// 0x0871a580
    virtual ~NetLaunchingInfo();
    /// 0x086fc8ca
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086fc9aa
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086fc912
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086fc95e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
