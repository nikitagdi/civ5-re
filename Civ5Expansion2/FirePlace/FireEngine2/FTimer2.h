/// origin guess
#pragma once

/// size ?
class FTimer2 {
public:
    /// 0x08cdef20
    FTimer2();
    /// 0x08cdef84
    UNK_RET GetCounterFrequency();
    /// 0x08cdefae
    UNK_RET Reset();
    /// 0x08cdf008
    UNK_RET GetAdjustedCurrentTime();
    /// 0x08cdf042
    UNK_RET Start();
    /// 0x08cdf0aa
    UNK_RET GetPerformanceCounter();
    /// 0x08cdf0d4
    UNK_RET Stop();
    /// 0x08cdf11a
    UNK_RET Advance();
    /// 0x08cdf166
    UNK_RET GetAbsoluteTime();
    /// 0x08cdf1b2
    UNK_RET GetTime();
    /// 0x08cdf220
    UNK_RET GetTimeValues(double* a1, double* a2, float* a3);
    /// 0x08cdf2fe
    UNK_RET GetElapsedTime();
    /// 0x08cdf380
    UNK_RET IsStopped();
};
