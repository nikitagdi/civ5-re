/// origin guess
#pragma once
#include <Windows.h>

class FCamera;

/// size ?
class FManualCameraController {
public:
    /// 0x088a5e4c
    FManualCameraController();
    /// 0x088a5eae
    ~FManualCameraController();
    /// 0x088a5f5c
    UNK_RET DeActivate();
    /// 0x088a6000
    UNK_RET Activate(FCamera* a1);
    /// 0x088a607e
    UNK_RET ProcessInput(HWND__* a1, unsigned int a2, unsigned int a3, long a4);
    /// 0x088a61e2
    UNK_RET Update(float a1);
    /// 0x09212c8c
    static UNK_DATA DEFAULT_ACCELERATION[0x4];
};

extern "C" {
    /// 0x092131c0
    extern UNK_DATA cvNumRoadTypes[0x20];
}
