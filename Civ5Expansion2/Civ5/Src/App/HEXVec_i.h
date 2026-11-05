/// origin guess
#pragma once

// Cube hex coordinates; z = -(x + y) where the callers build one.
/// size 0xc
class HEXVec_i {
public:
    /// 0x0a3c8bf8
    static HEXVec_i TRANSLATIONS[6];
    /// 0x0a3c8c40
    static HEXVec_i ZERO;

    int x;  ///< +0x0
    int y;  ///< +0x4
    int z;  ///< +0x8
};
