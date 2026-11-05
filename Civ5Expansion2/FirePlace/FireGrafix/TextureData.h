/// origin guess
#pragma once
#include "FirePlace/FireWorks/FMemoryStream.h"

class FGXVector3;
class FGXVector4;
class _ALPHA_SET;
class _ZSTATE_SET;
namespace FireGrafix {
class CCommandStream;
class SurfaceSet;
class TextureData;
enum _TextureType : int;
enum CONSTANTTYPE : int;
enum DXT1_BLOCK : int;
enum DXT3_BLOCK : int;
enum DXT5_BLOCK : int;
}

namespace FireGrafix {
/// size ?
class TextureData {
public:
    // One mip level: extents, row pitch, bytes of one face and the face pointers (six for a cube).
    /// size 0x2c
    struct MipLevel {
        unsigned int m_uWidth;          ///< +0x0
        unsigned int m_uHeight;         ///< +0x4
        unsigned int m_uDepth;          ///< +0x8
        unsigned int m_uPitch;          ///< +0xc
        unsigned int m_uSize;           ///< +0x10
        unsigned char* m_apFaces[6];    ///< +0x14
    };

    /// 0x08d02b0e
    TextureData(unsigned char* a1, unsigned int a2, unsigned long a3);
    // ulFlags bit 0: the caller keeps ownership of pData; bit 7: describe the mips without data pointers.
    /// 0x08d04362
    TextureData(unsigned long ulFormat, FireGrafix::_TextureType eType, int iWidth, int iHeight, int iDepth, int nMips, unsigned long ulFlags, unsigned char* pData, unsigned int a9);
    /// 0x08d04a82
    TextureData(FireGrafix::TextureData* a1, unsigned long a2);
    /// 0x08d068d4
    ~TextureData();
    /// 0x08d03386
    UNK_RET LoadTGA(unsigned char* a1, unsigned int a2, unsigned long a3);
    /// 0x08d0366e
    UNK_RET CreateTGA(FMemoryStream& a1);
    /// 0x08d03822
    UNK_RET CreateDDS(FMemoryStream& a1);
    /// 0x08d041d4
    UNK_RET CalcMemRequired(unsigned long a1, FireGrafix::_TextureType a2, int a3, int a4, int a5, int a6);
    /// 0x08d05026
    UNK_RET ComputeSincFilter(unsigned int a1, float* a2, float* a3, int a4, int a5, float a6, float a7, bool a8);
    /// 0x08d055f2
    UNK_RET ComputeSincMIP(int a1, int a2, float a3, float a4, bool a5, bool a6);
    /// 0x08d060d0
    UNK_RET ComputeNormalFilter(unsigned long a1, float a2, float a3);
    /// 0x08d06270
    UNK_RET Fill3DTexture(void (*a1)(FGXVector4*, FGXVector3 const&, FGXVector3 const&, void*), void* a2);
    /// 0x08d064ee
    bool SaveDDS(char const* a1);
    /// 0x08d066ae
    UNK_RET SaveTGA(char const* a1);
    /// 0x08d0686e
    UNK_RET DoDX11ColorSwap();

    unsigned long m_ulFormat;           ///< +0x0
    FireGrafix::_TextureType m_eType;   ///< +0x4
    unsigned int m_uWidth;              ///< +0x8
    unsigned int m_uHeight;             ///< +0xc
    unsigned int m_uDepth;              ///< +0x10
    unsigned int field_14;              ///< +0x14
    unsigned int m_uBitsPerPixel;       ///< +0x18
    int m_bOwnsData;                    ///< +0x1c
    int field_20;                       ///< +0x20
    int m_bHasMips;                     ///< +0x24
    int m_bPowerOfTwo;                  ///< +0x28
    unsigned char* m_pData;             ///< +0x2c
    MipLevel m_aMips[16];               ///< +0x30
    char m_szName[0x24];                ///< +0x2f0
    char m_szTypeName[0xc];             ///< +0x314
    int field_320;                      ///< +0x320
    unsigned int m_nMips;               ///< +0x324
};
}

namespace FireGrafix {
/// 0x08d035a9
UNK_RET GetChannels(unsigned long a1);
/// 0x08d03b51
UNK_RET Decode565(FGXVector4* a1, unsigned short a2);
/// 0x08d03bb8
UNK_RET ConvertDXT1(FGXVector4* a1, unsigned int a2, FireGrafix::DXT1_BLOCK* a3);
/// 0x08d03dd6
UNK_RET ConvertDXT3(FGXVector4* a1, unsigned int a2, FireGrafix::DXT3_BLOCK* a3);
/// 0x08d03e8f
UNK_RET ConvertDXT5(FGXVector4* a1, unsigned int a2, FireGrafix::DXT5_BLOCK* a3);
/// 0x08d0402d
UNK_RET ConvertLevel(unsigned char* a1, unsigned long a2, unsigned char* a3, unsigned long a4, unsigned int a5, unsigned int a6, unsigned int a7);
/// 0x08d04108
UNK_RET GetBitDepth(unsigned long a1);
/// 0x08d04f8d
UNK_RET Sinc1D(float a1, float a2, float a3);
/// 0x08d054c4
UNK_RET ConvertToLinear(float* a1, int a2, int a3);
/// 0x08d05559
UNK_RET ConvertToGamma(float* a1, int a2, int a3);
/// 0x08d05ad7
UNK_RET BilateralWeight(float a1, float a2, float a3);
/// 0x08d05b1f
UNK_RET FilterLevel(float* a1, float* a2, int a3, int a4, float a5, float a6);
/// 0x08d0690b
unsigned int GetTextureSize(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned long a5);
/// 0x08d06a1b
UNK_RET CreateColoredMipTexture(unsigned int a1, unsigned int a2);
/// 0x08d08c30
UNK_RET StatsPrintf(FMemoryStream* a1, char const* a2, ...);
/// 0x08d08ca4
UNK_RET WriteStats(char* a1);
/// 0x08d0926e
UNK_RET GetAlphaModeString(_ALPHA_SET a1);
/// 0x08d0928e
UNK_RET GetZModeString(_ZSTATE_SET a1);
/// 0x08d09fb0
UNK_RET GetStereoConvergence();
/// 0x08d09fb3
UNK_RET SetStereoConvergence(float a1);
}

namespace FireGrafix {
// Element of the array at CRenderer9::m_kCSManager+0x2020c8 (stride 0x38); the Lock* methods read this from the stack.
/// size 0x38
class FGXConstantSet {
public:
    /// 0x08d0930c
    UNK_RET Initialize(FireGrafix::CONSTANTTYPE a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8);
    /// 0x08d09392
    void* Lock(unsigned int a1, unsigned int a2);
    /// 0x08d0939e
    UNK_RET Unlock(unsigned int a1);
    /// 0x08d093a8
    UNK_RET UnlockDynamic();
    /// 0x08d093b2
    UNK_RET Apply();
    /// 0x08d09430
    UNK_RET ApplyUM(void* a1);
    /// 0x08d094c4
    UNK_RET ApplyUMPartial(void* a1, unsigned int a2);
    /// 0x08d09588
    unsigned int Copy();
    /// 0x08d095fa
    void* LockGlobal(unsigned int uSize, unsigned int a2);
    /// 0x08d09606
    void* LockStatic(unsigned int uSize, unsigned int a2);
    /// 0x08d09612
    void* LockMutable(unsigned int uSize, unsigned int a2);
    // Returns the next uSize bytes of the stream's dynamic constant buffer.
    /// 0x08d0961e
    void* LockDynamic(FireGrafix::CCommandStream* pStream, unsigned int uSize);
    /// 0x08d0964e
    void* LockDynamic(FireGrafix::SurfaceSet* pSurfaceSet, unsigned int uSize);
    // Returns the new set created in the constant-set manager.
    /// 0x08d0967e
    FireGrafix::FGXConstantSet* AliasGlobal(FireGrafix::CCommandStream* a1);

    char unk_0[0x14];          ///< +0x0
    void* m_pData;             ///< +0x14
    char unk_18[0x14];         ///< +0x18
    bool m_bLocked;            ///< +0x2c
    char unk_2d[0x7];          ///< +0x2d
    unsigned short field_34;   ///< +0x34
    char unk_36[0x2];          ///< +0x36
};
}

namespace FireGrafix { namespace FGXConstantSetManager {
/// 0x08d0971c
unsigned int CreateConstantSet(FireGrafix::CONSTANTTYPE a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8);
// The manager pointer is the implicit first stack word; the mangled name has 8 parameters.
/// 0x08d09964
void InitializeGlobalCS(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8);
/// 0x08d09a56
UNK_RET ReleaseConstantSet(unsigned int a1);
}}
