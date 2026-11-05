/// origin guess
#pragma once
#include "Civ5/Src/App/NexusSupport/Mode.h"
#include <Windows.h>

/// size ?
/// vtable 0x0a300650
class NexusLandmarkEditor : public Nexus::Mode {
public:
    /// 0x08b58874
    NexusLandmarkEditor();
    /// 0x08b58922
    /// deleting 0x08b588d6
    virtual ~NexusLandmarkEditor();
    /// 0x08b58a36
    virtual UNK_RET Shutdown();
    /// 0x08b58968
    virtual UNK_RET Startup();
    /// 0x08b58a6e
    virtual UNK_RET Update();
    /// 0x08b589e8
    UNK_RET SetWindow(HWND__* a1);
};
