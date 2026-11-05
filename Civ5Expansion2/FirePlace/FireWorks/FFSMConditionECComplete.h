/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIFSMCondition.h"

class FContext;
class FXmlElement;

/// size 0x24
/// vtable 0x0a3032e0
class FFSMConditionECComplete : public FIFSMCondition {
public:
    unsigned short m_usEventCode;  ///< +0x18
    float m_fValue;                ///< +0x1c
    float m_fDuration;             ///< +0x20
    /// 0x08cf1280
    FFSMConditionECComplete();
    /// absent
    virtual ~FFSMConditionECComplete();
    /// 0x08cf1362
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08cf13e4
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08cf12cc
    virtual bool evaluate(FContext& a1);
    /// 0x08cf1454
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08cf1474
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08cf133c
    float get_value() const;
    /// 0x08cf1344
    void set_value(float a1);
    /// 0x08cf1354
    void set_event_code(unsigned short a1);
};
