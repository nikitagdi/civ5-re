/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fafa0
class NetRenameUnit : public FNetMessage {
public:
    /// 0x0870cf9c
    NetRenameUnit();
    /// 0x0870cffe
    NetRenameUnit(PlayerTypes a1, int a2, CvString a3);
    /// 0x0871adb4
    /// deleting 0x0871adea
    virtual ~NetRenameUnit();
    /// 0x0870d078
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870d174
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870d0c0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870d11a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
