/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2faf60
class NetSellBuilding : public FNetMessage {
public:
    /// 0x0870ca8c
    NetSellBuilding();
    /// 0x0870cad8
    NetSellBuilding(PlayerTypes a1, int a2, BuildingTypes a3);
    /// 0x0871ad2e
    virtual ~NetSellBuilding();
    /// 0x0870cb28
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x0870cc24
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870cb70
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x0870cbca
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
