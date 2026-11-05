/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"
#include "FirePlace/FireWorks/FDataStream.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa300
class NetFileInfo : public FNetMessage {
public:
    /// 0x086f77d8
    NetFileInfo();
    /// 0x086f782a
    NetFileInfo(char* a1, unsigned int a2);
    /// 0x0871a148
    virtual ~NetFileInfo();
    /// 0x086f78b0
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f78f8
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
    /// 0x086f8992
    virtual UNK_RET PutInBuffer(FDataStream& a1) const;
    /// 0x086f89ee
    virtual UNK_RET SetFromBuffer(FDataStream& a1);
};
