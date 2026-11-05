/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include <d3d9.h>

class FGXVector2;
class FGXVector3;
class FGXVector4;
template <class T0, class T1, unsigned int T2> class FRangeCoder;
namespace FireGrafix {
class TextureData;
}

namespace FireGrafix {
/// size ?
class FImageCoder {
public:
    class ColorSpace;
    class CompressionHeader;
    class UpsampleFilter;
    /// 0x08cf9784
    FImageCoder(bool a1);
    /// 0x08cf9898
    ~FImageCoder();
    /// 0x08cf9862
    UNK_RET ClearCount();
    /// 0x08cf98fe
    UNK_RET AllocateTempSpace(unsigned int a1);
    /// 0x08cf9944
    UNK_RET DeallocateTempSpace();
    /// 0x08cf9982
    UNK_RET EncodeImage(FireGrafix::TextureData& a1, FireGrafix::FImageCoder::ColorSpace a2, FireGrafix::FImageCoder::UpsampleFilter a3, unsigned int const* a4, unsigned int const* a5);
    /// 0x08cf9b42
    UNK_RET BuildLevelData();
    /// 0x08cf9c34
    UNK_RET EncodeHeader(FireGrafix::TextureData& a1, FireGrafix::FImageCoder::CompressionHeader const& a2);
    /// 0x08cf9e5a
    UNK_RET EncodeColorTransform(FireGrafix::TextureData& a1, unsigned int const* a2);
    /// 0x08cf9f10
    UNK_RET EncodeBase(int& a1);
    /// 0x08cf9fd0
    UNK_RET EncodeSmallLevels(int& a1, unsigned int const* a2);
    /// 0x08cfa28a
    UNK_RET EncodeLargeLevels(int a1, unsigned int const* a2);
    /// 0x08cfa4ee
    UNK_RET ColorTransform(unsigned char* a1, short* a2, int a3, unsigned int const* a4);
    /// 0x08cfa740
    UNK_RET CollectStats(short const* a1, unsigned int a2, unsigned int a3, unsigned int a4);
    /// 0x08cfa93c
    UNK_RET WriteStats(FRangeCoder<FFastVector<unsigned char, true, 10u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator>, unsigned int, 4u>& a1, unsigned int& a2, unsigned int& a3, unsigned int (*a4)[3][3]);
    /// 0x08cfac1e
    UNK_RET EncodeChunk(FRangeCoder<FFastVector<unsigned char, true, 10u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator>, unsigned int, 4u>& a1, unsigned int a2, unsigned int a3, short* a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int const (*a8)[3][3]);
    /// 0x08cfb040
    UNK_RET DecodeImage(unsigned int a1, unsigned int a2);
    /// 0x08cfb0c8
    UNK_RET DecodeHeader(unsigned int a1, FireGrafix::FImageCoder::CompressionHeader& a2);
    /// 0x08cfb35c
    UNK_RET DecodeBase(int& a1);
    /// 0x08cfb426
    UNK_RET DecodeSmallLevels(int& a1);
    /// 0x08cfb54c
    UNK_RET DecodeLargeLevels(int a1, unsigned int a2);
    /// 0x08cfb778
    UNK_RET ReadStats(FRangeCoder<FFastVector<unsigned char, true, 10u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator>, unsigned int, 4u>& a1, unsigned int& a2, unsigned int& a3, unsigned int (*a4)[3][3]);
    /// 0x08cfb9aa
    UNK_RET DecodeChunk(FRangeCoder<FFastVector<unsigned char, true, 10u, 0u, BaseVector<unsigned char, true>::FDefaultFastVectorAllocator>, unsigned int, 4u>& a1, unsigned int a2, unsigned int a3, short* a4, unsigned int a5, unsigned int a6, unsigned int a7, unsigned int const (*a8)[3][3]);
    /// 0x08cfbe2e
    UNK_RET EncodeDifferences();
    /// 0x08cfbea6
    UNK_RET DecodeDifferences();
    /// 0x08cfbf4a
    UNK_RET DecodeColorTransform(FireGrafix::TextureData& a1);
    /// 0x08cfc002
    UNK_RET GetResultImage(FireGrafix::TextureData& a1);
    /// 0x08cfc0d0
    UNK_RET SetIndex(FireGrafix::TextureData* a1);
};
}

/// 0x08d015f4
UNK_RET FGXAAFrustrumIntersect(FGXVector3& a1, FGXVector3& a2, D3DXMATRIX* a3, FGXVector4* a4);
/// 0x08d017a0
UNK_RET FGXGetClipPlanesFromMatrix(D3DXMATRIX* a1, FGXVector4* a2);
/// 0x08d0197e
UNK_RET FGXGetClipPlanesFromViewMatrix(D3DXMATRIX* a1, D3DXMATRIX* a2, FGXVector4* a3);
/// 0x08d019de
bool FGXRaySphereIntersect(FGXVector3* pvCenter, float fRadius, FGXVector3* pvOrigin, FGXVector3* pvDir, FGXVector3* pvHit, float* pfDistance);
/// 0x08d01baf
UNK_RET FGX3PlaneIntersect(FGXVector3* a1, FGXVector4* a2, FGXVector4* a3, FGXVector4* a4);
/// 0x08d021bd
UNK_RET FGXDistancePointToLine(FGXVector3 const& a1, FGXVector3 const& a2, FGXVector3 const& a3);
/// 0x08d023ce
UNK_RET FGXDistancePointToLineSq(FGXVector3 const& a1, FGXVector3 const& a2, FGXVector3 const& a3);
/// 0x08d025d5
bool FGXIntersectLineSegments2D(FGXVector2& a1, FGXVector2 const& a2, FGXVector2 const& a3, FGXVector2 const& a4, FGXVector2 const& a5);
/// 0x08d026fb
UNK_RET FGXSphereCullConvexPolygonExact(FGXVector3 const& a1, float a2, FGXVector3 const* a3, unsigned int a4);

namespace FMatrix {
/// 0x08d01e60
UNK_RET IsRotationIdentity(D3DXMATRIX const& a1);
/// 0x08d01efa
UNK_RET MakeRotation(D3DXMATRIX& a1, float a2, FGXVector3 const& a3);
/// 0x08d01f36
UNK_RET MakeXRotation(D3DXMATRIX& a1, float a2);
/// 0x08d01f6a
UNK_RET MakeYRotation(D3DXMATRIX& a1, float a2);
/// 0x08d01f9e
UNK_RET MakeZRotation(D3DXMATRIX& a1, float a2);
/// 0x08d01fd2
UNK_RET MakeRotation(FGXVector3 const& a1, FGXVector3 const& a2, FGXVector3 const& a3);
/// 0x08d02088
UNK_RET SetRotation(D3DXMATRIX& a1, D3DXMATRIX const& a2);
/// 0x08d020e9
UNK_RET FromEulerAnglesZYX(D3DXMATRIX& a1, float a2, float a3, float a4);
}

namespace FQuaternion {
/// 0x08d01e5e
UNK_RET FromAngleAxis(D3DXQUATERNION& a1, float a2, FGXVector3 const& a3);
/// 0x08d01e5f
UNK_RET ToRotation(D3DXQUATERNION const& a1, D3DXMATRIX& a2);
}

namespace FireGrafix {
/// 0x08d02a38
UNK_RET EndianSwap(void* a1, void* a2, unsigned int a3);
/// 0x08d02a5d
UNK_RET D3D11ColorSwap(void* a1, void* a2, unsigned int a3);
/// 0x08d02aa1
UNK_RET MEndianSwap(void* a1, void* a2, unsigned int a3);
/// 0x08d02acf
UNK_RET FGXIsPow2(int a1);
/// 0x08d02ae6
UNK_RET RegisterTypeEnum(char** a1, unsigned int a2);
}
