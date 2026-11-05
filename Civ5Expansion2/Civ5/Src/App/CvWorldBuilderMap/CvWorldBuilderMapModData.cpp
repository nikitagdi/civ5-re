/// origin binary
#include "Civ5/Src/App/CvWorldBuilderMap/CvWorldBuilderMapModData.h"

/// 0x08cab0e8
bool ModDataEntry::GetFieldAsBool(uint uiField) const {
    return m_aFields[uiField] != 0;
}

/// 0x08cab0fe
int ModDataEntry::GetFieldAsInt(uint uiField) const {
    return m_aFields[uiField];
}

/// 0x08cab10e
float ModDataEntry::GetFieldAsFloat(uint uiField) const {
    // Float fields are stored as their IEEE bit pattern.
    return *reinterpret_cast<const float *>(&m_aFields[uiField]);
}

/// 0x08cab150
bool ModDataEntry::IsFieldDefaultValue(uint uiField) const {
    return m_aFields[uiField] == m_kModType.m_kFields[uiField].dwDefault;
}

/// 0x08caace4
uint ModType::GetFieldCount() const {
    return m_kFields.size();
}

/// 0x08caad28
ModType::FieldType ModType::GetFieldType(uint uiField) const {
    if (uiField >= m_kFields.size()) {
        return TYPE_INVALID;
    }
    const dword dwType = m_kFields[uiField].dwFieldType;
    return dwType <= static_cast<dword>(TYPE_ENUM) ? static_cast<FieldType>(dwType) : TYPE_ENUM;
}

/// 0x08caad50
bool ModType::GetFieldDefaultAsBool(uint uiField) const {
    if (uiField >= m_kFields.size()) {
        return false;
    }
    return m_kFields[uiField].dwDefault != 0;
}

/// 0x08caad74
int ModType::GetFieldDefaultAsInt(uint uiField) const {
    if (uiField >= m_kFields.size()) {
        return 0;
    }
    return m_kFields[uiField].dwDefault;
}

/// 0x08caad8e
float ModType::GetFieldDefaultAsFloat(uint uiField) const {
    // Default float values are stored as their IEEE bit pattern.
    if (uiField >= m_kFields.size()) {
        return 0.0f;
    }
    return *reinterpret_cast<const float *>(&m_kFields[uiField].dwDefault);
}

/// 0x08caadee
const ModEnum *ModType::GetFieldEnumType(uint uiField) const {
    if (uiField >= m_kFields.size()) {
        return nullptr;
    }
    const dword dwType = m_kFields[uiField].dwFieldType;
    if (dwType < static_cast<dword>(TYPE_ENUM)) {
        return nullptr;
    }
    const uint uiEnum = dwType - static_cast<dword>(TYPE_ENUM);
    if (uiEnum >= m_kEnums.size()) {
        return nullptr;
    }
    return &m_kEnums[uiEnum];
}

/// 0x08cab302
ModDataEntry CvWorldBuilderMapModData::GetMapDataEntry() {
    return ModDataEntry(m_kMapModData, m_kData.begin());
}

/// 0x08cab318
ModDataEntry CvWorldBuilderMapModData::GetPlotDataEntry(uint uiPlot) {
    const uint uiPlotFieldCount = m_kPlotModData.m_kFields.size();
    const uint uiMapFieldCount = m_kMapModData.m_kFields.size();
    return ModDataEntry(m_kPlotModData, m_kData.begin() + uiPlotFieldCount * uiPlot + uiMapFieldCount);
}

/// 0x08caa998
const char *ModEnum::GetMember(uint uiMember) const {
    if (uiMember >= m_kMembers.size() || m_pkStringBuffer == nullptr) {
        return nullptr;
    }
    return m_pkStringBuffer->GetString(m_kMembers[uiMember]);
}

/// 0x08cab11e
const char *ModDataEntry::GetFieldAsString(uint uiField) const {
    return m_kModType.m_kStringBuffer.GetString(m_aFields[uiField]);
}

/// 0x08caacec
const char *ModType::GetFieldName(uint uiField) const {
    if (uiField >= m_kFields.size()) {
        return nullptr;
    }
    return m_kStringBuffer.GetString(m_kFields[uiField].dwFieldName);
}

/// 0x08caadb6
const char *ModType::GetFieldDefaultAsString(uint uiField) const {
    if (uiField >= m_kFields.size()) {
        return "";
    }
    return m_kStringBuffer.GetString(m_kFields[uiField].dwDefault);
}
