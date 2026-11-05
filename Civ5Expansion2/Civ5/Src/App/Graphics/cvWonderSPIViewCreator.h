/// origin guess
#pragma once
#include "Civ5/Src/App/ShaderPackageInstanceViewCreator.h"
#include <granny.h>

/// size ?
/// vtable 0x0a2fd670
class cvWonderSPIViewCreator : public ShaderPackageInstanceViewCreator {
public:
    /// 0x0886618c
    virtual UNK_RET OnModel(granny_model const* a1);
    /// 0x08866098
    virtual UNK_RET vf_00(UNK_PARAMS);
};
