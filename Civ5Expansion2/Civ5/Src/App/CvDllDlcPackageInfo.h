/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include <Windows.h>

/// size ?
/// vtable 0x0a2fc6d0
class CvDllDlcPackageInfo : public ICvDlcPackageInfo1 {
public:
    /// 0x0876474c
    CvDllDlcPackageInfo(_GUID const& a1);
    /// 0x08764844
    virtual void Destroy();
    /// 0x0876487e
    virtual GUID GetPackageID();
    /// 0x08764788
    virtual void * QueryInterface(_GUID a1);
    /// 0x08764786
    ~CvDllDlcPackageInfo();
    /// 0x087647ea
    UNK_RET IncrementReference();
    /// 0x087647f6
    UNK_RET DecrementReference();
    /// 0x08764828
    void operator delete(void* a1);
    /// 0x0876483c
    UNK_RET GetReferenceCount();
    /// 0x0876486a
    void* operator new(unsigned int a1);
};
