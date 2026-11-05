/// origin guess
#pragma once
#include "FirePlace/FireWorks/FString.h"
#include <steam.h>

class FNetSessionIFace;

/// size ?
/// vtable 0x0a304ea0
class FPendingConnection {
public:
    /// 0x08d657f4
    FPendingConnection(FNetSessionIFace* a1, int a2, bool a3, FStringA a4, FStringA a5, char const* a6, unsigned int a7);
    /// 0x08d659ea
    FPendingConnection(FNetSessionIFace* a1, int a2, bool a3, CSteamID a4, unsigned long long a5, char const* a6, unsigned int a7);
    /// 0x08d65bae
    /// deleting 0x08d65b86
    virtual ~FPendingConnection();
    /// 0x08d668fc
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08d66904
    virtual UNK_RET vf_03(UNK_PARAMS);
    /// 0x08d6690c
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// 0x08d65980
    UNK_RET SetData(char const* a1, unsigned int a2);
};
