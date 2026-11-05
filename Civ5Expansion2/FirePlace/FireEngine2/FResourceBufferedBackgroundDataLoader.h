/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FFileLoader.h"

class FResource;
class FResourceBackgroundLoaderContext;

/// size ?
/// vtable 0x0a301d60
class FResourceBufferedBackgroundDataLoader : public FFileLoader {
public:
    /// 0x08cdbd84
    FResourceBufferedBackgroundDataLoader(FResource* a1, FResourceBackgroundLoaderContext* a2);
    /// 0x08cdc85c
    /// deleting 0x08cdc8a2
    virtual ~FResourceBufferedBackgroundDataLoader();
    /// 0x08cdbddc
    virtual UNK_RET Complete(bool a1);
};
