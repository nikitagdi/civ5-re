/// origin guess
#pragma once
#include "FirePlace/FireWorks/EventSystem/FastDelegate.h"

namespace Nexus {
class Communicator;
class DataOutStream;
class Message;
}

namespace Nexus {
/// size ?
/// vtable 0x0a306bb0
class Listener {
public:
    /// 0x08df5904
    Listener();
    /// 0x08df598a
    /// deleting 0x08df5962
    virtual ~Listener();
    /// 0x08df5b8a
    virtual UNK_RET RegisterModes();
    /// 0x08df5b8c
    virtual UNK_RET Update(float a1);
    /// 0x08df59f0
    UNK_RET Shutdown();
    /// 0x08df5a66
    UNK_RET Startup();
    /// 0x08df5aa0
    UNK_RET Disable();
    /// 0x08df5acc
    UNK_RET Send(Nexus::Message& a1);
    /// 0x08df5b0e
    UNK_RET Enabled() const;
    /// 0x08df5b40
    UNK_RET MessageAllActiveClients(char const* a1);
    /// 0x08df5bfa
    UNK_RET SendMessageA(unsigned short a1, fastdelegate::FastDelegate1<Nexus::DataOutStream&, void> const& a2);
    /// 0x08df5c9e
    UNK_RET ProcessInput();
    /// 0x08df5da0
    UNK_RET NewClient(unsigned short a1, char const* a2);
    /// 0x08df6068
    UNK_RET DestroyClient(unsigned short a1);
    /// 0x08df60ba
    UNK_RET GetCommunicator() const;
    /// 0x08df60c6
    UNK_RET SetCommunicator(Nexus::Communicator* a1);
    /// 0x08df60d6
    UNK_RET GetOutputMessage();
    /// 0x08df60e0
    UNK_RET GetInputMessage();
};
}

extern "C" {
    /// 0x0a3086d4
    extern UNK_DATA ms_defaultPopupTypeNames[0x17c];
}

namespace GraphicsSettings { namespace GeneralSettings {
/// 0x0a3086b4
extern UNK_DATA g_bAllowCheapDX11ShadowBlur[0x1];
/// 0x0a3086bc
extern UNK_DATA g_fIngame3DConvergenceDepthMax[0x4];
/// 0x0a3086b8
extern UNK_DATA g_fIngame3DConvergenceDepthMin[0x4];
/// 0x0a3086c0
extern UNK_DATA g_fStereoCursorDepthOffset[0x4];
}}
