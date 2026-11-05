/// origin binary
#pragma once

// One size line for every instantiation.
/// size 0x8
template <class T>
class FFastVector {
public:
    /// 0x08100000 FFastVector<int>
    void push_back(const T& v);

    /// inline
    int size() const { return m_iCount; }

    T* m_pData;    ///< +0x0
    int m_iCount;  ///< +0x4
};

/// size 0x8 FTMaker<int>
/// vtable 0x0a000100 FTMaker<int>
template <class T>
class FTMaker {
public:
    /// 0x08100100 FTMaker<int>
    /// base 0x08100120 FTMaker<int>
    /// deleting 0x08100140 FTMaker<int>
    virtual ~FTMaker();

    /// 0x08100180 FTMaker<int>
    virtual T Make() const;

    T m_value;  ///< +0x4
};

/// 0x08100200 Twice<int>
template <class T>
T Twice(T v);
