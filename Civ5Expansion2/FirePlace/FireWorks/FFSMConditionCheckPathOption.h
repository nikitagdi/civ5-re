/// origin guess
#pragma once
#include "FirePlace/FireWorks/FCompareValues.h"
#include "FirePlace/FireWorks/FIFSMCondition.h"
#include "FirePlace/FireWorks/FIFiniteStateMachineInstance.h"

class FContext;
class FXmlElement;

/// size 0x24
/// vtable 0x0a3061e0
class FFSMConditionCheckPathOption : public FIFSMCondition {
public:
    float m_fValue;  ///< +0x18
    FIFiniteStateMachineInstance::path_option m_path;  ///< +0x1c
    FCompareValues::Method m_method;                   ///< +0x20
    /// absent
    virtual ~FFSMConditionCheckPathOption();
    /// 0x08dae63e
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08dae704
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08dae582
    virtual bool evaluate(FContext& a1);
    /// 0x08dafc9c
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dafcbc
    virtual UNK_RET vf_01(UNK_PARAMS);
};
