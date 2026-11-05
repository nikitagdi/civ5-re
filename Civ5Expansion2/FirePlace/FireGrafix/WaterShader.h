/// origin guess
#pragma once

namespace FireGrafix {
class FGXConstantSet;

/// size 0x30
class WaterShader {
public:
    // Order of the (VS, PS) pairs in the package record at CRenderer9::m_ShaderPackages + 91 * 0x20c.
    enum Pass : int {
        PASS_OCEAN_LOW,
        PASS_RIVER_LOW,
        PASS_OCEAN_MEDIUM,
        PASS_OCEAN_HIGH,
        PASS_RIVER_HIGH,
        PASS_CHECK_TEXTURE,
        PASS_COUNT,
    };
    /// 0x08781526
    WaterShader();
    int m_iPackageId; ///< +0x0
    unsigned int m_uPackageInstance; ///< +0x4
    unsigned char unk_008[0x4]; ///< +0x8
    char const* m_pszName; ///< +0xc
    FGXConstantSet* m_pGlobalData; ///< +0x10
    FGXConstantSet* m_pInstanceData; ///< +0x14
    unsigned char unk_018[0x18]; ///< +0x18
    /// 0x0a30f534
    static UNK_DATA pPixelTextureReflection[0x100];
};
}
