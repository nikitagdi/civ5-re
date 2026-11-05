/// origin guess
#pragma once

/// size 0x1
struct CvUnit {
    /// inline
    int GetX() const { return 1; }
    /// inline
    int GetY(int a) const { return a; }
    /// inline
    void Go() const {}
};

/// size 0x1
struct CvLuaUnit {};

// A member function template of a class template: SPEC is `Class<CArgs>::name<MArgs>`.
/// size 0x1
template <class Derived, class InstanceType>
class CvLuaMethodWrapper {
public:
    /// 0x0a000100 CvLuaMethodWrapper<CvLuaUnit,CvUnit>::BasicLuaMethod<int>
    template <typename ret>
    static int BasicLuaMethod(int L, ret (InstanceType::*func)() const);
    /// 0x0a000110 CvLuaMethodWrapper<CvLuaUnit, CvUnit>::BasicLuaMethod<int, int>
    template <typename ret, typename arg1>
    static int BasicLuaMethod(int L, ret (InstanceType::*func)(arg1) const);
    /// inline
    static int BasicLuaMethod(int L, void (InstanceType::*func)() const);
    /// 0x0a000120 CvLuaMethodWrapper<CvLuaUnit,CvUnit>::Push<short>
    /// 0x0a000130 CvLuaMethodWrapper<CvLuaUnit,CvUnit>::Push<char *>
    template <typename T>
    void Push(T const& v);
    /// inline
    template <typename T>
    void Pop(T v) {}
};

/// 0x0a000100 CvLuaMethodWrapper<CvLuaUnit,CvUnit>::BasicLuaMethod<int>
template <class Derived, class InstanceType> template <typename ret>
int CvLuaMethodWrapper<Derived, InstanceType>::BasicLuaMethod(int L, ret (InstanceType::*func)() const)
{
    return 1;
}

/// inline
template <class Derived, class InstanceType>
int CvLuaMethodWrapper<Derived, InstanceType>::BasicLuaMethod(int L, void (InstanceType::*func)() const)
{
    return 0;
}

/// inline
inline void UseWrapper() {
    typedef CvLuaMethodWrapper<CvLuaUnit, CvUnit> W;
    char c = 0;
    char* p = &c;
    W::BasicLuaMethod(0, &CvUnit::GetX);
    W::BasicLuaMethod(0, &CvUnit::GetY);
    W::BasicLuaMethod(0, &CvUnit::Go);
    W w;
    w.Push<short>(1);
    w.Push<char*>(p);
}
