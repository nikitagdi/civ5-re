/// origin guess
#pragma once
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include "FirePlace/FireWorks/FSingleton.h"
#include "FirePlace/FireWorks/FString.h"
#include <functional>
#include <map>
#include <utility>

class FResource;
class FResourceBackgroundLoader;
class FResourceBackgroundLoaderCallback;
class FResourceLoadHelper;
class FResourceMgr;
class ResourceType;
template <class T0> class FTPointer;

/// size ?
/// vtable 0x0a301c70
class FResourceMgr : public FSingleton<FResourceMgr> {
public:
    /// 0x08cd8a5c
    FResourceMgr(FResourceBackgroundLoader* a1);
    /// 0x08cd8bfc
    /// deleting 0x08cd8bd4
    virtual ~FResourceMgr();
    /// 0x08cdac98
    virtual UNK_RET Update();
    /// 0x08cd8f0e
    virtual UNK_RET CreateResource(ResourceType a1, unsigned int a2);
    /// 0x08cdafcc
    virtual UNK_RET vf_04(UNK_PARAMS);
    /// 0x08cd8cc6
    UNK_RET Shutdown();
    /// 0x08cd8dcc
    UNK_RET GetResourceMap(ResourceType a1) const;
    /// 0x08cd8e1e
    UNK_RET GetResourceMap(ResourceType a1);
    /// 0x08cd8eac
    UNK_RET FindResourceA(std::map<char const* const, FTPointer<FResource>, std::less<char const*>, FSTL_Tagged_Allocator<std::pair<char const* const, FTPointer<FResource> >, 16u, 33u, 4u> >* a1, char const* a2);
    /// 0x08cd8f7a
    UNK_RET OnResourcePostLoadMemTrack(int a1, int a2, char const* a3);
    /// 0x08cd8f7c
    UNK_RET LoadResource(ResourceType a1, char const* a2, unsigned int a3, FResourceLoadHelper* a4);
    /// 0x08cd93ea
    UNK_RET AddPendingResource(FResource* a1);
    /// 0x08cd94e0
    UNK_RET RemovePendingResource(FResource* a1);
    /// 0x08cd9538
    UNK_RET LogMessage(int a1, char const* a2);
    /// 0x08cd953a
    UNK_RET BackgroundLoadResource(ResourceType a1, char const* a2, unsigned int a3, FResourceBackgroundLoaderCallback* a4, FResourceLoadHelper* a5);
    /// 0x08cd9a58
    UNK_RET CreatePendingResource(ResourceType a1, char const* a2, unsigned int a3);
    /// 0x08cd9d08
    UNK_RET LoadResource(void* a1, ResourceType a2, char const* a3, unsigned int a4, FResourceLoadHelper* a5);
    /// 0x08cd9db8
    UNK_RET RemoveResource(FResource* a1, std::map<char const* const, FTPointer<FResource>, std::less<char const*>, FSTL_Tagged_Allocator<std::pair<char const* const, FTPointer<FResource> >, 16u, 33u, 4u> >* a2, std::__map_iterator<std::__tree_iterator<std::__value_type<char const* const, FTPointer<FResource> >, std::__tree_node<std::__value_type<char const* const, FTPointer<FResource> >, void*>*, int> >& a3, bool a4, float a5);
    /// 0x08cda06a
    UNK_RET UnCacheResource(ResourceType a1, char const* a2, bool a3, float a4);
    /// 0x08cda1b6
    UNK_RET GetResourcePaletteName(char const* a1);
    /// 0x08cda2f2
    UNK_RET UnCacheResource(FResource* a1, bool a2, float a3);
    /// 0x08cda412
    UNK_RET UnCacheAllResources(bool a1, float a2);
    /// 0x08cda472
    UNK_RET IsResourceCached(ResourceType a1, char const* a2, FResource** a3);
    /// 0x08cda512
    UNK_RET CancelBackgroundLoad(char const* a1);
    /// 0x08cda552
    UNK_RET CancelAllBackgroundLoading();
    /// 0x08cda57a
    UNK_RET DumpStats(FStringA& a1);
    /// 0x08cda57c
    UNK_RET DumpResourcePalette(ResourceType a1);
    /// 0x08cda57e
    UNK_RET DumpAllResourcePalettes(char const* a1);
    /// 0x08cda580
    UNK_RET AddResource(unsigned char* a1, unsigned int a2, bool* a3, ResourceType a4, char const* a5, unsigned int a6);
    /// 0x08cda816
    UNK_RET AddResource(FResource* a1, unsigned char* a2, unsigned int a3, bool* a4);
    /// 0x08cda948
    UNK_RET AddResource(FResource* a1);
    /// 0x08cdaa4a
    UNK_RET IsResourcePending(char const* a1);
    /// 0x08cdabcc
    UNK_RET IsBackgroundLoading(FResource* a1);
    /// 0x08cdabf8
    UNK_RET IsBackgroundLoading(ResourceType a1, char const* a2);
    /// 0x08cdacc0
    UNK_RET RegisterResourceType(ResourceType a1, FResource* (*a2)(FResourceMgr*, unsigned int));
    /// 0x08cdad40
    UNK_RET UnRegisterResourceType(ResourceType a1);
    /// 0x0d27a968
    static UNK_DATA ms_bWarnOnSynchronousLoad[0x1];
    /// 0x0d27a96c
    static UNK_DATA ms_hMainThreadID[0x4];
};

/// 0x08cd8f7b
UNK_RET CheckMemUsed(FResource* a1);
