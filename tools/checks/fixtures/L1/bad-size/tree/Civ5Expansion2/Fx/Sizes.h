/// origin binary
#pragma once

/// size 0x8
class Plain {
public:
    int m_a;  ///< +0x0
};

/// size 0x8
template <class T>
class FBuf {
public:
    T m_data[2];  ///< +0x0
};

/// size 0xc
class Holder {
public:
    FBuf<int> m_a;   ///< +0x0
    FBuf<char> m_b;  ///< +0x8
};
