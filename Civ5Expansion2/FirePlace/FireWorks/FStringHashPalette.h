/// origin guess
#pragma once
#include "FirePlace/FireWorks/FDataStream.h"
#include "FirePlace/FireWorks/FString.h"

class FFile;
class FStringArray;
namespace FStringTable {
enum ESupportedLanguage : int;
}

/// size ?
class FStringHashPalette {
public:
    /// 0x08dc5508
    FStringHashPalette(int a1);
    /// 0x08dc554a
    ~FStringHashPalette();
    /// 0x08dc55b4
    UNK_RET Clear();
    /// 0x08dc5652
    UNK_RET Add(char const* a1);
    /// 0x08dc5740
    UNK_RET AllocateString(int a1);
    /// 0x08dc57c8
    UNK_RET Add(FStringA const& a1);
    /// 0x08dc57f4
    UNK_RET Add(unsigned int a1, char const* a2);
    /// 0x08dc58ac
    UNK_RET Add(unsigned int a1, FStringA const& a2);
    /// 0x08dc58e0
    UNK_RET GetCount() const;
    /// 0x08dc591a
    UNK_RET Find(unsigned int a1) const;
    /// 0x08dc598a
    UNK_RET FindStr(unsigned int a1) const;
    /// 0x08dc59fa
    UNK_RET GetUsage(unsigned int* a1, unsigned int* a2);
};

namespace FStringTable {
/// 0x08dc5f30
UNK_RET Empty();
/// 0x08dc60fc
UNK_RET Init(FStringA& a1, bool a2, bool a3);
/// 0x08dc62b6
UNK_RET OpenStringTableStream(char const* a1);
/// 0x08dc6346
UNK_RET ReadFileHeader(FDataStream& a1);
/// 0x08dc6416
UNK_RET ReadHashTable(FDataStream& a1);
/// 0x08dc6540
UNK_RET FillProximityCache(unsigned int a1);
/// 0x08dc6646
UNK_RET CloseStringTableStream();
/// 0x08dc6682
UNK_RET GetHashIndexFromID(unsigned int a1, unsigned int& a2);
/// 0x08dc66dc
UNK_RET GetString(unsigned int a1, FStringA& a2);
/// 0x08dc67aa
UNK_RET GetStringFromProximityCache(unsigned int a1, FStringA& a2);
/// 0x08dc685e
UNK_RET GetStringFromDisk(unsigned int a1, FStringA& a2);
/// 0x08dc698e
UNK_RET GetString(unsigned int a1, FStringW& a2);
/// 0x08dc6b92
UNK_RET GetStringFromProximityCache(unsigned int a1, FStringW& a2);
/// 0x08dc6c38
UNK_RET GetStringFromDisk(unsigned int a1, FStringW& a2);
/// 0x08dc6d5a
UNK_RET GetLanguageFromString(char const* a1);
/// 0x08dc6db2
UNK_RET GetLanguageCode(FStringTable::ESupportedLanguage a1);
/// 0x08dc6e56
UNK_RET SuspendAll();
/// 0x08dc6eca
UNK_RET ResumeAll();
/// 0x08dc6f2a
UNK_RET Build(FStringA& a1, FStringA& a2, FStringTable::ESupportedLanguage a3, FStringA* a4, FStringTable::ESupportedLanguage a5);
/// 0x08dc72c2
UNK_RET GenerateHashTable(FStringA* a1, FStringTable::ESupportedLanguage a2);
/// 0x08dc776c
UNK_RET CreateStringTable(FStringA& a1, FStringArray& a2);
/// 0x08dc7938
UNK_RET WriteFileHeader(FFile& a1);
/// 0x08dc7cce
UNK_RET WriteHashTable(FFile& a1);
/// 0x08dc7e12
UNK_RET WriteStrings(FFile& a1, FStringArray& a2);
}
