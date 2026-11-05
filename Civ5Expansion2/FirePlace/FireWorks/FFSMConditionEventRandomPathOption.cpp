/// origin guess
#include "FirePlace/FireWorks/FFSMConditionEventRandomPathOption.h"
#include "FirePlace/FireWorks/FXmlElement.h"

/// 0x08dadf2e
FFSMConditionEventRandomPathOption::FFSMConditionEventRandomPathOption()
    : m_bUseWeights(false) {
    m_fValue = 0.0f;
    m_path = (FIFiniteStateMachineInstance::path_option)0;
}

/// 0x08dae260
void FFSMConditionEventRandomPathOption::OnSave(FXmlElement& a1) const {
    FFSMConditionEventSetPathOption::OnSave(a1);
    a1.SetAttributeVal("useweights", 10, m_bUseWeights);
    FXmlElement weights;
    a1.LinkEndChild(weights);
    weights.SetName("weights");
    for (unsigned int i = 0; i < m_weights.size(); i++) {
        FXmlElement weight;
        weights.LinkEndChild(weight);
        weight.SetName("weight");
        weight.SetAttributeVal("v", 1, m_weights[i].m_fValue);
        weight.SetAttributeVal("w", 1, m_weights[i].m_fWeight);
    }
}

/// 0x08dae0d8
void FFSMConditionEventRandomPathOption::OnLoad(FXmlElement const& a1) {
    FFSMConditionEventSetPathOption::OnLoad(a1);
    int useWeights = 0;
    a1.GetFirstAttributeVal(useWeights, "useweights", 10);
    m_bUseWeights = useWeights != 0;
    m_weights.clear();
    FXmlElement weights;
    if (a1.GetFirstChild(weights, "weights", 7)) {
        FXmlElement weight;
        bool found = weights.GetFirstChild(weight, "weight", 6);
        while (found) {
            WeightEntry entry;
            entry.m_fValue = 0.0f;
            entry.m_fWeight = 0.0f;
            weight.GetFirstAttributeVal(entry.m_fValue, "v", 1);
            weight.GetFirstAttributeVal(entry.m_fWeight, "w", 1);
            m_weights.push_back(entry);
            found = weight.GetNextSibling(weight, "weight", 6);
        }
    }
}
