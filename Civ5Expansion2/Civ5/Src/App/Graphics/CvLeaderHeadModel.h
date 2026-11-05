/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/FGXTransparencyMesh.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include "FirePlace/FireWorks/FSTLBlockAllocatorMT.h"
#include <vector>

class CvLeaderHeadBuilderContext;
class CvLeaderHeadModelInstance;
class CvLeaderHeadScene;
class CvLeaderHeadTextureManager;
class FGXGrannyCompositeContainerTemplate;
class FGXPlane;
class FGXTransparencyScene;
class FGXVector3;
namespace FireGrafix {
class FGXModelNode;
enum _FGX_INDEXFORMATS : int;
template <int T0> class VStruct;
}

/// size ?
class CvLeaderHeadModel {
public:
    class ReflectionTri;
    class TransparencyMeshInfo;
    /// 0x08a3604c
    CvLeaderHeadModel();
    /// 0x08a360f8
    ~CvLeaderHeadModel();
    /// 0x08a36212
    UNK_RET Clear();
    /// 0x08a362f2
    UNK_RET CreateFromFile(char const* a1, CvLeaderHeadTextureManager* a2, CvLeaderHeadScene* a3, char const* a4);
    /// 0x08a363fe
    UNK_RET InitializeFromFile(char const* a1, CvLeaderHeadTextureManager* a2, CvLeaderHeadScene* a3, char const* a4);
    /// 0x08a36cbc
    UNK_RET CreateFallbackFromFile(char const* a1, CvLeaderHeadScene* a2);
    /// 0x08a36db8
    UNK_RET InitializeFallback(char const* a1, CvLeaderHeadScene* a2);
    /// 0x08a373a6
    UNK_RET Reload(char const* a1, char const* a2, CvLeaderHeadTextureManager* a3);
    /// 0x08a3743c
    UNK_RET InitInstance(CvLeaderHeadModelInstance* a1);
    /// 0x08a3751e
    UNK_RET ReloadFallback(char const* a1);
    /// 0x08a375a2
    UNK_RET RequestInstances(unsigned int a1);
    /// 0x08a37686
    UNK_RET SetupTransparency(CvLeaderHeadModelInstance* a1, FGXTransparencyScene& a2) const;
    /// 0x08a3774c
    UNK_RET CreateSkinSkirts(FGXGrannyCompositeContainerTemplate& a1, CvLeaderHeadBuilderContext const& a2);
    /// 0x08a37dee
    UNK_RET SplitModelByReflectionPlane(FGXGrannyCompositeContainerTemplate& a1, CvLeaderHeadBuilderContext const& a2, FFastVector<FGXPlane, true, 46u, 0u, BaseVector<FGXPlane, true>::FDefaultFastVectorAllocator>& a3);
    /// 0x08a37fe8
    UNK_RET AdjustFlags();
    /// 0x08a3818c
    UNK_RET CollectFurPhysicsInfo(CvLeaderHeadBuilderContext const& a1);
    /// 0x08a382a6
    UNK_RET CreateReversedFurGeometry(CvLeaderHeadBuilderContext const& a1);
    /// 0x08a38710
    UNK_RET CreateTransparencyMeshes(FGXGrannyCompositeContainerTemplate& a1, CvLeaderHeadBuilderContext const& a2, FFastVector<CvLeaderHeadModel::TransparencyMeshInfo, true, 46u, 0u, BaseVector<CvLeaderHeadModel::TransparencyMeshInfo, true>::FDefaultFastVectorAllocator>& a3);
    /// 0x08a38910
    UNK_RET CleanupTransparencyMeshes();
    /// 0x08a3891c
    UNK_RET CleanupMutableConstantData();
    /// 0x08a3892c
    UNK_RET CleanupFurPhysicsInfo();
    /// 0x08a3893c
    UNK_RET SetInstanceMutableConstants(CvLeaderHeadModelInstance* a1);
    /// 0x08a38b6e
    UNK_RET ExtractPlanarReflectionMesh(FGXGrannyCompositeContainerTemplate const& a1, CvLeaderHeadBuilderContext const& a2, FFastVector<FGXVector3, true, 46u, 0u, BaseVector<FGXVector3, true>::FDefaultFastVectorAllocator>& a3, FFastVector<CvLeaderHeadModel::ReflectionTri, true, 46u, 0u, BaseVector<CvLeaderHeadModel::ReflectionTri, true>::FDefaultFastVectorAllocator>& a4);
    /// 0x08a38d88
    UNK_RET ExtractReflectionPlanes(FFastVector<FGXVector3, true, 46u, 0u, BaseVector<FGXVector3, true>::FDefaultFastVectorAllocator> const& a1, float a2, float a3, FFastVector<CvLeaderHeadModel::ReflectionTri, true, 46u, 0u, BaseVector<CvLeaderHeadModel::ReflectionTri, true>::FDefaultFastVectorAllocator>& a4, FFastVector<FGXPlane, true, 46u, 0u, BaseVector<FGXPlane, true>::FDefaultFastVectorAllocator>& a5);
    /// 0x08a38ebc
    UNK_RET CreatePRNodesAndSurfaces(FGXGrannyCompositeContainerTemplate const& a1, FFastVector<CvLeaderHeadModel::ReflectionTri, true, 46u, 0u, BaseVector<CvLeaderHeadModel::ReflectionTri, true>::FDefaultFastVectorAllocator> const& a2, std::vector<FireGrafix::FGXModelNode, FSTL_Tagged_Allocator<FireGrafix::FGXModelNode, 3u, 29u, 128u> >& a3, std::vector<unsigned int, FSTL_Tagged_Allocator<unsigned int, 4u, 29u, 128u> >& a4);
    /// 0x08a3925c
    UNK_RET ReplacePRNodesAndSurfaces(std::vector<FireGrafix::FGXModelNode, FSTL_Tagged_Allocator<FireGrafix::FGXModelNode, 3u, 29u, 128u> > const& a1, std::vector<unsigned int, FSTL_Tagged_Allocator<unsigned int, 4u, 29u, 128u> > const& a2, FGXGrannyCompositeContainerTemplate& a3);
    /// 0x08a3961c
    UNK_RET AppendMeshVertices(unsigned int a1, unsigned char const* a2, unsigned int a3, FFastVector<FGXVector3, true, 46u, 0u, BaseVector<FGXVector3, true>::FDefaultFastVectorAllocator>& a4);
    /// 0x08a396ac
    UNK_RET AppendTrianglesForSurface(unsigned int a1, FireGrafix::_FGX_INDEXFORMATS a2, unsigned char const* a3, unsigned int a4, unsigned int a5, unsigned int a6, FFastVector<CvLeaderHeadModel::ReflectionTri, true, 46u, 0u, BaseVector<CvLeaderHeadModel::ReflectionTri, true>::FDefaultFastVectorAllocator>& a7);
    /// 0x08a397f8
    UNK_RET GetFurHeight(unsigned int a1);
    /// 0x08a3ac68 CopyTransparencyVerts<FireGrafix::VStruct<15> >
    template <class T0>
    void CopyTransparencyVerts(FGXTransparencyMesh::Vertex* a1, unsigned int a2, T0 const* a3);
};
