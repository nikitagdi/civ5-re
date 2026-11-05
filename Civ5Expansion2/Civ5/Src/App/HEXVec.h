/// origin guess
#pragma once

/// size 0x10
class HEXVec {
public:
    float x;  ///< +0x0
    float y;  ///< +0x4
    float z;  ///< +0x8
    float w;  ///< +0xc

    /// 0x0a3c8b88
    static HEXVec TRANSLATIONS[6];
    /// 0x0a3c8be8
    static UNK_DATA ZERO[0x10];
};
