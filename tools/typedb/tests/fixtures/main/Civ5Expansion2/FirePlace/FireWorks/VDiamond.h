/// origin guess
#pragma once

/// size 0x8
/// vtable 0x0a000300
struct VBase {
    /// 0x08120000
    virtual void f();
    int m_b;  ///< +0x4
};

/// size 0x10
/// vtable 0x0a000340
struct VLeft : virtual VBase {
    int m_l;  ///< +0x4
};

/// size 0x10
/// vtable 0x0a000380
struct VRight : virtual VBase {
    int m_r;  ///< +0x4
};

/// size 0x2c
/// vtable 0x0a0003c0
struct VDiamond : VLeft, VRight {
    /// 0x08120040
    /// thunk 0x08120080
    virtual void f();
    int m_d[5];  ///< +0x10
};
