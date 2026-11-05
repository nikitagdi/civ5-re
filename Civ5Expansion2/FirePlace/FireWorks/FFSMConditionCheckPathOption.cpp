/// origin guess
#include "FirePlace/FireWorks/FFSMConditionCheckPathOption.h"
#include "FirePlace/FireWorks/FXmlElement.h"

/// 0x08dae63e
void FFSMConditionCheckPathOption::OnLoad(FXmlElement const& a1) {
    a1.GetFirstAttributeVal(m_fOrder, "order", 5);
    int path = 0;
    int method = 0;
    a1.GetFirstAttributeVal(m_fValue, "value", 5);
    a1.GetFirstAttributeVal(path, "path", 4);
    a1.GetFirstAttributeVal(method, "method", 6);
    m_path = (FIFiniteStateMachineInstance::path_option)path;
    m_method = (FCompareValues::Method)method;
}

/// 0x08dae704
void FFSMConditionCheckPathOption::OnSave(FXmlElement& a1) const {
    a1.SetAttributeVal("order", 5, m_fOrder);
    a1.SetAttributeVal("value", 5, m_fValue);
    a1.SetAttributeVal("path", 4, m_path);
    a1.SetAttributeVal("method", 6, m_method);
}
