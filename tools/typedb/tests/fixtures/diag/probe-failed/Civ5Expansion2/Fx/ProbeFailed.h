/// origin guess
#pragma once

// probe-failed: a final dynamic class cannot be derived from by the vtable probe.
/// size 0x4
/// vtable 0x0a000000
class ProbeFailed final {
public:
    /// 0x08000000
    virtual void Run();
};
