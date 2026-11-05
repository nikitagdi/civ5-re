/// origin guess
#pragma once
#include "Civ5/Src/App/Graphics/BuilderContextSPICreator.h"
#include "Civ5/Src/App/ShaderPackageInstanceCreator.h"

/// size ?
/// vtable 0x0a2fda10
class cvPopBuildingSPICreator : public BuilderContextSPICreator, private ShaderPackageInstanceCreator {
public:
    /// 0x088af9aa
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x088af9b8
    virtual UNK_RET vf_02(UNK_PARAMS);
};
