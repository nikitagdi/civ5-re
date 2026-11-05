/// origin binary
#pragma once

/// size 0x4 cvBox<int>
/// size 0x4 cvBox<unsigned int>
template <class T>
class cvBox {
public:
    /// 0x08350300 cvBox<int>
    /// 0x08350300 cvBox<unsigned int>
    void Get();

    T m_value;  ///< +0x0
};
