/// origin guess
#pragma once

/// size ?
/// vtable 0x0a301eb0
class FTimerBase {
public:
    /// 0x08cded30
    FTimerBase();
    /// 0x08cdf38a
    /// deleting 0x08cdf38c
    virtual ~FTimerBase();
    /// 0x08cdedb6
    virtual UNK_RET Reset();
    /// 0x08cdee24
    virtual UNK_RET Update();
    /// 0x08cdeeb0
    virtual UNK_RET Pause();
    /// 0x08cdeee8
    virtual UNK_RET Unpause();
    /// pure
    virtual UNK_RET CalcSystemTime() = 0;
};
