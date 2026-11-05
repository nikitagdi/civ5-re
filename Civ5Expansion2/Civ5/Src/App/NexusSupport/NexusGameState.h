/// origin guess
#pragma once
#include "Civ5/Src/App/Civ5GameState.h"

/// size ?
/// vtable 0x0a2fd460
class NexusGameState : public Civ5GameState {
public:
    /// 0x0884b2fc
    NexusGameState();
    /// 0x0884b45a
    /// deleting 0x0884b45c
    virtual ~NexusGameState();
    /// 0x0884b32e
    virtual UNK_RET Begin();
    /// 0x0884b418
    virtual UNK_RET End();
    /// 0x0884b458
    virtual UNK_RET Update(float a1);
    /// 0x0884b470
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x0865f3b4
    virtual UNK_RET vf_06(UNK_PARAMS);
};
