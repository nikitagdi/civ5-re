/// origin guess
#pragma once
#include "FirePlace/FireWorks/FFSMConditionEventSetPathOption.h"
#include "FirePlace/FireWorks/FFastVector.h"

class FContext;
class FXmlElement;

/// size 0x58
/// vtable 0x0a305c50
class FFSMConditionEventRandomPathOption : public FFSMConditionEventSetPathOption {
public:
    /// size 0x8
    struct WeightEntry {
        float m_fValue;   ///< +0x0
        float m_fWeight;  ///< +0x4
    };
    FStaticVector<WeightEntry, 4u> m_weights;  ///< +0x24
    bool m_bUseWeights;                        ///< +0x54
    /// 0x08dadf2e
    FFSMConditionEventRandomPathOption();
    /// 0x08db0232
    /// deleting 0x08db029a
    virtual ~FFSMConditionEventRandomPathOption();
    /// 0x08dae0d8
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08dae260
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08dadf98
    /// thunk 0x08dae0ac
    virtual UNK_RET fire_event(FContext& a1) const;
    /// 0x08db01dc
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08db01fc
    virtual UNK_RET vf_01(UNK_PARAMS);
};
