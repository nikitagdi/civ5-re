/// origin binary
#pragma once

/// size 0x4
/// vtable 0x0a1b2c08
class cvBase {
public:
    /// inline
    virtual void F() {}
};

/// size 0x4
/// vtable ?
class cvIface {
public:
    /// inline
    virtual void Flush() {}
};

/// size 0x8 FTMaker<int>
template <class T>
class FTMaker {
public:
    /// inline
    virtual void Make() {}
    T m_value;  ///< +0x4
};

/// size 0x4
/// vtable 0x0a1b2e00
class cvNoVt {
public:
    /// inline
    virtual void F() {}
};
