/// origin guess
#pragma once

// A pure virtual destructor with a body: the class's own vtable slots are __cxa_pure_virtual, the destructor
// function itself (D1/D0 and the base variant) still exists and has addresses.
/// size 0x8
/// vtable 0x0a001000
class FAutoArchive {
public:
    /// 0x0a000500
    /// base 0x0a000510
    /// deleting 0x0a000520
    virtual ~FAutoArchive() = 0;
    /// pure
    virtual void Serialize(int a1) = 0;
    /// 0x0a000530
    virtual void Reset();

    int m_n;  ///< +0x4
};

/// 0x0a000500
/// base 0x0a000510
FAutoArchive::~FAutoArchive() {}

/// size 0x8
/// vtable 0x0a001100
class FConcrete : public FAutoArchive {
public:
    /// 0x0a000600
    /// deleting 0x0a000610
    virtual ~FConcrete();
    /// 0x0a000620
    virtual void Serialize(int a1);
};
