/// origin guess
#pragma once

/// size 0x4
class Pm {
public:
    /// 0x08360000
    Pm();

    /// base 0x08360100
    Pm(int x);

    /// 0x08360040 fastcc
    static int Count(int a1, int a2);

    /// 0x08360080
    void Run();

    /// inline
    void Spin();

    int m_i;  ///< +0x0

    /// 0x0b000200
    static int s_iCount;
};

/// size 0x4
template <class T>
struct Box {
    /// 0x08360300 Box<int>
    void Put(const T& v);
    T m_v;  ///< +0x0
};
