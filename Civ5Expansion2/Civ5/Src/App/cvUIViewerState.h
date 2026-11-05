/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"

class cvPath;

/// size ?
/// vtable 0x0a2fd4e0
class cvUIViewerState : public Civ5GameState {
public:
    /// 0x0884c988
    cvUIViewerState();
    /// 0x0884ca02
    /// deleting 0x0884c9cc
    virtual ~cvUIViewerState();
    /// 0x0884cb06
    virtual UNK_RET Begin();
    /// 0x0884cb36
    virtual UNK_RET End();
    /// 0x0884cb94
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// 0x0884cb66
    virtual UNK_RET Render(unsigned int a1);
    /// 0x0865f3b4
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x0884ca32
    UNK_RET Initialize(cvPath const& a1);
};
