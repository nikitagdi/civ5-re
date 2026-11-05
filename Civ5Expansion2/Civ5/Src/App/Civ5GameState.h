/// origin guess
#pragma once
#include "Civ5/Src/App/cvEventSubscriber.h"

/// size ?
/// vtable ?
class Civ5GameState : public cvEventSubscriber {
public:
    char unk_8[0x8];    ///< +0x8
    /// absent
    virtual ~Civ5GameState();
};
