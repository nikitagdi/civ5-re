/// origin guess
#pragma once
#include "FirePlace/FireGrafix/FGXShaderPackageInstanceView.h"

namespace FireGrafix {
/// size 0x30
class TerrainShader : public FGXShaderPackageInstanceView {
public:
    /// 0x08780070
    TerrainShader();
    /// 0x0a3e69f8
    static UNK_DATA pPixelTextureReflection[0x100];
};
}
