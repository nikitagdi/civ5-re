/// origin binary
#pragma once
#include "Civ5/Src/App/BuilderContext_Base.h"
#include "FirePlace/FireWorks/FArray.h"
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastList.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FFileIO.h"
#include "FirePlace/FireWorks/FIFile.h"
#include "FirePlace/FireWorks/FMemHooks.h"
#include "FirePlace/FireWorks/FString.h"
#include <granny.h>

class ConvertSourceNamesToEventCodes;
class FFileCatalogEntry;
class FFileManager;
class FPakFile;
class FStringArray;
template <class T0, class T1> class FFastTree;
namespace FireGrafix {
class FGXModelTreeNode;
}

/// size ?
/// vtable 0x0a2f8eb0
class FFile : public FFileIO {
public:
    /// 0x086691e0
    /// deleting 0x0866918e
    virtual ~FFile();
    /// 0x08d978a8
    virtual FileErr Close();
    /// 0x08da936c
    virtual uint GetLength() const;
    /// 0x08da9360
    virtual byte * GetViewCaret() const;
    /// 0x08da9364
    virtual byte * GetViewEnd() const;
    /// 0x08da9368
    virtual byte * GetViewPtr() const;
    /// 0x08da935c
    virtual bool IsMapped() const;
    /// 0x08da9330
    virtual bool IsOpen() const;
    /// 0x08d97946
    virtual FileErr Open(char const* a1, FIFile::OpenMode a2, unsigned long a3, unsigned int a4);
    /// 0x08d9806e
    virtual FileErr Open(wchar_t const* a1, FIFile::OpenMode a2, unsigned long a3, unsigned int a4);
    /// 0x08d98c68
    virtual uint Read(void* a1, unsigned int a2);
    /// 0x08da9386
    virtual uint Seek(long a1, FIFile::SeekMode a2);
    /// 0x08d97bb2
    UNK_RET IsFileInManagerPath(char const* a1, bool a2, FFileManager** a3);
    /// 0x08d97cba
    UNK_RET FindFile(char const* a1, FPakFile** a2, bool a3, FFileManager** a4);
    /// 0x08d97ea0
    UNK_RET OpenCatalogEntry(FFileCatalogEntry* a1, FPakFile* a2, FIFile::OpenMode a3, unsigned long a4, unsigned int a5);
    /// 0x08d9836e
    UNK_RET IsFileInManagerPath(wchar_t const* a1, bool a2, FFileManager** a3);
    /// 0x08d98410
    UNK_RET FindFile(wchar_t const* a1, FPakFile** a2, bool a3, FFileManager** a4);
    /// 0x08d985dc
    UNK_RET FindPakFile(FFileCatalogEntry* a1);
    /// 0x08d98694
    UNK_RET Exist(char const* a1);
    /// 0x08d9874a
    UNK_RET Exist(wchar_t const* a1);
    /// 0x08d98864
    UNK_RET GetLength(char const* a1);
    /// 0x08d988e2
    UNK_RET GetLength(wchar_t const* a1);
    /// 0x08d989c0
    UNK_RET ReadAll(char const* a1, unsigned long a2, FFileIO::Allocator& a3);
    /// 0x08d98cbe
    UNK_RET ReadAll(wchar_t const* a1, unsigned long a2, FFileIO::Allocator& a3);
    /// 0x08d98fa4
    UNK_RET Copy(char const* a1, char const* a2, bool a3);
    /// 0x08d99452
    UNK_RET Copy(wchar_t const* a1, wchar_t const* a2, bool a3);
    /// 0x08d998c0
    static FFileTimestamp GetLastWriteTime(char const* a1);
    /// 0x08d999d4
    UNK_RET GetLastWriteTime(wchar_t const* a1);
    /// 0x08d99ae8
    UNK_RET FindPakFile(char const* a1);
    /// 0x08d99ba0
    UNK_RET InitFileManager(char const* a1, char const* a2, void* a3);
    /// 0x08d99c1a
    UNK_RET UninitFileManager();
    /// 0x08d99c62
    UNK_RET RemoveSearchPaths(bool a1);
    /// 0x08d99cf4
    UNK_RET Enumerate(char const* a1, FStringArray& a2, char const* a3, unsigned int a4);
    /// 0x08d9a088
    UNK_RET EnumerateFolder(FStringA const& a1, FStringArray& a2, FStringA const& a3, unsigned int a4);
    /// 0x08d9a60e
    UNK_RET Enumerate(wchar_t const* a1, FArray<FStringW, (eMPoolType)2>& a2, wchar_t const* a3, unsigned int a4);
    /// 0x08d9a964
    UNK_RET EnumerateFolder(FStringW const& a1, FArray<FStringW, (eMPoolType)2>& a2, FStringW const& a3, unsigned int a4);
    /// 0x08d9aec6
    UNK_RET Enumerate(FStringArray& a1, char const* a2);
    /// 0x08d9af98
    UNK_RET Enumerate(FArray<FFileCatalogEntry const*, (eMPoolType)35>& a1, char const* a2);
    /// 0x08d9b06a
    UNK_RET Enumerate(char const* a1, FArray<FFileCatalogEntry const*, (eMPoolType)35>& a2);
    /// 0x08d9b198
    UNK_RET Enumerate(char const* a1, FFastList<FStringA, 35u, 0u>& a2, char const* a3, unsigned int a4);
    /// 0x08d9b52c
    UNK_RET EnumerateFolder(FStringA const& a1, FFastList<FStringA, 35u, 0u>& a2, FStringA const& a3, unsigned int a4);
    /// 0x08d9ba9c
    UNK_RET Enumerate(wchar_t const* a1, FFastList<FStringW, 35u, 0u>& a2, wchar_t const* a3, unsigned int a4);
    /// 0x08d9bdf2
    UNK_RET EnumerateFolder(FStringW const& a1, FFastList<FStringW, 35u, 0u>& a2, FStringW const& a3, unsigned int a4);
    /// 0x08d9c354
    UNK_RET AddSearchPath(char const* a1, char const* a2);
    /// 0x08d9c3b8
    UNK_RET AddSearchPath(FFileManager* a1);
    /// 0x08d9c424
    UNK_RET SearchPathPriorityChanged(FFileManager* a1);
    /// 0x08d9c4fa
    UNK_RET RemoveSearchPath(FFileManager const* a1, bool a2);
    /// 0x08d9c57e
    UNK_RET RemoveLastSearchPath();
    /// 0x08d9c5c4
    UNK_RET GetSearchPathCount();
    /// 0x08d9c5e2
    UNK_RET GetSearchPath(unsigned int a1);
    /// 0x08da934a
    UNK_RET ValidPakFileHandle() const;
    /// 0x0ff8baa0
    static UNK_DATA ms_kFileSystemLock[0x18];
    /// 0x0ff8ba9c
    static UNK_DATA ms_papkSearchPaths[0x4];
    /// 0x0ff8bab8
    static UNK_DATA ms_pkFileManager[0x4];
    /// 0x0ff8babc
    static UNK_DATA ms_uiNumOpenFiles[0x4];
};

/// 0x0866df5a FGrannyVariantIterator<BuilderContext_Base::BuilderContextGrannyExtraDataReader::FGXMeshStateVariantVisitor>
/// 0x08cd47b8 FGrannyVariantIterator<ConvertSourceNamesToEventCodes>
template <class T0>
void FGrannyVariantIterator(granny_variant& a1, T0& a2);
/// 0x0866e316 FTREE_insert_child_at_end<FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >, FireGrafix::FGXModelTreeNode>
template <class T0, class T1>
unsigned int FTREE_insert_child_at_end(T0* a1, unsigned int a2, T1 const& a3);
