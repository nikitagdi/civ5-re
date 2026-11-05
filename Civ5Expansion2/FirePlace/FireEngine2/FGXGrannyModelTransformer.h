/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include <granny.h>

class CachedGrannyPoseData;
class FGXGrannyCompositeContainerInstance;
class FGXVector2;
class FGXVector4;
template <class T0, class T1> class FFastTree;
namespace FireGrafix {
class FGXModelNode;
class FGXModelTreeNode;
class FGXModelTreeNodeData;
}

/// size ?
class FGXGrannyModelTransformer {
public:
    /// 0x08cd7a22
    FGXGrannyModelTransformer(granny_local_pose** a1, granny_world_pose** a2, granny_dag_pose_cache** a3);
    /// 0x08cd7a3e
    UNK_RET BuildModelTransforms(FireGrafix::FGXModelNode* a1, unsigned int a2, granny_blend_dag_node** a3, granny_mesh_binding** a4, FGXVector4* a5, FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >* a6, CachedGrannyPoseData* a7);
    /// 0x08cd7bc0
    UNK_RET BuildCompositeBuffers_Granny(unsigned int a1, FireGrafix::FGXModelNode* a2, granny_world_pose* a3, granny_skeleton* a4, granny_mesh_binding** a5, FGXVector4* a6);
    /// 0x08cd7ca6
    UNK_RET BuildModelTransformsWithoutDAG(FireGrafix::FGXModelNode* a1, unsigned int a2, granny_blend_dag_node** a3, granny_mesh_binding** a4, FGXVector4* a5, FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >* a6, CachedGrannyPoseData* a7);
    /// 0x08cd7e0a
    UNK_RET BuildModelTransformsWithoutDAGMasked(FireGrafix::FGXModelNode* a1, unsigned int a2, granny_blend_dag_node** a3, granny_mesh_binding** a4, granny_track_mask const** a5, FGXVector4* a6, FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >* a7, CachedGrannyPoseData* a8);
    /// 0x08cd8072
    UNK_RET BuildModelTransformsWithoutDAGAccelerated(FireGrafix::FGXModelNode* a1, unsigned int a2, granny_blend_dag_node** a3, granny_mesh_binding** a4, FGXVector4* a5, FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >* a6, CachedGrannyPoseData* a7);
    /// 0x08cd81d6
    UNK_RET BuildModelTransforms(FireGrafix::FGXModelNode* a1, unsigned int a2, float* a3, granny_dag2_instance** a4, granny_mesh_binding** a5, FGXVector4* a6, FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >* a7, CachedGrannyPoseData* a8);
    /// 0x08cd8374
    UNK_RET BuildCompositeBuffers_TreeNode(FireGrafix::FGXModelNode* a1, FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >* a2, FGXVector4* a3);
};

/// 0x08cd8657
UNK_RET PopulateDag2Data(FGXGrannyCompositeContainerInstance* a1, granny_dag2_instance** a2, float* a3);

namespace FGXModelTransformer_GrannyDag2Resolver {
/// 0x08cd8502
UNK_RET ResolveExternal(FireGrafix::FGXModelTreeNodeData* a1, FireGrafix::FGXModelTreeNodeData const* a2, CachedGrannyPoseData* a3);
}

namespace FGXModelTransformer_GrannyDagResolver {
/// 0x08cd8408
UNK_RET ResolveExternal(FireGrafix::FGXModelTreeNodeData* a1, FireGrafix::FGXModelTreeNodeData const* a2, CachedGrannyPoseData* a3);
}

namespace FNoise {
/// 0x08cd8724
UNK_RET Noise2D(FGXVector2 const& a1);
}
