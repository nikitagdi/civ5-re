/// origin guess
#pragma once
#include "Civ5/Src/App/CvGameCoreDLLUtil/CvDllInterfaces.h"
#include <Windows.h>
#include <list>
#include <memory>

/// size ?
/// vtable 0x0a2fc6f0
class CvDllDlcPackageInfoList : public ICvEnumerator {
public:
    /// 0x0876489c
    CvDllDlcPackageInfoList(std::list<_GUID, std::allocator<_GUID> > const& a1);
    /// 0x087649fc
    virtual void Destroy();
    /// 0x08764a6e
    virtual ICvUnknown * GetCurrent();
    /// 0x08764a24
    virtual bool MoveNext();
    /// 0x0876492a
    virtual void * QueryInterface(_GUID a1);
    /// 0x08764a62
    virtual void Reset();
    /// 0x087648fa
    ~CvDllDlcPackageInfoList();
    /// 0x0876498c
    UNK_RET IncrementReference();
    /// 0x08764998
    UNK_RET DecrementReference();
    /// 0x087649e0
    void operator delete(void* a1);
    /// 0x087649f4
    UNK_RET GetReferenceCount();
    /// 0x08764a10
    void* operator new(unsigned int a1);
};
