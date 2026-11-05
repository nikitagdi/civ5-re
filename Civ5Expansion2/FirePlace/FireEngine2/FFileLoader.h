/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FBackgroundDataLoader.h"

class FFileBackgroundLoader;

/// size ?
/// vtable 0x0a301cc0
class FFileLoader : public FBackgroundDataLoader {
public:
    /// 0x08d94be6
    FFileLoader(char const* a1, void* a2, bool a3);
    /// 0x08cdc616
    /// deleting 0x08cdc5ee
    virtual ~FFileLoader();
    /// 0x08d94fa8
    virtual UNK_RET Complete(bool a1);
    /// 0x08d94e9c
    virtual UNK_RET Load(FFileBackgroundLoader* a1);
    /// 0x08d94e4a
    virtual UNK_RET ObtainData();
    /// 0x08cdc800
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08cdc72c
    virtual UNK_RET vf_05(UNK_PARAMS);
    /// 0x08cdc814
    virtual UNK_RET vf_07(UNK_PARAMS);
};
