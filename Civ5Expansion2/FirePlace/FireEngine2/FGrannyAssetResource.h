/// origin binary
#pragma once
#include "FirePlace/FireEngine2/FResource.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include "FirePlace/FireWorks/FString.h"
#include <granny.h>
#include <list>
#include <vector>

class FCSVFile;
class FGrannyAsset;
class FGrannyAssetLoadFilter;
class FGrannyAssetMiscDataLoader;
class FResourceBackgroundLoaderContext;
class FResourceFileListEntry;
class FResourceLoadHelper;
class FResourceMgr;
class FXmlDocument;
class FXmlElement;

/// size ?
/// vtable 0x0a301b40
class FGrannyAssetResource : public FResource {
public:
    /// 0x08cd498e
    /// deleting 0x08cd49da
    virtual ~FGrannyAssetResource();
    /// 0x08cd3b56
    virtual UNK_RET CreateLoader(FResourceBackgroundLoaderContext* a1);
    /// 0x08cd3688
    virtual UNK_RET GetData(void* a1, unsigned int a2);
    /// 0x08cd3b14
    virtual UNK_RET InUse();
    /// 0x08cd35b2
    virtual UNK_RET Load(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
    /// 0x08cd3b52
    virtual UNK_RET SetData(unsigned char* a1, unsigned int a2, bool a3);
    /// 0x08cd4a02
    virtual UNK_RET SetData(void* a1);
    /// 0x08cd3648
    virtual UNK_RET Unload();
    /// 0x08cd4a4e
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08cd4a52
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08cd21de
    virtual UNK_RET LoadGrannyAsset(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
    /// 0x08cd0494
    UNK_RET FinishGr2Animation(FGrannyAsset* a1, granny_animation* a2, std::vector<unsigned int, FSTL_Tagged_Allocator<unsigned int, 16u, 27u, 4u> > const& a3, FCSVFile* a4, char const* a5);
    /// 0x08cd0532
    UNK_RET AnimRefGr2File(FGrannyAsset* a1, int a2);
    /// 0x08cd05b4
    UNK_RET LoadGr2FileHelper(FGrannyAsset* a1, FStringA const& a2, std::vector<unsigned int, FSTL_Tagged_Allocator<unsigned int, 16u, 27u, 4u> > const& a3, FCSVFile* a4, unsigned int a5);
    /// 0x08cd07e8
    UNK_RET GetAssetFileNames(char const* a1, unsigned int a2, FGrannyAssetLoadFilter* a3, std::list<FResourceFileListEntry, FSTL_Tagged_Allocator<FResourceFileListEntry, 4u, 27u, 4u> >* a4);
    /// 0x08cd0bda
    UNK_RET GetAssetFileNames(FXmlDocument const& a1, char const* a2, unsigned int a3, FGrannyAssetLoadFilter* a4, std::list<FResourceFileListEntry, FSTL_Tagged_Allocator<FResourceFileListEntry, 4u, 27u, 4u> >* a5);
    /// 0x08cd33b0
    UNK_RET LoadMiscAsset(FGrannyAsset* a1, FStringA const& a2, unsigned int a3, unsigned int* a4);
    /// 0x08cd36e0
    UNK_RET UnCacheAllSubFiles(char const* a1);
    /// 0x08cd3a82
    UNK_RET RemoveMiscDataLoader(FGrannyAssetMiscDataLoader* a1);
    /// 0x08cd3bba
    UNK_RET Register();
    /// 0x08cd3bee
    UNK_RET Create(FResourceMgr* a1, unsigned int a2);
    /// 0x08cd3c42
    UNK_RET UnRegister();
    /// 0x0d276c68
    static UNK_DATA m_vctMiscDataLoaders[0xc];
};

/// 0x08cd19ad
UNK_RET FindAssetFilenamesInFXml(FXmlDocument const& a1, FStaticVector<FXmlElement, 512u, false, 27u, 0u>& a2);
