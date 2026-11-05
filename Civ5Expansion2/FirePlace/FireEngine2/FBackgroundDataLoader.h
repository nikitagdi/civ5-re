/// origin guess
#pragma once

class FFileBackgroundLoader;

/// size ?
/// vtable 0x0a305a60
class FBackgroundDataLoader {
public:
    /// 0x08d94bac
    FBackgroundDataLoader(void* a1);
    /// 0x08d96670
    /// deleting 0x08d96672
    virtual ~FBackgroundDataLoader();
    /// pure
    virtual UNK_RET Load(FFileBackgroundLoader* a1) = 0;
    /// 0x08cd4b80
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08cd4b84
    virtual UNK_RET ObtainData();
    /// 0x08cd4b88
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// pure
    virtual UNK_RET Complete(bool a1) = 0;
    /// 0x08d94bda
    UNK_RET SetStatus(int a1);
    /// 0x0ff8b9f0
    static UNK_DATA ms_csBackgroundLock[0x18];
};

extern "C" {
    /// 0x0ff8ba08
    extern UNK_DATA gSharedBackgroundLoader[0x4];
}
