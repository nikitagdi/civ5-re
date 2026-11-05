/// origin binary
#pragma once

class FDataTransfer;

/// size ?
/// vtable 0x0a303a50 FQueue<FDataTransfer*>
template <class T0>
class FQueue {
public:
    class FQueueNode;
    /// base 0x08d4142a FQueue<FDataTransfer*>
    /// deleting 0x08d429c6 FQueue<FDataTransfer*>
    virtual ~FQueue();
    /// 0x08d413dc FQueue<FDataTransfer*>
    UNK_RET Pop();
    /// 0x08d41482 FQueue<FDataTransfer*>
    UNK_RET Push(T0 const& a1);
};
