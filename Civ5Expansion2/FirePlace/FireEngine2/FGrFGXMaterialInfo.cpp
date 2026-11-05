/// origin guess
#include "FGrFGXMaterialInfo.h"

#include <cstring>

namespace {

// granny_material at +0x10 holds its extended data (type table, object).
struct MaterialExtendedData {
    unsigned char unk_0[0x10];
    void const* m_type;
    void const* m_object;
};

// The count of parameter sets sits at a different offset and width per version.
const auto GetParamSetCount = [](FGrFGXMaterialInfo const& info) -> int {
    if (info.m_version == 3) {
        return *reinterpret_cast<int const*>(reinterpret_cast<unsigned char const*>(&info) + 0x78);
    }
    if (info.m_version == 4) {
        return *reinterpret_cast<short const*>(reinterpret_cast<unsigned char const*>(&info) + 0x7a);
    }
    return *reinterpret_cast<short const*>(reinterpret_cast<unsigned char const*>(&info) + 0x76);
};

const auto IsTextureSemantic = [](int semantic) -> bool {
    return semantic >= 0 && semantic <= 10 && ((0x4c0u >> semantic) & 1) != 0;
};

}

/// 0x08cd5b20
UNK_RET FGrFGXMaterialInfo::FGrFGXShaderSetInfoV0::Release() {
    std::memset(this, 0, 0x74);
}

/// 0x08cd5b30
UNK_RET FGrFGXMaterialInfo::FGrFGXShaderSetInfoV2::Release() {
    std::memset(this, 0, 0x74);
}

/// 0x08cd5b40
UNK_RET FGrFGXMaterialInfo::FGrFGXShaderSetInfoV3::Release() {
    std::memset(this, 0, 0x78);
}

/// 0x08cd5b50
UNK_RET FGrFGXMaterialInfo::FGrFGXShaderSetInfoV4::Release() {
    std::memset(this, 0, 0x78);
}

/// 0x08cd5b60
FGrFGXMaterialInfo::FGrFGXMaterialInfo() {
    std::memset(this, 0, 0x78);
}

/// 0x08cd5b70
bool FGrFGXMaterialInfo::ReadShaderInformation(granny_material const* material) {
    if (material == nullptr) {
        return false;
    }
    MaterialExtendedData const& extended = *reinterpret_cast<MaterialExtendedData const*>(material);
    void* memberOut[3];
    if (GrannyFindMatchingMember(extended.m_type, extended.m_object, "Version", memberOut)) {
        GrannyConvertSingleObject(extended.m_type, extended.m_object, FGrFGXMaterialVersion, this);
    }
    if (!GrannyFindMatchingMember(extended.m_type, extended.m_object, "typeName", memberOut)) {
        return false;
    }
    char const* typeName = nullptr;
    GrannyConvertSingleObject(extended.m_type, extended.m_object, FGrFGXMaterialTypeName, &typeName);
    if (typeName == nullptr) {
        return false;
    }
    unsigned char const* typeTable;
    int expectedSize;
    switch (m_version) {
    case 0:
    case 1:
        typeTable = FGrMaterialInfoConverterCustAttribsTypeV0;
        expectedSize = 0x74;
        break;
    case 2:
        typeTable = FGrMaterialInfoConverterCustAttribsTypeV2;
        expectedSize = 0x74;
        break;
    case 3:
        typeTable = FGrMaterialInfoConverterCustAttribsTypeV3;
        expectedSize = 0x7c;
        break;
    case 4:
        typeTable = FGrMaterialInfoConverterCustAttribsTypeV4;
        expectedSize = 0x7c;
        break;
    default:
        return false;
    }
    if (GrannyGetTotalObjectSize(typeTable) != expectedSize) {
        return false;
    }
    GrannyConvertSingleObject(extended.m_type, extended.m_object, typeTable, &m_packageName);
    return true;
}

/// 0x08cd5cae
char const* FGrFGXMaterialInfo::GetPackageName() const {
    return m_packageName;
}

/// 0x08cd5cb6
void FGrFGXMaterialInfo::CopyConstantSetData(char const* setName, unsigned char* dest) const {
    int const setCount = GetParamSetCount(*this);
    for (int set = 0; set < setCount; ++set) {
        FGrFGXParamSetInfoV0 const* paramSet = m_paramSets[set];
        if (paramSet->m_hasData == 0 || std::strcmp(setName, paramSet->m_name) != 0) {
            continue;
        }
        for (int param = 0; param < paramSet->m_paramCount; ++param) {
            if (paramSet->m_overrideIndex[param] != -1) {
                continue;
            }
            unsigned int const semantic = static_cast<unsigned int>(paramSet->m_semantics[param]);
            if (semantic >= 6) {
                continue;
            }
            float const* source;
            if ((0x31u >> semantic) & 1) {
                source = paramSet->m_poolA + paramSet->m_poolIndex[param];
            } else if (((0xeu >> semantic) & 1) && semantic != 2) {
                source = paramSet->m_poolB + paramSet->m_poolIndex[param];
            } else {
                continue;
            }
            std::memcpy(dest, source, paramSet->m_byteSizes[param]);
            dest += paramSet->m_byteSizes[param];
        }
    }
}

/// 0x08cd5dde
void FGrFGXMaterialInfo::GetParamSetInfo(int& count, FGrFGXParamSetInfoV0* const*& sets) const {
    count = GetParamSetCount(*this);
    sets = m_paramSets;
}

/// 0x08cd5e14
void FGrFGXMaterialInfo::CopyAlphaZStateData(int& stateA, int& stateB, int& stateC) const {
    switch (m_version) {
    case 0:
    case 1:
        stateA = *reinterpret_cast<int const*>(reinterpret_cast<unsigned char const*>(this) + 0x68);
        stateB = *reinterpret_cast<int const*>(reinterpret_cast<unsigned char const*>(this) + 0x70);
        stateC = *reinterpret_cast<int const*>(reinterpret_cast<unsigned char const*>(this) + 0x6c);
        break;
    case 2:
    case 3:
    case 4:
        stateA = *reinterpret_cast<short const*>(reinterpret_cast<unsigned char const*>(this) + 0x68);
        stateB = *reinterpret_cast<short const*>(reinterpret_cast<unsigned char const*>(this) + 0x6c);
        stateC = *reinterpret_cast<short const*>(reinterpret_cast<unsigned char const*>(this) + 0x6a);
        break;
    default:
        break;
    }
}

/// 0x08cd5ea0
char const* FGrFGXMaterialInfo::GetTextureFilename(unsigned int slot) const {
    int const setCount = GetParamSetCount(*this);
    for (int set = 0; set < setCount; ++set) {
        FGrFGXParamSetInfoV0 const* paramSet = m_paramSets[set];
        if (paramSet == nullptr || paramSet->m_hasData == 0) {
            continue;
        }
        for (int param = 0; param < paramSet->m_paramCount; ++param) {
            if (!IsTextureSemantic(paramSet->m_semantics[param]) || paramSet->m_slotPairs[param * 2] != slot) {
                continue;
            }
            int const nameIndex = paramSet->m_poolIndex[param];
            if (nameIndex < paramSet->m_textureNameCount) {
                return paramSet->m_textureNames[nameIndex];
            }
            return nullptr;
        }
    }
    return nullptr;
}

/// 0x08cd5f4a
unsigned int FGrFGXMaterialInfo::FindTextures(FGrFGXMaterialInfo::TextureBindingInfo* out) const {
    int const setCount = GetParamSetCount(*this);
    unsigned int found = 0;
    for (int set = 0; set < setCount; ++set) {
        FGrFGXParamSetInfoV0 const* paramSet = m_paramSets[set];
        if (paramSet == nullptr || paramSet->m_hasData == 0) {
            continue;
        }
        for (int param = 0; param < paramSet->m_paramCount; ++param) {
            if (!IsTextureSemantic(paramSet->m_semantics[param])) {
                continue;
            }
            TextureBindingInfo& binding = out[found];
            binding.m_isSemantic7 = paramSet->m_semantics[param] == 7;
            binding.m_slotHi = paramSet->m_slotPairs[param * 2 + 1];
            binding.m_slot = paramSet->m_slotPairs[param * 2];
            binding.m_filename = paramSet->m_textureNames[paramSet->m_poolIndex[param]];
            binding.m_paramInt = paramSet->m_paramInts[param];
            ++found;
        }
    }
    return found;
}

/// 0x08cd6032
bool FGrFGXMaterialInfo::IsSkinned() const {
    if (m_version == 3) {
        return *reinterpret_cast<short const*>(reinterpret_cast<unsigned char const*>(this) + 0x76) > 0;
    }
    if (m_version == 4) {
        return *reinterpret_cast<short const*>(reinterpret_cast<unsigned char const*>(this) + 0x78) > 0;
    }
    return *reinterpret_cast<short const*>(reinterpret_cast<unsigned char const*>(this) + 0x74) > 0;
}

/// 0x08cd6060
bool FGrFGXMaterialInfo::HasParameters() const {
    return GetParamSetCount(*this) > 0;
}

/// 0x08cd608e
bool FGrFGXMaterialInfo::UseReflections() const {
    return (m_version == 2 || m_version == 3 || m_version == 4) && m_useReflections != 0;
}

/// 0x08cd60b4
bool FGrFGXMaterialInfo::UseShadows() const {
    return (m_version == 2 || m_version == 3 || m_version == 4) && m_useShadows != 0;
}

/// 0x08cd60da
bool FGrFGXMaterialInfo::UseAlphaSort() const {
    return (m_version == 2 || m_version == 3 || m_version == 4) && m_useAlphaSort != 0;
}

/// 0x08cd6100
bool FGrFGXMaterialInfo::UseInvisible() const {
    return (m_version == 3 || m_version == 4) && m_useInvisible != 0;
}

/// 0x08cd6120
bool FGrFGXMaterialInfo::UseZFail() const {
    return m_version == 4 && m_useZFail != 0;
}

/// 0x08cd613a
int FGrFGXMaterialInfo::GetVersion() const {
    return m_version;
}
