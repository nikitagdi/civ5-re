/// origin guess
#include "FirePlace/FireWorks/FIFSMCondition.h"
#include "FirePlace/FireWorks/FXmlElement.h"

/// 0x08dad064
FIFSMCondition::FIFSMCondition()
    : m_iUniqueId(0), m_fOrder(0.0f), m_pDescription(0) {
}

/// 0x08dad1b0
void FIFSMCondition::OnLoad(FXmlElement const& a1) {
    a1.GetFirstAttributeVal(m_fOrder, "order", 5);
}

/// 0x08dad1ee
void FIFSMCondition::OnSave(FXmlElement& a1) const {
    a1.SetAttributeVal("order", 5, m_fOrder);
}

/// 0x08dad2b6
float FIFSMCondition::get_order() const {
    return m_fOrder;
}

/// 0x08dad2be
void FIFSMCondition::set_order(float a1) {
    m_fOrder = a1;
}

/// 0x08dad286
char const* FIFSMCondition::get_description() const {
    return m_pDescription;
}
