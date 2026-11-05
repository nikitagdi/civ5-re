/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFileLoaderFactory.h"

/// size ?
/// vtable 0x0a301db0
class FResourceBackgroundLoaderFactory : public FFileLoaderFactory {
public:
    /// 0x08cdc5a8
    FResourceBackgroundLoaderFactory();
    /// 0x08cdc8f2
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cdc8f4
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cdc5c4
    virtual UNK_RET CreateLoader(char const* a1, void* a2);
};
