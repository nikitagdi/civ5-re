/// origin guess
#pragma once

// C1 inside the class name must not be taken for the structor code.
/// size 0x1
class C1Foo {
public:
    /// 0x08130000
    /// base 0x08130040
    C1Foo();

    /// base 0x08130080
    ~C1Foo();
};

/// size 0x4
/// vtable 0x0a000400
class VDtor {
public:
    /// 0x081300c0
    /// deleting 0x08130100
    virtual ~VDtor();
};
