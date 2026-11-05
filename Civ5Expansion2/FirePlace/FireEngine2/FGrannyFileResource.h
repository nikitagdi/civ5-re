/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FResource.h"

class FResourceLoadHelper;
class FResourceMgr;

/// size ?
/// vtable 0x0a301be0
class FGrannyFileResource : public FResource {
public:
    /// 0x08cd5a92
    /// deleting 0x08cd5ad8
    virtual ~FGrannyFileResource();
    /// 0x08cd579c
    virtual UNK_RET GetData(void* a1, unsigned int a2);
    /// 0x08cd57f4
    virtual UNK_RET InUse();
    /// 0x08cd5308
    virtual UNK_RET Load(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
    /// 0x08cd5832
    virtual UNK_RET SetData(void* a1);
    /// 0x08cd587e
    virtual UNK_RET SetData(unsigned char* a1, unsigned int a2, bool a3);
    /// 0x08cd575c
    virtual UNK_RET Unload();
    /// 0x08cd5b00
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08cd5b06
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08cd59e0
    UNK_RET Register();
    /// 0x08cd5a14
    UNK_RET Create(FResourceMgr* a1, unsigned int a2);
    /// 0x08cd5a68
    UNK_RET UnRegister();
};
