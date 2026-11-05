/// origin binary
#pragma once

/// size 0x10 FTMaker<int>
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
