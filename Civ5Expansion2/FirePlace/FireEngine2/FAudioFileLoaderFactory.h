/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFileLoaderFactory.h"

/// size ?
/// vtable 0x0a303710
class FAudioFileLoaderFactory : public FFileLoaderFactory {
public:
    /// 0x08cdc8f2
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08d2dd5e
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08d2daae
    virtual UNK_RET CreateLoader(char const* a1, void* a2);
};
