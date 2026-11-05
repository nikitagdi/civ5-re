/// origin binary
#pragma once
#include "FirePlace/FireWorks/FFileIO.h"
#include <forkparticles.h>

class FForkSDK;

/// size ?
/// vtable 0x0a3013b0
class FForkPSystemBase {
public:
    class FForkPSBData;
    class PSBAllocator;
    /// size ?
    class FForkPSBData {
    public:
        /// 0x08cb5be4
        FForkPSBData();
        /// 0x08cb5c02
        ~FForkPSBData();
        /// 0x08cb5c04
        UNK_RET GetForkPSB() const;
        /// 0x08cb5c0e
        UNK_RET IsInitialized() const;
        /// 0x08cb5c1c
        UNK_RET Release();
    };
    /// size ?
    /// vtable 0x0a3013e0
    class PSBAllocator : public FFileIO::Allocator {
    public:
        /// 0x08cb6352
        virtual byte * Allocate(unsigned int a1);
        /// 0x08cb632c
        virtual void Delete(unsigned char* a1);
    };
    /// 0x08cb5c70
    FForkPSystemBase();
    /// 0x08cb5ca0
    /// deleting 0x08cb5c8c
    virtual ~FForkPSystemBase();
    /// pure
    virtual UNK_RET LoadTexture(char const* a1, bool a2) = 0;
    /// pure
    virtual UNK_RET UnloadTexture(char const* a1) = 0;
    /// pure
    virtual UNK_RET vf_04(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_05(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_06(UNK_PARAMS) = 0;
    /// pure
    virtual UNK_RET vf_07(UNK_PARAMS) = 0;
    /// 0x08cb5ca2
    UNK_RET LoadForkPSBData(char const* a1, FForkPSystemBase::FForkPSBData& a2, FForkSDK const& a3);
    /// 0x08cb5ce8
    UNK_RET LoadForkPSBData(char const* a1, FForkPSystemBase::FForkPSBData& a2, FForkSDK const& a3, bool a4);
    /// 0x08cb5e4e
    UNK_RET UnloadForkPSBData(FForkPSystemBase::FForkPSBData& a1);
    /// 0x08cb5ef0
    UNK_RET UnloadEmitterInfoAssets(void* a1, _t_frkpemitterinfo* a2);
    /// 0x08cb602a
    UNK_RET ReloadForkPSBData(char const* a1, FForkPSystemBase::FForkPSBData& a2, FForkSDK const& a3);
    /// 0x08cb60c6
    UNK_RET LoadEmitterInfoAssets(void* a1, _t_frkpemitterinfo* a2, bool a3);
    /// 0x08cb6236
    UNK_RET LoadAndPreparePSB(char const* a1, FFileIO::Allocator& a2);
    /// 0x08cb627c
    UNK_RET GetCustomDataChunkOfType(_t_frkpemitterinfo* a1, int a2);
    /// 0x08cb62dc
    UNK_RET GetFilenameFromFullPath(char const* a1);
};
