/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fac40
class NetCityBuyPlot : public FNetMessage {
public:
    /// 0x08705b0e
    NetCityBuyPlot();
    /// 0x08705b60
    NetCityBuyPlot(PlayerTypes a1, int a2, int a3, int a4);
    /// 0x0871a9d6
    virtual ~NetCityBuyPlot();
    /// 0x08705bb6
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08705c10
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08705ce2
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08705d4c
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
