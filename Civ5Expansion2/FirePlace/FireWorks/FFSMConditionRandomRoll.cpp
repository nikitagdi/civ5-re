/// origin guess
#include "FirePlace/FireWorks/FFSMConditionRandomRoll.h"
#include "FirePlace/FireWorks/FXmlElement.h"

/// 0x08dadbce
void FFSMConditionRandomRoll::OnLoad(FXmlElement const& a1) {
    a1.GetFirstAttributeVal(m_fOrder, "order", 5);
    int invert = 0;
    a1.GetFirstAttributeVal(invert, "invert", 6);
    a1.GetFirstAttributeVal(m_fThreshold, "value", 5);
    m_bInvert = invert != 0;
}

/// 0x08dadc64
void FFSMConditionRandomRoll::OnSave(FXmlElement& a1) const {
    a1.SetAttributeVal("order", 5, m_fOrder);
    a1.SetAttributeVal("invert", 6, m_bInvert);
    a1.SetAttributeVal("value", 5, m_fThreshold);
}
