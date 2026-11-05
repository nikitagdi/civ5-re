/// origin binary
#pragma once

/// size 0x18
class Fields {
public:
    int m_iCount;           ///< +0x4
    short m_sKind;          ///< +0x4
    unsigned m_bFlagA : 1;  ///< +0x6
    unsigned m_bFlagB : 3;  ///< +0x7
    /// size 0x4
    union {
        int m_iRaw;         ///< +0x8
        float m_fRaw;       ///< +0x0
    };
    /// size 0x4
    union {
        int m_iA;           ///< +0x0
        float m_fA;         ///< +0x0
    } m_u;                  ///< +0xc
    char unk_10[0x8];       ///< +0x10
};

/// size 0x8
template <class T>
class FPair {
public:
    T m_first;   ///< +0x0
    T m_second;  ///< +0x4
};

/// size 0x10
class Users {
public:
    FPair<int> m_a;    ///< +0x0
    FPair<float> m_b;  ///< +0x8
};
