/// origin guess
#pragma once

namespace FireGrafix {
/// size ?
class WorldCurrents {
public:
    /// 0x08781792
    WorldCurrents();
    /// 0x0a30f734
    static UNK_DATA pPixelTextureReflection[0x100];
};
}

namespace FireGrafix {
// Records are scanned from the last one down; the first match wins.
/// size 0x8
struct ShaderPackageNameEntry {
    char const* pszName;       ///< +0x0
    int nPackageID;            ///< +0x4
};
/// 0x08781869
UNK_RET initFireGrafix_ShaderDefDX9();
/// 0x0878bba1
void* dispatchShaderPackageCreate_ShaderDefDX9(unsigned int a1);
/// 0x0878c786
void* dispatchShaderPackageCreateInPlace_ShaderDefDX9(unsigned int a1, void* a2);
/// 0x0a2f2700
extern ShaderPackageNameEntry g_ShaderPackageNameTable[0x5e];
/// 0x0878cf2d
int dispatchShaderPackageGetID_ShaderDefDX9(char const* a1);
/// 0x0878cf85
UNK_RET RecompileShaders_ShaderDefDX9();
/// 0x0a30f834
extern void* g_pShaderDefDX9_SPReflectionData[0x5e];
}
