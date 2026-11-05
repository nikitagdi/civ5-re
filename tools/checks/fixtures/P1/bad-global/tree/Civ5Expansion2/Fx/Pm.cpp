/// origin guess
#include "Fx/Pm.h"

/// 0x0b000208
int Pm::s_iCount = 0;

/// 0x08360000
Pm::Pm() : m_i(0) {}

/// base 0x08360100
Pm::Pm(int x) : m_i(x) {}

/// 0x08360040 fastcc
int Pm::Count(int a1, int a2) { return a1 + a2; }

/// 0x08360080
void Pm::Run() {}

/// inline
void Pm::Spin() {}

/// 0x08360300 Box<int>
template <class T>
void Box<T>::Put(const T& v) { m_v = v; }
