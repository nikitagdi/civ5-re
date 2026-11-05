/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa4a0
class NetVersionWarning : public FNetMessage {
public:
    /// 0x086f9846
    NetVersionWarning(PlayerTypes a1, bool a2);
    /// 0x0871a4f4
    virtual ~NetVersionWarning();
    /// 0x086f988c
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f997e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f98e6
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f9932
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
