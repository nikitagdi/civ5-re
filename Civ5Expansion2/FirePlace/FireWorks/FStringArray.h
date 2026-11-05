/// origin binary
#pragma once
#include "FirePlace/FireWorks/FArray.h"
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FString.h"
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

class FTextFile;
class FXmlDocument;
class FXmlTraverser;
namespace cvLibraryManager {
class UniqueEntry;
}

/// size ?
/// vtable 0x0a2fc330
class FStringArray : public FArray<FStringA, (eMPoolType)3> {
public:
    class CompareAscendingSingleLines;
    class CompareFileNamesAscending;
    class CompareFileNamesAscendingSingleLines;
    class CompareFloatsAscending;
    class CompareFloatsAscendingSingleLines;
    class CompareNoCaseAscendingSingleLines;
    class CompareNumbersAscendingSingleLines;
    /// size ?
    class CompareAscendingSingleLines {
    public:
        /// 0x08dd52f0
        UNK_RET operator()(FStringA const& a1, FStringA const& a2);
    };
    /// size ?
    class CompareFileNamesAscending {
    public:
        /// 0x08dd5004
        UNK_RET operator()(FStringA const& a1, FStringA const& a2);
    };
    /// size ?
    class CompareFileNamesAscendingSingleLines {
    public:
        /// 0x08dd5e72
        UNK_RET operator()(FStringA const& a1, FStringA const& a2);
    };
    /// size ?
    class CompareFloatsAscending {
    public:
        /// 0x08dd49ec
        UNK_RET operator()(FStringA const& a1, FStringA const& a2);
    };
    /// size ?
    class CompareFloatsAscendingSingleLines {
    public:
        /// 0x08dd5b9c
        UNK_RET operator()(FStringA const& a1, FStringA const& a2);
    };
    /// size ?
    class CompareNoCaseAscendingSingleLines {
    public:
        /// 0x08dd55c6
        UNK_RET operator()(FStringA const& a1, FStringA const& a2);
    };
    /// size ?
    class CompareNumbersAscendingSingleLines {
    public:
        /// 0x08dd58a0
        UNK_RET operator()(FStringA const& a1, FStringA const& a2);
    };
    /// 0x08745bce
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08745b3c
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dd419c
    UNK_RET FillFromString(FStringA const& a1, char a2, bool a3);
    /// 0x08dd43d4
    UNK_RET FillFromString(FStringA const& a1, char const* a2, bool a3);
    /// 0x08dd4606
    UNK_RET FillFromFile(FTextFile& a1, bool a2);
    /// 0x08dd4768
    UNK_RET FillFromFile(char const* a1, bool a2);
    /// 0x08dd48aa
    UNK_RET FillFromFile(wchar_t const* a1, bool a2);
};

/// 0x08745f16 GetGUIDSegment<unsigned char>
/// 0x08745dbf GetGUIDSegment<unsigned long>
/// 0x08745e6a GetGUIDSegment<unsigned short>
template <class T0>
bool GetGUIDSegment(FStringA const& a1, unsigned int* a2, T0& a3);

namespace CvCursor3D {
/// 0x08747994
UNK_RET LoadCursorFromFileA(char const* a1);
}

namespace cvLibraryManager {
/// 0x087472ac
UNK_RET GetTagCount(std::list<FXmlDocument, std::allocator<FXmlDocument> > const& a1, char const* a2, char const* a3);
/// 0x087472f8
UNK_RET GetTagCount(FXmlDocument const& a1, char const* a2, char const* a3);
/// 0x087473a0
UNK_RET GetXmlTraverser(FXmlTraverser* a1, FXmlDocument const& a2, char const* a3, char const* a4);
/// 0x08747480
UNK_RET GetUniqueTags(std::list<FXmlDocument, std::allocator<FXmlDocument> > const& a1, std::map<unsigned int, cvLibraryManager::UniqueEntry, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, cvLibraryManager::UniqueEntry> > >* a2, char const* a3, char const* a4, char const* a5);
/// 0x087474ec
UNK_RET GetUniqueTags(FXmlDocument const& a1, int a2, std::map<unsigned int, cvLibraryManager::UniqueEntry, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, cvLibraryManager::UniqueEntry> > >* a3, char const* a4, char const* a5, char const* a6);
/// 0x08747744
UNK_RET LoadXmlDocuments(std::vector<std::string, std::allocator<std::string> > const& a1, std::list<FXmlDocument, std::allocator<FXmlDocument> >* a2);
}
