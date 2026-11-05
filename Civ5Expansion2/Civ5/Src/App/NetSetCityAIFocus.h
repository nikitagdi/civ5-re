/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fadc0
class NetSetCityAIFocus : public FNetMessage {
public:
    /// 0x08707ada
    NetSetCityAIFocus();
    /// 0x08707b26
    NetSetCityAIFocus(PlayerTypes a1, int a2, CityAIFocusTypes a3);
    /// 0x0871aac6
    virtual ~NetSetCityAIFocus();
    /// 0x08707b76
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x08707bc6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08707c92
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x08707cec
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
