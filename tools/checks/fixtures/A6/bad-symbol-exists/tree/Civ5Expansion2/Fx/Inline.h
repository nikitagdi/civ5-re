/// origin binary
#pragma once

/// size 0x4
/// vtable ?
class cvBase {
public:
    /// inline
    void Init() {}

    /// absent
    void Fini();

    /// pure
    virtual void Update(float dt) = 0;
};

/// size 0x1
class cvOnlyBase {
public:
    /// inline
    cvOnlyBase() {}
};

/// inline
extern const int g_cvCounter = 4;
