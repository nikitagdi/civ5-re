/// origin guess
#pragma once
#include "FirePlace/FireWorks/FReferenceObject.h"

class FResourceBackgroundLoaderContext;
class FResourceLoadHelper;

/// size ?
/// vtable 0x0a3026f0
class FResource : public FReferenceObject {
public:
    /// 0x08ce7d28
    /// deleting 0x08ce7d14
    virtual ~FResource();
    /// 0x08ce7d44
    virtual UNK_RET Load(char const* a1, unsigned int a2, FResourceLoadHelper* a3) = 0;
    /// 0x08ce7d90
    virtual UNK_RET Unload();
    /// pure
    virtual UNK_RET GetData(void* a1, unsigned int a2) = 0;
    /// pure
    virtual UNK_RET SetData(void* a1) = 0;
    /// pure
    virtual UNK_RET SetData(unsigned char* a1, unsigned int a2, bool a3) = 0;
    /// pure
    virtual UNK_RET vf_07(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_08(UNK_PARAMS) = 0;
    /// 0x08cd4a66
    virtual UNK_RET vf_09(UNK_PARAMS);
    /// 0x08ce7d56
    virtual UNK_RET InUse();
    /// 0x08cd5b1a
    virtual UNK_RET CreateLoader(FResourceBackgroundLoaderContext* a1);
    /// 0x08cd4a68
    virtual UNK_RET vf_12(UNK_PARAMS);
};
