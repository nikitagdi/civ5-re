/// origin guess
#pragma once

class FAudioFileLoader;
class FBufferData;
class FFileBackgroundLoader;
class FSharedSoundData;

/// size ?
class FAudioBackgroundLoaderManager {
public:
    /// 0x08d2cbf4
    FAudioBackgroundLoaderManager();
    /// 0x08d2ccb2
    UNK_RET SetBGLoader(FFileBackgroundLoader* a1);
    /// 0x08d2ccbe
    UNK_RET IsBGLoadingEnabled() const;
    /// 0x08d2ccda
    UNK_RET Init(FFileBackgroundLoader* a1);
    /// 0x08d2cd1e
    UNK_RET Clear();
    /// 0x08d2cd84
    UNK_RET Shutdown();
    /// 0x08d2cd98
    UNK_RET IsInitialized();
    /// 0x08d2cda6
    UNK_RET ProcessLoadedBuffers();
    /// 0x08d2ceb4
    UNK_RET GetNextBufferInDoneList(FBufferData& a1);
    /// 0x08d2cf22
    UNK_RET RemoveHashFromDeletedHashList(int a1);
    /// 0x08d2cfbc
    UNK_RET ProcessLoadedBuffer(FSharedSoundData* a1, void* a2, FBufferData& a3);
    /// 0x08d2d048
    UNK_RET Update();
    /// 0x08d2d070
    UNK_RET CleanupDeletedContexts();
    /// 0x08d2d132
    UNK_RET AddContextToDeleteList(void* a1, bool a2);
    /// 0x08d2d1b4
    UNK_RET QueueFileOnContext(char const* a1, FSharedSoundData* a2, void* a3);
    /// 0x08d2d398
    UNK_RET RemoveContextHandleFromDeletedList(void* a1);
    /// 0x08d2d43a
    UNK_RET DoAsyncQueueFileOnContext(void* a1, char const* a2);
    /// 0x08d2d476
    UNK_RET DoSyncLoad(char const* a1);
    /// 0x08d2d530
    UNK_RET OnBackgroundLoadCompleted(FAudioFileLoader* a1);
    /// 0x08d2d708
    UNK_RET StartLoadingContext(void* a1);
    /// 0x08d2d746
    UNK_RET GetSoundGroupContextHandle(unsigned int a1);
    /// 0x08d2d750
    UNK_RET OnUnloadSharedSoundData(FSharedSoundData* a1);
    /// 0x08d2d7ac
    UNK_RET GetContextHandleForSharedSoundData(FSharedSoundData const* a1);
    /// 0x08d2d7b2
    UNK_RET UnloadContext(void* a1);
    /// 0x08d2d7ea
    UNK_RET RemoveAnyInfoContainingSharedSoundData(FSharedSoundData* a1);
    /// 0x08d2d89e
    UNK_RET OnContextUnloaded(void* a1);
    /// 0x0ff85af8
    static UNK_DATA m_pInst[0x4];
};
