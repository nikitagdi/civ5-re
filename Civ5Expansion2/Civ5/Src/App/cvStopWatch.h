/// origin guess
#pragma once

/// size ?
class cvStopWatch {
public:
    /// 0x089dcadc
    cvStopWatch(char const* a1, char const* a2);
    /// 0x089dcc02
    ~cvStopWatch();
    /// 0x089dcbb4
    UNK_RET StartPerfTest();
    /// 0x089dcc54
    UNK_RET EndPerfTest();
    /// 0x089dcd22
    UNK_RET InitPerfTest();
    /// 0x089dcd54
    UNK_RET PerfLog(char const* a1, double a2);
    /// 0x089dcdc8
    UNK_RET GetDeltaInSeconds() const;
    /// 0x0c083b08
    static UNK_DATA ms_bPerfInit[0x1];
    /// 0x0c083b10
    static UNK_DATA ms_ticksPerSecond[0x8];
};

/// 0x089dd480
UNK_RET InitFrac();
/// 0x089dd5ac
UNK_RET Fractalx(int a1, int a2);
/// 0x089dd63a
UNK_RET SmoothMove(int a1, int a2, int a3, int a4, int a5, int a6);
/// 0x089dd6d1
UNK_RET SmoothMoveXY(int a1, int a2, int a3, int a4, int a5, int a6, int a7);
extern "C" {
    /// 0x0c083cec
    extern UNK_DATA RandomA[0x18];
    /// 0x0c083d04
    extern UNK_DATA RandomS[0x18];
    /// 0x0c0844fc
    extern UNK_DATA g_LeaderBufferManager[0x30];
    /// 0x0c083b18
    extern UNK_DATA gs_Depth[0x30];
}

namespace ReplayMap { namespace Constants {
/// 0x0c08554c
extern UNK_DATA ui2HexSize[0x8];
}}
