/// origin guess
#pragma once

// internal: with an asm label the constructor has no C1 name to derive C2 from.
/// size 0x1
struct Internal {
    /// 0x08000000
    Internal() asm("internal_ctor");
};
