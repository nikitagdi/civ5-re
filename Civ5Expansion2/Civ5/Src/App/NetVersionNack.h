/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa3a0
class NetVersionNack : public FNetMessage {
public:
    /// 0x086f880e
    NetVersionNack(VersionTypes a1, int a2, char const* a3);
    /// 0x0871a482
    /// deleting 0x0871a4b8
    virtual ~NetVersionNack();
    /// 0x086f8880
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f8990
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f88dc
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f8936
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
