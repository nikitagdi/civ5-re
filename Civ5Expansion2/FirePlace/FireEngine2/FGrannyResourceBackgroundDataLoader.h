/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FResourceBackgroundDataLoader.h"

class FFileBackgroundLoader;
class FResourceBackgroundLoaderContext;

/// size ?
/// vtable 0x0a301af0
class FGrannyResourceBackgroundDataLoader : public FResourceBackgroundDataLoader {
public:
    /// 0x08cd3c6c
    FGrannyResourceBackgroundDataLoader(FResourceBackgroundLoaderContext* a1);
    /// 0x08cd3ca6
    FGrannyResourceBackgroundDataLoader(FResourceBackgroundLoaderContext* a1, bool a2);
    /// 0x08cd4b3a
    virtual ~FGrannyResourceBackgroundDataLoader();
    /// 0x08cd3ec8
    virtual UNK_RET Complete(bool a1);
    /// 0x08cd3ce4
    virtual UNK_RET Load(FFileBackgroundLoader* a1);
};
