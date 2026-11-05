/// origin guess
#pragma once

enum eMPoolType { eMPoolA, eMPoolB = 3 };

/// size ?
template <class T, eMPoolType pool>
class FArr {
public:
    /// 0x0a000100 FArr<char,(eMPoolType)3>
    /// 0x0a000110 FArr<short,3>
    void Add(T v);
    T m_v;  ///< +0x0
};

/// size ?
template <class T, bool B, unsigned N, char C>
struct FMix {
    T m_v;  ///< +0x0
};

/// inline
inline void UseEnum() {
    FArr<int, eMPoolB> a;
    FArr<char, eMPoolB> b;
    FArr<short, eMPoolB> s;
    (void)s;
    FMix<int, true, 4u, 'x'> m;
    b.Add(1);
    (void)a;
    (void)m;
}
