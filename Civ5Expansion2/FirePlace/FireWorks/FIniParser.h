/// origin binary
#pragma once
#include "FirePlace/FireWorks/FArray.h"
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FMemoryStream.h"
#include "FirePlace/FireWorks/FString.h"

class FTextFile;
enum KEYINFO : int;

/// size ?
class FIniParser {
public:
    /// 0x08d89438
    FIniParser();
    /// 0x08d89508
    FIniParser(char const* a1);
    /// 0x08d89776
    FIniParser(FMemoryStream& a1);
    /// 0x08d899c4
    FIniParser(FTextFile& a1);
    /// 0x08d89f96
    ~FIniParser();
    /// 0x08d894c0
    UNK_RET RemoveAllGroups();
    /// 0x08d89600
    UNK_RET Initialize(char const* a1);
    /// 0x08d8986e
    UNK_RET Initialize(FMemoryStream& a1);
    /// 0x08d89abc
    UNK_RET Initialize(FTextFile& a1);
    /// 0x08d8a030
    UNK_RET ClearHeaderComments();
    /// 0x08d8a0a8
    UNK_RET Initialize(wchar_t const* a1);
    /// 0x08d8a286
    UNK_RET AddHeaderComment(char const* a1);
    /// 0x08d8a3aa
    UNK_RET SetGroupKey(char const* a1);
    /// 0x08d8a416
    UNK_RET AddGroupKey(char const* a1);
    /// 0x08d8a50e
    UNK_RET RemoveGroupKey(char const* a1);
    /// 0x08d8a64e
    UNK_RET GetNumGroups() const;
    /// 0x08d8a65c
    UNK_RET GetGroupName(int a1, FStringA* a2) const;
    /// 0x08d8a6c0
    UNK_RET GetKeyValue(char const* a1, bool* a2) const;
    /// 0x08d8a704
    UNK_RET GetKeyValue(char const* a1, int* a2) const;
    /// 0x08d8a788
    UNK_RET GetKeyValue(char const* a1, short* a2) const;
    /// 0x08d8a7cc
    UNK_RET GetKeyValue(char const* a1, unsigned int* a2) const;
    /// 0x08d8a850
    UNK_RET GetKeyValue(char const* a1, float* a2) const;
    /// 0x08d8a8d8
    UNK_RET GetKeyValue(char const* a1, char* a2) const;
    /// 0x08d8a966
    UNK_RET GetKeyValue(char const* a1, FStringA* a2) const;
    /// 0x08d8aa02
    UNK_RET GetKeyValue(char const* a1, bool* a2, bool a3, char const* a4);
    /// 0x08d8aa82
    UNK_RET AddKeyValue(char const* a1, bool const* a2, char const* a3);
    /// 0x08d8aacc
    UNK_RET GetKeyValue(char const* a1, short* a2, short a3, char const* a4);
    /// 0x08d8ab50
    UNK_RET AddKeyValue(char const* a1, short const* a2, char const* a3);
    /// 0x08d8ab9a
    UNK_RET GetKeyValue(char const* a1, int* a2, int a3, char const* a4);
    /// 0x08d8ac0a
    UNK_RET AddKeyValue(char const* a1, int const* a2, char const* a3);
    /// 0x08d8ad40
    UNK_RET GetKeyValue(char const* a1, unsigned int* a2, unsigned int a3, char const* a4);
    /// 0x08d8adb2
    UNK_RET AddKeyValue(char const* a1, unsigned int const* a2, char const* a3);
    /// 0x08d8adfa
    UNK_RET GetKeyValue(char const* a1, float* a2, float a3, char const* a4);
    /// 0x08d8ae72
    UNK_RET AddKeyValue(char const* a1, float const* a2, char const* a3);
    /// 0x08d8afae
    UNK_RET GetKeyValue(char const* a1, char* a2, char const* a3, char const* a4);
    /// 0x08d8b022
    UNK_RET AddKeyValue(char const* a1, char const* a2, char const* a3);
    /// 0x08d8b170
    UNK_RET GetKeyValue(char const* a1, FStringA* a2, char const* a3, char const* a4);
    /// 0x08d8b1fc
    UNK_RET SetKeyValue(char const* a1, bool const* a2, char const* a3);
    /// 0x08d8b246
    UNK_RET SetKeyValue(char const* a1, int const* a2, char const* a3);
    /// 0x08d8b376
    UNK_RET SetKeyValue(char const* a1, short const* a2, char const* a3);
    /// 0x08d8b3c0
    UNK_RET SetKeyValue(char const* a1, unsigned int const* a2, char const* a3);
    /// 0x08d8b408
    UNK_RET SetKeyValue(char const* a1, float const* a2, char const* a3);
    /// 0x08d8b544
    UNK_RET SetKeyValue(char const* a1, char const* a2, char const* a3);
    /// 0x08d8b6c8
    UNK_RET RemoveKey(char const* a1);
    /// 0x08d8b750
    UNK_RET GetWholeGroup(char const* a1, FArray<KEYINFO*, (eMPoolType)0>*& a2);
    /// 0x08d8b7a0
    UNK_RET GetNumKeys() const;
    /// 0x08d8b7b8
    UNK_RET GetGroupName(int a1, char* a2) const;
    /// 0x08d8b80e
    UNK_RET WriteParser(char const* a1);
    /// 0x08d8b950
    UNK_RET WriteParser(FTextFile& a1);
    /// 0x08d8bcf0
    UNK_RET WriteParser(wchar_t const* a1);
    /// 0x08d8be32
    UNK_RET WriteParser(FMemoryStream& a1);
};

/// 0x08d8a21d
UNK_RET ReadLineX(FTextFile& a1, FStringA& a2);
/// 0x08d8a250
UNK_RET WriteLineX(FTextFile& a1, FStringA& a2);
