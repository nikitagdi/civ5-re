/// origin guess
#pragma once
#include "FirePlace/FireWorks/FCompareValues.h"
#include "FirePlace/FireWorks/FIFSMCondition.h"

class FContext;
class FXmlElement;

/// size 0x20
/// vtable 0x0a306160
class FFSMConditionCheckStateTime : public FIFSMCondition {
public:
    float m_fValue;  ///< +0x18
    FCompareValues::Method m_method;  ///< +0x1c
    /// absent
    virtual ~FFSMConditionCheckStateTime();
    /// 0x08dae462
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08dae4f6
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08dae3ac
    virtual bool evaluate(FContext& a1);
    /// 0x08dafa92
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dafab2
    virtual UNK_RET vf_01(UNK_PARAMS);
};
