/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fb060
class NetPlayerDealFinalized : public FNetMessage {
public:
    /// 0x0870fee6
    NetPlayerDealFinalized();
    /// 0x0870ff32
    NetPlayerDealFinalized(PlayerTypes a1, PlayerTypes a2, PlayerTypes a3, bool a4);
    /// 0x0871ae26
    virtual ~NetPlayerDealFinalized();
    /// 0x0870ff88
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x087100ae
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870ffda
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08710044
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
