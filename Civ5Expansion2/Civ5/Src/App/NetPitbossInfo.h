/// origin guess
#pragma once
#include "Civ5/Src/App/FNetMessage.h"

class FNetSessionIFace;

/// size ?
/// vtable 0x0a2fa540
class NetPitbossInfo : public FNetMessage {
public:
    /// 0x086fc814
    NetPitbossInfo();
    /// 0x0871a56c
    virtual ~NetPitbossInfo();
    /// 0x086fc84a
    virtual UNK_RET Debug(char* a1, unsigned int a2) const;
    // Empty body: the exe has no Pitboss active-player handling.
    /// 0x086fc87e
    virtual UNK_RET Execute(FNetSessionIFace* a1) const;
};
