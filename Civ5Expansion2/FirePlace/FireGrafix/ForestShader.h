/// origin guess
#pragma once

namespace FireGrafix {
class ForestDataStatic;

/// size 0x34
class ForestShader {
public:
    enum SHADER_METHOD : int;
    /// 0x0877deda
    ForestShader();
    /// 0x0a30d134
    static UNK_DATA pPixelTextureReflection[0x100];

    char unk_0[0x14];                 ///< +0x0
    ForestDataStatic* m_pForestData;  ///< +0x14
    char unk_18[0x18];                ///< +0x18
    unsigned int m_uiNextFree;        ///< +0x30
};
}
