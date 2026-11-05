/// origin guess
#pragma once
#include "FirePlace/FireEngine2/CompositeContainerBuilderContext.h"
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include <granny.h>
#include <vector>

class DecalVertex;
class FGXModelBuilderCounts;
class cvModelData;
enum eLandmarkDecalType : int;
template <class T0, class T1> class FFastTree;
namespace FireGrafix {
class FGXModelTemplate;
class FGXModelTreeNode;
}

// One decal of a model: ranges into the shared vertex and index vectors plus the draw state
// that InitializeMeshDecalsForModel hands to TerrainDecalLayer::CreateDecal.
/// size 0x20
class cvMeshDecal {
public:
    unsigned int m_uFirstVertex;  ///< +0x0
    unsigned int m_uFirstIndex;   ///< +0x4
    unsigned short m_usVertexCount; ///< +0x8
    unsigned short m_usIndexCount;  ///< +0xa
    // IsDecalMaterial result: 1 landmark, 2 shadow, 3 stencil.
    unsigned char m_ucKind;       ///< +0xc
    unsigned char m_ucMask;       ///< +0xd
    unsigned char unk_e[0x2];     ///< +0xe
    // TerrainDecalLayer sorts ascending by it.
    float m_fPriority;            ///< +0x10
    unsigned int m_hAlbedo;       ///< +0x14
    unsigned int m_hHeightmap;    ///< +0x18
    unsigned int m_hSpec;         ///< +0x1c
};

// Per-model entry of the builder's model table; the decal records of a model are
// [m_usFirstDecal, m_usFirstDecal + m_usDecalCount) of the shared decal vector.
/// size 0x15c
class DecalModelEntry {
public:
    unsigned char unk_0[0x158];   ///< +0x0
    unsigned short m_usFirstDecal; ///< +0x158
    unsigned short m_usDecalCount; ///< +0x15a
};

// Granny data type descriptor entry (Type, Name, ReferenceType, ArrayWidth, Extra, Ignored).
/// size 0x20
struct granny_data_type_definition {
    int Type;                                      ///< +0x0
    char const* Name;                              ///< +0x4
    granny_data_type_definition const* ReferenceType; ///< +0x8
    int ArrayWidth;                                ///< +0xc
    int Extra[3];                                  ///< +0x10
    int Ignored;                                   ///< +0x1c
};

extern "C" {
/// 0x08df847e
int GrannyGetMeshTriangleGroupCount(granny_mesh const* pMesh);
/// 0x08df8483
void const* GrannyGetMeshTriangleGroups(granny_mesh const* pMesh);
/// 0x08df8492
int GrannyGetMeshVertexCount(granny_mesh const* pMesh);
/// 0x08df849c
void GrannyCopyMeshVertices(granny_mesh const* pMesh, granny_data_type_definition const* pConversionType, void* pDest);
/// 0x08df84a6
void* GrannyGetMeshVertices(granny_mesh const* pMesh);
/// 0x08df84b5
int GrannyGetMeshIndexCount(granny_mesh const* pMesh);
/// 0x08df84bf
int GrannyGetMeshBytesPerIndex(granny_mesh const* pMesh);
/// 0x08df84c4
void* GrannyGetMeshIndices(granny_mesh const* pMesh);
/// 0x08df8a18
void GrannyConvertIndices(int iIndexCount, int iSourceBytesPerIndex, void const* pSource, int iDestBytesPerIndex, void* pDest);
}

/// size ?
class DecalContainerBuilderContext : public CompositeContainerBuilderContext {
public:
    /// 0x088be02c
    DecalContainerBuilderContext();
    /// 0x088be0b2
    UNK_RET CalcNumFGXModelNodesNeeded(granny_mesh const* a1);
    /// 0x088be0f4
    UNK_RET IsDecalMesh(granny_mesh const* a1);
    /// 0x088be13e
    UNK_RET CalcNumFGXSurfacesNeeded(granny_mesh const* a1);
    /// 0x088be180
    UNK_RET CalcNumMeshTransformsNeeded(granny_mesh const* a1);
    /// 0x088be1e0
    UNK_RET CalcNumMeshBindingsNeeded(granny_mesh const* a1);
    /// 0x088be228
    UNK_RET CalcNumShaderPackageInstanceViewsNeeded(granny_mesh const* a1);
    /// 0x088be26a
    UNK_RET PublishModelInstanceBuilderCounts(unsigned int a1, FGXModelBuilderCounts const& a2);
    /// 0x088be2c4
    UNK_RET OnModelBuildSurfacesEnd(granny_model const* a1);
    /// 0x088be302
    UNK_RET OnMeshBuildSurfaces(granny_mesh const* a1);
    /// 0x088be424
    UNK_RET InitMeshDecal(granny_mesh const* a1, granny_material const* a2, unsigned char a3);
    /// 0x088be768
    UNK_RET OnModelBuildTransformations(granny_model const* a1, FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >* a2, unsigned int a3, unsigned int& a4);
    /// 0x088be7de
    UNK_RET OnModelBuildTransformationsEnd();
    /// 0x088be7ee
    UNK_RET SetLibraryContainers(cvModelData* a1, unsigned int* a2, unsigned int a3, unsigned int a4, FFastVector<cvMeshDecal, true, 43u, 0u, BaseVector<cvMeshDecal, true>::FDefaultFastVectorAllocator>* a5, FFastVector<DecalVertex, true, 43u, 0u, BaseVector<DecalVertex, true>::FDefaultFastVectorAllocator>* a6, FFastVector<unsigned short, true, 43u, 0u, BaseVector<unsigned short, true>::FDefaultFastVectorAllocator>* a7);
    /// 0x088be83a
    UNK_RET CalcNumDecalMeshes(granny_model const* a1);
    /// 0x088be93e
    int IsDecalMaterial(granny_material const* a1);
    /// 0x088be9de
    UNK_RET AllocateVBsAndIBs(granny_model const* a1);
    /// 0x088bed2a
    UNK_RET SetDecalMeshCounts(granny_mesh const* a1, cvMeshDecal& a2, eLandmarkDecalType a3);

    unsigned int m_uModelIndex;       ///< +0x24
    unsigned int m_uDecalCursor;      ///< +0x28
    unsigned char unk_2c[0x8];      ///< +0x2c
    DecalModelEntry* m_pModels;       ///< +0x34
    unsigned int* m_pModelCount;      ///< +0x38
    unsigned char unk_3c[0x4];      ///< +0x3c
    FFastVector<cvMeshDecal, true, 43u, 0u, BaseVector<cvMeshDecal, true>::FDefaultFastVectorAllocator>* m_pDecals; ///< +0x40
    FFastVector<DecalVertex, true, 43u, 0u, BaseVector<DecalVertex, true>::FDefaultFastVectorAllocator>* m_pDecalVertices; ///< +0x44
    FFastVector<unsigned short, true, 43u, 0u, BaseVector<unsigned short, true>::FDefaultFastVectorAllocator>* m_pDecalIndices; ///< +0x48
};

/// 0x088bed81
UNK_RET CloneFGXModelNode(FireGrafix::FGXModelTemplate const& a1, FireGrafix::FGXModelTemplate& a2, unsigned int a3);
/// 0x088bee91
UNK_RET CloneFGXModelNodeWithSurfaceOffset(FireGrafix::FGXModelTemplate const& a1, FireGrafix::FGXModelTemplate& a2, unsigned int a3, unsigned int a4);
/// 0x088befac
UNK_RET CloneFGXSurfaces(std::vector<unsigned int, FSTL_Tagged_Allocator<unsigned int, 4u, 29u, 128u> >& a1, unsigned int a2, unsigned int a3, bool a4);
