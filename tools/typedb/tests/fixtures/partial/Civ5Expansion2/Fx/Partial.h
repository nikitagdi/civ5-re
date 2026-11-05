/// origin guess
#pragma once

// A primary class template and its partial specializations are separate records.
/// size ?
/// vtable ?
template <typename ObjectType, typename ContainerType>
class FAutoVariable {
public:
    /// inline
    virtual ~FAutoVariable() {}
    /// inline
    const ObjectType& get() const { return m_value; }

    ObjectType m_value;  ///< +0x4
};

/// size 0x8
template <typename ElementType, typename ClassContainer>
class FAutoVariable<ElementType*, ClassContainer> {
public:
    /// inline
    int size() const { return m_iCount; }

    ElementType* m_pData;  ///< +0x0
    int m_iCount;          ///< +0x4
};

/// size 0x1
template <class T> struct LuaAnalog { typedef T Result; };
/// size 0x1
template <class T> struct LuaAnalog<T&> { typedef T* Result; };
/// size 0x1
template <class T> struct LuaAnalog<const T> { typedef T Result; };

// A class template with a member template: its other members are still functions.
/// size 0x4
template <class T>
class FDebugHelper {
public:
    /// inline
    int Plain(int x) { return x; }
    /// inline
    template <class Function> void WalkStack(Function f) {}

    T m_value;  ///< +0x0
};

/// inline
inline int UseAll() {
    FAutoVariable<int, char> a;
    FAutoVariable<int*, char> b;
    LuaAnalog<int&>::Result r = 0;
    return b.size() + FDebugHelper<int>().Plain(a.get()) + (r != 0);
}
