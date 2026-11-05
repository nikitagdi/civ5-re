/// origin guess
#include "FirePlace/FireWorks/FFSMConditionECComplete.h"
#include "FirePlace/FireWorks/FXmlElement.h"

/// 0x08cf1280
FFSMConditionECComplete::FFSMConditionECComplete()
    : m_usEventCode(0), m_fValue(0.98f), m_fDuration(0.0f) {
}

/// 0x08cf1362
void FFSMConditionECComplete::OnLoad(FXmlElement const& a1) {
    FIFSMCondition::OnLoad(a1);
    int eventCode = 0;
    a1.GetFirstAttributeVal(eventCode, "ec", 2);
    a1.GetFirstAttributeVal(m_fValue, "value", 5);
    m_usEventCode = (unsigned short)eventCode;
}

/// 0x08cf13e4
void FFSMConditionECComplete::OnSave(FXmlElement& a1) const {
    a1.SetAttributeVal("ec", 2, m_usEventCode);
    a1.SetAttributeVal("value", 5, m_fValue);
    FIFSMCondition::OnSave(a1);
}

/// 0x08cf133c
float FFSMConditionECComplete::get_value() const {
    return m_fValue;
}

/// 0x08cf1344
void FFSMConditionECComplete::set_value(float a1) {
    m_fValue = a1;
}

/// 0x08cf1354
void FFSMConditionECComplete::set_event_code(unsigned short a1) {
    m_usEventCode = a1;
}
