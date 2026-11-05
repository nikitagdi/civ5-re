/// origin guess
#pragma once
#include "FirePlace/FireWorks/FIConditionEvent.h"
#include "FirePlace/FireWorks/FIFSMCondition.h"
#include "FirePlace/FireWorks/FIFiniteStateMachineInstance.h"

class FClassId;
class FContext;
class FXmlElement;

/// size 0x24
/// vtable 0x0a306260
class FFSMConditionEventSetPathOption : public FIFSMCondition, public FIConditionEvent {
public:
    float m_fValue;  ///< +0x1c
    FIFiniteStateMachineInstance::path_option m_path;  ///< +0x20
    /// 0x08dafefc
    /// deleting 0x08daff3c
    virtual ~FFSMConditionEventSetPathOption();
    /// 0x08dade0e
    virtual UNK_RET OnLoad(FXmlElement const& a1);
    /// 0x08dadea2
    virtual UNK_RET OnSave(FXmlElement& a1) const;
    /// 0x08daff82
    virtual bool evaluate(FContext& a1);
    /// 0x08dafea6
    virtual UNK_RET vf_00(UNK_PARAMS);
    /// 0x08dafec6
    virtual UNK_RET vf_01(UNK_PARAMS);
    /// 0x08dadd8a
    /// thunk 0x08daddcc
    virtual UNK_RET QueryInterface(FClassId const& a1);
    /// 0x08dadcf2
    /// thunk 0x08dadd5e
    virtual UNK_RET fire_event(FContext& a1) const;
};
