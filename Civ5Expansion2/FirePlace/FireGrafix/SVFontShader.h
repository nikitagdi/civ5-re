/// origin guess
#pragma once

namespace FireGrafix {
/// size ?
class SVFontShader {
public:
    /// 0x08c96772
    SVFontShader();
    /// 0x0a3116e0
    static UNK_DATA pPixelTextureReflection[0x100];
};
}

extern "C" {
    /// 0x0a312974
    extern unsigned char FGrFGXMaterialTypeName[0x40];
    /// 0x0a312934
    extern unsigned char FGrFGXMaterialVersion[0x40];
    /// 0x0a3117f4
    extern unsigned char FGrMaterialInfoConverterCustAttribsTypeV0[0x3e0];
    /// 0x0a311bd4
    extern unsigned char FGrMaterialInfoConverterCustAttribsTypeV2[0x440];
    /// 0x0a312014
    extern unsigned char FGrMaterialInfoConverterCustAttribsTypeV3[0x480];
    /// 0x0a312494
    extern unsigned char FGrMaterialInfoConverterCustAttribsTypeV4[0x4a0];
}

namespace FGranny {
/// 0x0a3117ec
extern UNK_DATA ms_uiMaximumBones[0x4];
/// 0x0a3117f0
extern UNK_DATA ms_uiMaximumDAGDepth[0x4];
}

namespace FireGrafix {
/// 0x08c968a8
UNK_RET initFireGrafix_StrategicViewShaderDefDX9();
/// 0x08c97746
UNK_RET dispatchShaderPackageCreate_StrategicViewShaderDefDX9(unsigned int a1);
/// 0x08c977d8
UNK_RET dispatchShaderPackageCreateInPlace_StrategicViewShaderDefDX9(unsigned int a1, void* a2);
/// 0x08c97839
UNK_RET dispatchShaderPackageGetID_StrategicViewShaderDefDX9(char const* a1);
/// 0x08c97891
UNK_RET RecompileShaders_StrategicViewShaderDefDX9();
/// 0x0a3117e0
extern UNK_DATA g_pStrategicViewShaderDefDX9_SPReflectionData[0xc];
}
