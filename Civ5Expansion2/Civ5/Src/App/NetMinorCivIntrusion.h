/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fabc0
class NetMinorCivIntrusion : public FNetMessage {
public:
    /// 0x08705314
    NetMinorCivIntrusion(PlayerTypes a1, PlayerTypes a2, int a3);
    /// 0x08705378
    /// deleting 0x08705364
    virtual ~NetMinorCivIntrusion();
    /// 0x0870537a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08705462
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x087053ae
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08705408
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
