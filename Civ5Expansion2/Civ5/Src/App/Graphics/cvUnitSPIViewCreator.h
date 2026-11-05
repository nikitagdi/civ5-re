/// origin guess
#pragma once
#include "Civ5/Src/App/ShaderPackageInstanceViewCreator.h"
#include <d3d9.h>

class FGXVector4;

/// size ?
/// vtable 0x0a2fdd30
class cvUnitSPIViewCreator : public ShaderPackageInstanceViewCreator {
public:
    /// 0x089315ac
    virtual UNK_RET vf_00(UNK_PARAMS);
};

/// 0x08933c0e
UNK_RET BuildTransformMatrix(D3DXMATRIX& a1, float a2, float a3, float a4, FGXVector4 const& a5);
