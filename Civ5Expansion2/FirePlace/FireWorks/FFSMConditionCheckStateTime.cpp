/// origin guess
#include "FirePlace/FireWorks/FFSMConditionCheckStateTime.h"
#include "FirePlace/FireWorks/FXmlElement.h"

/// 0x08dae462
void FFSMConditionCheckStateTime::OnLoad(FXmlElement const& a1) {
    a1.GetFirstAttributeVal(m_fOrder, "order", 5);
    int method = 0;
    a1.GetFirstAttributeVal(m_fValue, "value", 5);
    a1.GetFirstAttributeVal(method, "method", 6);
    m_method = (FCompareValues::Method)method;
}

/// 0x08dae4f6
void FFSMConditionCheckStateTime::OnSave(FXmlElement& a1) const {
    a1.SetAttributeVal("order", 5, m_fOrder);
    a1.SetAttributeVal("value", 5, m_fValue);
    a1.SetAttributeVal("method", 6, m_method);
}
