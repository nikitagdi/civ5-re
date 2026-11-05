/// origin binary
#pragma once

/// size 0x8
class Plain {
public:
    int m_a;  ///< +0x0
    int m_b;  ///< +0x4
};

/// size ?
class Unknown {
public:
    int m_a;  ///< +0x0
};

/// size 0x8
template <class T>
class FBuf {
public:
    T* m_p;    ///< +0x0
    int m_n;   ///< +0x4
};

/// size 0x10
class Holder {
public:
    FBuf<int> m_a;     ///< +0x0
    FBuf<char*> m_b;   ///< +0x8
};

/// size 0x8 FTMaker<int>
/// vtable 0x0a1b2d60 FTMaker<int>
template <class T>
class FTMaker {
public:
    /// 0x0834a420 FTMaker<int>
    /// deleting 0x0834a430 FTMaker<int>
    virtual ~FTMaker();

    /// 0x0834a440 FTMaker<int>
    virtual T Make() const;

    T m_value;  ///< +0x4
};
