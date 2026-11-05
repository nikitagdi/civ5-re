/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastAllocator.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include <granny.h>
#include <vector>

class CompositeContainerBuilderContext;
class CvLeaderHeadBuilderContext;
class FGXVector4;
class FlypathBuilderContext;
class ForestVisBuilderContext;
class LightRigBuilderContext;
class UnitBuilderContext;
class cvLandmarkDecalContainerBuilderContext;
class cvPopBuildingBuilderContext;
class cvVisEffectBuilderContext;
class cvWonderContainerBuilderContext;
template <class T0, class T1> class FFastTree;
template <class T0> class CvLeaderHeadBufferManagerRendererContext;
template <class T0> class CvLeaderHeadFallbackRendererContext;
template <class T0> class FGXCombinedResourceRendererContext;
template <class T0> class FGXDefaultRendererContext;
namespace FireGrafix {
class FGXModelNode;
class FGXModelTreeNode;
}

/// size ?
template <class T0>
class FGXModelComponentBuilder_Granny {
public:
    class FGXModelMeshData;
    /// 0x08a3f35a FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3d442 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x088775cc FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d3358 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x088631de FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c382e FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2defc FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866bc04 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a338e4 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x0892f1ce FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    UNK_RET InitFromModelList(granny_model** a1, unsigned int a2);
    /// 0x08a3f492 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3d578 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08877702 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d348e FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08863314 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c3966 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2e034 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866bd3e FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a33a1c FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x0892f306 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088ad65a FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET Build(std::vector<unsigned int, FSTL_Tagged_Allocator<unsigned int, 4u, 29u, 128u> >* a1, std::vector<FireGrafix::FGXModelNode, FSTL_Tagged_Allocator<FireGrafix::FGXModelNode, 3u, 29u, 128u> >* a2, FFastTree<FireGrafix::FGXModelTreeNode, FFastAllocator<FireGrafix::FGXModelTreeNode, false, 29u, 0u, BaseVector<FireGrafix::FGXModelTreeNode, false>::FDefaultFastVectorAllocator> >* a3, std::vector<FGXVector4, FSTL_Tagged_Allocator<FGXVector4, 4u, 29u, 128u> >* a4, std::vector<granny_mesh_binding*, FSTL_Tagged_Allocator<granny_mesh_binding*, 4u, 29u, 16u> >* a5, unsigned int a6);
    /// 0x08a3f79e FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3d884 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08877ec8 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d3b4c FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08863ada FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c3c72 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2e340 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866c352 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a33d28 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x0892f61c FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088adf48 FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET BuildTransformations();
    /// 0x08a3f820 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3d906 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08877f56 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d3bce FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08863b68 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c3cf4 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2e3c2 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866c3e0 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a33daa FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x0892f69e FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088adfd6 FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET BuildSurfacesAndFGXNodes(unsigned int a1);
    /// 0x08a3f9d8 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3dabe FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x0887814a FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d3d86 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08863d5c FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c3eac FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2e57a FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866c5b0 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a33f0a FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x0892f856 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088ae1ca FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET BuildNodesFromModelSkeleton(granny_model* a1);
    /// 0x08a3fb52 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3dc38 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x088782c4 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d3f00 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08863ed6 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c4026 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2e70e FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866c72a FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a34084 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x0892f9d0 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088ae344 FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET BuildSurfacesAndFGXNodesFromModel(granny_model* a1, unsigned int a2);
    /// 0x08a40020 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3e094 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08878720 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d435c FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08864332 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c44f4 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2ecbe FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866ce88 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a34552 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x0892fe9e FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088ae7a0 FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET BuildIndexBufferFromMesh(granny_mesh* a1, unsigned int a2);
    /// 0x08a40188 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3e1c8 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x088787e2 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d441e FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x088643f4 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c45e4 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2edae FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866cfcc FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a34642 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x0892ff8e FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088ae862 FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET BuildVertexBufferFromMesh(granny_mesh* a1, unsigned int a2);
    /// 0x08a402e4 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3e322 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x0887893c FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d4578 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x0886454e FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c46c0 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2ee8a FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866d124 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a3471e FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x089300dc FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088ae906 FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET FindFGXSurfaceHandlesInSurfaceVector(unsigned int a1, unsigned int a2, unsigned int& a3, unsigned int& a4) const;
    /// 0x08a403ee FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3e3da FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x088789f4 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d4630 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08864606 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c47ca FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2ef94 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866d29a FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a34828 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x089301e6 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088ae9be FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET ProcessTriGroupWithMaterial(FGXModelComponentBuilder_Granny<T0>::FGXModelMeshData& a1, unsigned int a2, bool a3, bool a4);
    /// 0x08a404c2 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3e4ae FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08878ac8 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d4704 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x088646da FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c489e FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2f068 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866d372 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a348fc FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x089302ba FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088aea92 FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET BuildFGXNodesFromModel(FGXModelComponentBuilder_Granny<T0>::FGXModelMeshData& a1, bool a2);
    /// 0x08a4067a FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3e666 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08878c80 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d48bc FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08864892 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c4a50 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2f21a FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866d532 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a34aae FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x08930472 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088aec4a FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET BuildSurfacesFromModel(FGXModelComponentBuilder_Granny<T0>::FGXModelMeshData& a1, unsigned int a2, bool a3);
    /// 0x08a40810 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3e830 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08878f6e FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d4c1e FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08864c18 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c4b76 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2f340 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866d8ba FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a34bd4 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x089306e4 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088af064 FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET AssignTransforms(FireGrafix::FGXModelNode* a1, granny_model const* a2, granny_mesh const* a3, unsigned int a4);
    /// 0x08a40876 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3e896 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08878fd4 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d4c84 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08864c7e FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c4bdc FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2f3a6 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866d920 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a34c3a FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x0893074a FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088af0ca FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET FindTransformNode(char const* a1, unsigned int a2, bool a3) const;
    /// 0x08a40982 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3e9a2 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x088791f2 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d4ea2 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08865006 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c4ce8 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2f4b2 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866e2b8 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a34d46 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x08930968 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088af24e FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET FindBoneBindingNameMatch(granny_bone const& a1) const;
    /// 0x08a409e0 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3ea00 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08879250 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d4f00 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x0886511a FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c4d46 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2f66c FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866e7ba FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a35b44 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x089309c6 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088af2ac FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET CalcCountsNeeded();
    /// 0x08a40c72 FGXModelComponentBuilder_Granny<CvLeaderHeadBufferManagerRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x08a3ec64 FGXModelComponentBuilder_Granny<CvLeaderHeadFallbackRendererContext<CvLeaderHeadBuilderContext> >
    /// 0x0887949c FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvLandmarkDecalContainerBuilderContext> >
    /// 0x088d5162 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvVisEffectBuilderContext> >
    /// 0x08865366 FGXModelComponentBuilder_Granny<FGXCombinedResourceRendererContext<cvWonderContainerBuilderContext> >
    /// 0x088c4fd0 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<CompositeContainerBuilderContext> >
    /// 0x08a2f8c8 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<FlypathBuilderContext> >
    /// 0x0866ea4e FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<ForestVisBuilderContext> >
    /// 0x08a35da0 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<LightRigBuilderContext> >
    /// 0x08930c48 FGXModelComponentBuilder_Granny<FGXDefaultRendererContext<UnitBuilderContext> >
    /// 0x088af4dc FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET CalcNodeCountFromBoneBoundNodes(granny_model* a1) const;
    /// 0x088ad414 FGXModelComponentBuilder_Granny<cvPopBuildingBuilderContext>
    UNK_RET InitFromGrannyFile(char const* a1);
};
