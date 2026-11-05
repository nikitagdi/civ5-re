/// origin guess
#pragma once
#include "FirePlace/FireEngine2/FResource.h"
#include "FirePlace/FireWorks/FEndian.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include <functional>
#include <set>

class FResourceLoadHelper;
class FResourceMgr;
class FTexture;

/// size ?
/// vtable 0x0a301e60
class FTextureResource : public FResource {
public:
    /// 0x08cddb20
    FTextureResource(FResourceMgr* a1);
    /// 0x08cddb68
    FTextureResource(FResourceMgr* a1, FTexture* a2, unsigned int a3);
    /// 0x08cddbee
    /// deleting 0x08cddbc6
    virtual ~FTextureResource();
    /// 0x08cde48a
    virtual UNK_RET GetData(void* a1, unsigned int a2);
    /// 0x08cde820
    virtual UNK_RET InUse();
    /// 0x08cddc6c
    virtual UNK_RET Load(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
    /// 0x08cde6d4
    virtual UNK_RET SetData(void* a1);
    /// 0x08cde73a
    virtual UNK_RET SetData(unsigned char* a1, unsigned int a2, bool a3);
    /// 0x08cde428
    virtual UNK_RET Unload();
    /// 0x08cdec70
    virtual UNK_RET vf_07(UNK_PARAMS);
    /// 0x08cdec76
    virtual UNK_RET vf_08(UNK_PARAMS);
    /// 0x08cdddb4
    virtual UNK_RET LoadFromMemory(unsigned char const* a1, unsigned int a2, char const* a3, unsigned int a4, FResourceLoadHelper* a5);
    /// 0x08cddafc
    UNK_RET Shutdown();
    /// 0x08cddf7a
    UNK_RET IsCubeMapFromFileInMemory(unsigned char const* a1, char const* a2, FEndian::EEndianness a3);
    /// 0x08cde18a
    UNK_RET CreateCubeTextureFromFileInMemory(unsigned char* a1, unsigned int a2, char const* a3);
    /// 0x08cde204
    UNK_RET CreateTextureFromFileInMemory(unsigned char* a1, unsigned int a2, char const* a3);
    /// 0x08cde27e
    UNK_RET CreateCubeTexture(char const* a1, int* a2);
    /// 0x08cde4e2
    UNK_RET GetCachedTexture(char const* a1);
    /// 0x08cde81c
    UNK_RET RemoveTopMipLevel(FTexture* a1);
    /// 0x08cde860
    UNK_RET Register();
    /// 0x08cde894
    UNK_RET Create(FResourceMgr* a1, unsigned int a2);
    /// 0x08cde8f6
    UNK_RET UnRegister();
    /// 0x0d27ab28
    static UNK_DATA ms_iMipLevelsToRemove[0x4];
};

extern "C" {
    /// 0x0d27abd0
    extern UNK_DATA g_pTimer[0x4];
}

namespace FTextureResourceMgr {
/// 0x08cde920
UNK_RET LoadResource(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
/// 0x08cde9a2
UNK_RET LoadTexture(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
/// 0x08cdea4c
unsigned int LoadTextureHandle(char const* a1, unsigned int a2, FResourceLoadHelper* a3);
/// 0x08cdeae8
UNK_RET ReleaseTextureHandle(unsigned int a1);
/// 0x08cdeb82
UNK_RET ReleaseTextureHandles(std::set<unsigned int, std::less<unsigned int const>, FSTL_Tagged_Allocator<unsigned int, 4u, 9u, 4u> > const& a1);
/// 0x08cdebd2
UNK_RET ReLoadTextures();
}
