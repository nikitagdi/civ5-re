/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"
#include <Windows.h>

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa980
class NetMPDropSave : public FNetMessage {
public:
    /// 0x08701e8c
    NetMPDropSave();
    /// 0x08701ef2
    NetMPDropSave(PlayerTypes a1, char* a2, _GUID const& a3, SaveGameTypes a4);
    /// 0x0871a824
    virtual ~NetMPDropSave();
    /// 0x08701f9e
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f65b0
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x08701fe6
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087020b2
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
