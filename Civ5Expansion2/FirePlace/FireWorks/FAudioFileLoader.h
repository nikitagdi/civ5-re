/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FFileLoader.h"

class FFileBackgroundLoader;

/// size ?
/// vtable 0x0a3036d0
class FAudioFileLoader : public FFileLoader {
public:
    /// 0x08d2d980
    FAudioFileLoader(char const* a1, void* a2);
    /// 0x08d2da42
    /// deleting 0x08d2da1a
    virtual ~FAudioFileLoader();
    /// 0x08d2d4f2
    virtual UNK_RET Load(FFileBackgroundLoader* a1);
    /// 0x08d2d952
    virtual UNK_RET OnFinishedLoading();
};
