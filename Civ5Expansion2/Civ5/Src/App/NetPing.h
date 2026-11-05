/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvEnums.h"
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa840
class NetPing : public FNetMessage {
public:
    /// 0x087012e8
    NetPing(PlayerTypes a1, int a2, int a3);
    /// 0x0871a770
    virtual ~NetPing();
    /// 0x08701338
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f659c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x0870136c
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x087013c6
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
