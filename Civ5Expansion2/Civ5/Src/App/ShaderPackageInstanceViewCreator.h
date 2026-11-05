/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/BuilderContextSPICreator.h"
#include "Civ5/Src/App/ShaderPackageInstanceCreator.h"
#include <granny.h>

class DBCMaterialEntry;

/// size ?
/// vtable 0x0a2f8f60
class ShaderPackageInstanceViewCreator : public ShaderPackageInstanceCreator, public BuilderContextSPICreator {
public:
    /// 0x0866fb12
    virtual UNK_RET GetTransformTypeElementCount(DBCMaterialEntry const& a1);
    /// 0x0866fa76
    virtual UNK_RET GetViewIDFromMaterialEntry(DBCMaterialEntry const& a1);
    /// 0x0866faba
    virtual UNK_RET OnModel(granny_model const* a1);
    /// 0x0866fafe
    virtual UNK_RET SetMaterialEntryArray(DBCMaterialEntry* a1, unsigned int a2);
    /// 0x0866f9ac
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x0866f9f6
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x0866fa48
    virtual UNK_RET vf_02(UNK_PARAMS);
};
