/// origin guess
#pragma once

class FGXPlane;
class FGXVector4;
class iRect;

namespace FireGrafix {
/// size ?
class CCommandStream {
public:
    /// 0x08cf1af0
    CCommandStream(unsigned int a1, unsigned int a2, unsigned int a3, unsigned long a4);
    /// 0x08cf1bc6
    ~CCommandStream();
    /// 0x08cf1c10
    UNK_RET DrawSurface(unsigned int a1, unsigned int a2, unsigned short a3);
    /// 0x08cf1c9e
    UNK_RET DrawSurfaceDataOffset(unsigned int a1, unsigned int a2, unsigned short a3, unsigned int a4);
    /// 0x08cf1d3a
    UNK_RET DrawSurfaceSet(unsigned int a1, unsigned int a2);
    /// 0x08cf1dc2
    UNK_RET AliasGlobal(unsigned int a1, unsigned int a2);
    /// 0x08cf1e06
    UNK_RET ApplyGlobalAliases(unsigned int a1, unsigned int a2);
    /// 0x08cf1e64
    unsigned int BeginCommandSet(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, char const* a5, FGXVector4 a6, float a7, unsigned char a8, unsigned char a9, unsigned int a10);
    /// 0x08cf1f50
    UNK_RET BeginCommandSetMultiUAV(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8, unsigned int a9, unsigned int a10, unsigned int a11, unsigned int a12, char const* a13, FGXVector4 a14, float a15, unsigned char a16, unsigned char a17, unsigned int a18);
    /// 0x08cf21e2
    UNK_RET BeginCommandSetZ(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, char const* a6, FGXVector4 a7, float a8, unsigned char a9, unsigned char a10, unsigned int a11);
    /// 0x08cf22d0
    unsigned int BeginCommandSetMulti(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8, char const* a9, FGXVector4 a10, float a11, unsigned char a12, unsigned char a13, unsigned int a14);
    /// 0x08cf23c8
    unsigned int BeginCommandSetSubResource(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, char const* a7, FGXVector4 a8, float a9, unsigned char a10, unsigned char a11, unsigned int a12);
    /// 0x08cf24b2
    UNK_RET BeginCommandSetSubResourceUAV(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int a8, unsigned int a9, unsigned int a10, unsigned int a11, char const* a12, FGXVector4 a13, float a14, unsigned char a15, unsigned char a16, unsigned int a17);
    /// 0x08cf271a
    UNK_RET BeginCommandSetSubResourceZ(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, char const* a8, FGXVector4 a9, float a10, unsigned char a11, unsigned char a12, unsigned int a13);
    /// 0x08cf2806
    UNK_RET BeginCommandSetUAV(unsigned int a1, unsigned int a2, unsigned int a3, unsigned int a4, unsigned int a5, unsigned int a6, unsigned int a7, char const* a8, float a9, unsigned char a10, unsigned char a11, unsigned int a12);
    /// 0x08cf2a2a
    UNK_RET SetUserClipPlane(FGXPlane const& a1);
    /// 0x08cf2a64
    UNK_RET EndCommandSet();
    /// 0x08cf2a92
    UNK_RET ApplyPixelCounter(unsigned int a1);
    /// 0x08cf2aac
    UNK_RET AddCommandSetToRenderQueue(unsigned int a1);
    /// 0x08cf2ae2
    UNK_RET AddAllCommandSetsToRenderQueue();
    /// 0x08cf2b28
    UNK_RET ClearStream();
    /// 0x08cf2b5e
    UNK_RET UpdateSubResource(unsigned int a1, unsigned int a2, iRect a3, void* a4, unsigned int a5, unsigned int a6, unsigned int a7);
    /// 0x08cf2c98
    UNK_RET CopyRenderTexture(unsigned int a1, unsigned int a2);
    /// 0x08cf2d1e
    UNK_RET GenerateMips(unsigned int a1);
    /// 0x08cf2d92
    UNK_RET ResolveRenderTexture(unsigned int a1, unsigned int a2, unsigned int a3);
    /// 0x08cf2e2c
    unsigned int CopyResource(unsigned int a1, unsigned int a2);
};
}
