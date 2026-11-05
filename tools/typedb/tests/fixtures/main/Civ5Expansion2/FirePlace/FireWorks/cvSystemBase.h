/// origin guess
#pragma once

namespace FireWorks {

/// size 0x4
/// vtable 0x0a000200
class cvSystemBase {
public:
    /// 0x08110000
    /// deleting 0x08110040
    virtual ~cvSystemBase();

    /// 0x08110080
    virtual void Update();

    /// 0x081100c0
    virtual int Prio() const;
};

/// size 0x4
/// vtable 0x0a000240
class IEventListener {
public:
    /// pure
    virtual void OnEvent(int e) = 0;
};

}  // namespace FireWorks
