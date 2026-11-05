/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFSMCondition.h"

class FContext;
class FXmlElement;

/// size 0x20
/// vtable 0x0a3064a0
class FFSMConditionRandomRoll : public FIFSMCondition {
public:
    float m_fThreshold;  ///< +0x18
    bool m_bInvert;      ///< +0x1c
    /// absent
    virtual ~FFSMConditionRandomRoll();
    /// 0x08dadbce
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08dadc64
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08dadb4c
    virtual bool evaluate(FContext& a1);
    /// 0x08db0fd0
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08db0ff0
    virtual UNK_RET vf_01(UNK_PARAMS);
};
