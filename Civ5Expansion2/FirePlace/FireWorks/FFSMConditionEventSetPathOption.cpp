/// origin guess
#include "FirePlace/FireWorks/FFSMConditionEventSetPathOption.h"
#include "FirePlace/FireWorks/FXmlElement.h"

/// 0x08dade0e
void FFSMConditionEventSetPathOption::OnLoad(FXmlElement const& a1) {
    a1.GetFirstAttributeVal(m_fOrder, "order", 5);
    int path = 0;
    a1.GetFirstAttributeVal(m_fValue, "value", 5);
    a1.GetFirstAttributeVal(path, "path", 4);
    m_path = (FIFiniteStateMachineInstance::path_option)path;
}

/// 0x08dadea2
void FFSMConditionEventSetPathOption::OnSave(FXmlElement& a1) const {
    a1.SetAttributeVal("order", 5, m_fOrder);
    a1.SetAttributeVal("value", 5, m_fValue);
    a1.SetAttributeVal("path", 4, m_path);
}
