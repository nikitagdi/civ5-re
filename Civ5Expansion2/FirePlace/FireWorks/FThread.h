/// origin binary
#pragma once

class FSemaphore;

/// size ?
/// vtable 0x0a3066f0
class FThread {
public:
    enum EPriority : int;
    /// 0x08dc2d3e
    FThread(bool a1);
    /// 0x08dc2e9a
    FThread(FThread::EPriority a1, unsigned int a2);
    /// 0x08dc31a0
    /// deleting 0x08dc3178
    virtual ~FThread();
    /// 0x08dc3560
    virtual UNK_RET Run();
    /// 0x087ab956
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x087ab958
    virtual UNK_RET Initial();
    /// 0x087ab95a
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x087ab95e
    virtual UNK_RET vf_06(UNK_PARAMS);
    /// 0x08dc2e50
    UNK_RET InitMainThread();
    /// 0x08dc3008
    UNK_RET Execute(void* a1);
    /// 0x08dc314a
    UNK_RET GetThread();
    /// 0x08dc3176
    UNK_RET SetPriority(FThread::EPriority a1);
    /// 0x08dc31fc
    UNK_RET Start(FSemaphore* a1);
    /// 0x08dc3234
    UNK_RET Detach(FSemaphore* a1);
    /// 0x08dc3266
    UNK_RET Terminate();
    /// 0x08dc32ee
    UNK_RET Stop();
    /// 0x08dc3324
    UNK_RET Exit();
    /// 0x08dc3356
    UNK_RET Close();
    /// 0x08dc33f8
    UNK_RET Suspend();
    /// 0x08dc3494
    UNK_RET Resume();
    /// 0x100a1530
    static UNK_DATA g_pthrMain[0x4];
    /// 0x100a1534
    static UNK_DATA ms_keySelf[0x4];
};

/// 0x087b29c4
UNK_RET CityArtTypeToIndex(char const* a1);
extern "C" {
    /// 0x100a19b0
    extern UNK_DATA g_StringHashPalette[0x30];
}

namespace FOutputLocation {
/// 0x100a20a8
extern UNK_DATA ms_strConfigFileName[0x4];
}

namespace FStringTable {
/// 0x100a1a74
extern UNK_DATA ms_kAccessLock[0x18];
/// 0x100a1a70
extern UNK_DATA ms_papkTableManager[0x4];
/// 0x100a1a8c
extern UNK_DATA ms_pkActiveStringTable[0x4];
/// 0x100a1a90
extern UNK_DATA ms_uiNumDiskAccesses[0x4];
/// 0x100a1a94
extern UNK_DATA ms_uiNumLRUCacheHits[0x4];
/// 0x100a1a98
extern UNK_DATA ms_uiNumLRUCacheMisses[0x4];
/// 0x100a1a9c
extern UNK_DATA ms_uiNumMissingStrings[0x4];
/// 0x100a1aa0
extern UNK_DATA ms_uiNumProximityCacheHits[0x4];
/// 0x100a1aa4
extern UNK_DATA ms_uiNumProximityCacheMisses[0x4];
}

namespace cvCity_Properties {
/// 0x087b2a1e
UNK_RET GameplayToEngineEra(int a1);
}
