/// origin guess
#pragma once

namespace Nexus {
/// size ?
class HView {
public:
    /// 0x101b3dd0
    static UNK_DATA INVALID[0x4];
};
}

extern "C" {
    /// 0x101b3f78
    extern UNK_DATA GlobalPoseCache[0x10];
    /// 0x101b3f88
    extern UNK_DATA MatchingMemberCallback[0x8];
    /// 0x101bb164
    extern UNK_DATA _g_CloseHandleCB[0x4];
    /// 0x101bb150
    extern UNK_DATA _g_CreateEventCB[0x4];
    /// 0x101bb14c
    extern UNK_DATA _g_CreateThreadCB[0x4];
    /// 0x101bb188
    extern UNK_DATA _g_LLMemPool[0x4];
    /// 0x101bb18c
    extern UNK_DATA _g_LLNodeMemPool[0x4];
    /// 0x101bb11c
    extern UNK_DATA _g_Log[0x4];
    /// 0x101c0248
    extern UNK_DATA _g_MemLog[0x8ca0];
    /// 0x101bb118
    extern UNK_DATA _g_MemSegmentHead[0x4];
    /// 0x101bb144
    extern UNK_DATA _g_PChunkTracer[0x4];
    /// 0x101bb160
    extern UNK_DATA _g_ResumeThreadCB[0x4];
    /// 0x101bb158
    extern UNK_DATA _g_SetEventCB[0x4];
    /// 0x101bb15c
    extern UNK_DATA _g_SetThreadPriorityCB[0x4];
    /// 0x101bb154
    extern UNK_DATA _g_WaitForSingleObjectCB[0x4];
    /// 0x101c8ee8
    extern UNK_DATA _g_aPEffecEmitterBuff[0x100];
    /// 0x101c8fe8
    extern UNK_DATA _g_aSubmitCB[0x80];
    /// 0x101bb148
    extern UNK_DATA _g_bUseEParticlePool[0x4];
    /// 0x101bb168
    extern UNK_DATA _g_nMultiCoreEnabled[0x4];
    /// 0x101bb13c
    extern UNK_DATA _g_nPBatchMem[0x4];
    /// 0x101bb140
    extern UNK_DATA _g_nPSortList[0x4];
    /// 0x101bb120
    extern UNK_DATA _g_nTrkKeyTracer[0x4];
    /// 0x101bb138
    extern UNK_DATA _g_pPBatchMem[0x4];
    /// 0x101bb130
    extern UNK_DATA _g_pPESortList[0x4];
    /// 0x101bb128
    extern UNK_DATA _g_pPEmitterPool[0x4];
    /// 0x101bb134
    extern UNK_DATA _g_pPSortList[0x4];
    /// 0x101bb124
    extern UNK_DATA _g_pPSystemPool[0x4];
    /// 0x101bb12c
    extern UNK_DATA _g_pParticlePool[0x4];
    /// 0x101bb16c
    extern UNK_DATA _g_pSDK[0x4];
    /// 0x101be248
    extern UNK_DATA g_CrcTable[0x2000];
    /// 0x101be244
    extern UNK_DATA g_bink_context[0x0];
}
