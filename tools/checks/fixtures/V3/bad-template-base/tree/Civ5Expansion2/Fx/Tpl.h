/// origin binary
#pragma once

/// size 0x4
template <class T, unsigned N> class cvTpl {
public:
    T m_Data;  ///< +0x0
};

/// size 0x4
/// vtable ?
class cvLeafT : public cvTpl<int, 4U> {
public:
    /// 0x08350900
    virtual void Run();
};
