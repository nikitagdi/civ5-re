/// origin guess
#pragma once

// marker-unsupported: a member class template of a class template (member function templates have SPEC).
/// size 0x1
template <class T>
struct Unsupported {
    /// size 0x1
    template <class U>
    struct Rebind {};

    /// inline
    template <class U>
    void Convert(U u) {}
};
