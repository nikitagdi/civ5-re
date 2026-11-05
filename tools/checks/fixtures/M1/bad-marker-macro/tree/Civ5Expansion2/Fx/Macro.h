/// origin guess
#pragma once

#define TWO_METHODS void First(); void Second();

// marker-macro: one invocation yields two markable entities.
/// size 0x1
struct Macro {
    /// 0x08000000
    TWO_METHODS
};
