/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"

/// size ?
/// vtable 0x0a2fc690
class StartupState : public Civ5GameState {
public:
    /// 0x0875f6fc
    StartupState();
    /// 0x0875f768
    /// deleting 0x0875f740
    virtual ~StartupState();
    /// 0x0875f7ca
    virtual UNK_RET Begin();
    /// 0x087600de
    virtual UNK_RET End();
    /// 0x08760156
    virtual UNK_RET Update(float a1);
    /// 0x087601b2
    virtual UNK_RET Render(unsigned int a1);
    /// 0x0865f3b4
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x0875f79c
    UNK_RET IsDone();
};
