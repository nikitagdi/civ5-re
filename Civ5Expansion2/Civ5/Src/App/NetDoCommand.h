/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa740
class NetDoCommand : public FNetMessage {
public:
    /// 0x086ff428
    NetDoCommand();
    /// 0x086ff486
    NetDoCommand(PlayerTypes a1, int a2, CommandTypes a3, int a4, int a5, bool a6);
    /// 0x0871a672
    virtual ~NetDoCommand();
    /// 0x086ff4ec
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f5cbe
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086ff90c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086ff994
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
