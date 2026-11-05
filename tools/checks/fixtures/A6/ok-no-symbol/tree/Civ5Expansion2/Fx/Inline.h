/// origin binary
#pragma once

/// size 0x4
/// vtable ?
class cvAbsent {
public:
    /// absent
    void Missing();

    /// inline
    int Peek() const { return 0; }

    /// pure
    virtual void Gone() = 0;

    /// inline
    cvAbsent() {}
};

/// inline
extern const int g_kLocal = 7;
