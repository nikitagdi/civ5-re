/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FString.h"
#include <Windows.h>
#include <list>
#include <memory>

class cvContentPackageIDList;

/// size ?
class cvContentPackageIDList {
public:
    /// 0x0873e17c
    cvContentPackageIDList();
    /// 0x0873e18e
    UNK_RET Add(_GUID const& a1);
    /// 0x0873e1f8
    UNK_RET Contains(_GUID const& a1) const;
    /// 0x0873e24c
    UNK_RET Add(std::list<_GUID, std::allocator<_GUID> > const& a1);
    /// 0x0873e2e6
    UNK_RET Remove(_GUID const& a1);
    /// 0x0873e350
    UNK_RET Remove(std::list<_GUID, std::allocator<_GUID> > const& a1, bool a2);
    /// 0x0873e45a
    UNK_RET operator==(cvContentPackageIDList const& a1) const;
    /// 0x0873e4b2
    UNK_RET operator!=(cvContentPackageIDList const& a1) const;
    /// 0x0873e4e0
    UNK_RET operator=(cvContentPackageIDList const& a1);
};

namespace cvContentPackageManager {
/// 0x0873e578
UNK_RET ExtractGUID(FStringA const& a1, _GUID& a2, unsigned int* a3);
/// 0x0873e65c
UNK_RET ExtractGUID(FStringA const& a1, FFastVector<unsigned char, false, 0u, 0u, BaseVector<unsigned char, false>::FDefaultFastVectorAllocator>& a2);
/// 0x0873e6d2
UNK_RET ExtractGUID(FStringA const& a1, cvContentPackageIDList& a2);
}

namespace cvPackageKeyVector {
/// 0x0873e528
UNK_RET contains(char const* a1);
}
