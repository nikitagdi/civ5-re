/// origin guess
#pragma once
#include "Civ5/Src/CvGameDatabase/Database.h"
#include "Civ5/Src/CvGameDatabase/DatabaseResults.h"
#include <functional>
#include <list>
#include <map>
#include <memory>
#include <utility>

class FXmlDocument;
class FXmlTraverser;
template <class T0> class NullableT;

/// size ?
class cvGameDatabaseUnitLibraryLoader {
public:
    class cvUnitLibraryUnitEntry;
    class cvUnitLibraryUnitMemberEntry;
    /// size ?
    class cvUnitLibraryUnitEntry {
    public:
        /// 0x0876a786
        cvUnitLibraryUnitEntry();
        /// 0x08767b70
        UNK_RET LoadEntry(FXmlTraverser const& a1);
        /// 0x0876816c
        UNK_RET GetType() const;
        /// 0x08768172
        UNK_RET GetNumUnitMembers();
        /// 0x0876817e
        UNK_RET GetUnitMemberType(unsigned int a1);
        /// 0x0876819e
        UNK_RET GetUnitMemberCount(unsigned int a1);
    };
    /// size ?
    class cvUnitLibraryUnitMemberEntry {
    public:
        /// 0x0876a82a
        cvUnitLibraryUnitMemberEntry();
        /// 0x0876a87a
        ~cvUnitLibraryUnitMemberEntry();
        /// 0x0876917c
        UNK_RET LoadEntry(FXmlTraverser const& a1, bool a2, std::map<unsigned int, cvGameDatabaseUnitLibraryLoader::cvUnitLibraryUnitMemberEntry const*, std::less<unsigned int>, std::allocator<std::pair<unsigned int const, cvGameDatabaseUnitLibraryLoader::cvUnitLibraryUnitMemberEntry const*> > > const* a3);
        /// 0x0876a87c
        UNK_RET LoadWeaponEntry(FXmlTraverser const& a1, int a2, int a3);
    };
    /// 0x08767558
    cvGameDatabaseUnitLibraryLoader(Database::Connection& a1);
    /// 0x087676e8
    UNK_RET LoadUnitEntries(std::list<FXmlDocument, std::allocator<FXmlDocument> > const& a1);
    /// 0x087681c4
    UNK_RET LoadUnitMemberEntries(std::list<FXmlDocument, std::allocator<FXmlDocument> > const& a1);
};

/// 0x0876b0af BindNullableT<bool>
/// 0x0876b066 BindNullableT<float>
/// 0x0876afa6 BindNullableT<int>
template <class T0>
void BindNullableT(Database::Results& a1, unsigned int a2, NullableT<T0> const& a3);
