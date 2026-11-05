/// origin binary
#pragma once
#include "FirePlace/FireWorks/FIGameIniParser.h"
#include "FirePlace/FireWorks/FString.h"

/// size ?
/// vtable 0x0a3058d0
class FGameIniParser : public FIGameIniParser {
public:
    /// 0x08d88fc4
    FGameIniParser();
    /// 0x08d8904a
    /// deleting 0x08d89022
    virtual ~FGameIniParser();
    /// 0x08d893ce
    virtual void ClearCorruptedWarning();
    /// 0x08d893ae
    virtual void EnableAutoWriting(bool a1);
    /// 0x08d891c2
    virtual bool GetKeyValue(char const* a1, char const* a2, FStringA* a3, char const* a4, char const* a5);
    /// 0x08d8923c
    virtual bool GetKeyValue(char const* a1, char const* a2, int* a3, char const* a4, int a5);
    /// 0x08d892b6
    virtual bool GetKeyValue(char const* a1, char const* a2, unsigned int* a3, char const* a4, unsigned int a5);
    /// 0x08d89330
    virtual bool GetKeyValue(char const* a1, char const* a2, float* a3, char const* a4, float a5);
    /// 0x08d893d8
    virtual FileErr GetLastFileErr();
    /// 0x08d893ba
    virtual bool IsDirty() const;
    /// 0x08d891b8
    virtual bool IsInitialized();
    /// 0x08d893c4
    virtual bool WarnCorrupted() const;
    /// 0x08d8915c
    virtual bool Write();
    /// 0x08d8918a
    virtual bool WriteIfDirty();
    /// 0x08d890ae
    UNK_RET Initialize(char const* a1);
    /// 0x08d890f6
    UNK_RET Initialize(wchar_t const* a1);
};
