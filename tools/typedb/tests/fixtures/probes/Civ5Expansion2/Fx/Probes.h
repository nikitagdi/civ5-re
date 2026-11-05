/// origin guess
#pragma once

// Probes must name entities the way clang resolves them, not as spelled at the declaration.

// An out-of-class explicit specialization of a member template: its parameter types are spelled in class scope.
/// size 0x1
class FEndian {
public:
    enum EEndianness {
        ENDIAN_LITTLE,
        ENDIAN_BIG
    };

    /// inline
    template <class T> static void SwapEndian(T* kData, EEndianness kInputEndian);
    /// inline
    template <class T> int Count(T v, EEndianness e) const;
};

/// inline
template <> inline int FEndian::Count(char v, EEndianness e) const { return 1; }

/// inline
template <class T> inline void FEndian::SwapEndian(T* kData, EEndianness kInputEndian) {}

/// inline
template <> inline void FEndian::SwapEndian(short* kData, EEndianness kInputEndian) {}

namespace FLua {
namespace Details {
    // Explicit specializations of a namespace function template: `&DescribeType` alone is ambiguous.
    /// inline
    template <class T> static const char* DescribeType() { return "?"; }
    /// inline
    template <> const char* DescribeType<bool>() { return "bool"; }
    /// inline
    template <> const char* DescribeType<const bool>() { return "const bool"; }
}
}

// An out-of-class definition of a static member of a class template.
/// size 0x1
template <typename T, unsigned int uBlockCountT, unsigned int pool_type = 0>
class FBlockAllocator {
public:
    /// inline
    void* Pop() { return s_pFreeStack; }

    /// 0x0a000000 FBlockAllocator<int, 4u, 0u>
    static void* s_pFreeStack;
};

/// 0x0a000000 FBlockAllocator<int, 4u, 0u>
/// 0x0a000004 FBlockAllocator<char, 8u, 0u>
template <typename T, unsigned int uBlockCountT, unsigned int pool_type>
void* FBlockAllocator<T, uBlockCountT, pool_type>::s_pFreeStack = 0;

/// inline
inline void* UseAllocator() { return FBlockAllocator<int, 4u>().Pop(); }
