/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fabe0
class NetMinorCivEnterTerritory : public FNetMessage {
public:
    /// 0x08705464
    NetMinorCivEnterTerritory(PlayerTypes a1, PlayerTypes a2);
    /// 0x0871a9ae
    virtual ~NetMinorCivEnterTerritory();
    /// 0x087054ae
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870557a
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087054e2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870552e
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
