/// origin guess
#pragma once

/// size 0x10
class FGXVector4 {
public:
    float x;  ///< +0x0
    float y;  ///< +0x4
    float z;  ///< +0x8
    float w;  ///< +0xc

    /// 0x0d27c064
    static UNK_DATA UNIT_W[0x10];
    /// 0x0d27c034
    static UNK_DATA UNIT_X[0x10];
    /// 0x0d27c044
    static UNK_DATA UNIT_Y[0x10];
    /// 0x0d27c054
    static UNK_DATA UNIT_Z[0x10];
    /// 0x0d27c024
    static UNK_DATA ZERO[0x10];
};

namespace FMatrix {
/// 0x0d27c104
extern UNK_DATA IDENTITY[0x40];
/// 0x0d27c084
extern UNK_DATA ROT_IDENTITY[0x40];
/// 0x0d27c0c4
extern UNK_DATA ROT_ZERO[0x40];
}

namespace FQuaternion {
/// 0x0d27c074
extern UNK_DATA IDENTITY[0x10];
}

namespace FireGrafix {
/// 0x0d27c1d8
extern UNK_DATA g_nTypeNames[0x4];
/// 0x0d27c1d4
extern UNK_DATA g_ppcTypeNames[0x4];
}
