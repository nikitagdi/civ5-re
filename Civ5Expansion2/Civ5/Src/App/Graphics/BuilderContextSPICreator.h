/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFastVector.h"
#include <granny.h>

class DBCMaterialEntry;
namespace FireGrafix {
class FGXShaderPackageInstanceView;
enum FGX_SPIV_GENERIC : int;
}

/// size ?
/// vtable 0x0a301c30
class BuilderContextSPICreator {
public:
    /// 0x08cd7038
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x088af9b4
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cd703c
    virtual UNK_RET vf_02(UNK_PARAMS);
    /// 0x08cd6144
    virtual UNK_RET GetViewIDFromMaterialEntry(DBCMaterialEntry const& a1);
    /// 0x08cd614a
    virtual UNK_RET OnModel(granny_model const* a1);
    /// 0x08cd614c
    virtual UNK_RET OnMesh(granny_mesh const* a1);
    /// 0x08cd614e
    virtual UNK_RET GetWorkingSPIIndex(FireGrafix::FGXShaderPackageInstanceView* a1);
    /// 0x08cd6178
    virtual UNK_RET GetWorkingSPIVectorSize();
    /// 0x08cd618a
    virtual UNK_RET ReserveWorkingSPIVectorSize(unsigned int a1);
    /// 0x08cd61c0
    virtual UNK_RET SetWorkingSPIVector(FFastVector<FireGrafix::FGX_SPIV_GENERIC, false, 29u, 0u, BaseVector<FireGrafix::FGX_SPIV_GENERIC, false>::FDefaultFastVectorAllocator>* a1);
    /// 0x08cd61cc
    virtual UNK_RET SetMaterialEntryArray(DBCMaterialEntry* a1, unsigned int a2);
    /// 0x08cd61ce
    virtual UNK_RET GetTransformTypeElementCount(DBCMaterialEntry const& a1);
    /// 0x08cd61d4
    UNK_RET AddToWorkingSPIVector(unsigned int a1);
    /// 0x08cd6250
    UNK_RET AddToWorkingSPIVector(FireGrafix::FGXShaderPackageInstanceView const* a1);
    /// 0x08cd628c
    UNK_RET FindWorkingSPIOfType(unsigned int a1);
};
