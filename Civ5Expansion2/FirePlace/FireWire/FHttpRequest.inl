/// origin binary
#pragma once

class cvLuaHttpRequest;

/// size ?
/// vtable 0x0a3005c0 FHttpRequest<cvLuaHttpRequest>
template <class T0>
class FHttpRequest {
public:
    class RequestVerb;
    /// base 0x08b532d4 FHttpRequest<cvLuaHttpRequest>
    /// deleting 0x08b53324 FHttpRequest<cvLuaHttpRequest>
    virtual ~FHttpRequest();
    /// 0x08b525c0 FHttpRequest<cvLuaHttpRequest>
    UNK_RET Connect(char const* a1, unsigned int a2, int a3);
    /// 0x08b5268e FHttpRequest<cvLuaHttpRequest>
    UNK_RET Request(FHttpRequest<T0>::RequestVerb a1, char const* a2, unsigned int a3);
    /// 0x08b527a0 FHttpRequest<cvLuaHttpRequest>
    UNK_RET AddHeaders(char const* a1, unsigned int a2);
    /// 0x08b53156 FHttpRequest<cvLuaHttpRequest>
    UNK_RET WinHTTPCallback(void* a1, unsigned long a2, unsigned long a3, void* a4, unsigned long a5);
    /// 0x08b531a8 FHttpRequest<cvLuaHttpRequest>
    UNK_RET WinHTTPLocalCallback(unsigned long a1, void* a2, unsigned long a3);
    static UNK_DATA sm_kSession[0x8];
};
/// 0x0ccce164 FHttpRequest<cvLuaHttpRequest>
template <class T0>
UNK_DATA FHttpRequest<T0>::sm_kSession[0x8];

extern "C" {
    /// 0x0ccce184
    extern UNK_DATA sgLuaAllocationMap[0xc];
    /// 0x0ccce178
    extern UNK_DATA sgLuaCallstacks[0xc];
}
