/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa340
class NetMapScriptAck : public FNetMessage {
public:
    /// 0x086f7a7c
    NetMapScriptAck(bool a1, char const* a2, BandwidthType a3);
    /// 0x0871a374
    virtual ~NetMapScriptAck();
    /// 0x086f7b10
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f7c50
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f7b74
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f7be2
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
