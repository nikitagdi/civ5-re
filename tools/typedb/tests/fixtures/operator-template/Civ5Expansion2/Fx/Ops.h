/// origin guess
#pragma once

/// size 0x4
struct FStream {
    int m_pos;  ///< +0x0
};

// The name of an operator template is spelled as in C++: `operator<< <int>`.
/// 0x0a000300 operator<< <int>
/// 0x0a000310 operator<< <char>
template <class T>
FStream& operator<<(FStream& s, const T& v);

/// 0x0a000320 operator< <int>
template <class T>
bool operator<(const FStream& s, const T& v);

/// 0x0a000330 operator>> <FStream *>
template <class T>
FStream& operator>>(FStream& s, T& v);

/// size 0x4
class FOut {
public:
    /// 0x0a000400 operator<< <int>
    /// 0x0a000410 operator<< <FStream>
    template <class T>
    FOut& operator<<(T v);

    /// 0x0a000420 operator() <int>
    template <class T>
    int operator()(T v) const;

    int m_n;  ///< +0x0
};

/// inline
inline void UseOps() {
    FStream s;
    FOut o;
    s << 1 << 'c';
    (void)(s < 2);
    FStream* p = 0;
    s >> p;
    o << 1 << s;
    (void)o(3);
}
