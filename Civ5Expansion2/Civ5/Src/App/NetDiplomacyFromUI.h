/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa260
class NetDiplomacyFromUI : public FNetMessage {
public:
    /// 0x086f6890
    NetDiplomacyFromUI(PlayerTypes a1, PlayerTypes a2, FromUIDiploEventTypes a3, int a4, int a5);
    /// 0x0871a986
    virtual ~NetDiplomacyFromUI();
    /// 0x086f68ee
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f69e0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f68f0
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f6968
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
