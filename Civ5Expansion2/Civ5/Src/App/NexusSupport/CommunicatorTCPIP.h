/// origin guess
#pragma once
#include "Civ5/Src/App/NexusSupport/Communicator.h"

class FInetHostAddress;
namespace Nexus {
class Listener;
class Message;
}

namespace Nexus {
/// size ?
/// vtable 0x0a306b80
class CommunicatorTCPIP : public Nexus::Communicator {
public:
    /// 0x08df526c
    CommunicatorTCPIP();
    /// 0x08df5368
    virtual UNK_RET Send(unsigned char const* a1, unsigned int a2);
    /// 0x08df539c
    virtual UNK_RET Update(float a1, Nexus::Message& a2, Nexus::Listener& a3);
    /// 0x08df58f2
    virtual UNK_RET IsEnabled() const;
    /// 0x08df589c
    virtual UNK_RET Disable();
    /// 0x08df551e
    UNK_RET ReadPending(Nexus::Message& a1, Nexus::Listener& a2);
    /// 0x08df5684
    UNK_RET Enable(FInetHostAddress const& a1, unsigned short a2);
    /// 0x08df56de
    UNK_RET Enable(char const* a1, unsigned short a2);
    /// 0x08df580c
    UNK_RET Enable(unsigned short a1);
    /// 0x08df58e6
    UNK_RET SetForceQuitOnDisconnect(bool a1);
};
}
