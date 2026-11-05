/// origin guess
#pragma once

class FNetMessage;

/// size ?
/// vtable 0x0a3050d8
class FNetQueue {
public:
    /// 0x08d6f3e6
    FNetQueue();
    /// 0x08d6f44a
    /// deleting 0x08d6f416
    virtual ~FNetQueue();
    /// 0x08d6f40a
    UNK_RET Initialize();
    /// 0x08d6f46e
    UNK_RET ClearAll();
    /// 0x08d6f4bc
    UNK_RET Add(FNetMessage& a1);
    /// 0x08d6f65e
    UNK_RET Get(int a1);
    /// 0x08d6f6d4
    UNK_RET GetTotalNodes();
};
