/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa480
class NetCivChosen : public FNetMessage {
public:
    /// 0x086f9662
    NetCivChosen(PlayerTypes a1, CvString const& a2, CvString const& a3);
    /// 0x0871a21e
    /// deleting 0x0871a264
    virtual ~NetCivChosen();
    /// 0x086f9728
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f9844
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f9790
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f97ea
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
