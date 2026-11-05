/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvString.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa3c0
class NetPickYourCiv : public FNetMessage {
public:
    /// 0x086f8a4a
    NetPickYourCiv();
    /// 0x086f8bcc
    NetPickYourCiv(CivilizationTypes const* a1, LeaderHeadTypes const* a2, CvString const* a3, CvString const* a4);
    /// 0x086f8d70
    /// deleting 0x086f8d48
    virtual ~NetPickYourCiv();
    /// 0x086f8f1a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f90c2
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f8f4e
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f9008
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
    /// 0x086f8e4e
    UNK_RET getCivAt(int a1) const;
    /// 0x086f8e68
    UNK_RET getLeaderAt(int a1) const;
    /// 0x086f8e82
    UNK_RET getCivNameAt(int a1) const;
    /// 0x086f8ece
    UNK_RET getLeaderNameAt(int a1) const;
};
