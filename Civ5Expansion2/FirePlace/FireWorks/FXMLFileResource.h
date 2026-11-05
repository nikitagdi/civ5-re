/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FResource.h"

class FResourceLoadHelper;
class FResourceMgr;

/// size ?
/// vtable 0x0a301f20
class FXMLFileResource : public FResource {
public:
    /// 0x08cdfde4
    /// deleting 0x08cdfdbc
    virtual ~FXMLFileResource();
    /// 0x08ce019a
    virtual UNK_RET GetData(void* a1, unsigned int a2);
    /// 0x08cdfe26
    virtual UNK_RET Load(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
    /// 0x08ce01c4
    virtual UNK_RET SetData(unsigned char* a1, unsigned int a2, bool a3);
    /// 0x08ce0322
    virtual UNK_RET SetData(void* a1);
    /// 0x08ce0166
    virtual UNK_RET Unload();
    /// 0x08ce032e
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08ce0334
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08ce0270
    UNK_RET Register();
    /// 0x08ce02a4
    UNK_RET Create(FResourceMgr* a1, unsigned int a2);
    /// 0x08ce02f8
    UNK_RET UnRegister();
};
