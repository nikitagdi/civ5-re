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

/// size 0x8
/// vtable 0x0a1b2cc0
class cvUnitVisSystem : public cvBase {
public:
    /// 0x0834a0a0
    cvUnitVisSystem();

    /// 0x0834a0e0
    /// base 0x0834a0f0
    /// deleting 0x0834a100
    virtual ~cvUnitVisSystem();

    /// 0x0834a120
    virtual void Update(float dt);

    /// 0x0834a160
    virtual void Flush();

    int m_iCount;  ///< +0x4
};
