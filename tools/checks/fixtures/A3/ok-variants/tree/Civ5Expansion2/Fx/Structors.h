/// origin binary
#pragma once

/// size 0x4
/// vtable 0x0a1b2c00
class cvBase {
public:
    /// 0x0834a000
    cvBase();

    /// 0x0834a030
    /// deleting 0x0834a040
    virtual ~cvBase();

};

/// size 0x4
/// vtable ?
class cvUnitVisSystem {
public:
    /// 0x0834a0a0
    cvUnitVisSystem();

    /// 0x0834a0e0
    /// base 0x0834a0f0
    /// deleting 0x0834a100
    virtual ~cvUnitVisSystem();
};

/// size 0x1
class cvOnlyBase {
public:
    /// base 0x0834a220
    cvOnlyBase();
};

/// size 0x4
/// vtable ?
class cvLeaf {
public:
    /// base 0x0834a250
    /// deleting 0x0834a240
    virtual ~cvLeaf();
};

/// size 0x1
class FSocket {
public:
    /// 0x0834a270
    ~FSocket();
};

/// size 0x1
class FUDPSocket {
public:
    /// 0x0834a270
    ~FUDPSocket();
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
