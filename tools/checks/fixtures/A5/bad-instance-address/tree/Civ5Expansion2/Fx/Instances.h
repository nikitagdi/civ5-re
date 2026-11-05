/// origin binary
#pragma once

/// size 0x8
template <class T>
class FFastVector {
public:
    /// 0x0834a404 FFastVector<int>
    void Grow();

    T* m_pData;    ///< +0x0
    int m_iCount;  ///< +0x4
};

/// size 0x8 FTMaker<int>
/// vtable 0x0a1b2d60 FTMaker<int>
template <class T>
class FTMaker {
public:
    /// 0x0834a420 FTMaker<int>
    /// deleting 0x0834a438 FTMaker<int>
    virtual ~FTMaker();

    /// 0x0834a440 FTMaker<int>
    virtual T Make() const;

    T m_value;  ///< +0x4
};
