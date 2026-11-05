/// origin guess
#pragma once

namespace ns {
/// size 0x1
struct Key {
    enum Method { eA, eB };
};
/// size ?
template <class A, class B> struct Pair {
    A a;  ///< +0x0
    B b;  ///< +0x4
};
}

/// size 0x1
class FXml {
public:
    // Overloaded member templates; arguments with `::` and pointer arguments.
    /// 0x0a000100 SetVal<char const>
    template <class T>
    bool SetVal(char const* a1, unsigned int a2, T* a3);
    /// 0x0a000110 SetVal<ns::Key::Method>
    /// 0x0a000120 SetVal<char *>
    /// 0x0a000130 SetVal<ns::Pair<ns::Key::Method, int> >
    template <class T>
    bool SetVal(char const* a1, unsigned int a2, T a3);
    /// 0x0a000140 Put<int>
    template <class T>
    void Put(T const& a1);
    /// 0x0a000150 Put<char *>
    template <class T>
    static void Put(T& a1, int a2);
};

namespace ns {
// Overloaded function templates in a namespace.
/// 0x0a000200 toStr<ns::Key::Method>
/// 0x0a000210 toStr<char *>
template <class T>
int toStr(T const& v);
/// 0x0a000220 toStr<ns::Pair<int, ns::Key::Method> >
template <class T>
int toStr(T* v);
}

// The same SPEC on two overloads: each binds to its own declaration.
/// 0x0a000300 Each<ns::Key::Method>
template <class T>
void Each(FXml& a1, T& a2);
/// 0x0a000310 Each<ns::Key::Method>
template <class T>
void Each(ns::Pair<int, int>& a1, T& a2);

/// inline
inline void UseOver() {
    ns::Key::Method em = ns::Key::eA;
    ns::Pair<int, int> ep = {1, 2};
    FXml ex;
    Each(ex, em);
    Each(ep, em);
    FXml x;
    char c = 0;
    char* p = &c;
    ns::Key::Method m = ns::Key::eA;
    ns::Pair<ns::Key::Method, int> pr = {ns::Key::eA, 1};
    ns::Pair<int, ns::Key::Method> pr2 = {1, ns::Key::eA};
    x.SetVal("a", 1u, &c);
    x.SetVal("a", 1u, m);
    x.SetVal("a", 1u, p);
    x.SetVal("a", 1u, pr);
    x.Put(1);
    FXml::Put(p, 1);
    ns::toStr(m);
    ns::toStr(p);
    ns::toStr(&pr2);
}
