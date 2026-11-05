/// origin guess
#include "FirePlace/FireWorks/FFSMConditionVariable.h"
#include "FirePlace/FireWorks/FXmlElement.h"

#include <cstring>

/// 0x08dad408
void FFSMConditionVariable::OnSave(FXmlElement& a1) const {
    a1.SetAttributeVal("order", 5, m_fOrder);
    a1.SetAttributeVal("var", 3, m_szName);
    a1.SetAttributeVal("value", 5, m_fValue);
}

/// 0x08dad32a
void FFSMConditionVariable::set_name(char const* a1) {
    if (std::strlen(a1) < 0x10) {
        std::strcpy(m_szName, a1);
    } else {
        m_szName[0] = '\0';
    }
    m_pEntry = 0;
}
