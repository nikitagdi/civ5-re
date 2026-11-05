/// origin guess
#pragma once
#include "FirePlace/FireGrafix/FGXShaderPackageInstanceView.h"

namespace FireGrafix {
// m_pConstantSet is g_pBuildingGlobal; m_pDynamicConstantSet is a BuildingDataDynamic created by the constructor.
/// size 0x30
class BuildingShader_CODE : public FGXShaderPackageInstanceView {
public:
    /// 0x0877a4de
    BuildingShader_CODE();
    /// 0x0a30ac34
    static UNK_DATA pPixelTextureReflection[0x100];
};
}
