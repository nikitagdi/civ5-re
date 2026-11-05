/// origin guess
#pragma once
#include "Civ5/Src/App/BuilderContext_Base.h"
#include "FirePlace/FireWorks/FFastVector.h"
#include <granny.h>

class BuilderContextSPICreator;
namespace FireGrafix {
class FGXShaderPackageInstanceView;
class FGXSurface;
enum FGX_SPIV_GENERIC : int;
}

// Entry of the material table at DefaultBuilderContext +0x290 (count at +0x1390): the granny
// material and its FGrFGXMaterialInfo, which starts at +0x4.
/// size 0x88
class BuilderMaterialSlot {
public:
    granny_material const* m_pMaterial; ///< +0x0
    unsigned char m_info[0x84];         ///< +0x4
};

/// size ?
class DefaultBuilderContext: public BuilderContext_Base {
public:
    /// 0x08cd62ba
    DefaultBuilderContext();
    /// 0x08cd6392
    UNK_RET OnModelBuildSurfaces(granny_model const* a1);
    /// 0x08cd63d4
    UNK_RET OnMeshBuildSurfaces(granny_mesh const* a1);
    /// 0x08cd6416
    UNK_RET GetVertexFormat(granny_material const* a1, granny_mesh const* a2);
    /// 0x08cd6444
    UNK_RET GetShaderPackageInstanceIndex(granny_material const* a1) const;
    /// 0x08cd6480
    UNK_RET FindMaterialInfoIndex(granny_material const* a1) const;
    /// 0x08cd64bc
    UNK_RET GetShaderPackageInstance(granny_material const* a1);
    /// 0x08cd658e
    UNK_RET BindTextures(granny_material const* a1, FireGrafix::FGXSurface* a2);
    /// 0x08cd673e
    BuilderMaterialSlot const* FindMaterialInfo(granny_material const* a1) const;
    /// 0x08cd677e
    UNK_RET NumElementsPerTransform(granny_mesh const* a1) const;
    /// 0x08cd683c
    UNK_RET NumElementsPerTransform(granny_material const* a1) const;
    /// 0x08cd68a2
    UNK_RET InitTextures(granny_material const** a1, int a2);
    /// 0x08cd6930
    UNK_RET SetWorkingSPICreator(BuilderContextSPICreator* a1, FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>* a2);
    /// 0x08cd698a
    UNK_RET CalcShaderPackageInstanceViewIndex(FireGrafix::FGXShaderPackageInstanceView* a1);
};
