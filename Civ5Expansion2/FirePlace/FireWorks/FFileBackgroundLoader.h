/// origin guess
#pragma once
#include "FirePlace/FireWorks/FThread.h"
#include <memory>
#include <vector>

class FBackgroundDataLoader;
class FFileBackgroundLoader;
class FFileLoaderFactory;

/// size ?
/// vtable 0x0a305a90
class FFileBackgroundLoader : public FThread {
public:
    class FFileBackgroundContext;
    /// 0x08d94fd2
    /// deleting 0x08d94faa
    virtual ~FFileBackgroundLoader();
    /// 0x08d9619a
    virtual UNK_RET Initial();
    /// 0x08d95fd6
    virtual UNK_RET Run();
    /// 0x08d95128
    UNK_RET Reset();
    /// 0x08d95150
    UNK_RET DestroyAllLoaders();
    /// 0x08d951a4
    UNK_RET DestroyAllContexts();
    /// 0x08d9522e
    UNK_RET GetFileLoader(void* a1, unsigned int a2);
    /// 0x08d9529a
    UNK_RET FindContext(void* a1, bool a2, bool a3);
    /// 0x08d953b2
    UNK_RET AddContext(void* a1);
    /// 0x08d9540c
    UNK_RET AddFile(void* a1, char const* a2, unsigned int a3, bool a4);
    /// 0x08d954ae
    UNK_RET DoAddFile(char const* a1, FFileBackgroundLoader::FFileBackgroundContext* a2, unsigned int a3, void* a4, FBackgroundDataLoader* a5, bool a6);
    /// 0x08d9555c
    UNK_RET AddFile(void* a1, char const* a2, FBackgroundDataLoader* a3, unsigned int a4, bool a5);
    /// 0x08d95614
    UNK_RET AppendFile(void* a1, char const* a2, bool a3);
    /// 0x08d9569a
    UNK_RET AppendFile(void* a1, char const* a2, FBackgroundDataLoader* a3, bool a4);
    /// 0x08d9572c
    UNK_RET RemoveAllFiles(void* a1);
    /// 0x08d9578a
    UNK_RET RemoveFile(void* a1, unsigned int a2);
    /// 0x08d95808
    UNK_RET GetNumFiles(void* a1);
    /// 0x08d95860
    UNK_RET FilesLoaded(void* a1, bool a2);
    /// 0x08d958ac
    UNK_RET StartLoading(void* a1);
    /// 0x08d95968
    UNK_RET IsStartedLoading(void* a1);
    /// 0x08d959c4
    UNK_RET GetBufferSize(void* a1, unsigned int a2);
    /// 0x08d95a4e
    UNK_RET CopyPointer(void* a1, unsigned int a2, void** a3);
    /// 0x08d95b2c
    UNK_RET DestroyPointer(void* a1);
    /// 0x08d95ba8
    UNK_RET RemoveFromLoadedContextsListIfExists(FFileBackgroundLoader::FFileBackgroundContext* a1);
    /// 0x08d95c12
    UNK_RET DestroyLoaders(void* a1);
    /// 0x08d95c78
    UNK_RET MarkContextForDeletion(void* a1, bool a2);
    /// 0x08d95cc4
    UNK_RET DoMarkContextForDeletion(FFileBackgroundLoader::FFileBackgroundContext* a1);
    /// 0x08d95d56
    UNK_RET MarkContextForDeletionIfDoneLoading(void* a1);
    /// 0x08d95db4
    UNK_RET TryToMarkContextForDeletion(void* a1, bool a2, bool* a3);
    /// 0x08d95e48
    UNK_RET IsMarkedForDeletion(void* a1);
    /// 0x08d95ece
    UNK_RET DestroyContexts();
    /// 0x08d961d4
    UNK_RET AddFileLoaderFactory(FFileLoaderFactory* a1);
    /// 0x08d9623a
    UNK_RET GetFileLoaderFactory(int a1);
    /// 0x08d9625e
    UNK_RET RemoveFileLoaderFactory(int a1);
};

/// 0x08d962ab
UNK_RET SetSharedBackgroundLoader(FFileBackgroundLoader* a1);
/// 0x08d962c4
UNK_RET ReleaseSharedBackgroundLoader();
/// 0x08d965d5 SetAtGrow<FBackgroundDataLoader*>
/// 0x08d96622 SetAtGrow<char*>
template <class T0>
void SetAtGrow(std::vector<T0, std::allocator<T0> >& a1, unsigned int a2, T0 const& a3);
