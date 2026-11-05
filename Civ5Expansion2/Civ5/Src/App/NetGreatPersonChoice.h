/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fad00
class NetGreatPersonChoice : public FNetMessage {
public:
    /// 0x08706b30
    NetGreatPersonChoice();
    /// 0x08706b74
    NetGreatPersonChoice(PlayerTypes a1, UnitTypes a2);
    /// 0x0871aa4e
    virtual ~NetGreatPersonChoice();
    /// 0x08706bbe
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08706bfc
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08706cbe
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08706d0a
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
