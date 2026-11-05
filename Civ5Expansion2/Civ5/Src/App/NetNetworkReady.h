/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa240
class NetNetworkReady : public FNetMessage {
public:
    /// 0x086f5222
    NetNetworkReady();
    /// 0x0871a092
    virtual ~NetNetworkReady();
    /// 0x086f5258
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f528c
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
};
