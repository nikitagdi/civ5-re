/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa2e0
class NetInterimNotice : public FNetMessage {
public:
    /// 0x086f776c
    NetInterimNotice();
    /// 0x0871a134
    virtual ~NetInterimNotice();
    /// 0x086f77a2
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    /// 0x086f77d6
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
};
