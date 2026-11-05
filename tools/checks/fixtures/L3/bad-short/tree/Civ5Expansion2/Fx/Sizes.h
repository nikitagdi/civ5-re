/// origin binary
#pragma once

/// size 0x8
class Short {
public:
    int m_a;  ///< +0x0
    int m_b;  ///< +0x4
};

/// size 0x8
template <class T>
class FBuf {
public:
    T* m_p;    ///< +0x0
    int m_n;   ///< +0x4
};

/// size 0x8
class Holder {
public:
    FBuf<int> m_a;  ///< +0x0
};
