/// origin guess
#pragma once
#include "Civ5/Src/App/NexusSupport/Mode.h"
#include <Windows.h>

/// size ?
/// vtable 0x0a300710
class NexusUnitBuilder : public Nexus::Mode {
public:
    /// 0x08b688c0
    NexusUnitBuilder();
    /// 0x08b689d6
    /// deleting 0x08b6898a
    virtual ~NexusUnitBuilder();
    /// 0x08b68fe0
    virtual UNK_RET Shutdown();
    /// 0x08b68a1c
    virtual UNK_RET Startup();
    /// 0x08b6904e
    virtual UNK_RET Update();
    /// 0x08b68d98
    UNK_RET SetWindow(HWND__* a1);
    /// 0x08b68e68
    UNK_RET WindowResizing();
    /// 0x08b68e7c
    UNK_RET WindowResized();
    /// 0x08b68ebe
    UNK_RET MouseMove(int a1, int a2);
    /// 0x08b68fd0
    UNK_RET MiddleMouseButton(bool a1);
    /// 0x08b69254
    UNK_RET UpdateGlobals();
    /// 0x08b6991e
    UNK_RET UpdateCameraAspectRatio();
};
