/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faf80
class NetRenameCity : public FNetMessage {
public:
    /// 0x0870ccee
    NetRenameCity();
    /// 0x0870cd50
    NetRenameCity(PlayerTypes a1, int a2, CvString a3);
    /// 0x0871ad42
    /// deleting 0x0871ad78
    virtual ~NetRenameCity();
    /// 0x0870cdca
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870cec6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870ce12
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870ce6c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
