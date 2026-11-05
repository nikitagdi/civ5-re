/// origin guess
#pragma once
#include "FirePlace/FireGrafix/SVFontShader.h"
#include <granny.h>

/// size ?
class FGrFGXParamSetInfoV0 {
public:
    char const* m_name; ///< +0x0
    int m_paramCount; ///< +0x4
    unsigned char unk_008[0x4]; ///< +0x8
    int* m_paramInts; ///< +0xc
    unsigned char unk_010[0x4]; ///< +0x10
    int* m_semantics; ///< +0x14
    unsigned char unk_018[0xc]; ///< +0x18
    unsigned short* m_slotPairs; ///< +0x24
    unsigned char unk_028[0x4]; ///< +0x28
    int* m_byteSizes; ///< +0x2c
    unsigned char unk_030[0x4]; ///< +0x30
    int* m_overrideIndex; ///< +0x34
    unsigned char unk_038[0x4]; ///< +0x38
    int* m_poolIndex; ///< +0x3c
    unsigned char unk_040[0x4]; ///< +0x40
    float* m_poolA; ///< +0x44
    unsigned char unk_048[0x4]; ///< +0x48
    float* m_poolB; ///< +0x4c
    int m_textureNameCount; ///< +0x50
    char const** m_textureNames; ///< +0x54
    int m_hasData; ///< +0x58
};

/// size ?
class FGrFGXMaterialInfo {
public:
    class FGrFGXShaderSetInfoV0;
    class FGrFGXShaderSetInfoV2;
    class FGrFGXShaderSetInfoV3;
    class FGrFGXShaderSetInfoV4;
    // The four ShaderSetInfo classes are the Granny extended-data structs that ReadShaderInformation
    // converts into the material info at +0x4; offsets are relative to that start.
    /// size 0x74
    class FGrFGXShaderSetInfoV0 {
    public:
        char const* m_packageName; ///< +0x0
        FGrFGXParamSetInfoV0* m_paramSets[0x18]; ///< +0x4
        int m_stateA; ///< +0x64
        int m_stateC; ///< +0x68
        int m_stateB; ///< +0x6c
        short m_skinned; ///< +0x70
        short m_paramSetCount; ///< +0x72
        /// 0x08cd5b20
        UNK_RET Release();
    };
    /// size 0x74
    class FGrFGXShaderSetInfoV2 {
    public:
        char const* m_packageName; ///< +0x0
        FGrFGXParamSetInfoV0* m_paramSets[0x18]; ///< +0x4
        short m_stateA; ///< +0x64
        short m_stateC; ///< +0x66
        short m_stateB; ///< +0x68
        unsigned short m_useReflections; ///< +0x6a
        unsigned short m_useShadows; ///< +0x6c
        unsigned short m_useAlphaSort; ///< +0x6e
        short m_skinned; ///< +0x70
        short m_paramSetCount; ///< +0x72
        /// 0x08cd5b30
        UNK_RET Release();
    };
    /// size 0x7c
    class FGrFGXShaderSetInfoV3 {
    public:
        char const* m_packageName; ///< +0x0
        FGrFGXParamSetInfoV0* m_paramSets[0x18]; ///< +0x4
        short m_stateA; ///< +0x64
        short m_stateC; ///< +0x66
        short m_stateB; ///< +0x68
        unsigned short m_useReflections; ///< +0x6a
        unsigned short m_useShadows; ///< +0x6c
        unsigned short m_useAlphaSort; ///< +0x6e
        unsigned short m_useInvisible; ///< +0x70
        short m_skinned; ///< +0x72
        int m_paramSetCount; ///< +0x74
        unsigned char unk_078[0x4]; ///< +0x78
        /// 0x08cd5b40
        UNK_RET Release();
    };
    /// size 0x7c
    class FGrFGXShaderSetInfoV4 {
    public:
        char const* m_packageName; ///< +0x0
        FGrFGXParamSetInfoV0* m_paramSets[0x18]; ///< +0x4
        short m_stateA; ///< +0x64
        short m_stateC; ///< +0x66
        short m_stateB; ///< +0x68
        unsigned short m_useReflections; ///< +0x6a
        unsigned short m_useShadows; ///< +0x6c
        unsigned short m_useAlphaSort; ///< +0x6e
        unsigned short m_useInvisible; ///< +0x70
        unsigned short m_useZFail; ///< +0x72
        short m_skinned; ///< +0x74
        short m_paramSetCount; ///< +0x76
        unsigned char unk_078[0x4]; ///< +0x78
        /// 0x08cd5b50
        UNK_RET Release();
    };
    /// size 0x10
    class TextureBindingInfo {
    public:
        bool m_isSemantic7; ///< +0x0
        unsigned short m_slotHi; ///< +0x2
        unsigned short m_slot; ///< +0x4
        char const* m_filename; ///< +0x8
        int m_paramInt; ///< +0xc
    };
    int m_version; ///< +0x0
    char const* m_packageName; ///< +0x4
    FGrFGXParamSetInfoV0* m_paramSets[0x18]; ///< +0x8
    // From +0x68 the layout follows the ShaderSetInfo class of m_version (V0/V1: ints at +0x68/+0x6c/+0x70).
    unsigned short m_stateA; ///< +0x68
    unsigned short m_stateC; ///< +0x6a
    unsigned short m_stateB; ///< +0x6c
    unsigned short m_useReflections; ///< +0x6e
    unsigned short m_useShadows; ///< +0x70
    unsigned short m_useAlphaSort; ///< +0x72
    unsigned short m_useInvisible; ///< +0x74
    unsigned short m_useZFail; ///< +0x76
    unsigned short m_word78; ///< +0x78
    unsigned short m_word7a; ///< +0x7a
    unsigned char unk_07c[0x4]; ///< +0x7c
    /// 0x08cd5b60
    FGrFGXMaterialInfo();
    /// 0x08cd5b70
    bool ReadShaderInformation(granny_material const* material);
    /// 0x08cd5cae
    char const* GetPackageName() const;
    /// 0x08cd5cb6
    void CopyConstantSetData(char const* setName, unsigned char* dest) const;
    /// 0x08cd5dde
    void GetParamSetInfo(int& count, FGrFGXParamSetInfoV0* const*& sets) const;
    /// 0x08cd5e14
    void CopyAlphaZStateData(int& stateA, int& stateB, int& stateC) const;
    /// 0x08cd5ea0
    char const* GetTextureFilename(unsigned int slot) const;
    /// 0x08cd5f4a
    unsigned int FindTextures(FGrFGXMaterialInfo::TextureBindingInfo* out) const;
    /// 0x08cd6032
    bool IsSkinned() const;
    /// 0x08cd6060
    bool HasParameters() const;
    /// 0x08cd608e
    bool UseReflections() const;
    /// 0x08cd60b4
    bool UseShadows() const;
    /// 0x08cd60da
    bool UseAlphaSort() const;
    /// 0x08cd6100
    bool UseInvisible() const;
    /// 0x08cd6120
    bool UseZFail() const;
    /// 0x08cd613a
    int GetVersion() const;
};

extern "C" {
/// 0x08df6ca9
int GrannyGetTotalObjectSize(void const* typeTable);
/// 0x08df7ebd
bool GrannyFindMatchingMember(void const* typeTable, void const* object, char const* memberName, void* outMember);
/// 0x08df7ec2
void GrannyConvertSingleObject(void const* srcType, void const* srcObject, void const* dstType, void* dstObject);
}
