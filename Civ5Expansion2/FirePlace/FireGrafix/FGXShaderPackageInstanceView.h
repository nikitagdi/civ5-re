/// origin guess
#pragma once

namespace FireGrafix {
class FGXConstantSet;

// Generated shader classes (TerrainShader, CompositeShader, ...) fill it in their constructors; +0x14.. holds their global-data pointers.
/// size 0x30
class FGXShaderPackageInstanceView {
public:
    /// 0x08d13df0
    ~FGXShaderPackageInstanceView();

    unsigned int m_uPackage;   ///< +0x0
    unsigned int m_hInstance;  ///< +0x4
    int field_8;               ///< +0x8
    char const* m_pszName;     ///< +0xc
    FGXConstantSet* m_pConstantSet; ///< +0x10
    FGXConstantSet* m_pDynamicConstantSet; ///< +0x14
    char unk_18[0x18];         ///< +0x18
};
}
