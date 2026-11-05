/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FBackgroundDataLoader.h"

class FFileBackgroundLoader;
class FResource;
class FResourceBackgroundLoaderContext;

/// size ?
/// vtable 0x0a301d30
class FResourceBackgroundDataLoader : public FBackgroundDataLoader {
public:
    /// 0x08cdbc7a
    FResourceBackgroundDataLoader(FResourceBackgroundLoaderContext* a1);
    /// 0x08cdbce4
    FResourceBackgroundDataLoader(FResource* a1, FResourceBackgroundLoaderContext* a2);
    /// 0x08cd4afc
    /// deleting 0x08cdc816
    virtual ~FResourceBackgroundDataLoader();
    /// 0x08cdbd82
    virtual UNK_RET Complete(bool a1);
    /// 0x08cdbd2a
    virtual UNK_RET Load(FFileBackgroundLoader* a1);
};
