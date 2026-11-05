/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"
#include <Windows.h>
#include <utility>

class FAllocation;
class FSmallObjectAllocatorInitParams;

/// size ?
class FSmallObjectAllocator {
public:
    /// 0x08dbeb76
    FSmallObjectAllocator(FSmallObjectAllocatorInitParams const& a1);
    /// 0x08dbebdc
    UNK_RET Init();
    /// 0x08dbed3e
    UNK_RET Allocate(unsigned int a1);
    /// 0x08dbed9a
    UNK_RET IsMemorySmallBlock(void const* a1, unsigned int& a2);
    /// 0x08dbedd2
    UNK_RET Deallocate(void* a1, unsigned int a2);
    /// 0x08dbee3a
    UNK_RET GetFixedAllocatorForSize(unsigned int a1);
    /// 0x08dbee9c
    UNK_RET DumpStats(FStringA& a1);
    /// 0x08dbf49c
    UNK_RET DumpPageStats();
    /// 0x08dbf49e
    UNK_RET GetStats(unsigned int* a1, unsigned int* a2, unsigned int* a3);
    /// 0x08dbf522
    UNK_RET Validate(unsigned int a1);
};

/// 0x08dbee89
UNK_RET SortByAddress(std::pair<void const*, FAllocation> const& a1, std::pair<void const*, FAllocation> const& a2);
/// 0x08dbf998
UNK_RET LogAssert(char const* a1);
/// 0x08dc06ec
UNK_RET ShowLastError(char const* a1, char const* a2, int a3);
/// 0x08dc091a
UNK_RET ShowLastError(wchar_t const* a1, wchar_t const* a2, int a3);
/// 0x08dc0b44
UNK_RET _CRTUnitTestReportHook(int a1, char* a2, int* a3);
/// 0x08dc0b93
UNK_RET _InvalidParameterHandler(wchar_t const* a1, wchar_t const* a2, wchar_t const* a3, unsigned int a4, unsigned int a5);
/// 0x08dc0bce
UNK_RET GetErrorModeFxs();
/// 0x08dc0be3
UNK_RET DisableCRTErrorDialogs();

namespace FEventTrace {
/// 0x08dc0c2c
UNK_RET Initialize(_GUID a1);
/// 0x08dc0c32
UNK_RET Uninitialize();
/// 0x08dc0c38
UNK_RET WriteEvent(wchar_t const* a1, long long a2, int a3);
/// 0x08dc0c39
UNK_RET WriteEventMsg(char* a1);
}

namespace FMD5 {
/// 0x08dc0d96
UNK_RET Update(unsigned char const* a1, unsigned int a2);
/// 0x08dc0e82
UNK_RET Finalize();
/// 0x08dc1208
UNK_RET Init();
/// 0x08dc1236
UNK_RET Transform(unsigned char const* a1);
/// 0x08dc1994
UNK_RET Encode(unsigned char* a1, unsigned int const* a2, unsigned int a3);
/// 0x08dc19d0
UNK_RET Decode(unsigned int* a1, unsigned char const* a2, unsigned int a3);
}
