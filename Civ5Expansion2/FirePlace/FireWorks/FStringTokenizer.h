/// origin binary
#pragma once
#include "FirePlace/FireWorks/FJob.h"
#include "FirePlace/FireWorks/FString.h"

class _TAL_ID;

/// size ?
class FStringTokenizer {
public:
    /// 0x08dc1b50
    FStringTokenizer(char const* a1, char const* a2);
    /// 0x08dc1caa
    FStringTokenizer(char const* a1);
    /// 0x08dc1c9a
    UNK_RET Reset();
    /// 0x08dc1dcc
    UNK_RET Reset(FStringA const& a1, FStringA const& a2);
    /// 0x08dc1e3a
    UNK_RET HasMoreTokens() const;
    /// 0x08dc1e4e
    UNK_RET GetNext();
    /// 0x08dc1fb2
    UNK_RET is_eol() const;
    /// 0x08dc1fc6
    UNK_RET is_sep() const;
    /// 0x08dc1ffc
    UNK_RET is_quote() const;
    /// 0x08dc2010
    UNK_RET PeekNext();
};

/// 0x08dc2258
UNK_RET myFtol(float a1);
/// 0x08dc2320
UNK_RET FGetUserName(char* a1, unsigned int a2, unsigned int a3, bool a4);
/// 0x08dc255f
UNK_RET JobManager_AddJob(FJob* a1, FJobSet* a2, char const* a3, _TAL_ID* a4);
/// 0x08dc2795
UNK_RET JobManager_Initialize(unsigned int a1, unsigned int a2);
/// 0x08dc280e
UNK_RET JobManager_IsInitialized();
/// 0x08dc282b
unsigned int JobManager_GetNumberThreads();
/// 0x08dc283e
UNK_RET JobManager_Destroy();
/// 0x08dc2881
UNK_RET JobManager_GetAffinity();
extern "C" {
    /// 0x08dc252e
    UNK_RET FJobSetAddJobImpl(UNK_PARAMS);
    /// 0x08dc2484
    UNK_RET FJobSetInitImpl(UNK_PARAMS);
    /// 0x08dc2493
    UNK_RET FJobSetUninitImpl(UNK_PARAMS);
    /// 0x08dc24a7
    UNK_RET FJobSetWaitImpl(UNK_PARAMS);
}
