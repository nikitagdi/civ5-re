/// origin guess
#pragma once
#include "FirePlace/FireGrafix/core/FImageCoder.h"
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"

class FGXVector4;
class IDirect3DCubeTexture9;
class IDirect3DTexture9;
template <class T0, class T1> class FFastTree;
namespace FireGrafix {
class FGXModelInstance;
class FGXModelNode;
class FGXModelNodeDistanceSorter;
class FGXModelNodeStateSorter;
class FGXModelTreeNode;
class FGXNodeMatchByModelInstanceID;
enum FGX_SPIV_GENERIC : int;
}

namespace FireGrafix {
/// size ?
class FGXModelInstance {
public:
    /// 0x08cf4c8c
    FGXModelInstance();
    /// 0x08cf4d1c
    FGXModelInstance(FireGrafix::FGXModelInstance const& a1);
    /// 0x08cf4da8
    ~FGXModelInstance();
    /// 0x08cf4dee
    UNK_RET Release();
    /// 0x08cf4fc0
    UNK_RET ReleaseShaders();
    /// 0x08cf5004
    UNK_RET ReleaseSurfaces();
};
}

namespace FireGrafix {
/// 0x08cf592f SortFGXElements<FireGrafix::FGXModelNodeDistanceSorter>
/// 0x08cf5052 SortFGXElements<FireGrafix::FGXModelNodeStateSorter>
template <class T0>
void SortFGXElements(unsigned short a1, T0 const* a2, FireGrafix::FGXModelNode* a3, unsigned int a4, unsigned int* a5, unsigned int a6, FireGrafix::FGX_SPIV_GENERIC* a7, unsigned int a8, FGXVector4* a9, unsigned int a10, FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >* a11, unsigned int** a12, unsigned int a13);
/// 0x08cf620c MoveMatchingNodes<FireGrafix::FGXNodeMatchByModelInstanceID>
template <class T0>
int MoveMatchingNodes(FireGrafix::FGXModelNode* a1, int a2, int a3, T0& a4);
/// 0x08cf92d0
UNK_RET ComputeDifferenceLevel(FireGrafix::FImageCoder::UpsampleFilter a1, unsigned int a2, unsigned int a3, unsigned int a4, short const* a5, short* a6);
/// 0x08cf94ce
UNK_RET ApplyDifferenceLevel(FireGrafix::FImageCoder::UpsampleFilter a1, unsigned int a2, unsigned int a3, unsigned int a4, short const* a5, short* a6);
}

namespace FireGrafix { namespace TextureManager {
/// 0x08cf91b4
UNK_RET CreateCubeTexture(IDirect3DCubeTexture9* a1);
/// 0x08cf91ea
UNK_RET CreateTexture(IDirect3DTexture9* a1);
/// 0x08cf9220
UNK_RET CreateCubeTextureFromFileInMemory(unsigned char* a1, unsigned int a2, char const* a3);
/// 0x08cf924a
UNK_RET CreateTextureFromFileInMemory(unsigned char* a1, unsigned int a2, char const* a3);
}}
