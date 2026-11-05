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

    /// 0x0834a060
    virtual void Update(float dt);

    /// pure
    virtual void Draw() = 0;

    /// 0x0834a380
    void Init();

    /// 0x0834a390
    void Fini();
};

/// size 0x4
/// vtable ?
class cvLeaf : public cvBase {
public:
    /// inline
    virtual void Draw() {}
};
